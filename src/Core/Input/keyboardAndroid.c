#include <jni.h>
#include <android_native_app_glue.h>

android_app* _app;
void init(android_app* app) {

	_app = app;

}
/*https://groups.google.com/forum/?fromgroups=#!topic/android-ndk/Tk3g00wLKhk*/
void displayKeyboard(bool show) {

	jint lResult;
    jint lFlags = 0;

    JavaVM* lJavaVM = app->activity->vm;
    JNIEnv* lJNIEnv = app->activity->env;

    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name = "NativeThread";
    lJavaVMAttachArgs.group = NULL;

    lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
    if (lResult == JNI_ERR) {
        return;
    }

    // Retrieves NativeActivity.
    jobject lNativeActivity = app->activity->clazz;
    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

    // Retrieves Context.INPUT_METHOD_SERVICE.
    jclass ClassContext = lJNIEnv->FindClass("android/content/Context");
    jfieldID FieldINPUT_METHOD_SERVICE = lJNIEnv->GetStaticFieldID(ClassContext,
            "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
    jobject INPUT_METHOD_SERVICE =
        lJNIEnv->GetStaticObjectField(ClassContext,
            FieldINPUT_METHOD_SERVICE);
    jniCheck(INPUT_METHOD_SERVICE);

    // Runs getSystemService(Context.INPUT_METHOD_SERVICE).
    jclass ClassInputMethodManager = lJNIEnv->FindClass(
        "android/view/inputmethod/InputMethodManager");
    jmethodID MethodGetSystemService = lJNIEnv->GetMethodID(
        ClassNativeActivity, "getSystemService",
        "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject lInputMethodManager = lJNIEnv->CallObjectMethod(
        lNativeActivity, MethodGetSystemService,
        INPUT_METHOD_SERVICE);

    // Runs getWindow().getDecorView().
    jmethodID MethodGetWindow = lJNIEnv->GetMethodID(
        ClassNativeActivity, "getWindow",
        "()Landroid/view/Window;");
    jobject lWindow = lJNIEnv->CallObjectMethod(lNativeActivity,
        MethodGetWindow);
    jclass ClassWindow = lJNIEnv->FindClass(
        "android/view/Window");
    jmethodID MethodGetDecorView = lJNIEnv->GetMethodID(
        ClassWindow, "getDecorView", "()Landroid/view/View;");
    jobject lDecorView = lJNIEnv->CallObjectMethod(lWindow,
        MethodGetDecorView);

    if (pShow) {
        // Runs lInputMethodManager.showSoftInput(...).
        jmethodID MethodShowSoftInput = lJNIEnv->GetMethodID(
            ClassInputMethodManager, "showSoftInput",
            "(Landroid/view/View;I)Z");
        jboolean lResult = lJNIEnv->CallBooleanMethod(
            lInputMethodManager, MethodShowSoftInput,
            lDecorView, lFlags);
    } else {
        // Runs lWindow.getViewToken()
        jclass ClassView = lJNIEnv->FindClass(
            "android/view/View");
        jmethodID MethodGetWindowToken = lJNIEnv->GetMethodID(
            ClassView, "getWindowToken", "()Landroid/os/IBinder;");
        jobject lBinder = lJNIEnv->CallObjectMethod(lDecorView,
            MethodGetWindowToken);

        // lInputMethodManager.hideSoftInput(...).
        jmethodID MethodHideSoftInput = lJNIEnv->GetMethodID(
            ClassInputMethodManager, "hideSoftInputFromWindow",
            "(Landroid/os/IBinder;I)Z");
        jboolean lRes = lJNIEnv->CallBooleanMethod(
            lInputMethodManager, MethodHideSoftInput,
            lBinder, lFlags);
    }

    // Finished with the JVM.
    lJavaVM->DetachCurrentThread();

}