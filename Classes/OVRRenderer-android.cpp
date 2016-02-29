#include "OVRRenderer.h"
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;

#define NUM_MULTI_SAMPLES	4

#define RENDER_ORDER_ID 100.0f

OVRRenderer::OVRRenderer()
	: _ovr(nullptr)
	, _frameIndex(1)
{
}

OVRRenderer::~OVRRenderer()
{
	//vrapi_Shutdown();
}

bool OVRRenderer::init(cocos2d::CameraFlag flag)
{
	//OVR::System::Init();
	//// Initializes LibOVR, and the Rift
	//ovrResult result = ovr_Initialize(nullptr);
	//if (!OVR_SUCCESS(result)) {
	//	CCLOG("Failed to initialize libOVR.");
	//	return false;
	//};

	//ovrGraphicsLuid luid;
	//result = ovr_Create(&_HMD, &luid);
	//if (!OVR_SUCCESS(result)) {
	//	CCLOG("Failed to create HMD.");
	//	return false;
	//}
	//ovrHmdDesc hmdDesc = ovr_GetHmdDesc(_HMD);

	//// Make eye render buffers
	//for (int eye = 0; eye < 2; ++eye){
	//	ovrSizei idealTextureSize = ovr_GetFovTextureSize(_HMD, ovrEyeType(eye), hmdDesc.DefaultEyeFov[eye], 1);
	//	_eyeRenderTexture[eye] = new TextureBuffer(_HMD, true, true, idealTextureSize, 1, nullptr, 1);
	//	_eyeDepthBuffer[eye] = new DepthBuffer(_eyeRenderTexture[eye]->GetSize(), 0);

	//	if (!_eyeRenderTexture[eye]->TextureSet){
	//		CCLOG("Failed to create texture.");
	//		return false;
	//	}
	//}

	//// Create mirror texture and an FBO used to copy mirror texture to back buffer
	//result = ovr_CreateMirrorTextureGL(_HMD, GL_SRGB8_ALPHA8, Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height, reinterpret_cast<ovrTexture**>(&_mirrorTexture));
	//if (!OVR_SUCCESS(result)){
	//	CCLOG("Failed to create mirror texture.");
	//	return false;
	//}

	//// Configure the mirror read buffer
	//glGenFramebuffers(1, &_mirrorFBO);
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, _mirrorFBO);
	//glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _mirrorTexture->OGL.TexId, 0);
	//glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	//_eyeRenderDesc[0] = ovr_GetRenderDesc(_HMD, ovrEye_Left, hmdDesc.DefaultEyeFov[0]);
	//_eyeRenderDesc[1] = ovr_GetRenderDesc(_HMD, ovrEye_Right, hmdDesc.DefaultEyeFov[1]);

	//for (int eye = 0; eye < 2; ++eye) {
	//	float fov = CC_RADIANS_TO_DEGREES(atan(hmdDesc.DefaultEyeFov[eye].UpTan) + atan(hmdDesc.DefaultEyeFov[eye].DownTan));
	//	float aspectRadio = (hmdDesc.DefaultEyeFov[eye].LeftTan + hmdDesc.DefaultEyeFov[eye].RightTan) / (hmdDesc.DefaultEyeFov[eye].UpTan + hmdDesc.DefaultEyeFov[eye].DownTan);
	//	_eyeCamera[eye] = Camera::createPerspective(fov, aspectRadio, 0.2f, 5000.0f);
	//	_eyeCamera[eye]->setCameraFlag(flag);
	//	//_eyeCamera[eye]->setDepth(eye + 1);
	//	this->addChild(_eyeCamera[eye]);
	//}

	//this->setCameraMask((unsigned short)CameraFlag::USER1);
	//_ld.Header.Type  = ovrLayerType_EyeFov;
	//_ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;   // Because OpenGL.


	// Create an EGLContext and get the suggested FOV and suggested
	// resolution to setup a projection matrix and eye texture swap chains.

	_java.Vm = OVRHelper::java.Vm;
	_java.Vm->AttachCurrentThread(&_java.Env, NULL);
	_java.ActivityObject = OVRHelper::java.ActivityObject;

	const float suggestedEyeFovDegreesX = vrapi_GetSystemPropertyFloat(&_java, VRAPI_SYS_PROP_SUGGESTED_EYE_FOV_DEGREES_X);
	const float suggestedEyeFovDegreesY = vrapi_GetSystemPropertyFloat(&_java, VRAPI_SYS_PROP_SUGGESTED_EYE_FOV_DEGREES_Y);
	const float halfWidth = VRAPI_ZNEAR * tanf(suggestedEyeFovDegreesX * (VRAPI_PI / 180.0f * 0.5f));
	const float halfHeight = VRAPI_ZNEAR * tanf(suggestedEyeFovDegreesY * (VRAPI_PI / 180.0f * 0.5f));

	for (int eye = 0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++)
	{
		_eyeCamera[eye] = Camera::createPerspective(suggestedEyeFovDegreesY, halfWidth / halfHeight, VRAPI_ZNEAR, 5000.0f);
		_eyeCamera[eye]->setCameraFlag(flag);
		this->addChild(_eyeCamera[eye]);
	}

	// Setup a projection matrix based on the 'ovrHmdInfo'.
	_projectionMatrix = ovrMatrix4f_CreateProjectionFov(suggestedEyeFovDegreesX,
		suggestedEyeFovDegreesY,
		0.0f, 0.0f, VRAPI_ZNEAR, 5000.0f);

	// Create the frame buffers.
	CCLOG("OVRRenderer::ovrFramebuffer_Create");
	for (int eye = 0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++)
	{
		ovrFramebuffer_Clear(&_frameBuffer[eye]);
		ovrFramebuffer_Create(&_frameBuffer[eye],
			VRAPI_TEXTURE_FORMAT_8888,
			vrapi_GetSystemPropertyInt(&_java, VRAPI_SYS_PROP_SUGGESTED_EYE_TEXTURE_WIDTH),
			vrapi_GetSystemPropertyInt(&_java, VRAPI_SYS_PROP_SUGGESTED_EYE_TEXTURE_HEIGHT),
			NUM_MULTI_SAMPLES);
	}

	auto backToForegroundListener = EventListenerCustom::create(EVENT_COME_TO_FOREGROUND,
		[=](EventCustom*)
	{
		CCLOG("OVRRenderer::vrapi_EnterVrMode");
		ovrModeParms modeParms = vrapi_DefaultModeParms(&_java);
		_ovr = vrapi_EnterVrMode(&modeParms);
	}
	);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(backToForegroundListener, -1);

	auto foregroundToBackListener = EventListenerCustom::create(EVENT_COME_TO_BACKGROUND,
		[=](EventCustom*)
	{
		CCLOG("OVRRenderer::vrapi_LeaveVrMode");
		vrapi_LeaveVrMode(_ovr);
		_ovr = nullptr;
	}
	);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(foregroundToBackListener, -1);


	update(0.0f);
	scheduleUpdate();
	return true;
}

