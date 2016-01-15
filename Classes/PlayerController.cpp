#include "PlayerController.h"
#include "OVRRenderer.h"
#include "Tank.h"

USING_NS_CC;

#define CANNON_STAGE_ROTATE_SPEED 45.0f
#define CANNON_GUN_ROTATE_SPEED   10.0f
#define TANK_MOVE_FORCE           5.0f
#define TANK_TURN_TORQUE          45.0f
#define TANK_BULLET_SPEED         100.0f

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
	}
	else if (code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		_cannonState = CannonState::ROTATE_RIGHT;
	}

	if (code == EventKeyboard::KeyCode::KEY_UP_ARROW) {
		_gunState = GunState::GUN_UP;
	}
	else if (code == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		_gunState = GunState::GUN_DOWN;
	}

	if (code == EventKeyboard::KeyCode::KEY_SPACE) {
		//_cannonState = CannonState::SHOT;
		_player->shot(TANK_BULLET_SPEED);
	}

	if (code == EventKeyboard::KeyCode::KEY_A) {
		_rotateState = PlayerRotateState::TURN_LEFT;
	}
	else if (code == EventKeyboard::KeyCode::KEY_D) {
		_rotateState = PlayerRotateState::TURN_RIGHT;
	}
	else if (code == EventKeyboard::KeyCode::KEY_W) {
		_moveState = PlayerMoveState::MOVE;
	}
}

void PlayerController::onKeyReleased(EventKeyboard::KeyCode code, Event *event)
{
	if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW
		|| code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		_cannonState = CannonState::STOP;
	}

	if (code == EventKeyboard::KeyCode::KEY_UP_ARROW
		|| code == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		_gunState = GunState::STOP;
	}

	if (code == EventKeyboard::KeyCode::KEY_A
		|| code == EventKeyboard::KeyCode::KEY_D) {
		_rotateState = PlayerRotateState::STOP;
	}

	if (code == EventKeyboard::KeyCode::KEY_W) {
		_moveState = PlayerMoveState::STOP;
	}
}

void PlayerController::update(float delta)
{
	if (_cannonState == CannonState::ROTATE_LEFT) {
		_player->rotateCannonStage(delta * CANNON_STAGE_ROTATE_SPEED);
		_ovrRenderer->setOffsetRot(Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(delta * CANNON_STAGE_ROTATE_SPEED)));
	}
	else if (_cannonState == CannonState::ROTATE_RIGHT) {
		_player->rotateCannonStage(-delta * CANNON_STAGE_ROTATE_SPEED);
		_ovrRenderer->setOffsetRot(Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(-delta * CANNON_STAGE_ROTATE_SPEED)));
	}


	if (_gunState == GunState::GUN_UP) {
		_player->rotateCannonGun(delta * CANNON_GUN_ROTATE_SPEED);
	}
	else if (_gunState == GunState::GUN_DOWN) {
		_player->rotateCannonGun(-delta * CANNON_GUN_ROTATE_SPEED);
	}

	if (_rotateState == PlayerRotateState::TURN_LEFT) {
		//auto rot = Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(delta * TANK_TURN_SPEED));
		//_player->setRotationQuat(_player->getRotationQuat() * rot);
		_player->turn(delta * TANK_TURN_TORQUE);
		//_ovrRenderer->setOffsetRot(rot);
	}
	else if (_rotateState == PlayerRotateState::TURN_RIGHT) {
		//auto rot = Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(-delta * TANK_TURN_SPEED));
		//_player->setRotationQuat(_player->getRotationQuat() * rot);
		_player->turn(-delta * TANK_TURN_TORQUE);
		//_ovrRenderer->setOffsetRot(rot);
	}

	if (_moveState == PlayerMoveState::MOVE) {
		_player->move(delta * TANK_MOVE_FORCE);
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
