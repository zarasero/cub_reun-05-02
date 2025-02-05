#include "../include/cub3d.h"


int ft_prepare_texture(t_data *data, float distance, t_draw_params *params)
{
    params->texture = &data->texture[data->textur_index];
    if (!params->texture->addr)
    {
        printf("Error: texture with index %d not loaded!\n", data->textur_index);
        return (0);
    }
    printf("Successfully loaded texture with index %d\n", data->textur_index);

    params->line_height = (float)WINDOW_H / distance;
    printf("Calculated line height: %.2f   %f\n", params->line_height, distance);

    params->texture_position = 0;
    params->texture_step = (float)params->texture->height / params->line_height;
    printf("Texture step: %.2f, Texture position: %.2f\n", params->texture_step, params->texture_position);

    if (params->line_height > WINDOW_H) {
        printf("Line height exceeds window height. Adjusting...\n");
        params->texture_position = 0.5 * (params->line_height - WINDOW_H) / params->line_height * params->texture->height;
        printf("Adjusted texture position: %.2f\n", params->texture_position);
        params->line_height = WINDOW_H;
    }

    return (1);
}

void ft_draw_column(t_data *data, int x, t_draw_params *params)
{
    unsigned int *texture_pixel_start;
    unsigned int *image_pixel_line;
    float texture_x;
    unsigned int i;

    i = 0;

    if (!params->texture || !params->texture->addr) {
        // Если текстура не была загружена, выходим из функции.
        printf("Error: texture not loaded or invalid\n");
        return;
    }

    texture_pixel_start = (unsigned int *)params->texture->addr;

    // Проверяем, чтобы texture_x не выходила за пределы.
    if (data->textur_shift >= 0)
        texture_x = data->textur_shift;
    else
        texture_x = 0;

    // Проверяем, что позиция в пределах ширины текстуры.
    int texture_offset = (int)(texture_x * params->texture->width);
    if (texture_offset >= params->texture->width) {
        texture_offset = params->texture->width - 1;  // Ограничиваем значение.
    }
    texture_pixel_start += texture_offset;

    // Проверка, что image_pixel_line в пределах допустимой памяти.
    image_pixel_line = (unsigned int *)data->img.addr + x + (WINDOW_H - (int)params->line_height) / 2 * WINDOW_W;

    // Проверка границ line_height
    while (i < (unsigned int)params->line_height)
    {
        // Убедитесь, что индексы остаются в пределах допустимого диапазона.
        if (image_pixel_line >= (unsigned int *)data->img.addr + WINDOW_W * WINDOW_H) {
            printf("Error: pixel out of bounds\n");
            break;
        }
        
        *image_pixel_line = *(texture_pixel_start + ((int)params->texture_position) * params->texture->width);
        image_pixel_line += WINDOW_W;

        // Проверка корректности увеличения позиции.
        params->texture_position += params->texture_step;

        i++;
    }
    printf("Texture position: %f\n", params->texture_position);
printf("Texture step: %f\n", params->texture_step);
printf("Line height: %f\n", params->line_height);
printf("Texture width: %d\n", params->texture->width);
printf("Image width: %d\n", WINDOW_W);

}


void ft_draw_vertical_line(t_data *data, int x, float distance)
{
    t_draw_params params;

    if (!ft_prepare_texture(data, distance, &params))
        return ;
    ft_draw_column(data, x, &params);
}


void ft_init_vertical_line(t_data *data)
{
    int x;
    float angle_step;
    float angle_premier;  
    float distance;
    float correct_distance;

    angle_premier = data->player_angle + (VIEW_ANGLE / 2);
    angle_step = VIEW_ANGLE / (WINDOW_W - 1);

    printf("Initial player angle: %f\n", data->player_angle);
    printf("View angle: %f\n", VIEW_ANGLE);
    printf("Angle step: %f\n", angle_step);

    x = 0;
    while (x < WINDOW_W)
    {
        printf("\nRaycasting for x = %d\n", x);
        printf("Angle premier before trace: %f\n", angle_premier);

        // Проверка на допустимость отрицательных значений
        distance = trace_ray(data, angle_premier);
        if (distance < 0)
        {
            printf("Error: Negative distance encountered (distance: %f)\n", distance);
            distance = 0;  // Устанавливаем в 0, чтобы избежать отрицательных значений
        }

        printf("Distance: %f\n", distance);

        // Корректировка расстояния
        correct_distance = distance * cos(data->player_angle - angle_premier);
        if (correct_distance < 0)
        {
            printf("Error: Corrected distance is negative (correct_distance: %f)\n", correct_distance);
            correct_distance = 0;  // Устанавливаем в 0, если оно отрицательное
        }

        printf("Corrected distance (after cosine correction): %f\n", correct_distance);

        // Вызов для отрисовки вертикальной линии
        ft_draw_vertical_line(data, x, correct_distance);

        printf("Angle premier after trace: %f\n", angle_premier);

        angle_premier -= angle_step;
        x++;
    }
}



void ft_draw_ceiling_and_floor(t_data *data)
{
    unsigned int *dst;
    unsigned int i;
    unsigned int mid_height;

    mid_height = WINDOW_W * (WINDOW_H / 2);
    dst = (unsigned int *) data->img.addr;
    i = mid_height;
    while (i > 0)
    {
        *dst++ = *(data->c_color);
        i--;
    }
    i = WINDOW_W * WINDOW_H - mid_height;
    while (i > 0)
    {
         *dst++ = *(data->f_color);
        i--;
    }
}

int ft_draw(t_data *data)
{
    data->img.img = mlx_new_image(data->mlx, WINDOW_W, WINDOW_H);
    if (!data->img.img)
    {
        printf("Error: Failed to create a new image!\n");
        return (0);
    }
    printf("Successfully created new image with size %d x %d\n", WINDOW_W, WINDOW_H);

    data->img.addr = mlx_get_data_addr(data->img.img,
                                       &data->img.bits_per_pixel,
                                       &data->img.line_length,
                                       &data->img.endian);
    if (!data->img.addr)
    {
        printf("Error: Failed to get image data address!\n");
        mlx_destroy_image(data->mlx, data->img.img);
        return (0);
    }
    printf("Successfully retrieved image data address: %p\n", data->img.addr);

    printf("Starting to draw ceiling and floor...\n");
    ft_draw_ceiling_and_floor(data);

    printf("Initializing vertical lines...\n");
    ft_init_vertical_line(data);

    printf("Putting image to window...\n");
    mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);

    printf("Destroying temporary image...\n");
    mlx_destroy_image(data->mlx, data->img.img);

    return (0);
}
