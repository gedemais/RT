#include "main.h"

void	rt_error(int exit_code)
{
	const char	*error_messages[ERROR_MAX] = {
		[ERROR_MLX_LAUNCH] = "Failed to launch MinilibX.",
		[ERROR_KERNEL_MAPPING_FAILED] = "Failed to map OpenCL kernels.",
		[ERROR_PROGRAM_CREATION_FAILED] = "Failed to create OpenCL program.",
		[ERROR_KERNEL_BUILD_FAILED] = "Failed to build OpenCL kernel.",
	};
	ft_putstr_fd("rt : ", 2);
	ft_putendl_fd(error_messages[exit_code], 2);
}
