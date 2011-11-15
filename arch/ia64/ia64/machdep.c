/*	$OpenBSD: machdep.c,v 1.149 2011/07/10 18:11:49 deraadt Exp $	*/
/*	$NetBSD: machdep.c,v 1.3 2003/05/07 22:58:18 fvdl Exp $	*/

/*-
 * Copyright (c) 1996, 1997, 1998, 2000 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Charles M. Hannum and by Jason R. Thorpe of the Numerical Aerospace
 * Simulation Facility, NASA Ames Research Center.
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
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*-
 * Copyright (c) 1982, 1987, 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * William Jolitz.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)machdep.c	7.4 (Berkeley) 6/3/91
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/signalvar.h>
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/user.h>
#include <sys/exec.h>
#include <sys/buf.h>
#include <sys/reboot.h>
#include <sys/conf.h>
#include <sys/file.h>
#include <sys/malloc.h>
#include <sys/mbuf.h>
#include <sys/msgbuf.h>
#include <sys/mount.h>
#include <sys/vnode.h>
#include <sys/extent.h>
#include <sys/core.h>
#include <sys/kcore.h>
#include <sys/syscallargs.h>

#ifdef KGDB
#include <sys/kgdb.h>
#endif

#include <dev/cons.h>
#include <stand/boot/bootarg.h>

#include <net/if.h>
#include <uvm/uvm.h>

#include <sys/sysctl.h>

#include <machine/cpu.h>
#include <machine/biosvar.h>
#include <machine/reg.h>

#ifdef DDB
#include <machine/db_machdep.h>
#include <ddb/db_extern.h>
extern int db_console;
#endif

#include "isa.h"
#include "isadma.h"
#include "ksyms.h"

#include "com.h"
#if NCOM > 0
#include <sys/tty.h>
#include <dev/ic/comvar.h>
#include <dev/ic/comreg.h>
#endif

/* the following is used externally (sysctl_hw) */
char machine[] = MACHINE;

/*
 * switchto vectors
 */
void (*cpu_idle_leave_fcn)(void) = NULL;
void (*cpu_idle_cycle_fcn)(void) = NULL;
void (*cpu_idle_enter_fcn)(void) = NULL;

/* the following is used externally for concurrent handlers */
int setperf_prio = 0;

#ifdef CPURESET_DELAY
int	cpureset_delay = CPURESET_DELAY;
#else
int     cpureset_delay = 2000; /* default to 2s */
#endif

int	physmem;
u_int64_t	dumpmem_low;
u_int64_t	dumpmem_high;
extern int	boothowto;
int	cpu_class;

paddr_t	dumpmem_paddr;
vaddr_t	dumpmem_vaddr;
psize_t	dumpmem_sz;


char	*ssym = NULL;
vaddr_t kern_end;

vaddr_t	msgbuf_vaddr;
paddr_t msgbuf_paddr;

vaddr_t	idt_vaddr;
paddr_t	idt_paddr;

vaddr_t lo32_vaddr;
paddr_t lo32_paddr;
paddr_t tramp_pdirpa;

int kbd_reset;
int lid_suspend;

/*
 * safepri is a safe priority for sleep to set for a spin-wait
 * during autoconfiguration or after a panic.
 */
int	safepri = 0;

struct vm_map *exec_map = NULL;
struct vm_map *phys_map = NULL;

/* UVM constraint ranges. */
struct uvm_constraint_range  isa_constraint = { 0x0, 0x00ffffffUL };
struct uvm_constraint_range  dma_constraint = { 0x0, 0xffffffffUL };
struct uvm_constraint_range *uvm_md_constraints[] = {
    &isa_constraint,
    &dma_constraint,
    NULL,
};

#ifdef DEBUG
int sigdebug = 0;
pid_t sigpid = 0;
#define SDB_FOLLOW      0x01
#endif

paddr_t avail_start;
paddr_t avail_end;

/*
 * Format of boot information passed to us by 32-bit /boot
 */
typedef struct _boot_args32 {
	int	ba_type;
	int	ba_size;
	int	ba_nextX;	/* a ptr in 32-bit world, but not here */
	char	ba_arg[1];
} bootarg32_t;

#define BOOTARGC_MAX	NBPG	/* one page */

/* locore copies the arguments from /boot to here for us */
u_int64_t pa_bootinfo;
char bootinfo[BOOTARGC_MAX];
int bootinfo_size = BOOTARGC_MAX;

void getbootinfo(char *, int);

struct consdev *cn_tab;

/*
 * Size of memory segments, before any memory is stolen.
 */
phys_ram_seg_t mem_clusters[VM_PHYSSEG_MAX];
int	mem_cluster_cnt;

int	cpu_dump(void);
int	cpu_dumpsize(void);
u_long	cpu_dump_mempagecnt(void);
void	dumpsys(void);
void	cpu_init_extents(void);
void	map_tramps(void);
void	init_ia64(paddr_t);
void	(*cpuresetfn)(void);

#ifdef KGDB
#ifndef KGDB_DEVNAME
#define KGDB_DEVNAME	"com"
#endif /* KGDB_DEVNAME */
char kgdb_devname[] = KGDB_DEVNAME;
#if NCOM > 0
#ifndef KGDBADDR
#define KGDBADDR	0x3f8
#endif /* KGDBADDR */
int comkgdbaddr = KGDBADDR;
#ifndef KGDBRATE
#define KGDBRATE	TTYDEF_SPEED
#endif /* KGDBRATE */
int comkgdbrate = KGDBRATE;
#ifndef KGDBMODE
#define KGDBMODE	((TTYDEF_CFLAG & ~(CSIZE | CSTOPB | PARENB)) | CS8)
#endif /* KGDBMODE */
int comkgdbmode = KGDBMODE;
#endif /* NCOM */
void	kgdb_port_init(void);
#endif /* KGDB */

