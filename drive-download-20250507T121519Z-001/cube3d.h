/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 21:50:29 by tcaccava          #+#    #+#             */
/*   Updated: 2025/05/07 18:46:15 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE3D_H
# define CUBE3D_H

# include "mlx.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# define TILE_SIZE 64
# define DISPLAY_WIDTH 1920
# define DISPLAY_HEIGHT 1080
# define FOV (M_PI / 3)

typedef struct s_intersect
{
	double	x;
	double	y;
	double	step_x;
	double	step_y;
}			t_intersect;


typedef struct s_img
{
	void	*ptr;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_img;

typedef struct s_map
{
	int		width;
	int		height;
	char	**matrix;

	t_img	floor_texture;
	t_img	wall_texture;
	// new for wall texture

}			t_map;

typedef struct s_env
{
	void	*mlx;
	void	*win;
	t_img	img;
}			t_env;

typedef struct s_angle_data
{
	double	radiant_angle;
	double	player_angle;

}			t_angle_data;

t_intersect	v_intersection(int x_player, int y_player, double radiant_angle);

t_intersect	h_intersection(int x_player, int y_player, double radiant_angle);

int			is_not_wall(t_map *map, double x, double y);
int			calc_wall_height(double corrected_dist);
double		no_fish_eye(double min_distance, double radiant_angle,
				double player_angle);



				
void render_column(int column_x, double distance, t_angle_data angles, t_env *env, t_map *map, int x_player, int y_player);
void render_frame(t_env *env, t_angle_data *angles, double *distances, t_map *map, int x_player, int y_player);





double		ray_casting(int x_player, int y_player, double radiant_angle,
				t_map *map);
int			ft_strlen(char *s);
char		*ft_strdup(char *s);

#endif
