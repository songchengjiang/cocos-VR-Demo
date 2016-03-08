#include "OVRHelper-android.h"

JavaVM * OVRHelper::javaVM = nullptr;
jobject OVRHelper::activity = 0;
#if DEEPOON
dpnnInstance OVRHelper::dpInstance = 0;
#endif

OVRHelper::OVRHelper()
{

}

OVRHelper::~OVRHelper()
{

}

void cocos_android_app_onCreate(JavaVM *vm, JNIEnv* env, jobject activity) {
	CCLOG("cocos_android_app_onCreate Begin");
	env->GetJavaVM(&OVRHelper::javaVM);
	//OVRHelper::java.Vm->AttachCurrentThread(&OVRHelper::java.Env, NULL);
	OVRHelper::activity = env->NewGlobalRef(activity);

#if GEAR_VR
	ovrJava java;
	java.Vm = OVRHelper::javaVM;
	java.Vm->AttachCurrentThread(&java.Env, NULL);
	java.ActivityObject = OVRHelper::activity;
	SystemActivities_Init(&java);
	const ovrInitParms initParms = vrapi_DefaultInitParms(&java);
	int32_t initResult = vrapi_Initialize(&initParms);
	if (initResult != VRAPI_INITIALIZE_SUCCESS)
	{
		char const * msg = initResult == VRAPI_INITIALIZE_PERMISSIONS_ERROR ?
			"Thread priority security exception. Make sure the APK is signed." :
			"VrApi initialization error.";
		SystemActivities_DisplayError(&java, SYSTEM_ACTIVITIES_FATAL_ERROR_OSIG, __FILE__, msg);
	}
#endif

#if DEEPOON
	OVRHelper::dpInstance = dpnnInit(1, DPNN_UM_DEFAULT, NULL, DPNN_DEVICE_GLES2, (void*)OVRHelper::activity);
#endif
	CCLOG("cocos_android_app_onCreate End");
}
