//
//  HGECCNexus.cpp
//  hybridge
//
//  Created by The Narrator on 10/7/12.
//
//

#include "graphics/HGECCNexus.h"

#include "service/HGERouter.h"

USING_NS_CC;

NS_HGE_BEGIN

bool HGECCNexus::destroyJSON(JSONValue& json, bool firstResponder)
{
	bool didDestroy = 0;
	
	if (firstResponder) {
		HGEDeleteNull(this->cc.node);
		didDestroy = !0;
		firstResponder = 0;
	}
	
	return didDestroy;
}

bool HGECCNexus::createJSON(JSONValue& json, bool firstResponder)
{
	bool didCreate = 0;
	
	if (json.IsObject()) {
		if (firstResponder) {
			HGEAssertC(!this->cc.node, "leaking");
			this->cc.node = new CCNode();
			didCreate = !0;
			firstResponder = 0;
		}
		
		bool implicit = !0;
		this->setRoot(json, implicit);
		this->setLocation(json, implicit);
		this->setMagnitude(json, implicit);
		this->setElevation(json, implicit);
		this->setOrientation(json, implicit);
	}
	
	return didCreate;
}

bool HGECCNexus::enactJSON(JSONValue& task, JSONValue& json, bool firstResponder)
{
	bool didEnact = 0;
	
	if (task.IsString()) {
		const char * taskStr = task.GetString();
		
		if (strcmp(taskStr, "addLeaf") == 0) {
			didEnact = this->addLeaf(json);
		} else if (strcmp(taskStr, "setRoot") == 0) {
			didEnact = this->setRoot(json);
		} else if (strcmp(taskStr, "setLoc") == 0) {
			didEnact = this->setLocation(json);
		} else if (strcmp(taskStr, "setMagn") == 0) {
			didEnact = this->setMagnitude(json);
		} else if (strcmp(taskStr, "setEle") == 0) {
			didEnact = this->setElevation(json);
		} else if (strcmp(taskStr, "setOri") == 0) {
			didEnact = this->setOrientation(json);
		}
	}
	
	return didEnact;
}



bool HGECCNexus::addLeaf(JSONValue const& json, bool implicit)
{
	JSONValue const * pointer = 0;
	
	pointer = &json["leafDn"];
	HGEBottomLevelDomainName domain = pointer->IsString() ? pointer->GetString() : 0;
	
	pointer = &json["leafId"];
	if (!pointer->IsNumber()) {
		HGEAssertC(implicit, "no numeric leafId");
		return 0;
	}
	
	HGEPortNumber port = pointer->GetUint64();
	if (!port) {
		HGEAssertC(implicit, "leafId was zero...");
		return 0;
	}
	
	HGEHandler * handler = this->MagicOnline::bdns->whois(domain, port);
	HGECCNexus * leaf = this->toKind<HGECCNexus>(handler);
	
	if (!leaf) {
		HGEAssertC(implicit, "leaf could not be resolved from %li/%li (domain/port)", domain, port);
		return 0;
		
	}
	
	pointer = &json["ele"];
	depth_nexus ele = pointer->IsNumber() ? pointer->GetInt64() : this->ccelevation;
	
	pointer = &json["code"];
	key_nexus code = pointer->IsNumber() ? pointer->GetInt64() : 0;
	
	return this->addLeaf(leaf, ele, code);
}

bool HGECCNexus::addLeaf(HGECCNexus * leaf, depth_nexus ele, key_nexus code)
{
	if (!leaf) {
		HGEAssertC(0, "no leaf given");
		return 0;
	}
	
	this->cc.node->addChild(leaf->cc.node, ele, code);
	
	return !0;
}

bool HGECCNexus::setRoot(JSONValue const& json, bool implicit)
{
	JSONValue const * pointer = 0;
	
	pointer = &json["root"];
	if (!pointer->IsArray()) {
		HGEAssertC(implicit, "no array root");
		return 0;
	}
	
	JSONValue const * x = 0;
	JSONValue const * y = 0;
	JSONValue const * z = 0;
	
	VAPIDJSON_GET_ELEMENTS(*pointer, &x, &y, &z);
	
	if (!x->IsNumber() &&
		!y->IsNumber() &&
		!z->IsNumber()) {
		HGEAssertC(implicit, "no root set");
		return 0;
	}
	
	return this->setRoot((x && x->IsNumber() ? x->GetDouble() : this->cc.node->getScaleX()),
						 (y && y->IsNumber() ? y->GetDouble() : this->cc.node->getScaleY()),
						 (z && z->IsNumber() ? z->GetDouble() : 1));
}

