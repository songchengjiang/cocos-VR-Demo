#include "HelloWorldScene.h"
#include "Tank.h"
#include "OVRRenderer.h"
#include "PlayerController.h"
#include "physics3d/CCPhysics3D.h"

USING_NS_CC;

#define HP_REDUCE_VALUE 10
#define HP_LOWER_LIMIT  50

#define SMOKE_PS_TAG  0x0F
#define TARGET_TAG    0xFF

#define WALKING   1
#define TRACKING  2
#define ESCAPING  3
#define DEAD      4

unsigned short ENEMY_STATE = WALKING;

Scene* HelloWorld::createScene()
{
    // 'layer' is an autorelease object
    auto scene = HelloWorld::create();
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	if (!initWithPhysics())
	{
		return false;
	}

	//getPhysics3DWorld()->setDebugDrawEnable(true);


    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

	//{
	//	auto camera = Camera::createPerspective(30.0f, visibleSize.width / visibleSize.height, 1.0f, 1000.0f);
	//	camera->setPosition3D(Vec3(0.0f, 0.0f, 50.0f));
	//	camera->lookAt(Vec3::ZERO);
	//	camera->setCameraFlag(CameraFlag::USER1);
	//	this->addChild(camera);
	//}

	//{
	//	auto camera = Camera::createPerspective(30.0f, visibleSize.width / visibleSize.height, 1.0f, 5000.0f);
	//	camera->setPosition3D(Vec3(10.0f, -50.0f, -3.0f));
	//	camera->lookAt(Vec3(0.0f, -50.0f, -3.0f));
	//	camera->setCameraFlag(CameraFlag::USER1);
	//	this->addChild(camera);
	//	this->setPhysics3DDebugCamera(camera);
	//}

	_player = Tank::create();
	_player->setPosition3D(Vec3(0.0f, -50.0f, 120.0f));
	_player->setCameraMask((unsigned short)CameraFlag::USER1);
	this->addChild(_player);

	_crashTexture = Director::getInstance()->getTextureCache()->addImage("models/tank/TexturesMods/Berezka/T-54_crash.png");
	generateEnemy();

	auto ovrRenderer = OVRRenderer::create(CameraFlag::USER1);
	_player->addChild(ovrRenderer);
	ovrRenderer->setOffsetPos(Vec3(0.0f, 2.7f, 0.0f));

	auto pc = PlayerController::create();
	this->addChild(pc);
	pc->setPlayer(_player);
	pc->setOVRRenderer(ovrRenderer);

	auto skybox = Sprite3D::create("effects/skybox/skybox.c3t");
	skybox->setCameraMask((unsigned short)CameraFlag::USER1);
	skybox->setScale(1000.0f);
	this->addChild(skybox);

	Terrain::DetailMap r("models/terrain/dirt.jpg"), g("models/terrain/Grass2.jpg"), b("models/terrain/rock.png"), a("models/terrain/snow.png");

	Terrain::TerrainData data("models/terrain/terrain.png", "models/terrain/alphamap.png", r, g, b, a);
	data._mapHeight = 100.0;
	data._mapScale = 1.0f;

	auto terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
	terrain->setLODDistance(3.2f, 6.4f, 9.6f);
	terrain->setMaxDetailMapAmount(4);
	this->addChild(terrain);
	terrain->setCameraMask((unsigned short)CameraFlag::USER1);
	terrain->setDrawWire(false);
	terrain->setLightDir(-Vec3::UNIT_Y);

	std::vector<float> heidata = terrain->getHeightData();
	auto size = terrain->getTerrainSize();
	Physics3DColliderDes colliderDes;
	colliderDes.shape = Physics3DShape::createHeightfield(size.width, size.height, &heidata[0], 1.0f, -50.0f, 50.0f, true, false, true);
	_terrainCollider = Physics3DCollider::create(&colliderDes);
	auto component = Physics3DComponent::create(_terrainCollider);
	terrain->addComponent(component);
	component->syncNodeToPhysics();
	component->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NONE);


	//Physics3DRigidBodyDes rbDes;
	//rbDes.mass = 10.0f;
	//rbDes.shape = Physics3DShape::createBox(Vec3(1.0f, 1.0f, 1.0f));
	//auto box = PhysicsSprite3D::create("models/box.c3t", &rbDes);
	//box->setTexture("models/crate6.png");
	//box->setPosition3D(Vec3(0.0f, -45.0f, -20.0f));
	//box->syncNodeToPhysics();
	//box->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
	//box->setCameraMask((unsigned short)CameraFlag::USER1);
	//box->setScale(1.0f);
	//this->addChild(box);


	_player->setAttackCallback([=](const Physics3DCollisionInfo &ci) {
		if (ci.objB == _terrainCollider) {
			PUParticleSystem3D *ps = PUParticleSystem3D::create("effects/Particle3D/scripts/dirtExplosion.pu");
			ps->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([ps]() {
				ps->removeFromParent();
			}), nullptr));
			ps->setPosition3D(ci.collisionPointList[0].worldPositionOnB);
			ps->startParticleSystem();
			ps->setCameraMask((unsigned short)CameraFlag::USER1);
			this->addChild(ps);
		}
		else {
			PUParticleSystem3D *ps = PUParticleSystem3D::create("effects/Particle3D/scripts/metalExplosion.pu");
			ps->setScale(0.5f);
			ps->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([ps]() {
				ps->removeFromParent();
			}), nullptr));
			ps->setPosition3D(ci.collisionPointList[0].worldPositionOnB);
			ps->startParticleSystem();
			ps->setCameraMask((unsigned short)CameraFlag::USER1);
			this->addChild(ps);

			_enemy->setHP(_enemy->getHP() - HP_REDUCE_VALUE);

			if (_enemy->getHP() <= HP_LOWER_LIMIT) {
				if (!_enemy->getChildByTag(SMOKE_PS_TAG)) {
					auto smoke = PUParticleSystem3D::create("effects/Particle3D/scripts/smoke.pu");
					smoke->setScale(0.05f);
					smoke->startParticleSystem();
					smoke->setCameraMask(_enemy->getCameraMask());
					_enemy->addChild(smoke, 0, SMOKE_PS_TAG);

					_enemy->setTexture(_crashTexture);
				}
			}

			if (_enemy->getHP() == 0) {
				auto enemy = _enemy;
				_enemy->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([enemy]() {
					enemy->removeFromParent();
				}), nullptr));

				auto explosion = PUParticleSystem3D::create("effects/Particle3D/scripts/explosionSystem.pu");
				explosion->setScale(5.0f);
				explosion->startParticleSystem();
				explosion->setCameraMask(_enemy->getCameraMask());
				explosion->setPosition3D(_enemy->getPosition3D());
				this->addChild(explosion);
				explosion->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([explosion]() {
					explosion->removeFromParent();
				}), nullptr));

				this->runAction(Sequence::create(DelayTime::create(6.0f), CallFunc::create([this]() {
					this->generateEnemy();
					this->scheduleUpdate();
				}), nullptr));

				this->unscheduleUpdate();
			}
		}

		//CCLOG("(%f, %f, %f)", ci.collisionPointList[0].worldPositionOnB.x, ci.collisionPointList[0].worldPositionOnB.y, ci.collisionPointList[0].worldPositionOnB.z);
		if (_enemy) {
			if ((ci.collisionPointList[0].worldPositionOnB - _enemy->getPosition3D()).length() < 10.0f) {
				ENEMY_STATE = TRACKING;
			}
		}
	});


	scheduleUpdate();
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::update(float delta)
{
	Vec3 safeDis = _player->getPosition3D() - _enemy->getPosition3D();
	if (safeDis.length() < 50.0f) {
		_enemy->getChildByTag(TARGET_TAG)->setVisible(false);
	}
	else {
		_enemy->getChildByTag(TARGET_TAG)->setVisible(true);
	}
	enemyAI(delta);
}

