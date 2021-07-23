
typedef struct	s_vec3d
{
	float	x;
	float	y;
	float	z;
}				t_vec3d;

typedef struct	s_sphere
{
	t_vec3d	origin; // Center point of the sphere
	float	radius; // Radius of the sphere
}				t_sphere;

typedef struct	s_object
{
	union
	{
		t_sphere	sphere;
	};
	char	type;
}				t_object;

__kernel void	ray_caster(__global int *img, __global t_object *objects)
{
	const unsigned short x = get_global_id(0);
	const unsigned short y = get_global_id(1);

	img[y * 1280 + x] = 0xffffff;
}
