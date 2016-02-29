#ifndef __OVR_RENDERER_H__
#define __OVR_RENDERER_H__
#include "cocos2d.h"
#include "OVRHelper-android.h"

class OVRRenderer : public cocos2d::Node
{
public:
	OVRRenderer();
	~OVRRenderer();

	static OVRRenderer* create(cocos2d::CameraFlag flag);

	virtual void update(float delta) override;
	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
	void setOffsetPos(const cocos2d::Vec3 &pos);
	void setOffsetRot(const cocos2d::Quaternion &rot);

private:

	bool init(cocos2d::CameraFlag flag);
	void onBeginDraw();
	void onEndDraw();

private:

	cocos2d::Camera *_eyeCamera[VRAPI_FRAME_LAYER_EYE_MAX];
	cocos2d::Vec3       _offsetPos;
	cocos2d::Quaternion _offsetRot;

	ovrFramebuffer  _frameBuffer[VRAPI_FRAME_LAYER_EYE_MAX];
	ovrMobile      *_ovr;
	ovrJava         _java;
	long long       _frameIndex;
	ovrMatrix4f     _projectionMatrix;
	ovrTracking     _tracking;
	ovrFrameParms   _frameParams;


	cocos2d::CustomCommand _beginRenderCommand;
	cocos2d::CustomCommand _endRenderCommand;
};

#endif