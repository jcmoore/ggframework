//
//  HGECCField.cpp
//  hybridge
//
//  Created by The Narrator on 10/28/12.
//
//

#include "graphics/HGECCField.h"

//#include "service/HGERouter.h"

USING_NS_CC;

NS_HGE_BEGIN

bool HGECCField::destroyJSON(JSONValue& json, bool firstResponder)
{
	bool didDestroy = 0;
	
	if (firstResponder) {
		HGEDeleteNull(this->cc.layer);
		didDestroy = !0;
		firstResponder = 0;
	}
	
	return HGECCNexus::createJSON(json, firstResponder) || didDestroy;
}

bool HGECCField::createJSON(JSONValue& json, bool firstResponder)
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
	
	return HGECCNexus::createJSON(json, firstResponder) || didCreate;
}

bool HGECCField::enactJSON(JSONValue& task, JSONValue& json, bool firstResponder)
{
	bool didEnact = 0;
	
	return didEnact || HGECCNexus::enactJSON(task, json, firstResponder);
}

NS_HGE_END
