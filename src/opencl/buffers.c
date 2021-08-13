#include "main.h"

int		create_cl_buffers(t_rt_env *env)
{
	// Buffers settings arrays
	size_t			buffers_sizes[CL_BUFF_MAX] = {};
	cl_mem_flags	buffers_flags[CL_BUFF_MAX] = {};
	t_opencl		*cl;
	t_camera		*cam;

	cl = (t_opencl*)&env->cl_env;
	cam = (t_camera*)&env->scene.cam;

	// Image buffer
	buffers_sizes[CL_BUFF_IMAGE] = cam->img_wdt * cam->img_hgt * 4;
	buffers_flags[CL_BUFF_IMAGE] = CL_MEM_WRITE_ONLY;

	// Objects buffer
	buffers_sizes[CL_BUFF_OBJECTS] = sizeof(t_object) * MAX_OBJECTS;
	buffers_flags[CL_BUFF_OBJECTS] = CL_MEM_READ_ONLY;

	// Lights buffer
	buffers_sizes[CL_BUFF_LIGHTS] = sizeof(t_light) * MAX_LIGHTS;
	buffers_flags[CL_BUFF_LIGHTS] = CL_MEM_READ_ONLY;

	// Materials buffer
	buffers_sizes[CL_BUFF_LIGHTS] = sizeof(t_material) * MAX_MTLS;
	buffers_flags[CL_BUFF_LIGHTS] = CL_MEM_READ_ONLY;

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
