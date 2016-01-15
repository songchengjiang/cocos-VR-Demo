#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class Tank;
class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float delta) override;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:

	void playerUpdate(float delta);
	void enemyAI(float delta);
	void enemyWalking(float delta);
	void enemyTracking(float delta);
	void enemyEscaping(float delta);

private:

	Tank *_enemy;
	Tank *_player;
};

#endif // __HELLOWORLD_SCENE_H__
