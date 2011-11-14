struct cpu_attach_args {
	const char *caa_name;
	int cpu_number;
	int cpu_role;
	struct cpu_functions *cpu_func;
};
