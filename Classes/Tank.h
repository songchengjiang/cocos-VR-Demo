#ifndef __TANK_H__
#define __TANK_H__

#include "cocos2d.h"

class Tank : public cocos2d::Sprite3D
{
public:

	CREATE_FUNC(Tank);

	virtual bool init();

	void moveTo(const cocos2d::Vec3 &target);
	void shot();
	void rotateCannonStage(float angle);
	void rotateCannonGun(float angle);

private:

	Tank();

private:

	cocos2d::Vec3 _moveTarget;
	cocos2d::Node *_cannon;
	float _cannonStageAngle;
	float _cannonGunAngle;
};

#endif // __HELLOWORLD_SCENE_H__
