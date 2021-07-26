#include "main.h"

int		find_var_line(const char *var_name, unsigned int *index, char **lines)
{
	for (unsigned int i = 0; lines[i]; i++)
	{
		if (ft_strlen(lines[i]) > (ft_strlen("set ") + ft_strlen(var_name))
			&& ft_strncmp(lines[i], "set ", 4) == 0)
		{
			if (ft_strncmp(&lines[i][4], var_name, ft_strlen(var_name)) == 0)
			{
				*index = i;
				return (0);
			}
		}
	}
	ft_putstr_fd(var_name, 2);
	ft_putendl_fd(" variable not found in scene file.", 2);
	return (ERROR_VARIABLE_NOT_FOUND);
}

int		get_variables(t_rt_env *env, char **lines)
{
	static int	(*var_parsing_fts[NB_SCENE_VARS])(t_rt_env*, char*) = {
												parse_window_dim,
												parse_window_dim,
												parse_cam_position,
												parse_fov,
												parse_brightness
											};

	const char	*var_names[NB_SCENE_VARS] = {
												"window_width",
												"window_height",
												"cam_position",
												"fov",
												"brightness"
											};
	// Tableau psf pour chaque var

	int				ret;
	unsigned int	line_index;

	for (unsigned int i = 0; i < NB_SCENE_VARS; i++)
	{
		if ((ret = find_var_line(var_names[i], &line_index, lines)))
			return (ret);
		if ((ret = var_parsing_fts[i](env, lines[line_index])))
			return (ret);
	}
	return (0);
}