bool HGECCNexus::setRoot(float x, float y, float z)
{
	this->cc.node->setAnchorPoint(CCPointMake(x, y));
	
	return !0;
}

bool HGECCNexus::setLocation(JSONValue const& json, bool implicit)
{
	JSONValue const * pointer = 0;
	
	pointer = &json["loc"];
	if (!pointer->IsArray()) {
		HGEAssertC(implicit, "no array loc");
		return 0;
	}
	
	JSONValue const * x = 0;
	JSONValue const * y = 0;
	JSONValue const * z = 0;
	
	VAPIDJSON_GET_ELEMENTS(*pointer, &x, &y, &z);
	
	if (!x->IsNumber() &&
		!y->IsNumber() &&
		!z->IsNumber()) {
		HGEAssertC(implicit, "no location set");
		return 0;
	}
	
	return this->setLocation((x && x->IsNumber() ? x->GetDouble() : this->cc.node->getPositionX()),
							 (y && y->IsNumber() ? y->GetDouble() : this->cc.node->getPositionY()),
							 (z && z->IsNumber() ? z->GetDouble() : 0));
}

bool HGECCNexus::setLocation(float x, float y, float z)
{
	this->cc.node->setPosition(CCPointMake(x, y));
	
	return !0;
}

bool HGECCNexus::setMagnitude(JSONValue const& json, bool implicit)
{
	JSONValue const * pointer = 0;
	
	pointer = &json["magn"];
	if (!pointer->IsArray()) {
		HGEAssertC(implicit, "no array magn");
		return 0;
	}
	
	JSONValue const * x = 0;
	JSONValue const * y = 0;
	JSONValue const * z = 0;
	
	VAPIDJSON_GET_ELEMENTS(*pointer, &x, &y, &z);
	
	if (!x->IsNumber() &&
		!y->IsNumber() &&
		!z->IsNumber()) {
		HGEAssertC(implicit, "no magnitude set");
		return 0;
	}
	
	return this->setMagnitude((x && x->IsNumber() ? x->GetDouble() : this->cc.node->getScaleX()),
							  (y && y->IsNumber() ? y->GetDouble() : this->cc.node->getScaleY()),
							  (z && z->IsNumber() ? z->GetDouble() : 1));
}

bool HGECCNexus::setMagnitude(float x, float y, float z)
{
	this->cc.node->setScaleX(x);
	this->cc.node->setScaleY(y);
	
	return !0;
}

bool HGECCNexus::setElevation(JSONValue const& json, bool implicit)
{
	JSONValue const * pointer = 0;
	
	depth_nexus ele = this->ccelevation;
	pointer = &json["ele"];
	if (pointer->IsNumber()) {
		ele = pointer->GetInt64();
	} else {
		HGEAssertC(implicit, "no orientation set");
		return 0;
		ele = this->cc.node->getZOrder();
	}
	
	return this->setElevation(ele);
}

bool HGECCNexus::setElevation(depth_nexus elevation)
{
	this->ccelevation = elevation;
	
	CCNode * parent = this->cc.node->getParent();
	if (parent) {
		parent->reorderChild(this->cc.node, elevation);
	}
	
	return !0;
}

bool HGECCNexus::setOrientation(JSONValue const& json, bool implicit)
{
	JSONValue const * pointer = 0;
	
	float ori = 0;
	pointer = &json["ori"];
	if (pointer->IsNumber()) {
		ori = pointer->GetDouble();
	} else {
		HGEAssertC(implicit, "no orientation set");
		return 0;
		ori = CC_DEGREES_TO_RADIANS(this->cc.node->getRotation());
	}
	
	return this->setOrientation(ori);
}

bool HGECCNexus::setOrientation(float orient)
{
	this->cc.node->setRotation(-CC_RADIANS_TO_DEGREES(orient));
	return !0;
}


NS_HGE_END
