/* $OpenBSD: atomic.h,v 1.1 2011/07/04 23:29:08 pirofti Exp $ */

/*
 * Written by Paul Irofti <pirofti@openbsd.org>. Public Domain.
 */

#ifndef _MACHINE_ATOMIC_H_
#define _MACHINE_ATOMIC_H_

static __inline void
atomic_setbits_int(volatile u_int32_t *ptr, u_int32_t bits)
{
	*ptr |= bits;
	/* 
	 * XXX until I can figure out the retarded AT&T syntax.
	 * __asm __volatile("or %0=[%0, %1]" : "+m" (*ptr) : "ir" (bits));
	 */
}
static __inline void
atomic_clearbits_int(volatile u_int32_t *ptr, u_int32_t bits)
{
	*ptr &= ~bits;
}

#endif /* _MACHINE_ATOMIC_H_ */