void HelloWorld::enemyAI(float delta)
{
	if (_enemy->getHP() <= 50.0f) {
		ENEMY_STATE = ESCAPING;
	}

	switch (ENEMY_STATE)
	{
	case WALKING:
		enemyWalking(delta);
		break;

	case TRACKING:
		enemyTracking(delta);
		break;

	case ESCAPING:
		enemyEscaping(delta);
		break;

	case DEAD:
		CCLOG("MISSION COMPLETE!");
		break;

	default:
		break;
	}
}

void HelloWorld::enemyWalking(float delta)
{
	if (_enemy->getHP() <= 0.0f) return;
	static bool needChangeDir = false;
	static float moveTime = 0.0f;
	static float randomAngle = 0.0f;

	moveTime -= delta;
	if (moveTime <= 0.0f) {
		moveTime = CCRANDOM_0_1() * 50.0f + 50.0f;
		needChangeDir = true;
	}

	if (needChangeDir) {
		randomAngle = 2.0;
		needChangeDir = false;
	}

	if (0.0f < abs(randomAngle)) {
		_enemy->turn(randomAngle);
		float latestAngle = randomAngle;
		randomAngle -= delta * randomAngle;
		if (latestAngle * randomAngle < 0.0)
			randomAngle = 0.0f;
	}

	if (!_enemy->move(delta * 3.0f)) {
		needChangeDir = true;
	}

	Vec3 safeDis = _player->getPosition3D() - _enemy->getPosition3D();
	if (safeDis.length() < 50.0f) {
		ENEMY_STATE = TRACKING;
	}
}

