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

void
pmap_page_protect(struct vm_page *pg, vm_prot_t prot)
{
}

/*
 * pmap_clear_attrs: change a page's attributes
 *
 * => we return TRUE if we cleared one of the bits we were asked to
 */
boolean_t
pmap_clear_attrs(struct vm_page *pg, unsigned long clearbits)
{
	return 0;
}

/*
 * pmap_test_attrs: test a page's attributes
 */
boolean_t
pmap_test_attrs(struct vm_page *pg, unsigned int testbits)
{
	return 0;
}

/*
 * pmap_activate: activate a process' pmap
 *
 * => called from cpu_fork() and when switching pmaps during exec
 * => if p is the curproc, then load it into the MMU
 */
void
pmap_activate(struct proc *p)
{
}

/*
 * pmap_deactivate: deactivate a process' pmap
 */
void
pmap_deactivate(struct proc *p)
{
}

/*
 * pmap_create: create a pmap
 *
 * => note: old pmap interface took a "size" args which allowed for
 *	the creation of "software only" pmaps (not in bsd).
 */
struct pmap *
pmap_create(void)
{
	return NULL;
}

/*
 * pmap_copy_page: copy a page
 */

void
pmap_copy_page(struct vm_page *srcpg, struct vm_page *dstpg)
{
}

/*
 * pmap_destroy: drop reference count on pmap.   free pmap if
 *	reference count goes to zero.
 */
void
pmap_destroy(struct pmap *pmap)
{
}

/*
 * pmap_enter: enter a mapping into a pmap
 *
 * => must be done "now" ... no lazy-evaluation
 */
int
pmap_enter(struct pmap *pmap, vaddr_t va, paddr_t pa, vm_prot_t prot, int flags)
{
	return 0;
}

/*
 * pmap_extract: extract a PA for the given VA
 */
boolean_t
pmap_extract(struct pmap *pmap, vaddr_t va, paddr_t *pap)
{
	return 0;
}

/*
 * pmap_init: called from uvm_init, our job is to get the pmap
 * system ready to manage mappings... this mainly means initing
 * the pv_entry stuff.
 */
void
pmap_init(void)
{
}

/*
 * pmap_kenter_pa: enter a kernel mapping without R/M (pv_entry) tracking
 *
 * => no need to lock anything, assume va is already allocated
 * => should be faster than normal pmap enter function
 */
void
pmap_kenter_pa(vaddr_t va, paddr_t pa, vm_prot_t prot)
{
}

/*
 * pmap_kremove: remove a kernel mapping(s) without R/M (pv_entry) tracking
 *
 * => no need to lock anything
 * => caller must dispose of any vm_page mapped in the va range
 * => note: not an inline function
 * => we assume the va is page aligned and the len is a multiple of PAGE_SIZE
 * => we assume kernel only unmaps valid addresses and thus don't bother
 *    checking the valid bit before doing TLB flushing
 */
void
pmap_kremove(vaddr_t sva, vsize_t len)
{
}

/*
 *	Add a reference to the specified pmap.
 */
void
pmap_reference(struct pmap *pmap)
{
}

/*
 * pmap_remove: top level mapping removal function
 *
 * => caller should not be holding any pmap locks
 */
void
pmap_remove(struct pmap *pmap, vaddr_t sva, vaddr_t eva)
{
}

void
pmap_virtual_space(vaddr_t *vstartp, vaddr_t *vendp)
{
}

/*
 * pmap_zero_page: zero a page
 */
void
pmap_zero_page(struct vm_page *pg)
{
}
