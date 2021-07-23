#include "main.h"

int		launch_ray_caster_kernel(t_rt_env *env)
{
	t_opencl	*cl;
	t_mlx		*mlx;

	cl = ((t_opencl*)&env->cl_env);
	mlx = (t_mlx*)&env->mlx;

	clSetKernelArg(cl->kernels[K_RAY_CASTER], 0, sizeof(cl_mem), (void*)&cl->buffers[CL_BUFF_IMAGE]);

	clEnqueueNDRangeKernel(cl->queue, cl->kernels[K_RAY_CASTER], 2, NULL,
							(size_t[]){mlx->width, mlx->height}, NULL, 0, NULL, NULL);

	if (clEnqueueReadBuffer(cl->queue, cl->buffers[CL_BUFF_IMAGE],
							CL_TRUE, 0, mlx->width * mlx->height * 4,
							mlx->img_data, 0, NULL, NULL) != CL_SUCCESS)
	{
		ft_putstr_fd("Copy from GPU failed\n", 2);
		return (-1); // To add
	}
	return (0);
}
