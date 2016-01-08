#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include "cocos2d.h"

class OVRRenderer;
class Tank;

enum class PlayerMoveState
{
	MOVE,
	STOP,
	TURN_LEFT,
	TURN_RIGHT,
};

enum class CannonState
{
	ROTATE_LEFT,
	ROTATE_RIGHT,
	GUN_UP,
	GUN_DOWN,
	SHOT,
	NONE,
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
	PlayerMoveState _moveState;
	CannonState     _cannonState;
};

#endif