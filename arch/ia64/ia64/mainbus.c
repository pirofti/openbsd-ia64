#include <sys/param.h>
#include <sys/systm.h>
#include <sys/device.h>

#include <machine/cpuvar.h>

int	mainbus_match(struct device *, void *, void *);
void	mainbus_attach(struct device *, struct device *, void *);

int	mainbus_print(void *, const char *);

struct cfattach mainbus_ca = {
	sizeof(struct device), mainbus_match, mainbus_attach, NULL,
	config_activate_children
};

struct cfdriver mainbus_cd = {
	NULL, "mainbus", DV_DULL
};

int
mainbus_match(struct device *parent, void *match, void *aux)
{
	return (1);
}

void
mainbus_attach(struct device *parent, struct device *self, void *aux)
{
	printf("\n");

	struct cpu_attach_args caa;

	memset(&caa, 0, sizeof(caa));
	caa.caa_name = "cpu";
	caa.cpu_number = 0;
	config_found(self, &caa, mainbus_print);
}

int
mainbus_print(void *aux, const char *pnp)
{
	return pnp != NULL ? QUIET : UNCONF;
}
