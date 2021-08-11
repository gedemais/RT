#include "main.h"

// Tools

bool		is_vec_in(cl_float3 v, float min, float max)
{
	return (v.x >= min && v.x <= max
			&& v.y >= min && v.y <= max
			&& v.z >= min && v.z <= max);
}

bool		is_float_in(float f, float min, float max)
{
	return (f > min && f <= max);
}


int			add_object_to_scene(t_rt_env *env, t_object *obj)
{
	if (env->scene.objects.byte_size == 0
		&& init_dynarray(&env->scene.objects, sizeof(t_object), MAX_OBJECTS))
		return (ERROR_MALLOC_FAILED);

	if (push_dynarray(&env->scene.objects, obj, false))
		return (ERROR_MALLOC_FAILED);

	return (0);
}

int			cmd_addobj(t_rt_env *env, char *line)
{
	static int	(*types_add_fts[TYPE_MAX])(t_rt_env*, char**) = {
															add_sphere,
															add_polygon,
															add_cone
																};
	const char	*objects_types[TYPE_MAX] = {
											"sphere",
											"polygon",
											"cone",
											"plane",
											"cylinder"
											};
	char		**split;
	int			ret;

	if (!(split = ft_strsplit(line, " ")))
		return (ERROR_MALLOC_FAILED);

	if (ft_tablen(split) < 2)
	{
		ft_free_ctab(split);
		return (ERROR_NO_TYPE_FOR_ADDOBJ_CMD);
	}

	for (unsigned int i = 0; i < TYPE_MAX; i++)
	{
		if (ft_strcmp(objects_types[i], split[1]) == 0)
		{
			ret = types_add_fts[i](env, split);
			ft_free_ctab(split);
			return (ret);
		}
	}
	ft_free_ctab(split);
	return (ERROR_INVALID_TYPE_FOR_ADDOBJ_CMD);
}
