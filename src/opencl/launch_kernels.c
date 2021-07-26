#include "main.h"

static void	print_objects(t_rt_env *env)
{
	t_object	*obj;

	printf("%d\n", env->objects.nb_cells);
	for (unsigned int i = 0; i < (unsigned)env->objects.nb_cells; i++)
	{
		obj = dyacc(&env->objects, i);
		printf("SPHERE : %f %f %f | radius : %f | color : %x\n", obj->sphere.origin.x, obj->sphere.origin.y, obj->sphere.origin.z, obj->sphere.radius, obj->color);
	}
}

int		launch_ray_caster_kernel(t_rt_env *env)
{
	t_opencl	*cl;
	t_mlx		*mlx;
	cl_int		errcode;

	cl = ((t_opencl*)&env->cl_env);
	mlx = (t_mlx*)&env->mlx;

	print_objects(env);
	// Update data in buffers and params
	cl->cam.nb_objects = env->objects.nb_cells;
	errcode = clEnqueueWriteBuffer(cl->queue, cl->buffers[CL_BUFF_OBJECTS],
									CL_TRUE, 0, env->objects.nb_cells * sizeof(t_object),
									env->objects.c, 0, NULL, NULL);

	// Set kernel function's arguments
	clSetKernelArg(cl->kernels[K_RAY_CASTER], 0, sizeof(cl_mem), (void*)&cl->buffers[CL_BUFF_IMAGE]);
	clSetKernelArg(cl->kernels[K_RAY_CASTER], 1, sizeof(cl_mem), (void*)&cl->buffers[CL_BUFF_OBJECTS]);
	clSetKernelArg(cl->kernels[K_RAY_CASTER], 2, sizeof(struct s_camera), (void*)&cl->cam);

	// Launch the kernel on the work-group
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
