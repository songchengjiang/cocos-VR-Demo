#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include "cocos2d.h"

class OVRRenderer;
class Tank;

enum class PlayerMoveState
{
	MOVE,
	STOP,
};

enum class PlayerRotateState
{
	TURN_LEFT,
	TURN_RIGHT,
	STOP,
};

enum class CannonState
{
	ROTATE_LEFT,
	ROTATE_RIGHT,
	STOP,
};

enum class GunState
{
	GUN_UP,
	GUN_DOWN,
	STOP,
};

class PlayerController : public cocos2d::Node
{
public:
	~PlayerController();

	static PlayerController* create();

	virtual void update(float delta) override;
	void setOVRRenderer(OVRRenderer *renderer) { _ovrRenderer = renderer; }
	void setPlayer(Tank *tank) { _player = tank; }

private:
	PlayerController();

private:

	bool init();
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event);

private:

	OVRRenderer *_ovrRenderer;
	Tank        *_player;
	PlayerMoveState   _moveState;
	PlayerRotateState _rotateState;
	CannonState       _cannonState;
	GunState          _gunState;
};

#endif