#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "utils/expect.h"
#include "utils/darray.h"
#include "core/renderer.h"

static b32 pd_darray_is_str_eq(const void* _l, const void* _r);
static b32 vk_r_instance_exts(pd_darray* _exts);
static b32 vk_r_instance_layers(pd_darray* _layers);
static b32 vk_r_supported_exts(pd_darray* _exts);
static b32 vk_r_supported_layers(pd_darray* _layers);
static void vk_print_str_darray(const pd_darray* _arr, const char* _title);
static b32 vk_is_required_exts_present(const pd_darray* _supported_exts,
                                       const pd_darray* _req_exts);
static b32 vk_is_requested_layers_present(const pd_darray* _supported_layers,
                                          const pd_darray* _req_layers);
static void vk_application_info_init(VkApplicationInfo* _app_info,
                                     const char* _app_name);
static void vk_instance_create_info_init(VkInstanceCreateInfo* _create_info,
                                         const VkApplicationInfo* _app_info,
                                         const pd_darray* _exts,
                                         const pd_darray* _layers);
static b32 vk_create_instance(VkInstance* _instance);


static b32 pd_darray_is_str_eq(const void* _l, const void* _r) {
    return (strcmp((const char*) _l, (const char*) _r) == 0);
}

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
    PD_expect_nonnull(_exts);
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

static b32 vk_r_supported_layers(pd_darray* _layers) {
    PD_expect_nonnull(_layers);
    u32 property_count = 0;
    VkResult res = vkEnumerateInstanceLayerProperties(
        &property_count,
        VK_NULL_HANDLE);
    if((res != VK_SUCCESS) && (res != VK_INCOMPLETE)) return 0;

    VkLayerProperties* props =
        (VkLayerProperties*) malloc(sizeof(*props) * property_count);
    if(!props) return 0;

    res = vkEnumerateInstanceLayerProperties(&property_count, props);
    if((res != VK_SUCCESS) && (res != VK_INCOMPLETE)) return 0;
    if(!pd_darray_init(_layers, property_count)) return 0;

    for(u32 i = 0; i < property_count; i++) {
        const char* layer = strdup(props[i].layerName);
        if(!layer) return 0;
        if(!pd_darray_append(_layers, (void*) layer)) return 0;
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

static b32 vk_is_required_exts_present(const pd_darray* _supported_exts,
                                       const pd_darray* _req_exts) {
    PD_expect_nonnull(_supported_exts);
    PD_expect_nonnull(_req_exts);
    return pd_darray_contains_array(
        _supported_exts, _req_exts,
        &pd_darray_is_str_eq);
}

static b32 vk_is_requested_layers_present(const pd_darray* _supported_layers,
                                          const pd_darray* _req_layers) {
    PD_expect_nonnull(_supported_layers);
    PD_expect_nonnull(_req_layers);
    return pd_darray_contains_array(
        _supported_layers, _req_layers,
        &pd_darray_is_str_eq);
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

static b32 vk_create_instance(VkInstance* _instance) {
    PD_expect_nonnull(_instance);
    VkApplicationInfo app_info;
    vk_application_info_init(&app_info, "Parrydice");

    pd_darray exts;
    if(!pd_darray_init(&exts, 0)) return 0;
    if(!vk_r_instance_exts(&exts)) return 0;

    pd_darray layers;
    if(!pd_darray_init(&layers, 0)) return 0;
    if(!vk_r_instance_layers(&layers)) return 0;

    VkInstanceCreateInfo create_info;
    vk_instance_create_info_init(&create_info, &app_info, &exts, NULL);
    pd_darray supported_exts;
    if(!vk_r_supported_exts(&supported_exts)) return 0;
    if(!vk_is_required_exts_present(&supported_exts, &exts)) return 0;

    pd_darray supported_layers;
    if(!vk_r_supported_layers(&supported_layers)) return 0;
    if(!vk_is_requested_layers_present(&supported_layers, &layers)) return 0;

    // TODO: add debug macros to disable functions
    vk_print_str_darray(&exts, "Required Extensions:");
    vk_print_str_darray(&layers, "Requested Validation Layers:");
    vk_print_str_darray(&supported_exts, "Supported Extensions:");
    vk_print_str_darray(&supported_layers, "Supported Layers:");
    for(u32 i = 0; i < pd_darray_r_size(&supported_exts); i++) {
        free(pd_darray_at(&supported_exts, i));
    }

    // TODO: figure out a good way to delete darrays
    pd_darray_deinit(&supported_exts);
    pd_darray_deinit(&supported_layers);
    VkResult res = vkCreateInstance(&create_info, VK_NULL_HANDLE, _instance);
    if(res != VK_SUCCESS) return 0;

    pd_darray_deinit(&exts);
    for(u32 i = 0; i < pd_darray_r_size(&layers); i++) {
        free(pd_darray_at(&layers, i));
    }

    pd_darray_deinit(&layers);
    return 1;
}

b32 pd_renderer_init(pd_renderer* _renderer) {
    PD_expect_nonnull(_renderer);
    if(!vk_create_instance(&_renderer->instance)) return 0;

    return 1;
}

void pd_renderer_deinit(pd_renderer* _renderer) {
    PD_expect_nonnull(_renderer);
    vkDestroyInstance(_renderer->instance, VK_NULL_HANDLE);
}
