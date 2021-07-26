#include "main.h"

int	rt_setup(t_rt_env *env, int argc, char **argv)
{
	int			ret;

	if (argc != 2)
		return (ERROR_NO_SCENE);

	if ((ret = parse_scene(env, argv[1]))
		|| (ret = init_opencl(env))
		|| (ret = init_mlx(env)))
		return (ret);

	t_object object;

	object.type = TYPE_SPHERE;
	object.color = (cl_float3){{0.0f, 0.0f, 1.0f}};
	object.sphere.origin = (cl_float3){{0.0f, 0.0f, -3.0f}};
	object.sphere.radius = 1.0f;

	return (0);
}
