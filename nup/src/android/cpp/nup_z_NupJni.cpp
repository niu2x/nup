#include "nup_z_NupJni.h"
#include "nup_type.h"
#include "nup_android_window.h"
#include "nup_core.h"

namespace nup {

IntSize android_window_size = { 0, 0 };

}

extern "C" {



JNIEXPORT void JNICALL Java_nup_z_NupJni_on_1gl_1surface_1created(
    JNIEnv*, jclass)
{
    nup::Core::create_instance();
}

JNIEXPORT void JNICALL Java_nup_z_NupJni_on_1gl_1surface_1changed(
    JNIEnv*, jclass, jint w, jint h)
{
    nup::android_window_size = { w, h };
}

JNIEXPORT void JNICALL Java_nup_z_NupJni_on_1gl_1surface_1draw(JNIEnv*, jclass)
{
    nup::Core::get()->step(1 / 60.0);
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) { 
    nup::unused(vm);
    nup::unused(reserved);
    return JNI_VERSION_1_4; }
}
