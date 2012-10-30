//
//  HGECCField.cpp
//  hybridge
//
//  Created by The Narrator on 10/28/12.
//
//

#include "graphics/HGECCField.h"

//#include "service/HGEDispatch.h"

USING_NS_CC;

NS_HGE_BEGIN

bool HGECCField::destroyJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox)
{
	bool didDestroy = 0;
	
	if (firstResponder) {
		HGEDeleteNull(this->cc.layer);
		didDestroy = !0;
		firstResponder = 0;
	}
	
	return HGECCNexus::createJSON(json, firstResponder, toolbox) || didDestroy;
}

bool HGECCField::createJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox)
{
	bool didCreate = 0;
	
	if (json.IsObject()) {
		if (firstResponder) {
			HGEAssertC(!this->cc.layer, "leaking");
			this->cc.layer = new CCLayer();
			this->cc.layer->init();
			didCreate = !0;
			firstResponder = 0;
		}
		
		
	}
	
	return HGECCNexus::createJSON(json, firstResponder, toolbox) || didCreate;
}

bool HGECCField::enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox)
{
	bool didEnact = 0;
	
	return didEnact || HGECCNexus::enactJSON(task, json, toolbox);
}

NS_HGE_END
