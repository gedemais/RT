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
