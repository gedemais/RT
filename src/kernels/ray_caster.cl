__kernel void	ray_caster(__global int *img)
{
	const unsigned short x = get_global_id(0);
	const unsigned short y = get_global_id(1);

	img[y * 400 + x] = 0xffffff;
}
