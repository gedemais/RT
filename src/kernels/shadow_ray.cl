
#define EPSILON 0.000001f

enum	e_object_type
{
	TYPE_POLYGON,
	TYPE_SPHERE,
	TYPE_PLANE,
	TYPE_CYLINDER,
	TYPE_CONE,
	TYPE_MAX
};

typedef struct	s_hitpoint
{
	float3	p;
	float3	n;
	int		obj_index;
}				t_hitpoint;

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

typedef struct	s_cone
{
	float3	color;
	float3	tip;
	float3	axis;
	float	cosa;
	float	height;
}				t_cone;

typedef struct	s_polygon
{
	float3 v0;
	float3 v1;
	float3 v2;
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
		t_cone		cone;
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

static float	ray_polygon_intersection(float3 ray_o, float3 ray_dir, t_polygon poly)
{
    const float3 E1 = poly.v1 - poly.v0;
    const float3 E2 = poly.v2 - poly.v0;
    const float3 T = ray_o - poly.v0;
    const float3 D = ray_dir;
    const float3 P = cross(D, E2);
    const float3 Q = cross(T, E1);

    const float	f = 1.0f / dot(P, E1);
    const float t = f * dot(Q, E2);
    const float u = f * dot(P, T);
    const float v = f * dot(Q, D);

    if (u > -EPSILON && v > -EPSILON && u + v < 1.0f + EPSILON)
		return (t);
    else
		return (-1.0f);
}

//---------------------------------------------------------------------

static float3	color_pixel(__global t_light *light, float3 color, float3 obj_color, float3 n, float3 shadow_ray_dir)
{
	float3	c;

	c = obj_color * (light->brightness * fmax(0.0f, fmin(1.0f, dot(n, shadow_ray_dir))));
	c *= light->color;

	return (color + c);
}

static float3	compute_shadow(__global t_object *objects, __global t_light *lights, __global t_hitpoint *hit, t_camera cam)
{
	float3	shadow_ray_dir;
	float3	color;
	bool	in_shadow;

	color = 0;
	if (hit->obj_index == -1)
		return ((float3){0, 0, 0});
	for (unsigned int i = 0; i < cam.nb_lights; i++)
	{
		in_shadow = false;
		shadow_ray_dir = normalize(lights[i].origin - hit->p);
		for (unsigned int j = 0; j < cam.nb_objects; j++)
		{
			if ((objects[j].type == TYPE_SPHERE && ray_sphere_intersection(hit->p, shadow_ray_dir, objects[j].sphere) > 0)
				|| (objects[j].type == TYPE_POLYGON && ray_polygon_intersection(hit->p, shadow_ray_dir, objects[j].poly) > 0))
			{
				in_shadow = true;
				break;
			}
		}
		if (!in_shadow)
			color = color_pixel(&lights[i], color, objects[hit->obj_index].color, hit->n, shadow_ray_dir);
	}
	color = (color + (cam.brightness * objects[hit->obj_index].color)) * cam.ambiant_color;
	return (color);
}

static int		color_to_int(const float3 color)
{
	int				ret = 0;
	unsigned char	*ptr = (unsigned char*)(&ret);

	// RGB, not BGR
	ptr[2] = (unsigned char)((color.x >= 1.0 ? 255 : (color.x <= 0.0 ? 0 : (int)floor(color.x * 256.0f))));
	ptr[1] = (unsigned char)((color.y >= 1.0 ? 255 : (color.y <= 0.0 ? 0 : (int)floor(color.y * 256.0f))));
	ptr[0] = (unsigned char)((color.z >= 1.0 ? 255 : (color.z <= 0.0 ? 0 : (int)floor(color.z * 256.0f))));
	return (ret);
}

__kernel void	shadow_ray(__global int		*img,
						   __global t_object *objects,
							__global t_light *lights,
							__global t_hitpoint *hits,
							t_camera cam)
{
	const unsigned short	x = get_global_id(0);
	const unsigned short	y = get_global_id(1);
	const unsigned int		pos = y * cam.img_wdt + x;

	const float3			color = compute_shadow(objects, lights, &hits[pos], cam);

	img[pos] = color_to_int(color);
}
