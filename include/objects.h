#ifndef OBJECTS_H
# define OBJECTS_H

# define MAX_OBJECTS 4096

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
	t_vec3d		origin; // Center point of the sphere
	cl_float	radius; // Radius of the sphere
};

struct	s_object
{
	cl_int	color; // Color of the object
	union
	{
		// t_polygon poly;
		t_sphere	sphere;
		// t_plane	plane;
		// t_cylinder cylinder;
		// t_cone	cone;
		// t_;
	};
	cl_int	type;
};

#endif
