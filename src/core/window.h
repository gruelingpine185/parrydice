#ifndef PD_WINDOW_H
#define PD_WINDOW_H

#include "utils/defines.h"
#include "utils/types.h"
#include "core/renderer.h"

typedef struct GLFWwindow GLFWwindow;

typedef struct pd_window pd_window;

struct pd_window {
    GLFWwindow* handle;
    pd_renderer renderer;
    const char* title;
    i32 w;
    i32 h;
};

PD_C_DECLS_BEGIN
b32 pd_window_init(pd_window* _win, const char* _title, i32 _w, i32 _h);
void pd_window_deinit(pd_window* _win);
b32 pd_window_is_running(pd_window* _win);
void pd_window_poll_events(pd_window* _win);
PD_C_DECLS_END

#endif // PD_WINDOW_H
