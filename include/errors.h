#ifndef ERRORS_H
# define ERRORS_H



enum	e_errors
{
	ERROR_MLX_LAUNCH,
	ERROR_KERNEL_MAPPING_FAILED,
	ERROR_PROGRAM_CREATION_FAILED,
	ERROR_KERNEL_BUILD_FAILED,
	ERROR_MAX
};

void	rt_error(int exit_code);

#endif
