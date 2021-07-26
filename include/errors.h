#ifndef ERRORS_H
# define ERRORS_H



enum	e_errors
{
	ERROR_MLX_LAUNCH,
	ERROR_KERNEL_MAPPING_FAILED,
	ERROR_PROGRAM_CREATION_FAILED,
	ERROR_KERNEL_BUILD_FAILED,
	ERROR_KERNEL_CREATION_FAILED,
	ERROR_NO_CL_DEVICE,
	ERROR_CL_CONTEXT_CREATION_FAILED,
	ERROR_CL_CMD_QUEUE_CREATION_FAILED,
	ERROR_NO_VAR_SPECIFIED_FOR_SET_CMD,
	ERROR_MALLOC_FAILED,
	ERROR_SCENE_FILE_MEMORY_MAPPING_FAILED,
	ERROR_INVALID_SCENE_FILE_PATH,
	ERROR_VARIABLE_NOT_FOUND,
	ERROR_INVALID_VECTOR_SYNTAX,
	ERROR_MISSING_COMPONENT_IN_VECTOR,
	ERROR_INVALID_WINDOW_DIMENSION,
	ERROR_INVALID_FOV,
	ERROR_INVALID_BRIGHTNESS,
	ERROR_INVALID_SET_CMD_SYNTAX,
	ERROR_NO_SCENE,
	ERROR_MAX
};

void	rt_error(int exit_code);

#endif
