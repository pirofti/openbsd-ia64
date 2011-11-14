/* $OpenBSD$ */
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>

#include <uvm/uvm.h>

#include <machine/pmap.h>

/*
 * Public Domain.
 */
void
pmap_unwire(pmap_t pm, vaddr_t va)
{
	/*
	 * Nothing to do.
	 */
}

void
pmap_collect(pmap_t pm)
{
	/*
	 * Nothing to do.
	 */
}

void
pmap_protect(pmap_t pmap, vaddr_t sva, vaddr_t eva, vm_prot_t prot)
{
	/*
	 * Nothing to do.
	 */
}
