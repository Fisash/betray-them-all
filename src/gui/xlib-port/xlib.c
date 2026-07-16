#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "gui/xlib-port/xlib.h"

typedef struct {
    Display *display;
    Window window;
    GC gc;
    int width, height;
    int screen_num, depth;
} xlib_window_data_t;

static window_id_t 
       xlib_create_window(const char *title, int width, int height)
{
    xlib_window_data_t *data = malloc(sizeof(xlib_window_data_t));
    Display *display = XOpenDisplay(NULL);
    if(!display)
    {
        puts("Can`t connect to the X server!\n");
        exit(1);
    }

    data->display = display;
    data->screen_num = DefaultScreen(display);
    Window root = RootWindow(display, data->screen_num);
    data->depth = DefaultDepth(display, data->screen_num);

    data->window = XCreateSimpleWindow(display, root, 0, 0, 
                                          width, height, 0,
                     BlackPixel(display, data->screen_num),
                    WhitePixel(display, data->screen_num));
    XStoreName(display, data->window, title); 
    data->width = width;
    data->height = height;

    XSelectInput(display, data->window, 
        ExposureMask | KeyPressMask | 
        KeyReleaseMask | StructureNotifyMask);

    XMapWindow(display, data->window);

    data->gc = XCreateGC(display, data->window, 0, NULL);
    XSetForeground(display, data->gc, BlackPixel(display, 0));

    return (window_id_t)data;
}

static void xlib_destroy_winow(window_id_t id)
{
    xlib_window_data_t *data = (xlib_window_data_t*)id;
    XFreeGC(data->display, data->gc);
    XCloseDisplay(data->display);
    free(data);
}

static void xlib_draw_frame(window_id_t window_id, frame_buffer_t *buffer)
{
    xlib_window_data_t *data = (xlib_window_data_t*)window_id;
    if (!data || !data->display)
    {
        fprintf(stderr, "Invalid window or display data");
        return;
    }

    Visual *default_visual = DefaultVisual(data->display, data->screen_num);
    XImage *image = XCreateImage(data->display, default_visual, data->depth, 
                                            ZPixmap, 0, (char*)buffer->data, 
                            buffer->size.width, buffer->size.height, 32, 0);

    XPutImage(data->display, data->window, data->gc, image, 
                    0, 0, 0, 0, data->width, data->height);
    XFlush(data->display);
}

static void xlib_poll_event(window_id_t window_id, input_event_t *out_event)
{
    xlib_window_data_t *data = (xlib_window_data_t*)window_id;

    out_event->type = INP_EVENT_NONE;
    if(XPending(data->display) == 0)
        return;

    XEvent report;
    XNextEvent(data->display, &report);

    switch(report.type)
    {
        case Expose:
            if(report.xexpose.count != 0)
                break;
            out_event->type = INP_EVENT_EXPOSE;
            break;
        case ConfigureNotify:
            out_event->type = INP_EVENT_WINDOW_RESIZE;
            int width, height;
            width = report.xconfigure.width;
            height = report.xconfigure.height;
            out_event->values.size.width = width;
            out_event->values.size.height = height;
            data->width = width;
            data->height = height;
            break;
        case KeyPress:
            out_event->type = INP_EVENT_KEY_DOWN;
            out_event->values.keycode = report.xkey.keycode;
            break;
        case KeyRelease:
            out_event->type = INP_EVENT_KEY_UP;
            out_event->values.keycode = report.xkey.keycode;
            break;
        default:
            break;
    }
}

void xlib_init_interface(platform_interface_t *out)
{
    out->create_window = xlib_create_window;
    out->destroy_window = xlib_destroy_winow;
    out->draw_frame = xlib_draw_frame;
    out->poll_event = xlib_poll_event;
}
