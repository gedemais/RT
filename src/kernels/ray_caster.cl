
#define NULL (void*)0
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
	float	height;
	float	radius;
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

static float	ray_cone_intersection(float3 ray_o, float3 ray_dir, t_cone cone)
{
	const float3	theta = normalize(cone.axis - cone.tip);
	float3			w;
	float			m, d1, d2, a, b, c, d;
	float			t1, t2;

	m = pow(cone.radius, 2) / pow(cone.height, 2);
	w = ray_o - cone.tip;
	d1 = dot(ray_dir, theta);
	d2 = dot(w, theta);
	a = d1 * d1;
	a = dot(ray_dir, ray_dir) - m * a - a;
	b = 2.0f * (dot(ray_dir, w) - m * d1 * d2 - d1 * d2);
	c = dot(w, w) - m * pow(d2, 2) - pow(d2, 2);
	d = b * b - 4 * a * c;

	if (d < 0.0f)
		return (-1.0f);

	d = sqrt(d);
	t1 = (-b - d) / (2.0f * a);
	t2 = (-b + d) / (2.0f * b);

	if (t1 > 0.0f)
		return (t1);
	else if (t2 > 0.0f)
		return (t2);

	return (-1.0f);
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
			if ((objects[j].type == TYPE_SPHERE && ray_sphere_intersection(p, shadow_ray_dir, objects[j].sphere) > 0)
				|| (objects[j].type == TYPE_POLYGON && ray_polygon_intersection(p, shadow_ray_dir, objects[j].poly) > 0)
				|| (objects[j].type == TYPE_CONE && ray_cone_intersection(p, shadow_ray_dir, objects[j].cone) > 0))
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
			dist = ray_sphere_intersection(cam.o, ray_dir, objects[i].sphere);
		else if (objects[i].type == TYPE_POLYGON)
			dist = ray_polygon_intersection(cam.o, ray_dir, objects[i].poly);
		else if (objects[i].type == TYPE_CONE)
			dist = ray_cone_intersection(cam.o, ray_dir, objects[i].cone);

		if (dist > 0 && dist < min_dist)
		{
			closest = &objects[i];
			min_dist = dist;
		}
	}

	if (closest == NULL)
		return ((float3)(0.0, 0.0, 0.0));

	p = ray_dir * (min_dist - EPSILON);
	if (closest->type == TYPE_SPHERE)
		n = p - closest->sphere.origin;
	else if (closest->type == TYPE_POLYGON)
		n = cross(closest->poly.v1 - closest->poly.v0, closest->poly.v2 - closest->poly.v0);
	else if (closest->type == TYPE_CONE)
	{
		const float	r = sqrt((p.x - closest->cone.axis.x) * (p.x - closest->cone.axis.x) + (p.z - closest->cone.axis.z) * (p.z - closest->cone.axis.z));
		n = (float3)(p.x - closest->cone.axis.x, r * (closest->cone.radius / closest->cone.height), p.z - closest->cone.axis.z);
	}

	if (dot(n, ray_dir) > 0.0f)
		n *= -1;

	return (shadow_ray(cam, objects, lights, closest, ray_dir, p, n));
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
