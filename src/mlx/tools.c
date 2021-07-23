#include "main.h"

void		draw_pixel(t_mlx *mlx, uint32_t x, uint32_t y, int color)
{
	int			pos;

	if (x < 0 || x >= mlx->width || y < 0 || y >= mlx->height)
		return ;
	pos = (y * mlx->width + x) * 4;
	*(int*)&mlx->img_data[pos] = color;
}
