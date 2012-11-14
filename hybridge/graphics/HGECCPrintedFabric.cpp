//
//  HGECCPrintedFabric.cpp
//  hybridge
//
//  Created by The Narrator on 11/13/12.
//
//

#include "graphics/HGECCPrintedFabric.h"

USING_NS_CC;

NS_HGE_BEGIN

bool HGECCPrintedFabric::destroyJSON(JSONValue& json, bool firstResponder)
{
	bool didDestroy = 0;
	
	if (firstResponder) {
		HGEDeleteNull(this->cc.renderTexture);
		firstResponder = 0;
		didDestroy = !0;
	}
	
	return HGECCFabric::destroyJSON(json, firstResponder) || didDestroy;
}

bool HGECCPrintedFabric::createJSON(JSONValue& json, bool firstResponder)
{
	bool didCreate = 0;
	
	if (json.IsObject()) {
		if (firstResponder) {
			HGEAssertC(!this->cc.renderTexture, "leaking");
			this->cc.renderTexture = new CCRenderTexture();
			if (this->setSize(json, 0)) { // explicit! must succeed
				didCreate = !0;
				firstResponder = 0;
			} else {
				HGEAssertC(0, "failed to initialize printed fabric size");
				HGEDeleteNull(this->cc.renderTexture);
				return 0;
			}
		}
		
		//bool implicit = !0;
		//this->setURI(json, implicit);
	}
	
	return HGECCFabric::createJSON(json, firstResponder) || didCreate;
}

bool HGECCPrintedFabric::enactJSON(JSONValue& task, JSONValue& json, bool firstResponder)
{
	bool didEnact = 0;
	
	if (task.IsString()) {
		const char * taskStr = task.GetString();
		
		if (strcmp(taskStr, "start") == 0) {
			didEnact = this->start(json);
		} else if (strcmp(taskStr, "finish") == 0) {
			didEnact = this->finish(json);
		}
	}
	
	return HGECCFabric::enactJSON(task, json, firstResponder) || didEnact;
}



bool HGECCPrintedFabric::setURI(JSONValue const& json, bool implicit)
{
	return 0;
}

bool HGECCPrintedFabric::setURI(const char * uri)
{
	return 0;
}

bool HGECCPrintedFabric::cleanup(bool firstResponder)
{
	return 0;
}

bool HGECCPrintedFabric::setSize(JSONValue const& json, bool implicit)
{
	JSONValue const * pointer = 0;
	
	pointer = &json["width"];
	if (!pointer->IsNumber()) {
		HGEAssertC(implicit, "no numeric width");
		return 0;
	}
	
	unsigned int width = pointer->GetUint();
	if (width <= 0) {
		HGEAssertC(implicit, "width was invalid");
		return 0;
	}
	
	pointer = &json["height"];
	if (!pointer->IsNumber()) {
		HGEAssertC(implicit, "no numeric height");
		return 0;
	}
	
	unsigned int height = pointer->GetUint();
	if (height <= 0) {
		HGEAssertC(implicit, "height was invalid");
		return 0;
	}
	
	return this->setSize(width, height);
}

bool HGECCPrintedFabric::setSize(unsigned int width, unsigned int height)
{
	if (this->cc.renderTexture->getSprite()) {
		HGEAssertC(0, "printed fabric has already been sized!");
		return 0;
	}
	
	return this->cc.renderTexture->initWithWidthAndHeight(width, height, kCCTexture2DPixelFormat_RGBA8888, 0);
}

bool HGECCPrintedFabric::start(JSONValue const& json, bool implicit)
{
	JSONValue const * pointer = 0;
	
	bool clearColor = 0;
	JSONValue const * r = 0;
	JSONValue const * g = 0;
	JSONValue const * b = 0;
	JSONValue const * a = 0;
	
	bool clearDepth = 0;
	float depth = 0;
	
	bool clearStencil = 0;
	int stencil = 0;
	
	pointer = &json["color"];
	if (pointer->IsArray()) {
		VAPIDJSON_GET_ELEMENTS(*pointer, &r, &g, &b, &a);
		clearColor = !0;
	}
	
	pointer = &json["depth"];
	if (pointer->IsNumber()) {
		depth = pointer->GetDouble();
		clearDepth = !0;
	}
	
	pointer = &json["stencil"];
	if (pointer->IsNumber()) {
		stencil = pointer->GetInt();
		clearStencil = !0;
	}
	
	return this->start(clearColor,
					   (r && r->IsNumber() ? r->GetDouble() : 0),
					   (g && g->IsNumber() ? g->GetDouble() : 0),
					   (b && b->IsNumber() ? b->GetDouble() : 0),
					   (a && a->IsNumber() ? a->GetDouble() : 0),
					   clearDepth,
					   depth,
					   clearStencil,
					   stencil);
}

bool HGECCPrintedFabric::start(bool clearColor, float r, float g, float b, float a,
							   bool clearDepth, float depth,
							   bool clearStencil, int stencil)
{
	if (!this->cc.renderTexture->getSprite()) {
		HGEAssertC(0, "printed fabric has not been initialized");
		return 0;
	}
	
	if (clearStencil) {
		this->cc.renderTexture->beginWithClear(r, g, b, a, depth, stencil);
	} else if (clearDepth) {
		this->cc.renderTexture->beginWithClear(r, g, b, a, depth);
	} else if (clearColor) {
		this->cc.renderTexture->beginWithClear(r, g, b, a);
	} else {
		this->cc.renderTexture->begin();
	}
	
	return !0;
}

bool HGECCPrintedFabric::finish(JSONValue const& json, bool implicit) {
	return this->finish();
}

bool HGECCPrintedFabric::finish() {
	if (!this->cc.renderTexture->getSprite()) {
		HGEAssertC(0, "printed fabric has not been initialized");
		return 0;
	}
	
	this->cc.renderTexture->end();
	
	return !0;
}


NS_HGE_END
