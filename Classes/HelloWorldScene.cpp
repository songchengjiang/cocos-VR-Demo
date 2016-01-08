#include "HelloWorldScene.h"
#include "Tank.h"
#include "OVRRenderer.h"
#include "PlayerController.h"
#include "physics3d/CCPhysics3D.h"

USING_NS_CC;

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

	auto pc = PlayerController::create();
	this->addChild(pc);

	auto tank = Tank::create();
	tank->setPosition3D(Vec3(0.0f, -50.0f, 0.0f));
	//tank->getPhysics3DComponent()->syncNodeToPhysics();
	tank->setCameraMask((unsigned short)CameraFlag::USER1);
	this->addChild(tank);
	pc->setPlayer(tank);

	auto ovrRenderer = OVRRenderer::create(CameraFlag::USER1);
	tank->addChild(ovrRenderer);
	ovrRenderer->setOffsetPos(Vec3(0.0f, 2.7f, 0.0f));
	pc->setOVRRenderer(ovrRenderer);

	//auto skybox = Skybox::create("effects/skybox/left.jpg", "effects/skybox/right.jpg", "effects/skybox/top.jpg", "effects/skybox/bottom.jpg", "effects/skybox/front.jpg", "effects/skybox/back.jpg");
	//skybox->setCameraMask((unsigned short)CameraFlag::USER1);
	//this->addChild(skybox);

	auto skybox = Sprite3D::create("effects/skybox/skybox.c3t");
	skybox->setCameraMask((unsigned short)CameraFlag::USER1);
	skybox->setScale(1000.0f);
	this->addChild(skybox);
	//static bool rotateON = false;
	//static float x = 0.0;
	//static float y = 0.0;
	//auto mouse = EventListenerMouse::create();
	//mouse->onMouseDown = [tank](EventMouse* event) {
	//	rotateON = true;
	//	x = event->getCursorX();
	//	y = event->getCursorY();
	//};
	//mouse->onMouseUp = [tank](EventMouse* event) {
	//	rotateON = false;
	//};
	//mouse->onMouseMove = [tank, ovrRenderer](EventMouse* event) {
	//	if (rotateON) {
	//		float deltaX = (event->getCursorX() - x) * 0.1;
	//		float deltaY = (event->getCursorY() - y) * 0.1;
	//		x = event->getCursorX();
	//		y = event->getCursorY();
	//		tank->rotateCannonStage(-deltaX);
	//		//tank->rotateCannonGun(deltaY);
	//		ovrRenderer->setOffsetRot(Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(-deltaX))/* * Quaternion(Vec3::UNIT_X, deltaY * 0.01)*/);
	//		CCLOG("(%f, %f)", deltaX, deltaY);
	//	}
	//};
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(mouse, this);

	//auto keyboard = EventListenerKeyboard::create();
	//keyboard->onKeyPressed = [tank, ovrRenderer](EventKeyboard::KeyCode code, Event *event) {
	//	if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
	//		tank->rotateCannonStage(5.0f);
	//	}
	//	else if (code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
	//		tank->rotateCannonStage(-5.0f);
	//	}
	//	else if (code == EventKeyboard::KeyCode::KEY_UP_ARROW) {
	//		tank->rotateCannonGun(1.0f);
	//	}
	//	else if (code == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
	//		tank->rotateCannonGun(-1.0f);
	//	}

	//	if (code == EventKeyboard::KeyCode::KEY_A) {
	//		ovrRenderer->setOffsetPos(-Vec3::UNIT_X * 0.1);
	//	}
	//	else if (code == EventKeyboard::KeyCode::KEY_D) {
	//		ovrRenderer->setOffsetPos(Vec3::UNIT_X * 0.1);
	//	}
	//	else if (code == EventKeyboard::KeyCode::KEY_W) {
	//		ovrRenderer->setOffsetPos(-Vec3::UNIT_Z * 0.1);
	//	}
	//	else if (code == EventKeyboard::KeyCode::KEY_S) {
	//		ovrRenderer->setOffsetPos(Vec3::UNIT_Z * 0.1);
	//	}
	//	//else if (code == EventKeyboard::KeyCode::KEY_HOME) {
	//	//	ovrRenderer->setOffsetPos(Vec3::UNIT_Y * 0.1);
	//	//}
	//	//else if (code == EventKeyboard::KeyCode::KEY_END) {
	//	//	ovrRenderer->setOffsetPos(-Vec3::UNIT_Y * 0.1);
	//	//}
	//};
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, this);


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
	auto collider = Physics3DCollider::create(&colliderDes);
	auto component = Physics3DComponent::create(collider);
	terrain->addComponent(component);
	component->syncNodeToPhysics();
	component->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NONE);


    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
