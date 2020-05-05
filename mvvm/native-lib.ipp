//
// Created by test on 2020/4/26.
//
#include <mvvm/jni/ActivityView.h>
#include <jni.h>
#include <string>
#include <logic/application.h>
#include <base/message_loop/message_loop.h>
#include <base/log.h>
#include <android/log.h>
#include <base/jni_util/JNI_OnLoad.h>
#include <base/jni_util/var_cache.h>
#include <base/jni_util/scoped_jstring.h>
#include <base/jni_util/scope_jenv.h>
#include <logic/logic_define.h>
#include <kingkong/base/jni_util/comm_function.h>
#include <logic/pb/base.pb.h>

pthread_key_t g_env_key;
static void __DetachCurrentThread(void* a) {
    if (NULL != VarCache::Singleton()->GetJvm()) {
        VarCache::Singleton()->GetJvm()->DetachCurrentThread();
    }
}

template <typename T>
std::vector<T>& BOOT_REGISTER_CONTAINER()
{
    static std::vector<T> s_register;
    return s_register;
}

JavaVM* g_jvm;

template<class T>
::google::protobuf::Message* DecodePbMessage(const std::string& buffer){
    ::google::protobuf::Message* result = new T;
    result->ParseFromString(buffer);
    return result;
}


#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved) {
    g_jvm = jvm;
    if (0 != pthread_key_create(&g_env_key, __DetachCurrentThread)) {
        __android_log_print(ANDROID_LOG_ERROR, "SuperSocks5Proxy", "create g_env_key fail");
        return (-1);
    }

    ScopeJEnv jenv(jvm);
    VarCache::Singleton()->SetJvm(jvm);


    LoadClass(jenv.GetEnv());
    LoadStaticMethod(jenv.GetEnv());
    LoadMethod(jenv.GetEnv());

    std::vector<JniOnload_t>& ref = BOOT_REGISTER_CONTAINER<JniOnload_t>();
    for (std::vector<JniOnload_t>::const_iterator it = ref.begin(); it != ref.end(); ++it) {
        it->func(jvm, reserved);
    }
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
    VarCache::Release();
}
/*
* Class:     com_mvvm_MVVMHelp
* Method:    Init
* Signature: (Ljava/lang/String;)V
*/
JNIEXPORT void JNICALL Java_com_mvvm_MVVMHelp_Init
        (JNIEnv *env, jclass, jstring data_dir){
    std::string str_data_dir = ScopedJstring(env,data_dir).GetChar();
    Application::get_mutable_instance().Init(str_data_dir);
}

/*
 * Class:     com_mvvm_MVVMHelp
 * Method:    UnInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_mvvm_MVVMHelp_UnInit
        (JNIEnv *, jclass){
    Application::get_mutable_instance().UnInit();
}

JNIEXPORT jlong JNICALL Java_com_mvvm_MVVMHelp_NativeCreateViewModel
        (JNIEnv *env, jclass, jint viewmode_type) {
    ActivityView* aview = new ActivityView;
    aview->CreateVM(viewmode_type);
    LogDebug << "viewmode_type:"<<viewmode_type << " aview:"<<aview;
    return (int64_t)aview;
}

/*
 * Class:     com_mvvm_MVVMHelp
 * Method:    NativeBindViewModel
 * Signature: (J)J
 */
JNIEXPORT void JNICALL Java_com_mvvm_MVVMHelp_NativeBindViewModel
        (JNIEnv *, jclass, jlong instance) {
    ActivityView* aview = (ActivityView*)instance;
    aview->BindVM();
}

JNIEXPORT void JNICALL Java_com_mvvm_MVVMHelp_NativeUnbindViewModel
        (JNIEnv *, jclass, jlong instance) {
    ActivityView* aview = (ActivityView*)instance;
    aview->UnBindVM();
    delete aview;
}

/*
 * Class:     com_mvvm_MVVMHelp
 * Method:    NativeHandleEvent
 * Signature: (JI[B)V
 */
JNIEXPORT void JNICALL Java_com_mvvm_MVVMHelp_NativeHandleEvent__JI_3B
        (JNIEnv *env, jclass, jlong instance, jint event_id, jbyteArray buffer){
    std::string pb_buffer = JNU_JbyteArray2Buffer(env, buffer);
    ActivityView* aview = (ActivityView*)instance;
    ::google::protobuf::Message* pb = nullptr;
    switch (event_id){
      case Socks5Server::Event ::kEventAddSocks5Server:
        {
            pb = DecodePbMessage<mvvm::mvvm_Socks5Info>( pb_buffer);
        }
            break;
        case Socks5Server::Event ::kEventDeleteSocks5Server:
        case SelectApp::Event::kEventGetSelectApp:
        {
            pb = DecodePbMessage<mvvm::mvvm_ParamInt32>( pb_buffer);
        }
        break;
        case SelectApp::Event::kEventUpdateSelectApp :
        {
            pb = DecodePbMessage<mvvm::mvvm_AppInfoList>( pb_buffer);
        }
        break;
      case Socks5Server::Event::kEventClearSocks5Server :{
      }
        break;
        default:{
            BOOST_VERIFY(false);
        }
        break;
    }

    aview->HandleEvent(event_id, pb);
    delete pb;
}
#ifdef __cplusplus
}
#endif

