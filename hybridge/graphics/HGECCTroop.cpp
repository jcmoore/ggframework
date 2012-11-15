//
//  HGECCTroop.cpp
//  hybridge
//
//  Created by The Narrator on 11/8/12.
//
//

#include "graphics/HGECCTroop.h"

#include "service/HGERouter.h"

#include "graphics/HGEGraphicsMacros.h"
#include "graphics/HGECCFabric.h"

USING_NS_CC;

NS_HGE_BEGIN

bool HGECCTroop::destroyJSON(JSONValue& json, bool firstResponder)
{
	bool didDestroy = 0;
	
	if (firstResponder) {
		if (this->cc.batch) {
			this->cc.batch->removeFromParentAndCleanup(!0);
			this->cc.batch->release();
			this->cc.batch = 0;
		}
		didDestroy = !0;
		firstResponder = 0;
	}
	
	return HGECCNexus::createJSON(json, firstResponder) || didDestroy;
}

bool HGECCTroop::createJSON(JSONValue& json, bool firstResponder)
{
	bool didCreate = 0;
	
	if (json.IsObject()) {
		if (firstResponder) {
			HGEAssertC(!this->cc.batch, "leaking");
			this->cc.batch = new CCSpriteBatchNode();
			this->cc.batch->init();
			didCreate = !0;
			firstResponder = 0;
		}
		
		bool implicit = !0;
		this->setFabric(json, implicit);
	}
	
	return HGECCNexus::createJSON(json, firstResponder) || didCreate;
}

bool HGECCTroop::enactJSON(JSONValue& task, JSONValue& json, bool firstResponder)
{
	bool didEnact = 0;
	
	if (task.IsString()) {
		const char * taskStr = task.GetString();
		
		if (strcmp(taskStr, "setFabric") == 0) {
			didEnact = this->setFabric(json);
		}
	}
	
	return didEnact || HGECCNexus::enactJSON(task, json, firstResponder);
}


bool HGECCTroop::setFabric(JSONValue const& json, bool implicit)
{
	JSONValue const * pointer = 0;
	
	pointer = &json["fabricDn"];
	HGEBottomLevelDomainName domain = pointer->IsString() ? pointer->GetString() : 0;
	
	pointer = &json["fabricId"];
	if (!pointer->IsNumber()) {
		HGEAssertC(implicit, "no numeric fabricId");
		return 0;
	}
	
	HGEPortNumber port = pointer->GetUint64();
	if (!port) {
		HGEAssertC(implicit, "fabricId was zero...");
		return 0;
	}
	
	HGEHandler * handler = this->connector.whois(domain, port);
	HGECCFabric * fabric = handler ? handler->canTo<HGECCFabric>() : 0;
	if (!fabric) {
		HGEAssertC(implicit, "fabric could not be resolved from %li/%li (domain/id)", domain, port);
		return 0;
	}
	
	return this->setFabric(fabric);
}

bool HGECCTroop::setFabric(HGECCFabric * fabric)
{
	HGECCFabric * other = this->fabricRef.coerce<HGECCFabric>();
	
	if (fabric != other) {
		this->fabricRef.set(fabric);
	} else {
		this->fabricRef.clean();
	}
	
	CCTexture2D * previous = this->cc.batch->getTexture();
	CCTexture2D * texture = fabric ? fabric->getTexture() : 0;
	
	if (texture != previous) {
		this->cc.batch->setTexture(texture);
	}
	
	return !0;
}

void HGECCTroop::autoCorrect(update_priority phase)
{
	if (this->fabricRef.dirty()) {
		this->setFabric(this->fabricRef.arrow());
		HGEAssertC(!this->fabricRef.dirty(), "fabric reference was not cleaned...");
	}
}

NS_HGE_END
