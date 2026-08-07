#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "gui/platform_interface.h"

struct xlib_window_data {
    Display *display;
    Window window;
    GC gc;
    int width, height;
    int screen_num, depth;
};

window_id platform_create_window(const char *title, int width, int height)
{
    struct xlib_window_data *data = malloc(sizeof(struct xlib_window_data));
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

    return (window_id)data;
}

void platform_destroy_window(window_id win)
{
    struct xlib_window_data *data = (struct xlib_window_data*)win;
    XFreeGC(data->display, data->gc);
    XCloseDisplay(data->display);
    free(data);
}

void platform_draw_frame(window_id win, struct frame_buffer *fb)
{
    struct xlib_window_data *data = (struct xlib_window_data* )win;
    if (!data || !data->display)
    {
        fprintf(stderr, "Invalid window or display data");
        return;
    }

    Visual *default_visual = DefaultVisual(data->display, data->screen_num);
    XImage *image = XCreateImage(data->display, default_visual, data->depth, 
                                                ZPixmap, 0, (char*)fb->data, 
                                    fb->size.width, fb->size.height, 32, 0);

    XPutImage(data->display, data->window, data->gc, image, 
                    0, 0, 0, 0, data->width, data->height);
    XFlush(data->display);
    XFree(image);
}

void platform_poll_event(window_id win, struct input_event *event)
{
    struct xlib_window_data *data = (struct xlib_window_data *)win;

    event->type = INP_EVENT_NONE;
    if(XPending(data->display) == 0)
        return;

    XEvent report;
    XNextEvent(data->display, &report);

    switch(report.type)
    {
        case Expose:
            if(report.xexpose.count != 0)
                break;
            event->type = INP_EVENT_EXPOSE;
            break;
        case ConfigureNotify:
            event->type = INP_EVENT_WINDOW_RESIZE;
            int width, height;
            width = report.xconfigure.width;
            height = report.xconfigure.height;
            event->values.size.width = width;
            event->values.size.height = height;
            data->width = width;
            data->height = height;
            break;
        case KeyPress:
            event->type = INP_EVENT_KEY_DOWN;
            event->values.keycode = report.xkey.keycode;
            break;
        case KeyRelease:
            event->type = INP_EVENT_KEY_UP;
            event->values.keycode = report.xkey.keycode;
            break;
        default:
            break;
    }
}
