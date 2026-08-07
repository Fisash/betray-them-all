#include "gui/gui-frontend/game.h"

#include <stdlib.h>
#include <string.h>
#define WINDOW_NAME "gui-demo"

static struct game_resources GLOBAL_resources;

void game_init(struct game *game)
{
    core_init(&game->core);

    frame_buffer_init(&game->fb, START_WIDTH, START_HEIGHT);
    game->window = platform_create_window(WINDOW_NAME, 
                           START_WIDTH, START_HEIGHT);
    game_resources_load(&GLOBAL_resources);
    game->resources = &GLOBAL_resources;
    memset(&game->active_scene, 0, sizeof(struct scene));

    game->is_running = 0;
}

static void process_input(struct game *game)
{
    struct input_event event;
    do {
        platform_poll_event(game->window, &event);
        switch(event.type)
        {
            case INP_EVENT_WINDOW_RESIZE:
                struct window_size new_size = event.values.size;
                free(game->fb.data);
                frame_buffer_init(&game->fb, new_size.width, new_size.height);
                layout_node_update_root(&game->active_scene.root, 
                                        new_size.width, 
                                        new_size.height);
                scene_update_layout(&game->active_scene);
                break;
            default:
                break;
        }
        scene_process_ie(&game->active_scene, &event);
    }
    while(event.type != INP_EVENT_NONE);
}

static void process_scene_request(struct game *game)
{
    struct scene *s = &game->active_scene;
    switch(s->req.type)
    {
        case SCENE_REQ_EXIT:
            game->is_running = 0;
            break;
        case SCENE_REQ_LOAD_SCENE:
            scene_load(s, s->req.props.scene_id, 
                  game->resources, &game->core);
            break;
        default:
            break;
    }
}

void game_destory(struct game *game)
{
    platform_destroy_window(game->window);
    free(game->fb.data);
}

void game_run(struct game *game)
{
    core_new_session(&game->core); /* take it out to menu save choise */
    scene_load(&game->active_scene, SCENE_MAIN, 
                 game->resources, &game->core);

    game->is_running = 1;
    while(game->is_running)
    {
        process_input(game);
        scene_draw(&game->active_scene, &game->fb);
        platform_draw_frame(game->window, &game->fb);
        process_scene_request(game);
    }
    game_destory(game);
}
