#include "nup_z_NupJni.h"

extern "C" {
JNIEXPORT void JNICALL Java_nup_z_NupJni_on_1gl_1surface_1created(
    JNIEnv*, jclass)
{
}

JNIEXPORT void JNICALL Java_nup_z_NupJni_on_1gl_1surface_1changed(
    JNIEnv*, jclass, jint, jint)
{
}

JNIEXPORT void JNICALL Java_nup_z_NupJni_on_1gl_1surface_1draw(JNIEnv*, jclass)
{
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) { return JNI_VERSION_1_4; }
}
