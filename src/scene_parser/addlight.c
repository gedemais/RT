#include "main.h"

static int	get_arg_value(t_var var, t_light *light, char *split)
{
	int	ret = 0;

	if (var.type == DT_VECTOR_P)
	{
		ret = parse_vector(split, "()", &light->color);
		if (ret == 0 && !is_vec_in(light->color, 0.0f, 1.0f))
			return (ERROR_COLOR_VALUE_OUT_OF_RANGE);
	}
	else if (var.type == DT_VECTOR_B)
		ret = parse_vector(split, "[]", &light->origin);
	else // DT_FLOAT
	{
		light->brightness = atof(&split[ft_strlen(var.name)]);
		if (!is_float_in(light->brightness, 0.0f, INFINITY))
			return (ERROR_INVALID_BRIGHTNESS);
	}
	return (ret);
}

static int	add_light_to_scene(t_rt_env *env, t_light *new)
{
	if (env->scene.lights.byte_size == 0
		&& init_dynarray(&env->scene.lights, sizeof(t_light), MAX_LIGHTS))
		return (ERROR_MALLOC_FAILED);

	if (push_dynarray(&env->scene.lights, new, false))
		return (ERROR_MALLOC_FAILED);

	return (0);
}

int			cmd_addlight(t_rt_env *env, char *line)
{
	const t_var	vars[NARG_LIGHT] ={
									{"color=", DT_VECTOR_P},
									{"position=", DT_VECTOR_B},
									{"brightness=", DT_FLOAT}
									};
	t_light		new;
	char		**split;
	int			ret;
	bool		found;

	if (!(split = ft_strsplit(line, " ")))
		return (ERROR_MALLOC_FAILED);

	if (ft_tablen(split) < 2)
	{
		ft_free_ctab(split);
		return (ERROR_NO_TYPE_FOR_ADDOBJ_CMD);
	}

	for (unsigned int i = 0; i < NARG_LIGHT; i++)
	{
		found = false;
		for (unsigned int j = 0; split[j]; j++)
		{
			if (ft_strncmp(vars[i].name, split[j], ft_strlen(vars[i].name)) == 0)
			{
				found = true;
				if ((ret = get_arg_value(vars[i], &new, split[j])))
					return (ret);
			}
		}
		if (!found)
			return (ERROR_ARG_NOT_FOUND);
	}
	return (add_light_to_scene(env, &new));
}
