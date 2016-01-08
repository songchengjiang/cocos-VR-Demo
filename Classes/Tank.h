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

	void move(float force);
	void turnLeft(float torque);
	void turnRight(float torque);
	void shot();
	void rotateCannonStage(float angle);
	void rotateCannonGun(float angle);

	cocos2d::Physics3DComponent* getPhysics3DComponent() { return _componet; }


private:

	Tank();

private:

	cocos2d::Vec3 _moveTarget;
	cocos2d::Node *_cannon;
	float _cannonStageAngle;
	float _cannonGunAngle;

	cocos2d::Physics3DRigidBody *_rigid;
	cocos2d::Physics3DComponent *_componet;

	cocos2d::PUParticleSystem3D *_gunfire;
};

#endif // __HELLOWORLD_SCENE_H__
