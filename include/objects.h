#ifndef OBJECTS_H
# define OBJECTS_H

# define MAX_OBJECTS 4096
# define MAX_LIGHTS 128

# include <OpenCL/cl.h>
# include <stdbool.h>

enum	e_data_type
{
	DT_VECTOR_P, // Parenthesis
	DT_VECTOR_B, // Brackets
	DT_FLOAT,
	DT_INT,
	DT_MAX
};

enum	e_nb_args
{
	NARG_SPHERE = 3,
	NARG_LIGHT = 3
};

typedef struct	s_var
{
	char	name[63];
	char	type;
}				t_var;
enum	e_object_type
{
	TYPE_POLYGON,
	TYPE_SPHERE,
	TYPE_PLANE,
	TYPE_CYLINDER,
	TYPE_CONE,
	TYPE_MAX
};

/*
struct	s_polygon
{
	
};

struct	s_plane
{
	
};

struct	s_cylinder
{
	
};

struct	s_cone
{
	
};*/

struct	s_sphere
{
	cl_float	radius; // Radius of the sphere
};

struct	s_object
{
	union
	{
		// t_polygon poly;
		t_sphere	sphere;
		// t_plane	plane;
		// t_cylinder cylinder;
		// t_cone	cone;
		// t_;
	};
	cl_float3	origin; // Center point of the sphere
	cl_float3	color; // Color of the object
	cl_int		type;
};

struct	s_light
{
	cl_float3	origin;
	cl_float3	color;
	cl_float	brightness;
};

bool		is_vec_in(cl_float3 v, float min, float max);
bool		is_float_in(float f, float min, float max);

#endif
