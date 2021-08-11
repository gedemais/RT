#include "main.h"

static int	get_arg_value(t_rt_env *env, t_var var, t_object *obj, char *split)
{
	cl_float3	tmp;
	cl_float	t;
	int			ret = 0;

	if (var.type == DT_VECTOR_B)
	{
		ret = parse_vector(split, "[]", &tmp);
		if (ft_strcmp(var.name, "tip=") == 0)
			obj->cone.tip = tmp;
		else
			obj->cone.axis = tmp;
	}
	else if (var.type == DT_STRING)
		ret = find_material(&env->scene.mtls, obj, split);
	else // DT_FLOAT
	{
		t = ft_atof(&split[ft_strlen(var.name)]);
		if (ft_strcmp(var.name, "radius=") == 0)
			obj->cone.radius = t;
		else
		{
			if (!is_float_in(t, 0.0f, INFINITY))
				return (ERROR_INVALID_CONE_HEIGHT_VALUE);
			obj->cone.height = t;
		}
	}
	return (ret);
}

int			add_cone(t_rt_env *env, char **split)
{
	const t_var	vars[NARG_CONE] ={
									{"material=", DT_STRING},
									{"radius=", DT_FLOAT},
									{"height=", DT_FLOAT},
									{"tip=", DT_VECTOR_B},
									{"axis=", DT_VECTOR_B}
								};
	t_object	new;
	bool		found;
	int			ret;

	ft_bzero(&new, sizeof(t_object));
	new.type = TYPE_CONE;
	for (unsigned int i = 0; i < NARG_CONE; i++)
	{
		found = false;
		for (unsigned int j = 0; split[j]; j++)
		{
			if (ft_strncmp(vars[i].name, split[j], ft_strlen(vars[i].name)) == 0)
			{
				found = true;
				if ((ret = get_arg_value(env, vars[i], &new, split[j])))
					return (ret);
			}
		}
		if (!found)
			return (ERROR_ARG_NOT_FOUND);
	}
	return (add_object_to_scene(env, &new));
}