OVRRenderer* OVRRenderer::create(cocos2d::CameraFlag flag)
{
	auto renderer = new OVRRenderer;
	if (!renderer->init(flag)) {
		delete renderer;
		return nullptr;
	}

	renderer->autorelease();
	return renderer;
}

void OVRRenderer::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	if (!_ovr) return;
	if (Camera::getVisitingCamera() == _eyeCamera[0] || Camera::getVisitingCamera() == _eyeCamera[1]) {
		_beginRenderCommand.init(-RENDER_ORDER_ID);
		_beginRenderCommand.func = CC_CALLBACK_0(OVRRenderer::onBeginDraw, this);
		renderer->addCommand(&_beginRenderCommand);

		_endRenderCommand.init(RENDER_ORDER_ID);
		_endRenderCommand.func = CC_CALLBACK_0(OVRRenderer::onEndDraw, this);
		renderer->addCommand(&_endRenderCommand);
	}
}

void OVRRenderer::onBeginDraw()
{
	glEnable(GL_SCISSOR_TEST);
	glDepthMask(true);
	int eye = Camera::getVisitingCamera() == _eyeCamera[0] ? 0 : 1;
	ovrFramebuffer * frameBuffer = &_frameBuffer[eye];
	ovrFramebuffer_SetCurrent(frameBuffer);
	glScissor(0, 0, frameBuffer->Width, frameBuffer->Height);
	glViewport(0, 0, frameBuffer->Width, frameBuffer->Height);
	glClearColor(0.125f, 0.0f, 0.125f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void OVRRenderer::onEndDraw()
{
	int eye = Camera::getVisitingCamera() == _eyeCamera[0] ? 0 : 1;
	ovrFramebuffer * frameBuffer = &_frameBuffer[eye];

	// Explicitly clear the border texels to black because OpenGL-ES does not support GL_CLAMP_TO_BORDER.
	{
		// Clear to fully opaque black.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// bottom
		glScissor(0, 0, frameBuffer->Width, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		// top
		glScissor(0, frameBuffer->Height - 1, frameBuffer->Width, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		// left
		glScissor(0, 0, 1, frameBuffer->Height);
		glClear(GL_COLOR_BUFFER_BIT);
		// right
		glScissor(frameBuffer->Width - 1, 0, 1, frameBuffer->Height);
		glClear(GL_COLOR_BUFFER_BIT);
	}


	ovrFramebuffer_Resolve(frameBuffer);

	_frameParams.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].ColorTextureSwapChain = frameBuffer->ColorTextureSwapChain;
	_frameParams.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].TextureSwapChainIndex = frameBuffer->TextureSwapChainIndex;
	_frameParams.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].TexCoordsFromTanAngles = ovrMatrix4f_TanAngleMatrixFromProjection(&_projectionMatrix);
	_frameParams.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Textures[eye].HeadPose = _tracking.HeadPose;
	ovrFramebuffer_Advance(frameBuffer);

	glDepthMask(false);
	glDisable(GL_SCISSOR_TEST);

	if (eye == 1){
		ovrFramebuffer_SetNone();
		vrapi_SubmitFrame(_ovr, &_frameParams);
	}
}

void OVRRenderer::update(float delta)
{
	if (!_ovr) return;
	_frameIndex++;
	// Get the HMD pose, predicted for the middle of the time period during which
	// the new eye images will be displayed. The number of frames predicted ahead
	// depends on the pipeline depth of the engine and the synthesis rate.
	// The better the prediction, the less black will be pulled in at the edges.
	const double predictedDisplayTime = vrapi_GetPredictedDisplayTime(_ovr, _frameIndex);
	const ovrTracking baseTracking = vrapi_GetPredictedTracking(_ovr, predictedDisplayTime);
	// Apply the head-on-a-stick model if there is no positional tracking.
	const ovrHeadModelParms headModelParms = vrapi_DefaultHeadModelParms();
	_tracking = vrapi_ApplyHeadModel(&headModelParms, &baseTracking);
	const ovrMatrix4f centerEyeViewMatrix = vrapi_GetCenterEyeViewMatrix(&headModelParms, &_tracking, NULL);

	for (int eye = 0; eye < VRAPI_FRAME_LAYER_EYE_MAX; eye++){
		const ovrMatrix4f eyeViewMatrix = vrapi_GetEyeViewMatrix(&headModelParms, &centerEyeViewMatrix, eye);
		Mat4 viewMat;
		viewMat.set((const GLfloat *)(ovrMatrix4f_Transpose(&eyeViewMatrix).M[0]));
		viewMat.inverse();

		Quaternion quat;
		Vec3 pos;
		viewMat.decompose(nullptr, &quat, &pos);
		quat = _offsetRot * quat;
		Vec3 up = quat * Vec3::UNIT_Y;
		Vec3 forword = quat * -Vec3::UNIT_Z;
		Vec3 shiftedEyePos = _offsetPos + _offsetRot * pos;
		_eyeCamera[eye]->setPosition3D(shiftedEyePos);
		_eyeCamera[eye]->lookAt(shiftedEyePos + forword, up);
		//CCLOG("OVRRenderer::camera[%d]-ROT(%f, %f, %f, %f) -POS(%f, %f, %f)", eye, quat.w, quat.x, quat.y, quat.z, pos.x, pos.y, pos.z);
	}

	_frameParams = vrapi_DefaultFrameParms(&_java, VRAPI_FRAME_INIT_DEFAULT, vrapi_GetTimeInSeconds(), NULL);
	_frameParams.FrameIndex = _frameIndex;
	_frameParams.MinimumVsyncs = 1;
	_frameParams.PerformanceParms = vrapi_DefaultPerformanceParms();
	_frameParams.Layers[VRAPI_FRAME_LAYER_TYPE_WORLD].Flags |= VRAPI_FRAME_LAYER_FLAG_CHROMATIC_ABERRATION_CORRECTION;

	CCLOG("OVRRenderer::update");
}

void OVRRenderer::setOffsetPos(const cocos2d::Vec3 &pos)
{
	_offsetPos += pos;
}

void OVRRenderer::setOffsetRot(const cocos2d::Quaternion &rot)
{
	_offsetRot *= rot;
}