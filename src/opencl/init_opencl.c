#include "main.h"

static void	print_log(t_opencl *cl, unsigned int index)
{
	char	log[65536];

	clGetProgramBuildInfo(cl->programs[index], cl->device, CL_PROGRAM_BUILD_LOG, 65536, log, NULL);
	printf("Compilation failed :\n%s\n", log);
}

static int	build_kernel(t_opencl *cl, char *code, unsigned int index)
{
	cl_int	errcode;

	cl->programs[index] = clCreateProgramWithSource(cl->context, 1,
													(const char**)&code, NULL, &errcode);

	if (errcode != CL_SUCCESS)
		return (ERROR_PROGRAM_CREATION_FAILED);

	if (clBuildProgram(cl->programs[index], 0, NULL, NULL, NULL, NULL) != CL_SUCCESS)
	{
		print_log(cl, index);
		return (ERROR_KERNEL_BUILD_FAILED);
	}

	return (0);
}

static int	map_kernels(t_opencl *cl)
{
	const char	*kernels_paths[K_MAX] = {
				[K_DEV] = "src/kernels/ray_caster.cl"
				};
	struct stat	info;
	int			fd;
	char		*code;
	int			err_code;

	for (unsigned int i = 0; i < K_MAX; i++)
	{
		if ((fd = open(kernels_paths[i], O_RDONLY)) == -1 || fstat(fd, &info) == -1)
			return (ERROR_KERNEL_MAPPING_FAILED);

		if ((code = mmap(0, (size_t)info.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
			return (ERROR_KERNEL_MAPPING_FAILED);

		if ((err_code = build_kernel(cl, code, i) != 0))
		{
			munmap(code, info.st_size);
			return (err_code);
		}

		munmap(code, info.st_size);
	}
	return (0);
}

static int	opencl_arch(t_rt_env *env)
{
	t_opencl	*cl;
	cl_int		errcode;

	cl = (t_opencl*)&env->cl_env;

	// Look for a OpenCL compatible GPU
	if (clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &cl->device, NULL) != CL_SUCCESS)
		return (-1); // To spec

	// Create a new execution context
	cl->context = clCreateContext(NULL, 1, &cl->device, NULL, NULL, &errcode);
	if (errcode != CL_SUCCESS)
		return (-1); // To spec

	// Associate it a command queue
	cl->queue = clCreateCommandQueue(cl->context, cl->device, 0, &errcode);
	if (errcode != CL_SUCCESS)
		return (-1); // To spec
	return (0);
}

int			init_opencl(t_rt_env *env)
{
	int		ret;

	if ((ret = opencl_arch(env))
		|| (ret = map_kernels(&env->cl_env)) != 0)
		return (ret);
	return (0);
}
