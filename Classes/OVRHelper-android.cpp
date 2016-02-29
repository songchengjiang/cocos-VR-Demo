#include "OVRHelper-android.h"

ovrJava OVRHelper::java;

OVRHelper::OVRHelper()
{

}

OVRHelper::~OVRHelper()
{

}

void cocos_android_app_onCreate(JavaVM *vm, JNIEnv* env, jobject activity) {
	env->GetJavaVM(&OVRHelper::java.Vm);
	OVRHelper::java.Vm->AttachCurrentThread(&OVRHelper::java.Env, NULL);
	OVRHelper::java.ActivityObject = env->NewGlobalRef(activity);

	SystemActivities_Init(&OVRHelper::java);
	const ovrInitParms initParms = vrapi_DefaultInitParms(&OVRHelper::java);
	int32_t initResult = vrapi_Initialize(&initParms);
	if (initResult != VRAPI_INITIALIZE_SUCCESS)
	{
		char const * msg = initResult == VRAPI_INITIALIZE_PERMISSIONS_ERROR ?
			"Thread priority security exception. Make sure the APK is signed." :
			"VrApi initialization error.";
		SystemActivities_DisplayError(&OVRHelper::java, SYSTEM_ACTIVITIES_FATAL_ERROR_OSIG, __FILE__, msg);
	}
}
