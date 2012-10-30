//
//  HGECCSurface.cpp
//  hybridge
//
//  Created by The Narrator on 10/28/12.
//
//

#include "graphics/HGECCSurface.h"

//#include "service/HGEDispatch.h"

USING_NS_CC;

NS_HGE_BEGIN

bool HGECCSurface::destroyJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox)
{
	bool diDestroy = 0;
	
	if (firstResponder) {
		HGEDeleteNull(this->cc.scene);
		diDestroy = !0;
		firstResponder = 0;
	}
	
	return HGECCNexus::createJSON(json, firstResponder, toolbox) || diDestroy;
}

bool HGECCSurface::createJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox)
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
	
	return HGECCNexus::createJSON(json, firstResponder, toolbox) || didCreate;
}

bool HGECCSurface::enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox)
{
	bool didEnact = 0;
	
	if (task.IsString()) {
		const char * taskStr = task.GetString();
		
		if (strcmp(taskStr, "run") == 0) {
			didEnact = this->run(json);
		}
	}
	
	return didEnact || HGECCNexus::enactJSON(task, json, toolbox);
}



bool HGECCSurface::run(JSONValue const& json, bool implicit)
{
	return this->run();
}

bool HGECCSurface::run()
{
	CCDirector * sharedDirector = CCDirector::sharedDirector();
	
    sharedDirector->pushScene(this->cc.scene);
    sharedDirector->startAnimation();
	
	return !0;
}

NS_HGE_END
