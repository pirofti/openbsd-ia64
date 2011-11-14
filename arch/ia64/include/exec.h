/*	$OpenBSD: exec.h,v 1.1 2011/07/04 23:29:08 pirofti Exp $	*/

/*
 * Written by Paul Irofti <pirofti@openbsd.org>. Public Domain.
 */

#ifndef _IA64_EXEC_H_
#define _IA64_EXEC_H_

#define NATIVE_EXEC_ELF

#define ARCH_ELFSIZE 64

#define _NLIST_DO_ELF
#define _KERN_DO_ELF64

#define ELF_TARG_CLASS		ELFCLASS64
#define ELF_TARG_DATA		ELFDATA2LSB
#define ELF_TARG_MACH		EM_IA_64

#define  __LDPGSZ	4096

/*	Name			Value	   Field	Calculation */
#define	R_IA_64_NONE		0	/* None */
#define	R_IA_64_IMM14		0x21	/* immediate14	S + A */
#define	R_IA_64_IMM22		0x22	/* immediate22	S + A */
#define	R_IA_64_IMM64		0x23	/* immediate64	S + A */
#define	R_IA_64_DIR32MSB	0x24	/* word32 MSB	S + A */
#define	R_IA_64_DIR32LSB	0x25	/* word32 LSB	S + A */
#define	R_IA_64_DIR64MSB	0x26	/* word64 MSB	S + A */
#define	R_IA_64_DIR64LSB	0x27	/* word64 LSB	S + A */
#define	R_IA_64_GPREL22		0x2a	/* immediate22	@gprel(S + A) */
#define	R_IA_64_GPREL64I	0x2b	/* immediate64	@gprel(S + A) */
#define	R_IA_64_GPREL32MSB	0x2c	/* word32 MSB	@gprel(S + A) */
#define	R_IA_64_GPREL32LSB	0x2d	/* word32 LSB	@gprel(S + A) */
#define	R_IA_64_GPREL64MSB	0x2e	/* word64 MSB	@gprel(S + A) */
#define	R_IA_64_GPREL64LSB	0x2f	/* word64 LSB	@gprel(S + A) */
#define	R_IA_64_LTOFF22		0x32	/* immediate22	@ltoff(S + A) */
#define	R_IA_64_LTOFF64I	0x33	/* immediate64	@ltoff(S + A) */
#define	R_IA_64_PLTOFF22	0x3a	/* immediate22	@pltoff(S + A) */
#define	R_IA_64_PLTOFF64I	0x3b	/* immediate64	@pltoff(S + A) */
#define	R_IA_64_PLTOFF64MSB	0x3e	/* word64 MSB	@pltoff(S + A) */
#define	R_IA_64_PLTOFF64LSB	0x3f	/* word64 LSB	@pltoff(S + A) */
#define	R_IA_64_FPTR64I		0x43	/* immediate64	@fptr(S + A) */
#define	R_IA_64_FPTR32MSB	0x44	/* word32 MSB	@fptr(S + A) */
#define	R_IA_64_FPTR32LSB	0x45	/* word32 LSB	@fptr(S + A) */
#define	R_IA_64_FPTR64MSB	0x46	/* word64 MSB	@fptr(S + A) */
#define	R_IA_64_FPTR64LSB	0x47	/* word64 LSB	@fptr(S + A) */
#define	R_IA_64_PCREL60B	0x48	/* immediate60 form1 S + A - P */
#define	R_IA_64_PCREL21B	0x49	/* immediate21 form1 S + A - P */
#define	R_IA_64_PCREL21M	0x4a	/* immediate21 form2 S + A - P */
#define	R_IA_64_PCREL21F	0x4b	/* immediate21 form3 S + A - P */
#define	R_IA_64_PCREL32MSB	0x4c	/* word32 MSB	S + A - P */
#define	R_IA_64_PCREL32LSB	0x4d	/* word32 LSB	S + A - P */
#define	R_IA_64_PCREL64MSB	0x4e	/* word64 MSB	S + A - P */
#define	R_IA_64_PCREL64LSB	0x4f	/* word64 LSB	S + A - P */
#define	R_IA_64_LTOFF_FPTR22	0x52	/* immediate22	@ltoff(@fptr(S + A)) */
#define	R_IA_64_LTOFF_FPTR64I	0x53	/* immediate64	@ltoff(@fptr(S + A)) */
#define	R_IA_64_LTOFF_FPTR32MSB	0x54	/* word32 MSB	@ltoff(@fptr(S + A)) */
#define	R_IA_64_LTOFF_FPTR32LSB	0x55	/* word32 LSB	@ltoff(@fptr(S + A)) */
#define	R_IA_64_LTOFF_FPTR64MSB	0x56	/* word64 MSB	@ltoff(@fptr(S + A)) */
#define	R_IA_64_LTOFF_FPTR64LSB	0x57	/* word64 LSB	@ltoff(@fptr(S + A)) */
#define	R_IA_64_SEGREL32MSB	0x5c	/* word32 MSB	@segrel(S + A) */
#define	R_IA_64_SEGREL32LSB	0x5d	/* word32 LSB	@segrel(S + A) */
#define	R_IA_64_SEGREL64MSB	0x5e	/* word64 MSB	@segrel(S + A) */
#define	R_IA_64_SEGREL64LSB	0x5f	/* word64 LSB	@segrel(S + A) */
#define	R_IA_64_SECREL32MSB	0x64	/* word32 MSB	@secrel(S + A) */
#define	R_IA_64_SECREL32LSB	0x65	/* word32 LSB	@secrel(S + A) */
#define	R_IA_64_SECREL64MSB	0x66	/* word64 MSB	@secrel(S + A) */
#define	R_IA_64_SECREL64LSB	0x67	/* word64 LSB	@secrel(S + A) */
#define	R_IA_64_REL32MSB	0x6c	/* word32 MSB	BD + A */
#define	R_IA_64_REL32LSB	0x6d	/* word32 LSB	BD + A */
#define	R_IA_64_REL64MSB	0x6e	/* word64 MSB	BD + A */
#define	R_IA_64_REL64LSB	0x6f	/* word64 LSB	BD + A */
#define	R_IA_64_LTV32MSB	0x74	/* word32 MSB	S + A */
#define	R_IA_64_LTV32LSB	0x75	/* word32 LSB	S + A */
#define	R_IA_64_LTV64MSB	0x76	/* word64 MSB	S + A */
#define	R_IA_64_LTV64LSB	0x77	/* word64 LSB	S + A */
#define	R_IA_64_PCREL21BI	0x79	/* immediate21 form1 S + A - P */
#define	R_IA_64_PCREL22		0x7a	/* immediate22	S + A - P */
#define	R_IA_64_PCREL64I	0x7b	/* immediate64	S + A - P */
#define	R_IA_64_IPLTMSB		0x80	/* function descriptor MSB special */
#define	R_IA_64_IPLTLSB		0x81	/* function descriptor LSB speciaal */
#define	R_IA_64_SUB		0x85	/* immediate64	A - S */
#define	R_IA_64_LTOFF22X	0x86	/* immediate22	special */
#define	R_IA_64_LDXMOV		0x87	/* immediate22	special */
#define	R_IA_64_TPREL14		0x91	/* imm14	@tprel(S + A) */
#define	R_IA_64_TPREL22		0x92	/* imm22	@tprel(S + A) */
#define	R_IA_64_TPREL64I	0x93	/* imm64	@tprel(S + A) */
#define	R_IA_64_TPREL64MSB	0x96	/* word64 MSB	@tprel(S + A) */
#define	R_IA_64_TPREL64LSB	0x97	/* word64 LSB	@tprel(S + A) */
#define	R_IA_64_LTOFF_TPREL22	0x9a	/* imm22	@ltoff(@tprel(S+A)) */
#define	R_IA_64_DTPMOD64MSB	0xa6	/* word64 MSB	@dtpmod(S + A) */
#define	R_IA_64_DTPMOD64LSB	0xa7	/* word64 LSB	@dtpmod(S + A) */
#define	R_IA_64_LTOFF_DTPMOD22	0xaa	/* imm22	@ltoff(@dtpmod(S+A)) */
#define	R_IA_64_DTPREL14	0xb1	/* imm14	@dtprel(S + A) */
#define	R_IA_64_DTPREL22	0xb2	/* imm22	@dtprel(S + A) */
#define	R_IA_64_DTPREL64I	0xb3	/* imm64	@dtprel(S + A) */
#define	R_IA_64_DTPREL32MSB	0xb4	/* word32 MSB	@dtprel(S + A) */
#define	R_IA_64_DTPREL32LSB	0xb5	/* word32 LSB	@dtprel(S + A) */
#define	R_IA_64_DTPREL64MSB	0xb6	/* word64 MSB	@dtprel(S + A) */
#define	R_IA_64_DTPREL64LSB	0xb7	/* word64 LSB	@dtprel(S + A) */
#define	R_IA_64_LTOFF_DTPREL22	0xba	/* imm22	@ltoff(@dtprel(S+A)) */

#endif