void HelloWorld::enemyTracking(float delta)
{
	if (_enemy->getHP() <= 0.0f) return;

	Vec3 deltaDir = _player->getPosition3D() - _enemy->getPosition3D();
	deltaDir.normalize();

	Vec3 enemydir = _enemy->getRotationQuat() * -Vec3::UNIT_Z;
	enemydir.normalize();

	float theta = acos(clampf(enemydir.dot(deltaDir), 0.0f, 1.0f));
	Vec3 up;
	Vec3::cross(enemydir, deltaDir, &up);
	if (up.y < 0.0)
		theta = -theta;

	_enemy->turn(delta * CC_RADIANS_TO_DEGREES(theta));
	_enemy->move(delta * 3.0f);

	_enemy->shot(_player->getPosition3D(), 100.0f);
}

void HelloWorld::enemyEscaping(float delta)
{
	if (_enemy->getHP() <= 0.0f) return;

	Vec3 deltaDir = _enemy->getPosition3D() - _player->getPosition3D();
	deltaDir.normalize();

	Vec3 enemydir = _enemy->getRotationQuat() * -Vec3::UNIT_Z;
	enemydir.normalize();

	float theta = acos(clampf(enemydir.dot(deltaDir), 0.0f, 1.0f));
	Vec3 up;
	Vec3::cross(enemydir, deltaDir, &up);
	if (up.y < 0.0)
		theta = -theta;

	static float moveSpeed = 3.0f;
	if (_enemy->getHP() <= 50.0f && 30.0 < _enemy->getHP()) {
		moveSpeed = 2.0f;
	}
	else if (_enemy->getHP() <= 30.0f) {
		moveSpeed = 1.0f;
	}
	_enemy->turn(delta * CC_RADIANS_TO_DEGREES(theta));
	_enemy->move(delta * moveSpeed);

	_enemy->shot(_player->getPosition3D(), 100.0f);
}

void HelloWorld::playerUpdate(float delta)
{
	if (_player->getHP() <= 0.0) {
		CCLOG("GAME OVER!");
	}
}

void HelloWorld::generateEnemy()
{
	_enemy = Tank::create();
	_enemy->setPosition3D(Vec3(0.0f, -50.0f, -120.0f));
	_enemy->setRotation3D(Vec3(0.0f, 180.0f, 0.0f));
	auto tex = Director::getInstance()->getTextureCache()->addImage("models/tank/TexturesMods/Berezka/T-54.png");
	_enemy->setTexture(tex);
	_enemy->setCameraMask((unsigned short)CameraFlag::USER1);
	_enemy->retain();
	this->addChild(_enemy);


	auto sprite = BillBoard::create("target.png");
	sprite->setCameraMask((unsigned short)CameraFlag::USER1);
	sprite->setScale(0.05f);
	sprite->setPosition3D(Vec3(0.0f, 10.0f, 0.0f));
	_enemy->addChild(sprite, 0, TARGET_TAG);


	_enemy->setAttackCallback([=](const Physics3DCollisionInfo &ci) {
		if (ci.objB == _terrainCollider) {
			PUParticleSystem3D *ps = PUParticleSystem3D::create("effects/Particle3D/scripts/dirtExplosion.pu");
			ps->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([ps]() {
				ps->removeFromParent();
			}), nullptr));
			ps->setPosition3D(ci.collisionPointList[0].worldPositionOnB);
			ps->startParticleSystem();
			ps->setCameraMask((unsigned short)CameraFlag::USER1);
			this->addChild(ps);
		}
		else {
			PUParticleSystem3D *ps = PUParticleSystem3D::create("effects/Particle3D/scripts/metalExplosion.pu");
			ps->setScale(0.5f);
			ps->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([ps]() {
				ps->removeFromParent();
			}), nullptr));
			ps->setPosition3D(ci.collisionPointList[0].worldPositionOnB);
			ps->startParticleSystem();
			ps->setCameraMask((unsigned short)CameraFlag::USER1);
			this->addChild(ps);

			//if (ci.objB->getUserData()) {
			//	Tank *tank = static_cast<Tank *>(ci.objB->getUserData());
			//	tank->setHP(tank->getHP() - HP_REDUCE_VALUE);
			//}
		}
	});

	ENEMY_STATE = WALKING;
}
