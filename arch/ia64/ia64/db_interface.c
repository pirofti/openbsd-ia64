#include <machine/db_machdep.h>

#include <ddb/db_sym.h>
#include <ddb/db_command.h>
#include <ddb/db_extern.h>
#include <ddb/db_access.h>
#include <ddb/db_output.h>
#include <ddb/db_variables.h>
#include <ddb/db_interface.h>
#include <ddb/db_run.h>
#include <ddb/db_var.h>

db_regs_t ddb_regs;

int db_active = 0;

#define DB_OFFSET(x)    (db_expr_t *)offsetof(struct trapframe, x)
struct db_variable db_regs[] = {
	{"ip",		NULL,				FCN_NULL},
	{"cr.ifs",	DB_OFFSET(tf_special.cfm),	FCN_NULL},
	{"cr.ifa",	DB_OFFSET(tf_special.ifa),	FCN_NULL},
	{"ar.bspstore",	DB_OFFSET(tf_special.bspstore),	FCN_NULL},
	{"ndirty",	DB_OFFSET(tf_special.ndirty),	FCN_NULL},
	{"rp",		DB_OFFSET(tf_special.rp),	FCN_NULL},
	{"ar.pfs",	DB_OFFSET(tf_special.pfs),	FCN_NULL},
	{"psr",		DB_OFFSET(tf_special.psr),	FCN_NULL},
	{"cr.isr",	DB_OFFSET(tf_special.isr),	FCN_NULL},
	{"pr",		DB_OFFSET(tf_special.pr),	FCN_NULL},
	{"ar.rsc",	DB_OFFSET(tf_special.rsc),	FCN_NULL},
	{"ar.rnat",	DB_OFFSET(tf_special.rnat),	FCN_NULL},
	{"ar.unat",	DB_OFFSET(tf_special.unat),	FCN_NULL},
	{"ar.fpsr",	DB_OFFSET(tf_special.fpsr),	FCN_NULL},
	{"gp",		DB_OFFSET(tf_special.gp),	FCN_NULL},
	{"sp",		DB_OFFSET(tf_special.sp),	FCN_NULL},
	{"tp",		DB_OFFSET(tf_special.tp),	FCN_NULL},
	{"b6",		DB_OFFSET(tf_scratch.br6),	FCN_NULL},
	{"b7",		DB_OFFSET(tf_scratch.br7),	FCN_NULL},
	{"r2",		DB_OFFSET(tf_scratch.gr2),	FCN_NULL},
	{"r3",		DB_OFFSET(tf_scratch.gr3),	FCN_NULL},
	{"r8",		DB_OFFSET(tf_scratch.gr8),	FCN_NULL},
	{"r9",		DB_OFFSET(tf_scratch.gr9),	FCN_NULL},
	{"r10",		DB_OFFSET(tf_scratch.gr10),	FCN_NULL},
	{"r11",		DB_OFFSET(tf_scratch.gr11),	FCN_NULL},
	{"r14",		DB_OFFSET(tf_scratch.gr14),	FCN_NULL},
	{"r15",		DB_OFFSET(tf_scratch.gr15),	FCN_NULL},
	{"r16",		DB_OFFSET(tf_scratch.gr16),	FCN_NULL},
	{"r17",		DB_OFFSET(tf_scratch.gr17),	FCN_NULL},
	{"r18",		DB_OFFSET(tf_scratch.gr18),	FCN_NULL},
	{"r19",		DB_OFFSET(tf_scratch.gr19),	FCN_NULL},
	{"r20",		DB_OFFSET(tf_scratch.gr20),	FCN_NULL},
	{"r21",		DB_OFFSET(tf_scratch.gr21),	FCN_NULL},
	{"r22",		DB_OFFSET(tf_scratch.gr22),	FCN_NULL},
	{"r23",		DB_OFFSET(tf_scratch.gr23),	FCN_NULL},
	{"r24",		DB_OFFSET(tf_scratch.gr24),	FCN_NULL},
	{"r25",		DB_OFFSET(tf_scratch.gr25),	FCN_NULL},
	{"r26",		DB_OFFSET(tf_scratch.gr26),	FCN_NULL},
	{"r27",		DB_OFFSET(tf_scratch.gr27),	FCN_NULL},
	{"r28",		DB_OFFSET(tf_scratch.gr28),	FCN_NULL},
	{"r29",		DB_OFFSET(tf_scratch.gr29),	FCN_NULL},
	{"r30",		DB_OFFSET(tf_scratch.gr30),	FCN_NULL},
	{"r31",		DB_OFFSET(tf_scratch.gr31),	FCN_NULL},
	{"r32",		(db_expr_t*)0,			FCN_NULL},
	{"r33",		(db_expr_t*)1,			FCN_NULL},
	{"r34",		(db_expr_t*)2,			FCN_NULL},
	{"r35",		(db_expr_t*)3,			FCN_NULL},
	{"r36",		(db_expr_t*)4,			FCN_NULL},
	{"r37",		(db_expr_t*)5,			FCN_NULL},
	{"r38",		(db_expr_t*)6,			FCN_NULL},
	{"r39",		(db_expr_t*)7,			FCN_NULL},
	{"r40",		(db_expr_t*)8,			FCN_NULL},
	{"r41",		(db_expr_t*)9,			FCN_NULL},
	{"r42",		(db_expr_t*)10,			FCN_NULL},
	{"r43",		(db_expr_t*)11,			FCN_NULL},
	{"r44",		(db_expr_t*)12,			FCN_NULL},
	{"r45",		(db_expr_t*)13,			FCN_NULL},
	{"r46",		(db_expr_t*)14,			FCN_NULL},
	{"r47",		(db_expr_t*)15,			FCN_NULL},
	{"r48",		(db_expr_t*)16,			FCN_NULL},
	{"r49",		(db_expr_t*)17,			FCN_NULL},
	{"r50",		(db_expr_t*)18,			FCN_NULL},
	{"r51",		(db_expr_t*)19,			FCN_NULL},
	{"r52",		(db_expr_t*)20,			FCN_NULL},
	{"r53",		(db_expr_t*)21,			FCN_NULL},
	{"r54",		(db_expr_t*)22,			FCN_NULL},
	{"r55",		(db_expr_t*)23,			FCN_NULL},
	{"r56",		(db_expr_t*)24,			FCN_NULL},
	{"r57",		(db_expr_t*)25,			FCN_NULL},
	{"r58",		(db_expr_t*)26,			FCN_NULL},
	{"r59",		(db_expr_t*)27,			FCN_NULL},
	{"r60",		(db_expr_t*)28,			FCN_NULL},
	{"r61",		(db_expr_t*)29,			FCN_NULL},
	{"r62",		(db_expr_t*)30,			FCN_NULL},
	{"r63",		(db_expr_t*)31,			FCN_NULL},
	{"r64",		(db_expr_t*)32,			FCN_NULL},
	{"r65",		(db_expr_t*)33,			FCN_NULL},
	{"r66",		(db_expr_t*)34,			FCN_NULL},
	{"r67",		(db_expr_t*)35,			FCN_NULL},
	{"r68",		(db_expr_t*)36,			FCN_NULL},
	{"r69",		(db_expr_t*)37,			FCN_NULL},
	{"r70",		(db_expr_t*)38,			FCN_NULL},
	{"r71",		(db_expr_t*)39,			FCN_NULL},
	{"r72",		(db_expr_t*)40,			FCN_NULL},
	{"r73",		(db_expr_t*)41,			FCN_NULL},
	{"r74",		(db_expr_t*)42,			FCN_NULL},
	{"r75",		(db_expr_t*)43,			FCN_NULL},
	{"r76",		(db_expr_t*)44,			FCN_NULL},
	{"r77",		(db_expr_t*)45,			FCN_NULL},
	{"r78",		(db_expr_t*)46,			FCN_NULL},
	{"r79",		(db_expr_t*)47,			FCN_NULL},
	{"r80",		(db_expr_t*)48,			FCN_NULL},
	{"r81",		(db_expr_t*)49,			FCN_NULL},
	{"r82",		(db_expr_t*)50,			FCN_NULL},
	{"r83",		(db_expr_t*)51,			FCN_NULL},
	{"r84",		(db_expr_t*)52,			FCN_NULL},
	{"r85",		(db_expr_t*)53,			FCN_NULL},
	{"r86",		(db_expr_t*)54,			FCN_NULL},
	{"r87",		(db_expr_t*)55,			FCN_NULL},
	{"r88",		(db_expr_t*)56,			FCN_NULL},
	{"r89",		(db_expr_t*)57,			FCN_NULL},
	{"r90",		(db_expr_t*)58,			FCN_NULL},
	{"r91",		(db_expr_t*)59,			FCN_NULL},
	{"r92",		(db_expr_t*)60,			FCN_NULL},
	{"r93",		(db_expr_t*)61,			FCN_NULL},
	{"r94",		(db_expr_t*)62,			FCN_NULL},
	{"r95",		(db_expr_t*)63,			FCN_NULL},
	{"r96",		(db_expr_t*)64,			FCN_NULL},
	{"r97",		(db_expr_t*)65,			FCN_NULL},
	{"r98",		(db_expr_t*)66,			FCN_NULL},
	{"r99",		(db_expr_t*)67,			FCN_NULL},
	{"r100",	(db_expr_t*)68,			FCN_NULL},
	{"r101",	(db_expr_t*)69,			FCN_NULL},
	{"r102",	(db_expr_t*)70,			FCN_NULL},
	{"r103",	(db_expr_t*)71,			FCN_NULL},
	{"r104",	(db_expr_t*)72,			FCN_NULL},
	{"r105",	(db_expr_t*)73,			FCN_NULL},
	{"r106",	(db_expr_t*)74,			FCN_NULL},
	{"r107",	(db_expr_t*)75,			FCN_NULL},
	{"r108",	(db_expr_t*)76,			FCN_NULL},
	{"r109",	(db_expr_t*)77,			FCN_NULL},
	{"r110",	(db_expr_t*)78,			FCN_NULL},
	{"r111",	(db_expr_t*)79,			FCN_NULL},
	{"r112",	(db_expr_t*)80,			FCN_NULL},
	{"r113",	(db_expr_t*)81,			FCN_NULL},
	{"r114",	(db_expr_t*)82,			FCN_NULL},
	{"r115",	(db_expr_t*)83,			FCN_NULL},
	{"r116",	(db_expr_t*)84,			FCN_NULL},
	{"r117",	(db_expr_t*)85,			FCN_NULL},
	{"r118",	(db_expr_t*)86,			FCN_NULL},
	{"r119",	(db_expr_t*)87,			FCN_NULL},
	{"r120",	(db_expr_t*)88,			FCN_NULL},
	{"r121",	(db_expr_t*)89,			FCN_NULL},
	{"r122",	(db_expr_t*)90,			FCN_NULL},
	{"r123",	(db_expr_t*)91,			FCN_NULL},
	{"r124",	(db_expr_t*)92,			FCN_NULL},
	{"r125",	(db_expr_t*)93,			FCN_NULL},
	{"r126",	(db_expr_t*)94,			FCN_NULL},
	{"r127",	(db_expr_t*)95,			FCN_NULL},
};
struct db_variable *db_eregs = db_regs + sizeof(db_regs)/sizeof(db_regs[0]);

void
Debugger(void)
{
}

/*
 * Write bytes to kernel address space for debugger.
 */
void
db_write_bytes(vaddr_t addr, size_t size, char *data)
{
}

/*
 * Read bytes from kernel address space for debugger.
 */
void
db_read_bytes(vaddr_t addr, size_t size, char *data)
{
}
