#include <stdio.h>
#include <string.h>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "utils/expect.h"
#include "utils/darray.h"

static b32 vk_r_instance_exts(pd_darray* _exts);
static b32 vk_r_instance_layers(pd_darray* _layers);
static void vk_print_instance_exts(const pd_darray* _exts);
static void vk_print_instance_layers(const pd_darray* _layers);
static void vk_application_info_init(VkApplicationInfo* _app_info,
                                     const char* _app_name);
static void vk_instance_create_info_init(VkInstanceCreateInfo* _create_info,
                                         const VkApplicationInfo* _app_info,
                                         const pd_darray* _exts,
                                         const pd_darray* _layers);


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

static void vk_print_instance_exts(const pd_darray* _exts) {
    PD_expect_nonnull(_exts);
    printf("Extensions:\n");
    for(u32 i = 0; i < pd_darray_r_size(_exts); i++) {
        printf("  %d: %s\n", i, (const char*) pd_darray_at(_exts, i));
    }
}

static void vk_print_instance_layers(const pd_darray* _layers) {
    PD_expect_nonnull(_layers);
    printf("Layers:\n");
    for(u32 i = 0; i < pd_darray_r_size(_layers); i++) {
        printf("  %d: %s\n", i, (const char*) pd_darray_at(_layers, i));
    }
}

static void vk_application_info_init(VkApplicationInfo* _app_info,
                                     const char* _app_name) {
    PD_expect_nonnull(_app_info);
    _app_info->sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    _app_info->pNext = VK_NULL_HANDLE;
    _app_info->pApplicationName = _app_name;
    _app_info->applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    _app_info->pEngineName = "Custom Engine";
    _app_info->engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    _app_info->apiVersion = VK_API_VERSION_1_1;
}

static void vk_instance_create_info_init(VkInstanceCreateInfo* _create_info,
                                         const VkApplicationInfo* _app_info,
                                         const pd_darray* _exts,
                                         const pd_darray* _layers) {
    PD_expect_nonnull(_create_info);
    PD_expect_nonnull(_exts);
    _create_info->sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    _create_info->pNext = VK_NULL_HANDLE;
    _create_info->pApplicationInfo = _app_info;
#if __APPLE__
    _create_info->flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif // __APPLE__
    _create_info->enabledExtensionCount = pd_darray_r_size(_exts);
    _create_info->ppEnabledExtensionNames =
        (const char**) pd_darray_r_data(_exts);
    if(_layers) {
        _create_info->enabledLayerCount = pd_darray_r_size(_layers);
        _create_info->ppEnabledExtensionNames =
            (const char**) pd_darray_r_data(_layers);
    } else {
        _create_info->enabledLayerCount = 0;
        _create_info->ppEnabledLayerNames = VK_NULL_HANDLE;
    }
}