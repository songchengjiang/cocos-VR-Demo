#include "HelloWorldScene.h"
#include "Tank.h"
#include "OVRRenderer.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
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


	auto ovrRenderer = OVRRenderer::create(CameraFlag::USER1);
	this->addChild(ovrRenderer);

	auto camera = Camera::createPerspective(30.0f, visibleSize.width / visibleSize.height, 1.0f, 1000.0f);
	camera->setPosition3D(Vec3(0.0f, 0.0f, 1.0f));
	camera->lookAt(Vec3::ZERO);
	camera->setCameraFlag(CameraFlag::USER1);
	//this->addChild(camera);

	auto tank = Tank::create();

	tank->setRotation3D(Vec3(0.0f, 180.0f, 0.0f));
	tank->setPosition3D(Vec3::ZERO);
	tank->setScale(1.0f);
	tank->setCameraMask((unsigned short)CameraFlag::USER1);
	this->addChild(tank);

	static bool rotateON = false;
	static float x = 0.0;
	static float y = 0.0;
	auto mouse = EventListenerMouse::create();
	mouse->onMouseDown = [tank](EventMouse* event) {
		rotateON = true;
		x = event->getCursorX();
		y = event->getCursorY();
	};
	mouse->onMouseUp= [tank](EventMouse* event) {
		rotateON = false;
	};
	mouse->onMouseMove = [tank](EventMouse* event) {
		if (rotateON) {
			float deltaX = event->getCursorX() - x;
			float deltaY = event->getCursorY() - y;
			x = event->getCursorX();
			y = event->getCursorY();
			tank->rotateCannonStage(-deltaX);
			tank->rotateCannonGun(deltaY);
			CCLOG("(%f, %f)", deltaX, deltaY);
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouse, this);

	auto keyboard = EventListenerKeyboard::create();
	keyboard->onKeyPressed = [tank](EventKeyboard::KeyCode code, Event *event) {
		if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
			tank->rotateCannonStage(5.0f);
		}
		else if (code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
			tank->rotateCannonStage(-5.0f);
		}
		else if (code == EventKeyboard::KeyCode::KEY_UP_ARROW) {
			tank->rotateCannonGun(1.0f);
		}
		else if (code == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
			tank->rotateCannonGun(-1.0f);
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, this);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
