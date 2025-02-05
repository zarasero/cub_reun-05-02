/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: morgane <morgane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 18:23:50 by morgane           #+#    #+#             */
/*   Updated: 2025/02/04 19:33:46 by morgane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	init_data(t_data *data)
{
	data->file = NULL;
	data->c_color = NULL;
	data->f_color = NULL;
	//data->no_txt = NULL;
	//data->so_txt = NULL;
	//data->ea_txt = NULL;
	//data->we_txt = NULL;
	data->path_text[0] = NULL;
    data->path_text[1] = NULL;
    data->path_text[2] = NULL;
    data->path_text[3] = NULL;
	data->new_map = NULL;
	data->x_pos = 0;
	data->y_pos = 0;
	data->map_lines = 0; //hight??
	data->map_start = -1;
	data->end_map = -1;
	data->temp_j = 0;
	data->temp_i = 0;
	data->map_width = 0;
	data->map_length = 0;
	
    data->mlx = mlx_init();
    if (!data->mlx)
    {
         printf("Error: mlx_init() returned NULL\n");
         exit(1);
    }
    data->win = mlx_new_window(data->mlx, WINDOW_W, WINDOW_H, "Game");
    if (!data->win)
    {
        printf("Error: data.win == NULL\n");
        exit(1);
    }
}

void	parsing_cub(char *argv)
{
	t_data	*data;

	data = NULL;
	data = malloc(sizeof(t_data));
	if (!data)
		err(MALLOC);
	init_data(data);
	if (!is_valid_data_extension(argv))
		err(EXTENSION);
	copy_cub_file(argv, data);
	where_is_the_map(data, data->file, &data->map_lines);
	save_map(data, data->file, &data->map, &data->map_lines);
	extract_textures(data);
	extract_valid_colors(data);
	if (are_colors_and_textures_before_map(data, 0) == false)
		err(TXT_AFTER_MAP);
	file_is_clean(data);
	parsing_map(data);
	// FREE TEXTURES;
	// FREE COLORS;
	// FREE MAP
	free_char_tab(data->map);
	free_char_tab(data->file);
	free(data);
	//ft_resoures_initialisation(data);
	//printf("высота - %d ширина - %d\n", data->map_length, data->map_width);
	//ft_draw(data);
   // mlx_key_hook(data->win, ft_key, (void *)&data);
   // mlx_hook(data.win, 2, 1, ft_key, (void *)&data);
   // mlx_hook(data->win, 17, 0, ft_destroy_exit, (void *)&data); 
   // mlx_hook(data.win, 17, 0, mlx_loop_end, (void *)&data); 
   // mlx_loop(data->mlx);
   // ft_destroy_exit(data);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	parsing_cub(argv[1]);
	//ft_resoures_initialisation(&data);
    //find_player_position(&data);

    return (0);
}
