/* $OpenBSD$ */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/time.h>
#include <sys/kernel.h>
#include <sys/device.h>
#include <sys/timeout.h>
#include <sys/timetc.h>

#include <machine/cpu.h>
#include <machine/intr.h>

/*
 * initialize the system time from the time of day clock
 */
void
inittodr(time_t t)
{
}

/*
 * Reset the clock.
 */
void
resettodr(void)
{
}

void
setstatclockrate(int newhz)
{
}
