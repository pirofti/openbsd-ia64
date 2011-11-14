/* $OpenBSD: pcb.h,v 1.1 2011/07/04 23:29:08 pirofti Exp $ */

/*
 * Public Domain.
 */
#ifndef _MACHINE_PCB_H_
#define _MACHINE_PCB_H_

struct pcb {
	caddr_t pcb_onfault;		/* copyin/out fault recovery */
	struct  pmap *pcb_pmap;		/* back pointer to our pmap */
};

#endif /* _MACHINE_PCB_H_ */