#ifdef APERTURE
#ifdef INSECURE
int allowaperture = 1;
#else
int allowaperture = 0;
#endif
#endif

/*
 * Machine-dependent startup code
 */
void
cpu_startup(void)
{
}

/*  
 * machine dependent system variables.
 */ 
int
cpu_sysctl(int *name, u_int namelen, void *oldp, size_t *oldlenp, void *newp,
    size_t newlen, struct proc *p)
{
	return 1;
}

/*
 * Send an interrupt to process.
 *
 * Stack is set up to allow sigcode stored
 * in u. to call routine, followed by kcall
 * to sigreturn routine below.  After sigreturn
 * resets the signal mask, the stack, and the
 * frame pointer, it returns to the user
 * specified pc, psl.
 */
void
sendsig(sig_t catcher, int sig, int mask, u_long code, int type,
    union sigval val)
{
}

/*
 * System call to cleanup state after a signal
 * has been taken.  Reset signal mask and
 * stack state from context left by sendsig (above).
 * Return to previous pc and psl as specified by
 * context left by sendsig. Check carefully to
 * make sure that the user has not modified the
 * psl to gain improper privileges or to cause
 * a machine fault.
 */
int
sys_sigreturn(struct proc *p, void *v, register_t *retval)
{
	return 0;
}

/*
 * Notify the current process (p) that it has a signal pending,
 * process as soon as possible.
 */
void
signotify(struct proc *p)
{
	aston(p);
	cpu_unidle(p->p_cpu);
}

#ifdef MULTIPROCESSOR
void
cpu_unidle(struct cpu_info *ci)
{
	if (ci != curcpu())
		ia64_send_ipi(ci, X86_IPI_NOP);
}
#endif

int	waittime = -1;
struct pcb dumppcb;

void
boot(int howto)
{
	while (1) {};
}

/*
 * These variables are needed by /sbin/savecore
 */
u_long	dumpmag = 0x8fca0101;	/* magic number */
int 	dumpsize = 0;		/* pages */
long	dumplo = 0; 		/* blocks */

/*
 * cpu_dump: dump the machine-dependent kernel core dump headers.
 */
int
cpu_dump(void)
{
	return 0;
}

/*
 * This is called by main to set dumplo and dumpsize.
 * Dumps always skip the first PAGE_SIZE of disk space
 * in case there might be a disk label stored there.
 * If there is extra space, put dump at the end to
 * reduce the chance that swapping trashes it.
 */
void
dumpconf(void)
{
}

/*
 * Doadump comes here after turning off memory management and
 * getting on the dump stack, either when called above, or by
 * the auto-restart code.
 */
#define BYTES_PER_DUMP  MAXPHYS /* must be a multiple of pagesize */

void
dumpsys(void)
{
}

/*
 * Clear registers on exec
 */
void
setregs(struct proc *p, struct exec_package *pack, u_long stack,
    register_t *retval)
{
}

extern  struct user *proc0paddr;

#define	KBTOB(x)	((size_t)(x) * 1024UL)

void
cpu_init_extents(void)
{
}

#if defined(MULTIPROCESSOR) || \
    (NACPI > 0 && !defined(SMALL_KERNEL))
void
map_tramps(void) 
{
}
#endif

#define	IDTVEC(name)	__CONCAT(X, name)
typedef void (vector)(void);
extern vector IDTVEC(osyscall);
extern vector *IDTVEC(exceptions)[];

void
init_ia64(paddr_t first_avail)
{
}

#ifdef KGDB
void
kgdb_port_init(void)
{
#if NCOM > 0
	if (!strcmp(kgdb_devname, "com")) {
		bus_space_tag_t tag = X86_BUS_SPACE_IO;
		com_kgdb_attach(tag, comkgdbaddr, comkgdbrate, COM_FREQ,
		    comkgdbmode);
	}
#endif
} 
#endif /* KGDB */

void
cpu_reset(void)
{
}

/*
 * cpu_dumpsize: calculate size of machine-dependent kernel core dump headers.
 */
int
cpu_dumpsize(void)
{
	return 0;
}

/*
 * cpu_dump_mempagecnt: calculate the size of RAM (in pages) to be dumped.
 */
u_long
cpu_dump_mempagecnt(void)
{
	return 0;
}

void
cpu_initclocks(void)
{
}

void
need_resched(struct cpu_info *ci)
{
	ci->ci_want_resched = 1;

	/* There's a risk we'll be called before the idle threads start */
	if (ci->ci_curproc) {
		aston(ci->ci_curproc);
		cpu_unidle(ci);
	}
}

#ifdef DIAGNOSTIC
void
splassert_check(int wantipl, const char *func)
{
	int cpl = curcpu()->ci_ilevel;

	if (cpl < wantipl) {
		splassert_fail(wantipl, cpl, func);
	}

	if (wantipl == IPL_NONE && curcpu()->ci_idepth != 0) {
		splassert_fail(-1, curcpu()->ci_idepth, func);
	}
}
#endif

void
getbootinfo(char *bootinfo, int bootinfo_size)
{
}

bios_diskinfo_t *
bios_getdiskinfo(dev_t dev)
{
	return NULL;
}
