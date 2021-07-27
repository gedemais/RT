#include "main.h"

// Tools

int		parse_vector(char *vec, char *seps, cl_float3 *ret)
{
	char	**components;
	int		start;
	int		end;

	for (start = 0; vec[start] && vec[start] != seps[0]; start++);
	end = ft_strlen(vec) - 1;

	if (vec[start] != seps[0] || vec[end] != seps[1])
		return (ERROR_INVALID_VECTOR_SYNTAX);

	vec[0] = 0;
	vec[end] = 0;
	vec++;

	if (!(components = ft_strsplit(&vec[start], ",")))
		return (ERROR_MALLOC_FAILED);

	if (ft_tablen(components) != 3)
	{
		ft_free_ctab(components);
		return (ERROR_MISSING_COMPONENT_IN_VECTOR);
	}

	ret->x = atof(components[0]);
	ret->y = atof(components[1]);
	ret->z = atof(components[2]);

	ft_free_ctab(components);
	return (0);
}

static int		check_set_cmd_syntax(char ***parts, char *line)
{
	if (!(*parts = ft_strsplit(line, ":")))
		return (ERROR_MALLOC_FAILED);

	if (ft_tablen(*parts) != 2)
	{
		ft_free_ctab(*parts);
		return (ERROR_INVALID_SET_CMD_SYNTAX);
	}
	return (0);
}

// ----------------------------------------------------------

int			parse_window_dim(t_rt_env *env, char *line)
{
	char	**parts;
	int		dim;
	int		ret;

	if ((ret = check_set_cmd_syntax(&parts, line)))
		return (ret);
	
	dim = ft_atoi(parts[1]);

	if (dim <= 0 || dim > MAX_WINDOW_DIM)
	{
		ft_free_ctab(parts);
		return (ERROR_INVALID_WINDOW_DIMENSION);
	}

	if (ft_strstr(parts[0], "window_height") != NULL)
		env->scene.cam.img_hgt = dim;
	else
		env->scene.cam.img_wdt = dim;

	ft_free_ctab(parts);
	return (0);
}

int			parse_cam_position(t_rt_env *env, char *line)
{
	char	**parts;
	int		ret;

	if ((ret = check_set_cmd_syntax(&parts, line)))
		return (ret);

	parse_vector(line, "[]", &env->scene.cam.o);

	return (0);
}

int			parse_fov(t_rt_env *env, char *line)
{
	char	**parts;
	int		ret;
	float	try;

	if ((ret = check_set_cmd_syntax(&parts, line)))
		return (ret);

	try = ft_atof(parts[1]);

	if (try <= 0 || try > 180.0)
		return (ERROR_INVALID_FOV);

	env->scene.cam.fov = try;
	return (0);
}

int			parse_brightness(t_rt_env *env, char *line)
{
	char	**parts;
	int		ret;
	float	try;

	if ((ret = check_set_cmd_syntax(&parts, line)))
		return (ret);

	try = atof(parts[ft_tablen(parts) - 1]);

	if (try < 0.0 || try > 1.0)
		return (ERROR_INVALID_BRIGHTNESS);

	env->scene.cam.brightness = try;
	return (0);
}
