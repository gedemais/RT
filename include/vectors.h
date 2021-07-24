#ifndef VECTORS_H
# define VECTORS_H

# include <OpenCL/CL.h>

struct	s_point2d
{
	unsigned int	x;
	unsigned int	y;
};

struct	s_vec2d
{
	cl_float	x;
	cl_float	y;
};

struct	s_point3d
{
	unsigned int	x;
	unsigned int	y;
	unsigned int	z;
};

struct	s_vec3d
{
	cl_float	x;
	cl_float	y;
	cl_float	z;
};

#endif
