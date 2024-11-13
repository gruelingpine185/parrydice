#ifndef PD_RENDERER_H
#define PD_RENDERER_H

#include "utils/defines.h"
#include "utils/types.h"

typedef struct VkInstance_T* VkInstance;
typedef struct VkDebugUtilsMessengerEXT_T* VkDebugUtilsMessengerEXT;

typedef struct pd_renderer pd_renderer;

struct pd_renderer {
    VkInstance instance;
#if PD_USE_DEBUG
    VkDebugUtilsMessengerEXT messenger;
#endif // PD_USE_DEBUG
};

PD_C_DECLS_BEGIN
b32 pd_renderer_init(pd_renderer* _renderer);
void pd_renderer_deinit(pd_renderer* _renderer);
PD_C_DECLS_END

#endif // PD_RENDERER_H
