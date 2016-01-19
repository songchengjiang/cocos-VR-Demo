#include "PlayerController.h"
#include "OVRRenderer.h"
#include "Tank.h"

USING_NS_CC;

#define CANNON_STAGE_ROTATE_SPEED     2.0f
#define CANNON_STAGE_MAX_ROTATE_SPEED 1.0f
#define CANNON_GUN_ROTATE_SPEED       3.0f
#define CANNON_GUN_MAX_ROTATE_SPEED   0.3f
#define TANK_MOVE_SPEED           0.2f
#define TANK_MAX_MOVE_SPEED       0.3f

//#define TANK_MOVE_FORCE           5.0f
//#define TANK_TURN_TORQUE          45.0f
#define TANK_BULLET_SPEED         100.0f

static bool  CONNON_ROTATE_ON = false;
static float CONNON_ROTATE_TIME = 0.0f;

static bool  GUN_ROTATE_ON = false;
static float GUN_ROTATE_TIME = 0.0f;

static bool  TANK_ROTATE_ON = false;
static float TANK_ROTATE_TIME = 0.0f;

static bool  TANK_MOVE_ON = false;
static float TANK_MOVE_TIME = 0.0f;

PlayerController::PlayerController()
	: _ovrRenderer(nullptr)
	, _player(nullptr)
	, _moveState(PlayerMoveState::STOP)
	, _rotateState(PlayerRotateState::STOP)
	, _cannonState(CannonState::STOP)
	, _gunState(GunState::STOP)
{
}

PlayerController::~PlayerController()
{

}

void PlayerController::onKeyPressed(EventKeyboard::KeyCode code, Event *event)
{
	if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
		_cannonState = CannonState::ROTATE_LEFT;
		CONNON_ROTATE_TIME = 0.0f;
		CONNON_ROTATE_ON = true;
	}
	else if (code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		_cannonState = CannonState::ROTATE_RIGHT;
		CONNON_ROTATE_TIME = 0.0f;
		CONNON_ROTATE_ON = true;
	}

	if (code == EventKeyboard::KeyCode::KEY_UP_ARROW) {
		_gunState = GunState::GUN_UP;
		GUN_ROTATE_TIME = 0.0f;
		GUN_ROTATE_ON = true;
	}
	else if (code == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		_gunState = GunState::GUN_DOWN;
		GUN_ROTATE_TIME = 0.0f;
		GUN_ROTATE_ON = true;
	}

	if (code == EventKeyboard::KeyCode::KEY_SPACE) {
		//_cannonState = CannonState::SHOT;
		_player->shot(TANK_BULLET_SPEED);
	}

	if (code == EventKeyboard::KeyCode::KEY_A) {
		_rotateState = PlayerRotateState::TURN_LEFT;
		TANK_ROTATE_TIME = 0.0f;
		TANK_ROTATE_ON = true;
	}
	else if (code == EventKeyboard::KeyCode::KEY_D) {
		_rotateState = PlayerRotateState::TURN_RIGHT;
		TANK_ROTATE_TIME = 0.0f;
		TANK_ROTATE_ON = true;
	}
	else if (code == EventKeyboard::KeyCode::KEY_W) {
		_moveState = PlayerMoveState::MOVE;
		TANK_MOVE_TIME = 0.0f;
		TANK_MOVE_ON = true;
	}
}

void PlayerController::onKeyReleased(EventKeyboard::KeyCode code, Event *event)
{
	if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW
		|| code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		//_cannonState = CannonState::STOP;
		CONNON_ROTATE_ON = false;
	}

	if (code == EventKeyboard::KeyCode::KEY_UP_ARROW
		|| code == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		//_gunState = GunState::STOP;
		GUN_ROTATE_ON = false;
	}

	if (code == EventKeyboard::KeyCode::KEY_A
		|| code == EventKeyboard::KeyCode::KEY_D) {
		//_rotateState = PlayerRotateState::STOP;
		TANK_ROTATE_ON = false;
	}

	if (code == EventKeyboard::KeyCode::KEY_W) {
		//_moveState = PlayerMoveState::STOP;
		TANK_MOVE_ON = false;
	}
}

