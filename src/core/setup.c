#include "main.h"

int	rt_setup(t_rt_env *env, int argc, char **argv)
{
	t_camera	*cam;
	int			ret;

	(void)argc;
	(void)argv;
	cam = &env->cl_env.cam;

	// tmp data fill in
	env->mlx.width = 1280;
	env->mlx.height = 720;

	cam->img_wdt = 1280;
	cam->img_hgt = 720;
	cam->o = (cl_float3){{0, 0, 0, 0}};
	cam->fov = 90.0;
	cam->fov_rad = 1.0f / tan(cam->fov * 0.5f / 180.0f * 3.14159f);
	cam->aspect_ratio = (float)cam->img_wdt / (float)cam->img_hgt;

	t_object	sphere;

	sphere.sphere.origin = (cl_float3){{0, 0, -1, 0}};
	sphere.sphere.radius = 1.0;
	sphere.color = 0xffffff;
	sphere.type = TYPE_SPHERE;
	init_dynarray(&env->objects, sizeof(t_object), 8);
	push_dynarray(&env->objects, &sphere, false);
	//

	if ((ret = init_mlx(env))
		|| (ret = init_opencl(env)))
		return (ret);
	return (0);
}
