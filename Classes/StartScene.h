#ifndef __start_SCENE_H__
#define __start_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

class StartLayer : public cocos2d::CCLayer
{
public:

	StartLayer();
	~StartLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	void menuNewGameCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(StartLayer);
};

#endif  // __HELLOWORLD_SCENE_H__