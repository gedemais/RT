#include "main.h"

static int	map_scene_file(char *path, char **content, size_t *size)
{
	struct stat	info;
	int			fd;

	if ((fd = open(path, O_RDONLY)) == -1 || fstat(fd, &info) == -1)
		return (ERROR_INVALID_SCENE_FILE_PATH);

	*size = (size_t)info.st_size;
	if ((*content = mmap(0, *size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (ERROR_SCENE_FILE_MEMORY_MAPPING_FAILED);

	return (0);
}

static void	data_spread(t_rt_env *env)
{
	t_camera	*cam;

	cam = ((t_camera*)&env->scene.cam);
	env->mlx.width = cam->img_wdt;
	env->mlx.height = cam->img_hgt;
	cam->aspect_ratio = (float)cam->img_wdt / (float)cam->img_hgt;
	//cam->fov_rad = 1.0f / tan(cam->fov * 0.5f / 180.0f * 3.14159f);
}

void	parser_error(char **lines, unsigned int i)
{
	char	buff[4096];

	ft_strcpy(buff, "Scene parser failed");
	ft_strcat(buff, " :\n");
	ft_strcat(buff, lines[i]);

	ft_putendl(buff);
}

int			get_commands(t_rt_env *env, char **lines)
{
	static int	(*cmd_fts[NB_COMMANDS])(t_rt_env*, char*) = {
															cmd_addobj,
															cmd_addlight,
															cmd_addmtl
																};
	const char	*commands[NB_COMMANDS] = {
											"addobj",
											"addlight",
											"addmtl"
											};
	int			ret;

	for (unsigned int i = 0; lines[i]; i++)
	{
		for (unsigned int j = 0; j < NB_COMMANDS; j++)
		{
			if (ft_strncmp(lines[i], commands[j], ft_strlen(commands[j])) == 0)
				if ((ret = cmd_fts[j](env, lines[i])))
				{
					parser_error(lines, i);
					return (ret);
				}
		}
	}
	return (0);
}

int			parse_scene(t_rt_env *env, char *arg)
{
	char	**lines;
	char	*scene;
	size_t	scene_bs;
	int		ret;

	if ((ret = map_scene_file(arg, &scene, &scene_bs)))
		return (ret);

	if (!(lines = ft_strsplit(scene, "\n")))
		return (ERROR_MALLOC_FAILED);

	if ((ret = get_variables(env, lines))
		|| (ret = get_commands(env, lines)))
	{
		ft_free_ctab(lines);
		return (ret);
	}

	data_spread(env);

	munmap(scene, scene_bs);
	return (0);
}
