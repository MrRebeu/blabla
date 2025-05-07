/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:25:20 by tcaccava          #+#    #+#             */
/*   Updated: 2025/05/07 19:37:02 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:25:20 by tcaccava          #+#    #+#             */
/*   Updated: 2025/05/07 01:49:33 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

char	*g_map_data[] = {"11111111111111111111", 	
						"10000000000000000001",
						"10000000000000000001", 
						"10000000000000000001", 
						"10000000000000000001",
						"10000000111000000001",
						"10000000000000000001",
						"10000000000000000001",
		"10000000000000000001", "10000000000000000001", "11111111111111111111",
		NULL};

t_map	init_map(void)
{
	t_map	map;
	int		i;

	i = 0;
	while (g_map_data[i])
		i++;
	map.height = i;
	map.width = ft_strlen(g_map_data[0]);
	map.matrix = (char **)malloc(sizeof(char *) * map.height);
	i = 0;
	while (i < map.height)
	{
		map.matrix[i] = ft_strdup(g_map_data[i]);
		i++;
	}
	return (map);
}

int	main(void)
{
	t_env			env;
	t_map			map;
	int				x_player;
	int				y_player;
	double			player_angle;
	t_angle_data	angles[DISPLAY_WIDTH];
	double			distances[DISPLAY_WIDTH];
	int				i;
	double			ray_offset;
	double			radiant_angle;
	int width, height;
	
	//ici jai besoin de ces valeur pour la taille de limage 
	//pourquoi pas ajouter ces valeur dans la struct t_img

	
	// Inizializzazione MLX
	env.mlx = mlx_init();
	env.win = mlx_new_window(env.mlx, DISPLAY_WIDTH, DISPLAY_HEIGHT,
			"Raycaster");
	env.img.ptr = mlx_new_image(env.mlx, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	env.img.addr = mlx_get_data_addr(env.img.ptr, &env.img.bits_per_pixel,
			&env.img.line_length, &env.img.endian);
	// Inizializzazione mappa
	map = init_map();


///// ici jajoute laddresse de limage et apres je lui donne les data avec get data
	map.wall_texture.ptr = mlx_xpm_file_to_image(env.mlx, "./wall.xpm", &width , &height);
	if (!map.wall_texture.ptr)
		printf("galere avec limg de wall");
	map.wall_texture.addr = mlx_get_data_addr(map.wall_texture.ptr,
	 		&map.wall_texture.bits_per_pixel, &map.wall_texture.line_length,
	 		&map.wall_texture.endian);
///// ici jajoute laddresse de limage et apres je lui donne les data avec get data
	map.floor_texture.ptr =  mlx_xpm_file_to_image(env.mlx, "./floor.xpm", &width , &height);
	if (!map.floor_texture.ptr)
		printf("galere avec limg de floor");
	map.floor_texture.addr = mlx_get_data_addr(map.floor_texture.ptr,
	 		&map.floor_texture.bits_per_pixel, &map.floor_texture.line_length,
	 		&map.floor_texture.endian);



	
	// Posizione e orientamento iniziale del giocatore
	x_player = 100; // Posizione iniziale in pixel
	y_player = 100;
	player_angle = 0; // Guardando lungo l'asse X positivo
	// Calcolo dei raggi per ogni colonna dello schermo
	i = 0;
	while (i < DISPLAY_WIDTH)
	{
		// Calcolo dell'angolo del raggio
		ray_offset = FOV * ((double)i / DISPLAY_WIDTH - 0.5);
		radiant_angle = player_angle + ray_offset;
		// Normalizzazione dell'angolo
		while (radiant_angle < 0)
			radiant_angle = radiant_angle + 2 * M_PI;
		while (radiant_angle >= 2 * M_PI)
			radiant_angle = radiant_angle - 2 * M_PI;
		// Lancio del raggio e calcolo della distanza
		distances[i] = ray_casting(x_player, y_player, radiant_angle, &map);
		// Salvataggio degli angoli per il rendering
		angles[i].radiant_angle = radiant_angle;
		angles[i].player_angle = player_angle;
		i++;
	}
	// Rendering del frame


	//ajout de map pour pouvoir acceder a map dans render colummn
	render_frame(&env, angles, distances, &map, x_player, y_player);
	// Ciclo principale della finestra
	mlx_loop(env.mlx);
	// Pulizia della memoria (non verrà mai eseguita a causa di mlx_loop)
	i = 0;
	while (i < map.height)
	{
		free(map.matrix[i]);
		i++;
	}
	free(map.matrix);
	return (0);
}
