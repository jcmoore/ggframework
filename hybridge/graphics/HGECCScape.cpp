//
//  HGECCScape.cpp
//  hybridge
//
//  Created by The Narrator on 10/28/12.
//
//

#include "graphics/HGECCScape.h"

//#include "service/HGERouter.h"

USING_NS_CC;

NS_HGE_BEGIN

bool HGECCScape::destroyJSON(JSONValue& json, bool firstResponder)
{
	bool diDestroy = 0;
	
	if (firstResponder) {
		if (this->cc.scene) {
			if (this->cc.scene->isRunning()) {
				CCDirector::sharedDirector()->popScene();
			}
			this->cc.scene->removeFromParentAndCleanup(!0);
			this->cc.scene->release();
			this->cc.scene = 0;
		}
		diDestroy = !0;
		firstResponder = 0;
	}
	
	return HGECCNexus::destroyJSON(json, firstResponder) || diDestroy;
}

bool HGECCScape::createJSON(JSONValue& json, bool firstResponder)
{
	bool didCreate = 0;
	
	if (json.IsObject()) {
		if (firstResponder) {
			HGEAssertC(!this->cc.scene, "leaking");
			this->cc.scene = new CCScene();
			this->cc.scene->init();
			didCreate = !0;
			firstResponder = 0;
		}
		
		
	}
	
	return HGECCNexus::createJSON(json, firstResponder) || didCreate;
}

bool HGECCScape::enactJSON(JSONValue& task, JSONValue& json, bool firstResponder)
{
	bool didEnact = 0;
	
	if (task.IsString()) {
		const char * taskStr = task.GetString();
		
		if (strcmp(taskStr, "run") == 0) {
			didEnact = this->run(json);
		}
	}
	
	return didEnact || HGECCNexus::enactJSON(task, json, firstResponder);
}



bool HGECCScape::run(JSONValue const& json, bool implicit)
{
	return this->run();
}

bool HGECCScape::run()
{
	CCDirector * sharedDirector = CCDirector::sharedDirector();
	
    sharedDirector->pushScene(this->cc.scene);
    sharedDirector->startAnimation();
	
	return !0;
}

NS_HGE_END
