#include "main.h"

static int	get_arg_value(t_rt_env *env, t_var var, t_object *obj, char *split)
{
	cl_float3	tmp;
	int			ret = 0;

	if (var.type == DT_VECTOR_B)
	{
		if ((ret = parse_vector(split, "[]", &tmp)))
			return (ret);
		switch(var.name[1])
		{
			case('1'): obj->poly.v1 = tmp;
			case('2'): obj->poly.v2 = tmp;
			case('3'): obj->poly.v3 = tmp;
		}
	}
	else if (var.type == DT_STRING)
		ret = find_material(&env->scene.mtls, obj, split);
	return (ret);
}

int			add_polygon(t_rt_env *env, char **split)
{
	const t_var	vars[NARG_POLYGON] ={
									{"material=", DT_STRING},
									{"v1=", DT_VECTOR_B},
									{"v2=", DT_VECTOR_B},
									{"v3=", DT_VECTOR_B},
									};
	t_object	new;
	bool		found;
	int			ret;

	new.type = TYPE_POLYGON;
	ft_bzero(&new, sizeof(t_object));
	for (unsigned int i = 0; i < NARG_POLYGON; i++)
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
