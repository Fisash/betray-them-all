#ifndef CAMERA_H
#define CAMERA_H

#define MOV_STEP 10

typedef struct {
    int min_pos_x, pos_x, max_pos_x;
    int min_pos_y, pos_y, max_pos_y;
    int min_scale, scale, max_scale;
} camera_t;

void camera_init(camera_t *cam);

void camera_zoom_increase(camera_t *cam);
void camera_zoom_decrease(camera_t *cam);

void camera_move_right(camera_t *cam);
void camera_move_left(camera_t *cam);
void camera_move_up(camera_t *cam);
void camera_move_down(camera_t *cam);

void camera_process_input_key_down(camera_t *cam, int keycode);

#endif
