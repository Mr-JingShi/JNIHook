#include <jni.h>
#include <android/log.h>
#include <dispatchtable_hook.h>


static void *origin_calloc;
static void *origin_realloc;

static void *my_calloc(size_t __item_count, size_t __item_size) {
    __android_log_print(ANDROID_LOG_ERROR, "hello", "calloc hook %zu %zu", __item_count,
                        __item_size);
    MallocCalloc c = (MallocCalloc) origin_calloc;
    return c(__item_count, __item_size);
}

static void *my_realloc(void *ptr, size_t __item_size) {
    __android_log_print(ANDROID_LOG_ERROR, "hello", "realloc hook %p %zu", ptr,
                        __item_size);
    MallocRealloc r = (MallocRealloc) origin_realloc;
    return r(ptr, __item_size);
}

JNIEXPORT void JNICALL
Java_com_example_jnihook_DispatchTableHook_testDispatchTableHook(JNIEnv *env, jobject thiz) {
    init_dispatch_table();
    dispatch_table_hook(CALLOC, my_calloc, &origin_calloc);
    dispatch_table_hook(REALLOC, my_realloc, &origin_realloc);
    void* ptr = calloc(1,4);
    ptr = realloc(ptr, 8);
    free(ptr);
}
