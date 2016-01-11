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

void Tank::shot(float speed)
{
	shotBullet(speed);
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
	if (10.0f < _cannonGunAngle) _cannonGunAngle = 10.0f;
	if (_cannonGunAngle < -5.0f) _cannonGunAngle = -5.0f;
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

void Tank::shotBullet(float speed)
{
	Mat4 bulletWorldMat = _cannon->getChildByName("gun")->getChildByName("gunfire")->getNodeToWorldTransform();
	Vec3 bulletPos;
	Quaternion bulletRot;
	bulletWorldMat.decompose(nullptr, &bulletRot, &bulletPos);

	Physics3DRigidBodyDes rbDes;
	rbDes.mass = 100.f;
	rbDes.shape = Physics3DShape::createCapsule(0.1f, 0.2f);
	auto bullet = PhysicsSprite3D::create("models/bullet.c3b", &rbDes);
	bullet->setTexture("models/orange_edit.png");
	Director::getInstance()->getRunningScene()->addChild(bullet);

	Vec3 rotate = this->getRotation3D();

	Vec3 linearVel = Vec3(cos(CC_DEGREES_TO_RADIANS(_cannonGunAngle)) * sin(CC_DEGREES_TO_RADIANS(_cannonStageAngle))
		, sin(CC_DEGREES_TO_RADIANS(_cannonGunAngle))
		, cos(CC_DEGREES_TO_RADIANS(_cannonGunAngle)) * cos(CC_DEGREES_TO_RADIANS(_cannonStageAngle)));

	linearVel = this->getRotationQuat() * linearVel;

	auto rigidBody = static_cast<Physics3DRigidBody*>(bullet->getPhysicsObj());
	rigidBody->setLinearFactor(Vec3::ONE);
	rigidBody->setLinearVelocity(linearVel * speed);
	rigidBody->setAngularVelocity(Vec3::ZERO);
	rigidBody->setCcdMotionThreshold(0.5f);
	rigidBody->setCcdSweptSphereRadius(0.4f);
	//rigidBody->setMask(0xffffffff);
	rigidBody->setUserData(bullet);

	bullet->setCameraMask(this->getCameraMask());
	bullet->setPosition3D(bulletPos);
	bullet->setRotationQuat(Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(_cannonStageAngle)) * this->getRotationQuat() * Quaternion(Vec3::UNIT_X, CC_DEGREES_TO_RADIANS(-_cannonGunAngle + 85.0f)));
	//bullet->setRotation3D(this->getRotationQuat() * Vec3(0.0f, _cannonStageAngle, 0.0f) + Vec3(-_cannonGunAngle + 85.0f, 0.0f, 0.0f));
	bullet->syncNodeToPhysics();
	bullet->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);


	rigidBody->setCollisionCallback([bullet](const Physics3DCollisionInfo &ci) {
		if (!ci.collisionPointList.empty()) {
			auto ps = PUParticleSystem3D::create("effects/Particle3D/scripts/dirtExplosion.pu");
			ps->setPosition3D(ci.collisionPointList[0].worldPositionOnB);
			ps->startParticleSystem();
			ps->setCameraMask(bullet->getCameraMask());
			Director::getInstance()->getRunningScene()->addChild(ps);
			ps->runAction(Sequence::create(CallFunc::create([bullet]() {
				bullet->removeFromParent();
			}), DelayTime::create(5.0f), CallFunc::create([ps]() {
				ps->removeFromParent();
			}), nullptr));
		}
	});
}
