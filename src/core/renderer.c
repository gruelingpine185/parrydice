#include <string.h>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "utils/expect.h"
#include "utils/darray.h"

static b32 vk_r_instance_exts(pd_darray* _exts);
static b32 vk_r_instance_layers(pd_darray* _layers);


static b32 vk_r_instance_exts(pd_darray* _exts) {
    PD_expect_nonnull(_exts);
    u32 count = 0;
    const char** glfw_exts = glfwGetRequiredInstanceExtensions(&count);
    if(!glfw_exts) return 0;

    for(u32 i = 0; i < count; i++) {
        if(!pd_darray_append(_exts, (void*) glfw_exts[i])) return 0;
    }

#if __APPLE__
    const char* ext = strdup(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    if(!ext) return 0;
    if(!pd_darray_append(_exts, (void*) ext)) return 0;
#endif // __APPLE__

    return 1;
}

static b32 vk_r_instance_layers(pd_darray* _layers) {
    PD_expect_nonnull(_layers);
    const char* layer = strdup("VK_EXT_debug_utils");
    if(!layer) return 0;

    return pd_darray_append(_layers, (void*) layer);
}
