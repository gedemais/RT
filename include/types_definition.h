#ifndef TYPES_DEFINITION_H
# define TYPES_DEFINITION_H

// main.h
typedef struct s_rt_env t_rt_env;
typedef struct s_mlx t_mlx;
typedef struct s_camera t_camera;
typedef struct s_scene t_scene;

// vectors.h
typedef struct s_point2d t_point2d;
typedef struct s_vec2d t_vec2d;
typedef struct s_point3d t_point3d;
typedef struct s_vec3d t_vec3d;

// opencl.h
typedef struct s_opencl t_opencl;

// objects.h
typedef struct s_color t_color;
typedef struct s_object t_object;
typedef struct s_light t_light;
typedef struct s_material t_material;

typedef struct s_sphere	t_sphere;
typedef struct s_polygon t_polygon;
typedef struct s_cone t_cone;

#endif
