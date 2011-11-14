/* $OpenBSD: proc.h,v 1.1 2011/07/04 23:29:08 pirofti Exp $ */

/*
 * Public Domain.
 */
#ifndef _MACHINE_PROC_H_
#define _MACHINE_PROC_H_

#include <machine/frame.h>

/*
 * Machine-dependent part of the proc structure for Itanium.
 */
struct mdproc {
	struct	trapframe *md_regs;	/* registers on current frame */
	int	md_flags;
	__volatile int md_astpending;
};

#endif /* _MACHINE_PROC_H_ */
