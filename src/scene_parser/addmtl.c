#include "main.h"

# define NB_P_VEC_IN_MTL 3
# define MAX_MTL_NAME_LENGTH 64

typedef struct	s_p_vector
{
	char		name[MAX_MTL_NAME_LENGTH];
	cl_float3	*vec;
}				t_p_vector;

static int	get_arg_value(t_var var, t_material *mtl, char *split)
{
	const t_p_vector	p_vectors[NB_P_VEC_IN_MTL] = {
									{.name="ka=", .vec=&mtl->ka},
									{.name="ks=", .vec=&mtl->ks},
									{.name="kd=", .vec=&mtl->kd}
														};
	int	ret = 0;

	if (var.type == DT_VECTOR_P)
		for (unsigned int i = 0; i < NB_P_VEC_IN_MTL; i++)
		{
			if (ft_strncmp(var.name, p_vectors[i].name, ft_strlen(var.name)) == 0)
			{
				if ((ret = parse_vector(split, "()", p_vectors[i].vec)))
					return (ERROR_INVALID_VECTOR_SYNTAX);
				break ;
			}
		}

	if (var.type == DT_FLOAT)
	{
		if (ft_strncmp(var.name, "ns=", ft_strlen(var.name)) == 0)
		{
			if (!is_float_in((mtl->ns = ft_atof(&split[3])), 0.0f, 100.0f))
				return (ERROR_INVALID_SPECULAR_EXPONENT_VALUE);
		}
		else if (!is_float_in((mtl->d = ft_atof(&split[2])), 0.0f, 1.0f))
				return (ERROR_INVALID_OPACITY_VALUE);
	}
	return (ret);
}

static int	add_mtl_to_scene(t_dynarray *mtls, t_material *m)
{
	if (mtls->byte_size == 0)
		if (init_dynarray(mtls, sizeof(t_material), MAX_MTLS))
			return (ERROR_MALLOC_FAILED);

	if (push_dynarray(mtls, m, false))
		return (ERROR_MALLOC_FAILED);
	return (0);
}

static int	get_mtl_name(t_material *new, char **split)
{
	if (ft_tablen(split) < 2)
		return (ERROR_MTL_NAME_NOT_FOUND);
	for (unsigned int i = 0; split[1][i]; i++)
		if (!ft_isalnum(split[1][i]))
			return (ERROR_INVALID_CHAR_IN_MTL_NAME);

	if (ft_strlen(split[1]) >= MAX_MTL_NAME_LENGTH)
		return (ERROR_MTL_NAME_TOO_LONG);
	ft_strcpy(new->name, split[1]);
	return (0);
}

int		cmd_addmtl(t_rt_env *env, char *line)
{
	const t_var	vars[NARG_MTL] ={
									{"ka=", DT_VECTOR_P},
									{"ks=", DT_VECTOR_P},
									{"kd=", DT_VECTOR_P},
									{"ns=", DT_FLOAT},
									{"d=", DT_FLOAT}
									};
	t_material	new;
	char		**split;
	bool		found;
	int			ret;

	if (!(split = ft_strsplit(line, " ")))
		return (ERROR_MALLOC_FAILED);

	if ((ret = get_mtl_name(&new, split)))
		return (ret);

	for (unsigned int i = 1; i < NARG_MTL; i++)
	{
		found = false;
		for (unsigned int j = 0; split[j]; j++)
			if (ft_strncmp(vars[i].name, split[j], ft_strlen(vars[i].name)) == 0)
			{
				found = true;
				if ((ret = get_arg_value(vars[i], &new, split[j])))
					return (ret);
			}
	}
	if ((ret = add_mtl_to_scene(&env->scene.mtls, &new)))
		return (ret);
	return (0);
}
