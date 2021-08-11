#include "main.h"

unsigned int	find_material(t_dynarray *mtls, t_object *obj, char *name)
{
	t_material	*m;

	for (unsigned int i = 0; (int)i < mtls->nb_cells; i++)
	{
		m = dyacc(mtls, i);
		if (ft_strcmp(m->name, &name[9]) == 0)
		{
			obj->mtl = i;
			return (0);
		}
	}
	return (ERROR_MATERIAL_NOT_FOUND);
}

static int	get_arg_value(t_rt_env *env, t_var var, t_object *obj, char *split)
{
	int	ret = 0;

	if (var.type == DT_VECTOR_B)
		ret = parse_vector(split, "[]", &obj->sphere.origin);
	else if (var.type == DT_STRING)
		ret = find_material(&env->scene.mtls, obj, split);
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
	const t_var	vars[NARG_SPHERE] =	{
									{"material=", DT_STRING},
									{"position=", DT_VECTOR_B},
									{"radius=", DT_FLOAT}
									};
	t_object	new;
	bool		found;
	int			ret;

	new.type = TYPE_SPHERE;
	ft_bzero(&new, sizeof(t_object));
	for (unsigned int i = 0; i < NARG_SPHERE; i++)
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

