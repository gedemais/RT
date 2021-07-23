#include "main.h"

int		launch_ray_caster_kernel(t_rt_env *env)
{
	t_opencl	*cl;

	cl = ((t_opencl*)&env->cl_env);

	clSetKernelArg(cl->kernels[K_RAY_CASTER], 0, sizeof(int*), (void*)env->mlx.img_data);

	clEnqueueNDRangeKernel(cl->queue, cl->kernels[K_RAY_CASTER], 2, NULL,
							(size_t[]){env->mlx.width, env->mlx.height}, NULL, 0, NULL, NULL);

	if (clEnqueueReadBuffer(cl->queue, cl->buffers[BUFFER_IMAGE], CL_TRUE, 0, WDT * HGT * 4,
			env->mlx.img_data, 0, NULL, NULL) != CL_SUCCESS)
	{
		ft_putstr_fd("Copy from GPU failed\n", 2);
		return ;
	}
}

	return (0);
}
