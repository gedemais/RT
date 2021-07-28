
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

typedef struct	s_light
{
	float3	origin;
	float3	color;
	float	brightness;
}				t_light;

typedef struct	s_camera
{
	float3			o;
	short			img_wdt;
	short			img_hgt;
	float			aspect_ratio;
	float			fov;
	float			brightness;
	unsigned int	nb_objects;
	unsigned int	nb_lights;
}				t_camera;

typedef struct	s_sphere
{
	float3	origin; // Center point of the sphere
	float	radius; // Radius of the sphere
}				t_sphere;

typedef struct	s_object
{
	union
	{
		t_sphere	sphere;
	};
	float3	color; // Color of the object
	int		type;
}				t_object;

//---------------------------------------------------------------------

static float	ray_sphere_intersection(float3 ray_o, float3 ray_dir, t_sphere sphere)
{
	float	a, b, c, discriminant;

	const float3 oc = ray_o - sphere.origin;

	a = dot(ray_dir, ray_dir);
	b = 2.0 * dot(oc, ray_dir);
	c = dot(oc, oc) - (sphere.radius * sphere.radius);

	discriminant = b * b - 4.0 * a * c;

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

	return (normalize((float3){px, py, -1} - cam.o));
}

static float3	shadow_ray(t_camera cam, __global t_object *objects, __global t_light *lights, __global t_object *hit_obj, float3 hitpoint)
{
	float3	shadow_ray_dir;
	float3	color;
	bool	in_shadow;

	color = 0;
	for (unsigned int i = 0; i < cam.nb_lights; i++)
	{
		in_shadow = false;
		shadow_ray_dir = lights[i].origin - hitpoint;
		for (unsigned int j = 0; j < cam.nb_objects; j++)
		{
			if (objects[j].type == TYPE_SPHERE && ray_sphere_intersection(hitpoint, shadow_ray_dir, objects[j].sphere) > 0)
			{
				in_shadow = true;
				break;
			}
		}
		if (!in_shadow)
			color += hit_obj->color * lights[i].brightness * (dot(shadow_ray_dir, hitpoint - hit_obj->sphere.origin));
			
	}
	return (color);
}

static float3	cast_ray(__global t_object *objects, __global t_light *lights, t_camera cam, const float3 ray_dir)
{
	__global t_object	*closest = NULL;
	float				min_dist = INFINITY;
	float				dist;

	for (unsigned int i = 0; i < cam.nb_objects; i++)
	{
		if (objects[i].type == TYPE_SPHERE)
		{
			dist = ray_sphere_intersection(cam.o, ray_dir, objects[i].sphere);
			if (dist > 0.0f && dist < min_dist)
			{
				closest = &objects[i];
				min_dist = dist;
			}
		}
	}
	if (closest != NULL) // Si pas de spots, utiliser brightness uniquement
		return (shadow_ray(cam, objects, lights, closest, ray_dir * (min_dist - 0.0001f)));
	return ((float3)(0, 0, 0));
}

static int		color_to_int(const float3 color, float brightness)
{
	int				ret = 0;
	unsigned char	*ptr = (unsigned char*)(&ret);

	if (brightness >= 1.0f)
		brightness = 1.0f;

	// RGB, not BGR
	ptr[2] = (unsigned char)(brightness * (color.x >= 1.0 ? 255 : (color.x <= 0.0 ? 0 : (int)floor(color.x * 256.0f))));
	ptr[1] = (unsigned char)(brightness * (color.y >= 1.0 ? 255 : (color.y <= 0.0 ? 0 : (int)floor(color.y * 256.0f))));
	ptr[0] = (unsigned char)(brightness * (color.z >= 1.0 ? 255 : (color.z <= 0.0 ? 0 : (int)floor(color.z * 256.0f))));
	return (ret);
}

__kernel void	ray_caster(__global int *img, __global t_object *objects, __global t_light *lights, t_camera cam)
{
	const unsigned short x = get_global_id(0);
	const unsigned short y = get_global_id(1);

	const float3 ray_dir = compute_ray_direction(cam, x, y);

	/*if (x == 0 && y == 0)
	{
		printf("nb_lights = %d\n", cam.nb_lights);
		printf("origin : %f %f %f | color : %f %f %f | brightness : %f\n", lights[0].origin.x, lights[0].origin.y, lights[0].origin.z, lights[0].color.x, lights[0].color.y, lights[0].color.z, lights[0].brightness);
	}*/

	const float3 color = cast_ray(objects, lights, cam, ray_dir);

	img[y * cam.img_wdt + x] = color_to_int(color, cam.brightness);
}
