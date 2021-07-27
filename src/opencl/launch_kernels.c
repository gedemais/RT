#include "main.h"
/*
static void	print_objects(t_rt_env *env)
{
	t_object	*obj;

	printf("%d\n", env->scene.objects.nb_cells);
	for (unsigned int i = 0; i < (unsigned)env->scene.objects.nb_cells; i++)
	{
		obj = dyacc(&env->scene.objects, i);
		printf("SPHERE : %f %f %f | color : %f %f %f | radius : %f\n", obj->sphere.origin.x, obj->sphere.origin.y, obj->sphere.origin.z, obj->color.x, obj->color.y, obj->color.z, obj->sphere.radius);
	}
}*/

int		launch_ray_caster_kernel(t_rt_env *env)
{
	t_opencl	*cl;
	t_camera	*cam;
	cl_int		errcode;

	cl = ((t_opencl*)&env->cl_env);
	cam = ((t_camera*)&env->scene.cam);

	//print_objects(env);
	// Update data in buffers and params
	cam->nb_objects = env->scene.objects.nb_cells;
	errcode = clEnqueueWriteBuffer(cl->queue, cl->buffers[CL_BUFF_OBJECTS],
									CL_TRUE, 0, env->scene.objects.nb_cells * sizeof(t_object),
									env->scene.objects.c, 0, NULL, NULL);

	// Set kernel function's arguments
	clSetKernelArg(cl->kernels[K_RAY_CASTER], 0, sizeof(cl_mem), (void*)&cl->buffers[CL_BUFF_IMAGE]);
	clSetKernelArg(cl->kernels[K_RAY_CASTER], 1, sizeof(cl_mem), (void*)&cl->buffers[CL_BUFF_OBJECTS]);
	clSetKernelArg(cl->kernels[K_RAY_CASTER], 2, sizeof(struct s_camera), (void*)cam);

	// Launch the kernel on the work-group
	clEnqueueNDRangeKernel(cl->queue, cl->kernels[K_RAY_CASTER], 2, NULL,
							(size_t[]){cam->img_wdt, cam->img_hgt}, NULL, 0, NULL, NULL);

	if (clEnqueueReadBuffer(cl->queue, cl->buffers[CL_BUFF_IMAGE],
							CL_TRUE, 0, cam->img_wdt * cam->img_hgt * 4,
							env->mlx.img_data, 0, NULL, NULL) != CL_SUCCESS)
	{
		ft_putstr_fd("Copy from GPU failed\n", 2);
		return (-1); // To add
	}

	return (0);
}
