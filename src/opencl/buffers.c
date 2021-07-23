#include "main.h"

int		create_cl_buffers(t_rt_env *env)
{
	// Buffers settings arrays
	size_t			buffers_sizes[CL_BUFF_MAX] = {};
	cl_mem_flags	buffers_flags[CL_BUFF_MAX] = {};
	// Convenience pointers
	t_opencl	*cl;
	t_mlx		*mlx;

	cl = (t_opencl*)&env->cl_env;
	mlx = (t_mlx*)&env->mlx;

	// Image buffer
	buffers_sizes[CL_BUFF_IMAGE] = mlx->width * mlx->height * 4;
	buffers_flags[CL_BUFF_IMAGE] = CL_MEM_WRITE_ONLY;

	// Buffers allocations
	for (unsigned int i = 0; i < CL_BUFF_MAX; i++)
	{
		cl->buffers[i] = clCreateBuffer(cl->context,
										buffers_flags[i],
										buffers_sizes[i],
										NULL, NULL);
	}
	return (0);
}
