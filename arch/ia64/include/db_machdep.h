#ifndef	_MACHINE_DB_MACHDEP_H_
#define	_MACHINE_DB_MACHDEP_H_

/*
 * Machine-dependent defines for new kernel debugger.
 */

#include <sys/param.h>
#include <uvm/uvm_extern.h>
#include <machine/trap.h>
#include <sys/mutex.h>

typedef	vaddr_t		db_addr_t;	/* address - unsigned */
typedef	long		db_expr_t;	/* expression - signed */

#define	BKPT_ADDR(addr)	(addr)		/* breakpoint address */
#define	BKPT_INST	0xcc		/* breakpoint instruction */
#define	BKPT_SIZE	(1)		/* size of breakpoint inst */
#define	BKPT_SET(inst)	(BKPT_INST)

typedef struct trapframe db_regs_t;

extern db_regs_t ddb_regs;		/* register state */
#define	DDB_REGS	(&ddb_regs)

#define inst_trap_return(ins)	(ins & 0)
#define inst_return(ins)	(ins & 0)
#define inst_call(ins)		(ins & 0)
#define inst_branch(ins)	(ins & 0)
#define inst_load(ins)		(ins & 0)
#define inst_store(ins)		(ins & 0)
#define inst_unconditional_flow_transfer(ins) (ins & 0)

#define	PC_REGS(regs)	((db_addr_t)(regs)->tf_ip)
#define	SET_PC_REGS(regs, value) (regs)->tf_ip = (int64_t)(value)

#define	IS_BREAKPOINT_TRAP(type, code)	((type) == T_BPTFLT)
#define IS_WATCHPOINT_TRAP(type, code)	((type) == T_TRCTRAP && (code) & 15)

#endif
