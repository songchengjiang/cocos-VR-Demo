#include "Tank.h"

USING_NS_CC;

Tank::Tank()
	: _cannonStageAngle(180.0f)
	, _cannonGunAngle(0.0f)
{

}

bool Tank::init()
{
	Sprite3D::initWithFile("models/tank.c3b");
	this->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	this->setPosition3D(Vec3::ZERO);
	this->setScale(1.0f);

	//auto aabb = this->getAABBRecursively();
	//Vec3 size = Vec3(aabb._max - aabb._min);
	//Physics3DRigidBodyDes rigidDes;
	//rigidDes.mass = 39700.0f;//ton
	//rigidDes.originalTransform.setIdentity();
	//rigidDes.shape = Physics3DShape::createBox(size);
	//_rigid = Physics3DRigidBody::create(&rigidDes);
	//_componet = Physics3DComponent::create(_rigid, Vec3(0.0f, -size.y * 0.5f, -size.z * 0.2f), Quaternion());
	//this->addComponent(_componet);
	//_componet->syncNodeToPhysics();
	//_componet->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);


	_cannon = this->getChildByName("cannon");
	_gunfire = PUParticleSystem3D::create("effects/Particle3D/scripts/gunfire.pu");
	_cannon->getChildByName("gun")->getChildByName("gunfire")->addChild(_gunfire);
	return true;
}

void Tank::shot()
{
	_gunfire->startParticleSystem();
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
	auto gun = _cannon->getChildByName("gun");
	gun->setRotation3D(Vec3(-_cannonGunAngle, 0.0f, 0.0f));
}

void Tank::move(float force)
{
	//Vec3 f = this->getRotationQuat() * Vec3(0.0f, 0.0f, -force);
	//_rigid->applyCentralForce(f);
	this->setPosition3D(this->getPosition3D() + this->getRotationQuat() * Vec3(0.0f, 0.0f, -force));
}

void Tank::turnLeft(float torque)
{
	auto rot = Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(torque));
	this->setRotationQuat(this->getRotationQuat() * rot);
}

void Tank::turnRight(float torque)
{
	auto rot = Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(-torque));
	this->setRotationQuat(this->getRotationQuat() * rot);
}