void PlayerController::update(float delta)
{
	if (_cannonState == CannonState::ROTATE_LEFT) {
		float angle = CONNON_ROTATE_TIME * CONNON_ROTATE_TIME;
		_player->rotateCannonStage(angle);
		_ovrRenderer->setOffsetRot(Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(angle)));
	}
	else if (_cannonState == CannonState::ROTATE_RIGHT) {
		float angle = CONNON_ROTATE_TIME * CONNON_ROTATE_TIME;
		_player->rotateCannonStage(-angle);
		_ovrRenderer->setOffsetRot(Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(-angle)));
	}


	if (_gunState == GunState::GUN_UP) {
		float angle = GUN_ROTATE_TIME * GUN_ROTATE_TIME;
		_player->rotateCannonGun(angle);
	}
	else if (_gunState == GunState::GUN_DOWN) {
		float angle = GUN_ROTATE_TIME * GUN_ROTATE_TIME;
		_player->rotateCannonGun(-angle);
	}

	if (_rotateState == PlayerRotateState::TURN_LEFT) {
		//auto rot = Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(delta * TANK_TURN_SPEED));
		//_player->setRotationQuat(_player->getRotationQuat() * rot);
		float angle = TANK_ROTATE_TIME * TANK_ROTATE_TIME;
		_player->turn(angle);
		//_ovrRenderer->setOffsetRot(rot);
	}
	else if (_rotateState == PlayerRotateState::TURN_RIGHT) {
		//auto rot = Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(-delta * TANK_TURN_SPEED));
		//_player->setRotationQuat(_player->getRotationQuat() * rot);
		float angle = TANK_ROTATE_TIME * TANK_ROTATE_TIME;
		_player->turn(-angle);
		//_ovrRenderer->setOffsetRot(rot);
	}

	if (_moveState == PlayerMoveState::MOVE) {
		float moveSpeed = TANK_MOVE_TIME * TANK_MOVE_TIME;
		_player->move(moveSpeed);
	}

	if (CONNON_ROTATE_ON)
		CONNON_ROTATE_TIME += delta * CANNON_STAGE_ROTATE_SPEED;
	else
		CONNON_ROTATE_TIME -= delta * CANNON_STAGE_ROTATE_SPEED;

	if (CANNON_STAGE_MAX_ROTATE_SPEED < CONNON_ROTATE_TIME)
		CONNON_ROTATE_TIME = CANNON_STAGE_MAX_ROTATE_SPEED;

	if (CONNON_ROTATE_TIME < 0.0f) {
		CONNON_ROTATE_TIME = 0.0f;
		_cannonState = CannonState::STOP;
	}

	if (GUN_ROTATE_ON)
		GUN_ROTATE_TIME += delta * CANNON_GUN_ROTATE_SPEED;
	else
		GUN_ROTATE_TIME -= delta * CANNON_GUN_ROTATE_SPEED;

	if (CANNON_GUN_MAX_ROTATE_SPEED < GUN_ROTATE_TIME)
		GUN_ROTATE_TIME = CANNON_GUN_MAX_ROTATE_SPEED;

	if (GUN_ROTATE_TIME < 0.0f) {
		GUN_ROTATE_TIME = 0.0f;
		_gunState = GunState::STOP;
	}


	if (TANK_ROTATE_ON)
		TANK_ROTATE_TIME += delta * CANNON_STAGE_ROTATE_SPEED;
	else
		TANK_ROTATE_TIME -= delta * CANNON_STAGE_ROTATE_SPEED;

	if (CANNON_STAGE_MAX_ROTATE_SPEED < TANK_ROTATE_TIME)
		TANK_ROTATE_TIME = CANNON_STAGE_MAX_ROTATE_SPEED;

	if (TANK_ROTATE_TIME < 0.0f) {
		TANK_ROTATE_TIME = 0.0f;
		_rotateState = PlayerRotateState::STOP;
	}

	if (TANK_MOVE_ON)
		TANK_MOVE_TIME += delta * TANK_MOVE_SPEED;
	else
		TANK_MOVE_TIME -= delta * TANK_MOVE_SPEED;

	if (TANK_MAX_MOVE_SPEED < TANK_MOVE_TIME)
		TANK_MOVE_TIME = TANK_MAX_MOVE_SPEED;

	if (TANK_MOVE_TIME < 0.0f) {
		TANK_MOVE_TIME = 0.0f;
		_moveState = PlayerMoveState::STOP;
	}
}

PlayerController* PlayerController::create()
{
	auto pc = new PlayerController();
	if (!pc->init()) {
		delete pc;
		return nullptr;
	}
	pc->autorelease();
	return pc;
}

bool PlayerController::init()
{
	auto keyboard = EventListenerKeyboard::create();
	keyboard->onKeyPressed = CC_CALLBACK_2(PlayerController::onKeyPressed, this);
	keyboard->onKeyReleased = CC_CALLBACK_2(PlayerController::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, this);

	scheduleUpdate();

	return true;
}
