//
//  HGECCFabric.cpp
//  hybridge
//
//  Created by The Narrator on 10/29/12.
//
//

#include "graphics/HGECCFabric.h"

#include "service/HGEDispatch.h"

USING_NS_CC;

NS_HGE_BEGIN

bool HGECCFabric::destroyJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox)
{
	bool didDestroy = 0;
	
	if (firstResponder) {
		if (this->cleanup(firstResponder)) {
			firstResponder = 0;
		}
		didDestroy = !0;
	}
	
	return didDestroy;
}

bool HGECCFabric::createJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox)
{
	bool didCreate = 0;
	
	if (json.IsObject()) {
		if (firstResponder) {
			//HGEAssertC(!this->cc.texture, "leaking");
			//this->cc.texture = new CCTexture2D();
			didCreate = !0;
			firstResponder = 0;
		}
		
		bool implicit = !0;
		this->setURI(json, implicit);
	}
	
	return didCreate;
}

bool HGECCFabric::enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox)
{
	bool didEnact = 0;
	
	if (task.IsString()) {
		const char * taskStr = task.GetString();
		
		if (strcmp(taskStr, "setURI") == 0) {
			didEnact = this->setURI(json);
		}
	}
	
	return didEnact;
}



bool HGECCFabric::setURI(JSONValue const& json, bool implicit)
{
	JSONValue const * pointer = 0;
	
	pointer = &json["uri"];
	if (!pointer->IsString()) {
		HGEAssertC(implicit, "no string uri");
		return 0;
	}
	
	const char * uri = pointer->GetString();
	if (!uri) {
		HGEAssertC(implicit, "uri was null...");
		return 0;
	}
	
	return this->setURI(uri);
}

bool HGECCFabric::setURI(const char * uri)
{
	if (!uri) {
		HGEAssertC(0, "no uri given");
		return 0;
	}
	
	bool firstResponder = true; // WARNING: may need relevant firstResponder argument...
	this->cleanup(firstResponder);
	this->cc.texture = CCTextureCache::sharedTextureCache()->addImage(uri);
	if (this->cc.texture) {
		this->cc.texture->retain();
	}
	
	this->hgerevise();
	
	return !0;
}

bool HGECCFabric::cleanup(bool firstResponder)
{
	bool result = 0;
	
	if (firstResponder) {
		if (this->cc.texture) {
			if (this->cccahced) {
				CCTextureCache::sharedTextureCache()->removeTexture(this->cc.texture);
				this->cccahced = 0;
			}
			this->cc.texture->release();
			this->cc.texture = 0;
			result = !0;
			firstResponder = 0;
		}
	}
	
	return result;
}


NS_HGE_END
