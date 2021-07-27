#include "main.h"

static int	get_arg_value(t_var var, t_object *obj, char *split)
{
	int	ret = 0;

	if (var.type == DT_VECTOR_P)
	{
		ret = parse_vector(split, "()", &obj->color);
		if (ret == 0 && !is_vec_in(obj->color, 0.0f, 1.0f))
			return (ERROR_COLOR_VALUE_OUT_OF_RANGE);
	}
	else if (var.type == DT_VECTOR_B)
		ret = parse_vector(split, "[]", &obj->sphere.origin);
	else // DT_FLOAT
	{
		obj->sphere.radius = ft_atof(&split[ft_strlen(var.name)]);
		if (!is_float_in(obj->sphere.radius, 0.0f, INFINITY))
			return (ERROR_NEGATIVE_RADIUS_VALUE);
	}
	return (ret);
}

int			add_sphere(t_rt_env *env, char **split)
{
	const t_var	vars[NARG_SPHERE] ={
									{"color=", DT_VECTOR_P},
									{"position=", DT_VECTOR_B},
									{"radius=", DT_FLOAT}
									};
	t_object	new;
	bool		found;
	int			ret;

	new.type = TYPE_SPHERE;
	for (unsigned int i = 0; i < NARG_SPHERE; i++)
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
	
	return (add_object_to_scene(env, &new));
}

