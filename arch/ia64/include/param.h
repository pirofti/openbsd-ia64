/*	$OpenBSD: param.h,v 1.2 2011/09/08 03:40:32 guenther Exp $	*/

/*
 * Written by Paul Irofti <pirofti@openbsd.org>. Public Domain.
 */

#ifndef _IA64_PARAM_H_
#define _IA64_PARAM_H_

#define	_MACHINE	ia64
#define	MACHINE		"ia64"
#define	_MACHINE_ARCH	ia64
#define	MACHINE_ARCH	"ia64"
#define	MID_MACHINE	MID_IA64

#define	ALIGNBYTES		_ALIGNBYTES
#define	ALIGN(p)		_ALIGN(p)
#define	ALIGNED_POINTER(p,t)	_ALIGNED_POINTER(p,t)

#define	PGSHIFT		12		/* LOG2(NBPG) */
#define	NBPG		(1 << PGSHIFT)	/* bytes/page */
#define	PGOFSET		(NBPG-1)	/* byte offset into page */

#define	NMBCLUSTERS	4096		/* map size, max cluster allocation */

/*
 * Minimum and maximum sizes of the kernel malloc arena in PAGE_SIZE-sized
 * logical pages.
 */
#define	NKMEMPAGES_MIN_DEFAULT	((8 * 1024 * 1024) >> PAGE_SHIFT)
#define	NKMEMPAGES_MAX_DEFAULT	((128 * 1024 * 1024) >> PAGE_SHIFT)


#define	DEV_BSHIFT	9		/* log2(DEV_BSIZE) */
#define	DEV_BSIZE	(1 << DEV_BSHIFT)
#define	BLKDEV_IOSIZE	2048
#ifndef	MAXPHYS
#define	MAXPHYS		(64 * 1024)	/* max raw I/O transfer size */
#endif

/* bytes to disk blocks */
#define	dbtob(x)	((x) << DEV_BSHIFT)
#define	btodb(x)	((x) >> DEV_BSHIFT)

#define	UPAGES		5		/* pages of u-area */
#define	USPACE		(UPAGES * NBPG)	/* total size of u-area */
#define	USPACE_ALIGN	(0)		/* u-area alignment 0-none */

#define ovbcopy bcopy

#include <machine/cpu.h>	/* needed by param.c */

#endif /* _IA64_PARAM_H_ */
