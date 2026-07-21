#ifndef CAMERA_H
#define CAMERA_H

#define MOV_STEP 10

struct camera {
    int min_pos_x, pos_x, max_pos_x;
    int min_pos_y, pos_y, max_pos_y;
    int min_scale, scale, max_scale;
};


void camera_init(struct camera *cam);

void camera_zoom_increase(struct camera *cam);
void camera_zoom_decrease(struct camera *cam);

void camera_move_right(struct camera *cam);
void camera_move_left(struct camera *cam);
void camera_move_up(struct camera *cam);
void camera_move_down(struct camera *cam);

void camera_process_input_key_down(struct camera *cam, int keycode);

#endif
