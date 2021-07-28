
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
	float3			ambiant_color;
	short			img_wdt;
	short			img_hgt;
	float			aspect_ratio;
	float			fov;
	float			brightness;
	unsigned int	nb_objects;
	unsigned int	nb_lights;
}				t_camera;

typedef struct	s_polygon
{
	float3 v1;
	float3 v2;
	float3 v3;
}				t_polygon;

typedef struct	s_sphere
{
	float	radius; // Radius of the sphere
	float3	origin; // Origin point of the object
}				t_sphere;

typedef struct	s_object
{
	union
	{
		t_polygon	poly;
		t_sphere	sphere;
	};
	float3	color; // Color of the object
	int		type;
}				t_object;

//---------------------------------------------------------------------

static float	ray_sphere_intersection(float3 ray_o, float3 ray_dir, t_object obj)
{
	float	a, b, c, discriminant;

	const float3 oc = ray_o - obj.sphere.origin;

	a = dot(ray_dir, ray_dir);
	b = 2.0 * dot(oc, ray_dir);
	c = dot(oc, oc) - (obj.sphere.radius * obj.sphere.radius);

	discriminant = b * b - 4.0 * a * c;

	if (discriminant < 0)
		return (-1.0);
	return (-b - sqrt(discriminant)) / (2.0 * a);
}

//---------------------------------------------------------------------

static float3	mix_colors(float3 a, float3 b)
{
	return (normalize(a * b));
}

static float3	color_pixel(__global t_light *light, float3 color, float3 obj_color, float3 n, float3 shadow_ray_dir)
{
	float3	c;

	c = obj_color * (light->brightness * fmax(0.0f, fmin(1.0f, dot(n, shadow_ray_dir))));
	c *= light->color;

	return (color + c);
}

static float3	compute_ray_direction(t_camera cam, const unsigned short x, const unsigned short y)
{
	float	px = 2.0 * (((float)x + 0.5) / (float)cam.img_wdt) - 1.0;
	float	py = 1.0 - 2.0 * (((float)y + 0.5) / (float)cam.img_hgt);

	px *= tan(cam.fov / 2.0f * 3.141f / 180.0f) * cam.aspect_ratio;
	py *= tan(cam.fov / 2.0f * 3.141f / 180.0f);

	return (normalize((float3){px, py, -1} - cam.o));
}

static float3	shadow_ray(t_camera cam, __global t_object *objects, __global t_light *lights, __global t_object *hit_obj, const float3 ray_dir, float3 p, float3 n)
{
	float3	shadow_ray_dir;
	float3	color;
	bool	in_shadow;

	color = 0;
	for (unsigned int i = 0; i < cam.nb_lights; i++)
	{
		in_shadow = false;
		shadow_ray_dir = normalize(lights[i].origin - p);
		for (unsigned int j = 0; j < cam.nb_objects; j++)
		{
			if (objects[j].type == TYPE_SPHERE && ray_sphere_intersection(p, shadow_ray_dir, objects[j]) > 0)
			{
				in_shadow = true;
				break;
			}
		}
		if (!in_shadow)
			color = color_pixel(&lights[i], color, hit_obj->color, n, shadow_ray_dir);
	}
	color = (color + (cam.brightness / 3.141f * hit_obj->color)) * cam.ambiant_color;
	return (color);
}

static float3	cast_ray(__global t_object *objects, __global t_light *lights, t_camera cam, const float3 ray_dir)
{
	__global t_object	*closest = NULL;
	float3				p;
	float3				n;
	float				min_dist = INFINITY;
	float				dist;

	for (unsigned int i = 0; i < cam.nb_objects; i++)
	{
		if (objects[i].type == TYPE_SPHERE)
			dist = ray_sphere_intersection(cam.o, ray_dir, objects[i]);
		if (dist > 0.0f && dist < min_dist)
		{
			closest = &objects[i];
			min_dist = dist;
		}
	}
	if (closest != NULL) // Si pas de spots, utiliser brightness uniquement
	{
		p = ray_dir * (min_dist - 0.001f);
		n = p - closest->sphere.origin;
		if (dot(n, ray_dir) > 0)
			n *= -1;

		return (shadow_ray(cam, objects, lights, closest, ray_dir, p, n));
	}
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
