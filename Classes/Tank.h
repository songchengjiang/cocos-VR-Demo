#ifndef __TANK_H__
#define __TANK_H__

#include "cocos2d.h"
#include "extensions/Particle3D/PU/CCPUParticleSystem3D.h"
#include "physics3d/CCPhysics3D.h"

class Tank : public cocos2d::Sprite3D
{
public:

	CREATE_FUNC(Tank);

	virtual bool init();
	virtual void update(float delta) override;

	bool move(float force);
	void turn(float torque);
	bool shot(float speed);
	bool shot(const cocos2d::Vec3 &target, float speed);
	void rotateCannonStage(float angle);
	void rotateCannonGun(float angle);
	void setTexture(cocos2d::Texture2D *texture);
	float getCannonStageAngle() const { return _cannonStageAngle; }
	float getCannonGunAngle() const { return _cannonGunAngle; }
	
	void setAttackCallback(const cocos2d::Physics3DObject::CollisionCallbackFunc &func) { _callBack = func; }

	void setHP(float hp);
	float getHP() const { return _hp; }

private:

	Tank();
	void shotBullet(float speed);

private:

	cocos2d::Vec3 _moveTarget;
	cocos2d::Node *_cannon;
	float _cannonStageAngle;
	float _cannonGunAngle;
	float _hp;
	float _latestShootTime;
	bool  _needUpdateCannonStage;
	bool  _needUpdateGunAngle;

	cocos2d::PUParticleSystem3D *_gunfire;
	cocos2d::Physics3DObject::CollisionCallbackFunc _callBack;
};

#endif // __HELLOWORLD_SCENE_H__
