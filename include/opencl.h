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
	BUFF_IMAGE,
	BUFF_MAX
};

struct	s_opencl
{
	cl_command_queue	queue;
	cl_device_id		device;
	cl_context			context;
	cl_program			programs[K_MAX];
	cl_kernel			kernels[K_MAX];
	cl_mem				buffers[BUFF_MAX];
};

# endif
