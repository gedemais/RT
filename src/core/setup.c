#include "main.h"

int	rt_setup(t_rt_env *env, int argc, char **argv)
{
	int		ret;

	(void)argc;
	(void)argv;

	// tmp data fill in
	env->mlx.width = 1280;
	env->mlx.height = 720;

	t_object	sphere;

	sphere.type = TYPE_SPHERE;
	sphere.sphere.origin = (t_vec3d){0, 0, 1};
	sphere.sphere.radius = 1.0;
	init_dynarray(&env->objects, sizeof(t_object), 8);
	push_dynarray(&env->objects, &sphere, false);
	//

	if ((ret = init_mlx(env))
		|| (ret = init_opencl(env)))
		return (ret);
	return (0);
}
