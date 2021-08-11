#ifndef OBJECTS_H
# define OBJECTS_H

# define MAX_OBJECTS 4096
# define MAX_MTLS 256
# define MAX_LIGHTS 128

# include <OpenCL/cl.h>
# include <stdbool.h>

enum	e_data_type
{
	DT_VECTOR_P, // Parenthesis
	DT_VECTOR_B, // Brackets
	DT_STRING,
	DT_FLOAT,
	DT_INT,
	DT_MAX
};

enum	e_nb_args
{
	NARG_SPHERE = 2,
	NARG_POLYGON = 4,
	NARG_CONE = 5,
	NARG_LIGHT = 3,
	NARG_MTL = 5,
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
struct	s_plane
{
	
};

struct	s_cylinder
{
	
};
*/

struct	s_cone
{
	cl_float3	color;
	cl_float3	tip;
	cl_float3	axis;
	cl_float	height;
	cl_float	radius;
};

struct	s_polygon
{
	cl_float3 v1;
	cl_float3 v2;
	cl_float3 v3;
};

struct	s_sphere
{
	cl_float	radius; // Radius of the sphere
	cl_float3	origin; // Center point of the sphere
};

struct	s_material
{
	char		name[64];
	cl_float3	ka; // Ambiant color
	cl_float3	ks; // Specular color
	cl_float3	kd; // Diffuse color
	cl_float	ns; // Specular exponent
	cl_float	d; // Opacity
};

struct	s_object
{
	union
	{
		t_polygon	poly;
		t_sphere	sphere;
		t_cone		cone;
		// t_plane	plane;
		// t_cylinder cylinder;
		// t_;
	};
	t_material	mtl;
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
