#include <GLFW/glfw3.h>

#include "utils/expect.h"
#include "core/window.h"

b32 pd_window_init(pd_window* _win, const char* _title, i32 _w, i32 _h) {
    PD_expect_nonnull(_win);
    PD_expect_nonnull(_title);
    if(!glfwInit()) return 0;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    _win->handle = glfwCreateWindow(_w, _h, _title, NULL, NULL);
    if(!_win->handle) return 0;

    glfwGetWindowSize(_win->handle, &_win->w, &_win->h);
    _win->title = _title;
    return 1;
}

void pd_window_deinit(pd_window* _win) {
    PD_expect_nonnull(_win);
    PD_expect_nonnull(_win->handle);
    glfwDestroyWindow(_win->handle);
    glfwTerminate();
}

b32 pd_window_is_running(pd_window* _win) {
    PD_expect_nonnull(_win);
    PD_expect_nonnull(_win->handle);
    return (glfwWindowShouldClose(_win->handle) == 0);
}

void pd_window_poll_events(pd_window* _win) {
    PD_expect_nonnull(_win);
    PD_expect_nonnull(_win->handle);
    (void) _win;
    glfwPollEvents();
}
