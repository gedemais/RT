#include "main.h"

// Tools

static int		parse_vector(char *vec, char *seps, float ret[3])
{
	char	**components;
	int		end;

	end = ft_strlen(vec) - 1;
	if (vec[0] != seps[0] || vec[end] != seps[1])
		return (ERROR_INVALID_VECTOR_SYNTAX);

	vec[0] = 0;
	vec[end] = 0;
	vec++;

	if (!(components = ft_strsplit(vec, ",")))
		return (ERROR_MALLOC_FAILED);

	if (ft_tablen(components) != 3)
	{
		ft_free_ctab(components);
		return (ERROR_MISSING_COMPONENT_IN_VECTOR);
	}

	ret[0] = ft_atof(components[0]);
	ret[1] = ft_atof(components[1]);
	ret[2] = ft_atof(components[2]);

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
	float	vec[3];
	char	**parts;
	int		ret;

	if ((ret = check_set_cmd_syntax(&parts, line)))
		return (ret);

	parse_vector(line, "[]", vec);

	env->scene.cam.o.x = vec[0];
	env->scene.cam.o.y = vec[1];
	env->scene.cam.o.z = vec[2];

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

	try = ft_atof(parts[1]);

	if (try < 0.0 || try > 1.0)
		return (ERROR_INVALID_BRIGHTNESS);

	env->scene.cam.brightness = try;
	return (0);
}
