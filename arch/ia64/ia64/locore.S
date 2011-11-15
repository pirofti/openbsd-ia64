/*-
 * Copyright (c) 1998 Doug Rabson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: locore.S,v 1.44 2011/03/18 22:45:43 marcel Exp $
 */

#include "assym.h"

#include <sys/syscall.h>
#include <machine/asm.h>
#include <machine/ia64_cpu.h>
#include <machine/pte.h>
#include <machine/vmparam.h>

#define INTRCNT_CLOCK		0
#define INTRCNT_COUNT		256

/*
 * Maximum name length in intrnames table (including terminating '\0'.
 * Since vmstat(8) assumes a maximum length of 13 (including '\0'), we're
 * pretty much limited to that (unless we don't care about the alignment
 * of the columns :-)
 */
#define INTRNAME_LEN		13

/*
 * The Altix 350 needs more than the architected 16KB (8KB for stack and
 * 8KB for RSE backing store) when calling EFI to setup virtual mode.
 */
#define	FW_STACK_SIZE	3*PAGE_SIZE

	.section .data.kstack, "aw"
	.align	PAGE_SIZE
	.global	kstack
kstack:	.space	FW_STACK_SIZE
	.global	kstack_top
kstack_top:

	.text

/*
 * Not really a leaf but we can't return.
 * The EFI loader passes the physical address of the bootinfo block in
 * register r8.
 */
ENTRY_NOPROFILE(__start, 1)
	.prologue
	.save	rp,r0
	.body
{	.mlx
	mov	ar.rsc=0
	movl	r16=trap_type	// set up IVT early
	;;
}
{	.mlx
	mov	cr.iva=r16
	movl	r16=kstack
	;;
}
{	.mmi
	srlz.i
	;;
	ssm	IA64_PSR_DFH
	mov	r17=FW_STACK_SIZE-16
	;;
}
{	.mlx
	add	sp=r16,r17		// proc0's stack
	movl	gp=__gp			// find kernel globals
	;;
}
{	.mlx
	mov	ar.bspstore=r16		// switch backing store
	movl	r16=pa_bootinfo
	;;
}
{	.mmi
	st8	[r16]=r8		// save the PA of the bootinfo block
	loadrs				// invalidate regs
	mov	r17=IA64_DCR_DEFAULT
	;;
}
{	.mmi
	mov	cr.dcr=r17
	mov	ar.rsc=3		// turn rse back on
	nop	0
	;;
}
{	.mib
	nop	0
	nop	0
	br.call.sptk.many rp=init_ia64
	;;
}
	// We have the new bspstore in r8 and the new sp in r9.
	// Switch onto the new stack and call mi_startup().
{	.mmi
	mov	ar.rsc = 0
	;;
	mov	ar.bspstore = r8
	mov	sp = r9
	;;
}
{	.mmi
	loadrs
	;;
	mov	ar.rsc = 3
	nop	0
	;;
}
{	.mib
	nop	0
	nop	0
	br.call.sptk.many rp=main
	;;
}
	/* NOTREACHED */
1:	br.cond.sptk.few 1b
END(__start)

#ifdef SMP
/*
 * AP wake-up entry point. The handoff state is similar as for the BSP,
 * as described on page 3-9 of the IPF SAL Specification. The difference
 * lies in the contents of register b0. For APs this register holds the
 * return address into the SAL rendezvous routine.
 *
 * Note that we're responsible for clearing the IRR bit by reading cr.ivr
 * and issuing the EOI to the local SAPIC.
 */
	.align	32
