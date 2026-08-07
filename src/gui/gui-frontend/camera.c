#include "gui/gui-frontend/camera.h"

#include "gui/platform.h"

void camera_init(struct camera *cam)
{
    cam->min_pos_x = 0;
    cam->pos_x = 0;
    cam->max_pos_x = 300;

    cam->min_pos_y = 0;
    cam->pos_y = 0;
    cam->max_pos_y = 80;

    cam->min_scale = 1;
    cam->scale = 3;
    cam->max_scale = 9;
}

void camera_zoom_increase(struct camera *cam)
{
    cam->scale += 1;
    if(cam->scale > cam->max_scale)
        cam->scale = cam->max_scale;
}

void camera_zoom_decrease(struct camera *cam)
{
    cam->scale -= 1;
    if(cam->scale < cam->min_scale)
        cam->scale = cam->min_scale;
}

void camera_move_right(struct camera *cam)
{
    cam->pos_x += MOV_STEP;
    if(cam->pos_x > cam->max_pos_x)
        cam->pos_x = cam->max_pos_x;
}

void camera_move_left(struct camera *cam)
{
    cam->pos_x -= MOV_STEP;
    if(cam->pos_x < cam->min_pos_x)
        cam->pos_x = cam->min_pos_x;
}

void camera_move_up(struct camera *cam)
{
    cam->pos_y += MOV_STEP;
    if(cam->pos_y > cam->max_pos_y)
        cam->pos_y = cam->max_pos_y;
}

void camera_move_down(struct camera *cam)
{
    cam->pos_y -= MOV_STEP;
    if(cam->pos_y < cam->min_pos_y)
        cam->pos_y = cam->min_pos_y;
}

void camera_process_input_key_down(struct camera *cam, int keycode)
{
    switch(keycode)
    {
        case KEY_Z:
            camera_zoom_increase(cam);
            break;
        case KEY_X:
            camera_zoom_decrease(cam);
            break;
        case KEY_H:
            camera_move_left(cam);
            break;
        case KEY_L:
            camera_move_right(cam);
            break;
        case KEY_K:
            camera_move_down(cam);
            break;
        case KEY_J:
            camera_move_up(cam);
            break;
        default:
            break;
    }
}

