#include "Tank.h"

USING_NS_CC;

Tank::Tank()
	: _cannonStageAngle(0.0f)
	, _cannonGunAngle(0.0f)
{

}

bool Tank::init()
{
	Sprite3D::initWithFile("models/tank.c3b");
	_cannon = this->getChildByName("cannon");
	return true;
}

void Tank::moveTo(const Vec3 &target)
{
	_moveTarget = target;
}

void Tank::shot()
{
}

void Tank::rotateCannonStage(float angle)
{
	_cannonStageAngle += angle;
	_cannon->setRotation3D(Vec3(-90.0f, _cannonStageAngle, 0.0f));
}

void Tank::rotateCannonGun(float angle)
{
	_cannonGunAngle += angle;
	if (20.0f < _cannonGunAngle) _cannonGunAngle = 20.0f;
	if (_cannonGunAngle < 0.0f) _cannonGunAngle = 0.0f;
	auto gun = _cannon->getChildByName("cannon_gun");
	gun->setRotation3D(Vec3(-_cannonGunAngle, 0.0f, 0.0f));
}
