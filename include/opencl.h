#ifndef OPENCL_H
# define OPENCL_H

# include <OpenCL/cl.h>

enum	e_kernels
{
	K_RAY_CASTER,
	K_MAX
};

enum	e_buffers
{
	CL_BUFF_IMAGE,
	CL_BUFF_MAX
};

struct	s_opencl
{
	cl_command_queue	queue;
	cl_device_id		device;
	cl_context			context;
	cl_program			programs[K_MAX];
	cl_kernel			kernels[K_MAX];
	cl_mem				buffers[CL_BUFF_MAX];
};

const char	*clGetErrorString(cl_int error);

int		init_opencl(t_rt_env *env);
int		create_cl_buffers(t_rt_env *env);
int		launch_ray_caster_kernel(t_rt_env *env);

# endif