ENTRY_NOPROFILE(os_boot_rendez,0)
	mov	r16=cr.ivr	// clear IRR bit
	;;
	srlz.d
	mov	cr.eoi=r0	// ACK the wake-up
	;;
	srlz.d
	rsm	IA64_PSR_IC|IA64_PSR_I
	;;
	mov	r16 = (5<<8)|(PAGE_SHIFT<<2)|1
	movl	r17 = 5<<61
	;;
	mov	rr[r17] = r16
	;;
	srlz.d
	mov	r16 = (6<<8)|(IA64_ID_PAGE_SHIFT<<2)
	movl	r17 = 6<<61
	;;
	mov	rr[r17] = r16
	;;
	srlz.d
	mov	r16 = (7<<8)|(IA64_ID_PAGE_SHIFT<<2)
	movl	r17 = 7<<61
	;;
	mov	rr[r17] = r16
	;;
	srlz.d
	mov	r18 = 28<<2
	movl	r16 = PTE_PRESENT+PTE_MA_WB+PTE_ACCESSED+PTE_DIRTY+ \
			PTE_PL_KERN+PTE_AR_RWX+PTE_ED
	;;
	mov	cr.ifa = r17
	mov	cr.itir = r18
	ptr.d	r17, r18
	ptr.i	r17, r18
	;;
	srlz.i
	;;
	itr.d	dtr[r0] = r16
	mov	r18 = IA64_DCR_DEFAULT
	;;
	itr.i	itr[r0] = r16
	mov	cr.dcr = r18
	;;
	srlz.i
	;;
1:	mov	r16 = ip
	add	r17 = 2f-1b, r17
	movl	r18 = (IA64_PSR_AC|IA64_PSR_BN|IA64_PSR_DFH|IA64_PSR_DT|IA64_PSR_IC|IA64_PSR_IT|IA64_PSR_RT)
	;;
	add	r17 = r17, r16
	mov	cr.ipsr = r18
	mov	cr.ifs = r0
	;;
	mov	cr.iip = r17
	;;
	rfi

	.align	32
2:
{	.mlx
	mov	ar.rsc = 0
	movl	r16 = trap_type				// set up IVT early
	;;
}
{	.mlx
	mov	cr.iva = r16
	movl	r16 = ap_stack
	;;
}
{	.mmi
	srlz.i
	;;
	ld8	r16 = [r16]
	mov	r18 = KSTACK_PAGES*PAGE_SIZE-SIZEOF_PCB-SIZEOF_TRAPFRAME-16
	;;
}
{	.mlx
	mov	ar.bspstore = r16
	movl	gp = __gp
	;;
}
{	.mmi
	loadrs
	;;
	alloc	r17 = ar.pfs, 0, 0, 0, 0
	add	sp = r18, r16
	;;
}
{	.mib
	mov	ar.rsc = 3
	nop	0
	br.call.sptk.few rp = ia64_ap_startup
	;;
}
	/* NOT REACHED */
9:
{	.mib
	nop	0
	nop	0
	br.sptk	9b
	;;
}
END(os_boot_rendez)

#endif /* !SMP */

/*
 * Create a default interrupt name table. The first entry (vector 0) is
 * hardwaired to the clock interrupt.
 */
	.data
	.align 8
EXPORT(intrnames)
	.ascii "clock"
	.fill INTRNAME_LEN - 5 - 1, 1, ' '
	.byte 0
intr_n = 1
.rept INTRCNT_COUNT - 1
	.ascii "#"
	.byte intr_n / 100 + '0'
	.byte (intr_n % 100) / 10 + '0'
	.byte intr_n % 10 + '0'
	.fill INTRNAME_LEN - 1 - 3 - 1, 1, ' '
	.byte 0
	intr_n = intr_n + 1
.endr
EXPORT(eintrnames)
	.align 8
EXPORT(intrcnt)
	.fill INTRCNT_COUNT, 8, 0
EXPORT(eintrcnt)

	.data
	.align	16
	.global fptr_storage
fptr_storage:
	.space	4096*16			// XXX
fptr_storage_end:

/*
 * Signal trampoline; copied to top of user stack.
 * gdb's backtrace logic matches against the instructions in this.
 */
ENTRY(sigcode,0)
END(sigcode)
ENTRY(esigcode,0)
END(esigcode)
ENTRY(longjmp,0)
END(longjmp)
ENTRY(setjmp,0)
END(setjmp)
