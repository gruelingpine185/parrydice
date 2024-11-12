#include <stdio.h>
#include <string.h>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "utils/expect.h"
#include "utils/darray.h"

static b32 vk_r_instance_exts(pd_darray* _exts);
static b32 vk_r_instance_layers(pd_darray* _layers);
static b32 vk_r_supported_exts(pd_darray* _exts);
static void vk_print_str_darray(const pd_darray* _arr, const char* _title);
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
    const char* layer = strdup("VK_LAYER_KHRONOS_validation");
    if(!layer) return 0;

    return pd_darray_append(_layers, (void*) layer);
}

static b32 vk_r_supported_exts(pd_darray* _exts) {
    u32 property_count = 0;
    VkResult res = vkEnumerateInstanceExtensionProperties(
        VK_NULL_HANDLE,
        &property_count,
        VK_NULL_HANDLE);
    if((res != VK_SUCCESS) && (res != VK_INCOMPLETE)) return 0;

    VkExtensionProperties* props =
        (VkExtensionProperties*) malloc(sizeof(*props) * property_count);
    if(!props) return 0;

    res = vkEnumerateInstanceExtensionProperties(
        VK_NULL_HANDLE,
        &property_count,
        props);
    if((res != VK_SUCCESS) && (res != VK_INCOMPLETE)) return 0;

    if(!pd_darray_init(_exts, property_count)) return 0;

    for(u32 i = 0; i < property_count; i++) {
        const char* ext = strdup(props[i].extensionName);
        if(!ext) return 0;
        if(!pd_darray_append(_exts, (void*) ext)) return 0;
    }

    free(props);
    return 1;
}

static void vk_print_str_darray(const pd_darray* _arr, const char* _title) {
    PD_expect_nonnull(_arr);
    if(_title) printf("%s\n", _title);

    for(u32 i = 0; i < pd_darray_r_size(_arr); i++) {
        printf("  %s\n", (const char*) pd_darray_at(_arr, i));
    }

    printf("\n");
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
