/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:09:42 by tcaccava          #+#    #+#             */
/*   Updated: 2025/05/07 19:14:26 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

t_intersect	v_intersection(int x_player, int y_player, double radiant_angle)
{
	t_intersect	v;

	if (cos(radiant_angle) > 0) // se il player guarda a destra
	{
		v.x = floor(x_player / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
		// arrotonda in difetto alla successiva tiles
		v.step_x = TILE_SIZE;
		// lo step successivo sull'asse orizzontale e' la prossima tilesize
	}
	else
	{
		v.x = floor(x_player / TILE_SIZE) * TILE_SIZE - 0.0001;
		// se guarda a sinistra
		v.step_x = -TILE_SIZE;
	}
	v.y = y_player + (v.x - x_player) * tan(radiant_angle);
	// il . di intersezione dir proporzionale alla tangente dell'angolo
	if (sin(radiant_angle) > 0)
		v.step_y = TILE_SIZE * tan(radiant_angle); // se guarda in alto
	else
		v.step_y = -TILE_SIZE * tan(radiant_angle); // in basso
	return (v);
}

t_intersect	h_intersection(int x_player, int y_player, double radiant_angle)
{
	t_intersect	h;

	if (sin(radiant_angle) > 0) // se guarda in alto
	{
		h.y = floor(y_player / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
		h.step_y = TILE_SIZE;
	}
	else // in basso
	{
		h.y = floor(y_player / TILE_SIZE) * TILE_SIZE - 0.0001;
		h.step_y = -TILE_SIZE;
	}
	h.x = x_player + (h.y - y_player) / tan(radiant_angle);
	if (cos(radiant_angle) > 0) // destra
		h.step_x = TILE_SIZE / tan(radiant_angle);
	else // sinistra
		h.step_x = -TILE_SIZE / tan(radiant_angle);
	return (h);
}

int	is_not_wall(t_map *map, double x, double y)
{
	int	map_x;
	int	map_y;

	map_x = (int)(x / TILE_SIZE);
	// riconduce x e y alle rispettive celle della mappa
	map_y = (int)(y / TILE_SIZE);
	if (map_x < 0 || map_x >= map->width || map_y < 0 || map_y >= map->height)
		return (0);
	if (map->matrix[map_y][map_x] == '1') // controlla se e' muro
		return (0);
	else
		return (1);
}

double	ray_casting(int x_player, int y_player, double radiant_angle,
		t_map *map)
{
	t_intersect	v;
	t_intersect	h;
	double		dist_v;
	double		dist_h;

	v = v_intersection(x_player, y_player, radiant_angle);
	// calcola le intersezioni sui due assi
	h = h_intersection(x_player, y_player, radiant_angle);
	while (is_not_wall(map, v.x, v.y))
	// trova tutte le intersezioni fino a trovare il muro
	{
		v.x = v.x + v.step_x;
		v.y = v.y + v.step_y;
	}
	while (is_not_wall(map, h.x, h.y))
	{
		h.x = h.x + h.step_x;
		h.y = h.y + h.step_y;
	}
	// calcola la distanza minima tra il player e il muro con Pitagora
	dist_v = sqrt((v.x - x_player) * (v.x - x_player) + (v.y - y_player) * (v.y
				- y_player));
	dist_h = sqrt((h.x - x_player) * (h.x - x_player) + (h.y - y_player) * (h.y
				- y_player));
	if (dist_v < dist_h) // verifica quale delle due distanze sia quella minore
		return (dist_v);
	else
		return (dist_h);
}

double	no_fish_eye(double min_distance, double radiant_angle,
		double player_angle)
{
	double	corrected_dist;

	// calcola la distanza perpendicolare
	corrected_dist = min_distance * cos(radiant_angle - player_angle);
	return (corrected_dist);
}
int	calc_wall_height(double corrected_dist)
{
	double	distance_to_projection_plane;
	double	wall_height;

	// calcola l'altezza del muro in funzione della distanza
	distance_to_projection_plane = (DISPLAY_WIDTH / 2) / tan(FOV / 2);
	wall_height = (TILE_SIZE / corrected_dist) * distance_to_projection_plane;
	return ((int)wall_height);
}


//ajout de la struct map pour acceder a la struct des img qui est lier 
// plus simple pour acceder a tout img wall floor ciel etc
void render_column(int column_x, double distance, t_angle_data angles,
 t_env *env, t_map *map, int x_player, int y_player)
{
	(void)x_player;
	(void)y_player;
	
	double	corrected_dist;
	int		wall_height;
	int		draw_start;
	int		draw_end;
	int		y;
	int		color;
	char	*pixel;

	int texture_x;
	int texture_y;

	corrected_dist = no_fish_eye(distance, angles.radiant_angle,
			angles.player_angle);
	wall_height = calc_wall_height(corrected_dist);
	draw_start = (DISPLAY_HEIGHT / 2) - (wall_height / 2);//normalizza l'altezza del muro
	draw_end = (DISPLAY_HEIGHT / 2) + (wall_height / 2);
	if (draw_start < 0)
		draw_start = 0;
	if (draw_end >= DISPLAY_HEIGHT)
		draw_end = DISPLAY_HEIGHT - 1;
	y = 0;
	while (y < DISPLAY_HEIGHT)
	{
		if (y < draw_start)//colora il cielo
			color = 0x87CEEB;
		else if (y >= draw_start && y <= draw_end)//il muro
		{
			// calc de la pos dans la texture
			texture_x = column_x % TILE_SIZE;
			texture_y = (y - draw_start) * TILE_SIZE / wall_height;
			if (texture_x >= 0 && texture_x < TILE_SIZE && 
				texture_y >= 0 && texture_y < TILE_SIZE && 
				map->wall_texture.addr != NULL)
			{
				// Récupération de la couleur dans la texture
				char *texture_pixel = map->wall_texture.addr + (texture_y * map->wall_texture.line_length + texture_x * (map->wall_texture.bits_per_pixel / 8));
				color = *(unsigned int *)texture_pixel;
			}
			else
				color = 0x654321;
		}
		else // Pour le sol
		{
			// Approche simplifiée pour le sol
			// Calcul de la distance relative depuis l'horizon
			double floorDistPercent = (double)(y - draw_end) / (double)(DISPLAY_HEIGHT - draw_end);
			
			// Coordonnées de texture qui changent graduellement avec la distance
			texture_x = (column_x / 4 + (int)(floorDistPercent * 20)) % TILE_SIZE;
			texture_y = (y / 4 + (int)(floorDistPercent * 20)) % TILE_SIZE;
			
			if (texture_x >= 0 && texture_x < TILE_SIZE && 
				texture_y >= 0 && texture_y < TILE_SIZE && 
				map->floor_texture.addr != NULL)
			{
				// Récupération de la couleur dans la texture du sol
				char *texture_pixel = map->floor_texture.addr + (texture_y * map->floor_texture.line_length + texture_x * (map->floor_texture.bits_per_pixel / 8));
				color = *(unsigned int *)texture_pixel;
			}
			else
			{
				color = 0x228B22; // Couleur par défaut pour le sol
			}
		}
		pixel = env->img.addr + (y * env->img.line_length + column_x
				* (env->img.bits_per_pixel / 8));
		*(unsigned int *)pixel = color;
		y++;
	}
}

void	render_frame(t_env *env, t_angle_data *angles, double *distances, t_map *map, int x_player, int y_player)
{
	int col = 0;
	while (col < DISPLAY_WIDTH)
	{
		render_column(col, distances[col], angles[col], env, map, x_player, y_player);
		col++;
	}
	mlx_put_image_to_window(env->mlx, env->win, env->img.ptr, 0, 0);
}
