/* $OpenBSD: pmap.h,v 1.1 2011/07/04 23:29:08 pirofti Exp $ */

/*
 * Public Domain.
 */
#ifndef	_MACHINE_PMAP_H_
#define	_MACHINE_PMAP_H_

struct pmap {
};

typedef struct pmap	*pmap_t;

#define pmap_kernel()			0
#define	pmap_resident_count(pmap)	0
#define	pmap_update(pmap)		/* nothing (yet) */

#define pmap_clear_modify(pg)		pmap_clear_attrs(pg, 0)
#define pmap_clear_reference(pg)	pmap_clear_attrs(pg, 0)
#define pmap_copy(DP,SP,D,L,S)		
#define pmap_is_modified(pg)		pmap_test_attrs(pg, 0)
#define pmap_is_referenced(pg)		pmap_test_attrs(pg, 0)
#define pmap_move(DP,SP,D,L,S)		
#define pmap_valid_entry(E) 		((E) & PG_V) /* is PDE or PTE valid? */

void pmap_protect(pmap_t pmap, vaddr_t sva, vaddr_t eva, vm_prot_t prot);
boolean_t pmap_clear_attrs(struct vm_page *, unsigned long);
boolean_t pmap_test_attrs(struct vm_page *, unsigned);
void pmap_page_protect(struct vm_page *pg, vm_prot_t prot);

void pmap_unwire(pmap_t pm, vaddr_t va);
void pmap_collect(pmap_t pm);

#define pmap_proc_iflush(p,va,len)	/* nothing */
#define pmap_unuse_final(p)		/* nothing */
#define	pmap_remove_holes(map)		do { /* nothing */ } while (0)

#endif	/* _MACHINE_PMAP_H_ */
