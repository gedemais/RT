#include "main.h"

void	rt_error(int exit_code)
{
	const char	*error_messages[ERROR_MAX] = {
		[ERROR_MLX_LAUNCH] = "MinilibX failed to launch",
		//[] = "",
	};
	ft_putstr_fd("rt : ", 2);
	ft_putendl_fd(error_messages[exit_code], 2);
}
