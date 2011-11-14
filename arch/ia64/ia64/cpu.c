#include <sys/param.h>
#include <sys/proc.h>
#include <sys/systm.h>
#include <sys/device.h>

#include <machine/cpu.h>
#include <machine/cpuvar.h>
#include <machine/vmparam.h>
#include <machine/pcb.h>


int	cpu_match(struct device *, void *, void *);
void	cpu_attach(struct device *, struct device *, void *);

struct cpu_softc {
	struct device sc_dev;
	struct cpu_info *sc_ci;		/* cpu info */
};

struct cfattach cpu_ca = {
	sizeof(struct cpu_softc), cpu_match, cpu_attach
};

struct cfdriver cpu_cd = {
	NULL, "cpu", DV_DULL
};

/*
 * Primary CPU intialization.
 *
 * XXX: Add MP list initialization and function helpers.
 */
struct cpu_info cpu_info_primary = { 0, &cpu_info_primary };

struct cpu_info *cpu_info_list = &cpu_info_primary;

int
cpu_match(struct device *parent, void *match, void *aux)
{
	struct cfdata *cf = match;
	struct cpu_attach_args *caa = aux;

	if (strcmp(caa->caa_name, cf->cf_driver->cd_name) != 0)
		return 0;

	if (cf->cf_unit >= MAXCPUS)
		return 0;

	return 1;
}

/* XXX: Add application CPU support for MP */
void
cpu_attach(struct device *parent, struct device *self, void *aux)
{
	struct cpu_softc *sc = (void *) self;
	struct cpu_info *ci;

	ci = &cpu_info_primary;
	ci->ci_self = ci;
	ci->ci_cpuid = 0;	/* XXX Add MP counting */
	sc->sc_ci = ci;

	printf(": (uniprocessor)\n");

	ci->ci_flags |= CPUF_PRESENT | CPUF_PRIMARY;
}
