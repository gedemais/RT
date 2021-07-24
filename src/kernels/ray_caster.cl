
#define NULL (void*)0

enum	e_object_type
{
	TYPE_POLYGON,
	TYPE_SPHERE,
	TYPE_PLANE,
	TYPE_CYLINDER,
	TYPE_CONE,
	TYPE_MAX
};

typedef struct	s_camera
{
	float3			o;
	short			img_wdt;
	short			img_hgt;
	float			aspect_ratio;
	float			fov;
	float			fov_rad;
	unsigned int	nb_objects;
}				t_camera;

typedef struct	s_sphere
{
	void	*ptr;
	float3	origin; // Center point of the sphere
	float	radius; // Radius of the sphere
}				t_sphere;

typedef struct	s_object
{
	int		color; // Color of the object
	union
	{
		t_sphere	sphere;
	};
	int		type;
}				t_object;

//---------------------------------------------------------------------

static float	ray_sphere_intersection(float3 ray_o, float3 ray_dir, t_sphere sphere)
{
	float	a, b, c, discriminant;

	float3 oc = ray_o - sphere.origin;
	a = dot(ray_dir, ray_dir);
	b = 2.0 * dot(oc, ray_dir);
	c = dot(oc, oc) - sphere.radius * sphere.radius;
	discriminant = b * b - 4 * a * c;

	if (discriminant < 0)
		return (-1.0);
	return (-b - sqrt(discriminant)) / (2.0 * a);
}

//---------------------------------------------------------------------

static float3 compute_ray_direction(t_camera cam, const unsigned short x, const unsigned short y)
{
	float	px = 2.0 * (((float)x + 0.5) / (float)cam.img_wdt) - 1.0;
	float	py = 1.0 - 2.0 * (((float)y + 0.5) / (float)cam.img_hgt);

	px *= tan(cam.fov / 2.0f * 3.141f / 180.0f) * cam.aspect_ratio;
	py *= tan(cam.fov / 2.0f * 3.141f / 180.0f);

	// float3 w_p = ((float)-cam.img_wdt / 2.0f) * px + ((float)cam.img_hgt / 2.0f) * py - ((float)cam.img_hgt / 2.0f) / tan(cam.fov_rad * 0.5f);

	// return (normalize((float)x * px + (float)y * (-py) + w_p));
	return (normalize((float3){px, py, -1} - cam.o));
}

static int		cast_ray(const short x, __global t_object *objects, t_camera cam, float3 ray_dir)
{
	__global t_object	*closest = NULL;
	float				min_dist = INFINITY;
	float				dist;

	for (unsigned int i = 0; i < cam.nb_objects; i++)
	{
		if (objects[i].type == TYPE_SPHERE)
		{
			dist = ray_sphere_intersection(cam.o, ray_dir, objects[i].sphere);
			if (dist > 0 && dist < min_dist)
			{
				closest = &objects[i];
				min_dist = dist;
			}
		}
	}
	if (closest != NULL)
		return (closest->color);
	return (0);
}

__kernel void	ray_caster(__global int *img, __global t_object *objects, t_camera cam)
{
	const unsigned short x = get_global_id(0);
	const unsigned short y = get_global_id(1);

	const float3 ray_dir = compute_ray_direction(cam, x, y);

	if (x == 0 && y == 0)
		printf("origin : %f %f %f | radius : %f | color : %X\n", objects[0].sphere.origin.x, objects[0].sphere.origin.y, objects[0].sphere.origin.z, objects[0].sphere.radius, objects[0].color);

	const int color = cast_ray(x, objects, cam, ray_dir);

	img[y * cam.img_wdt + x] = color;
}
