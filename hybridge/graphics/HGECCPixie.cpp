//
//  HGECCPixie.cpp
//  hybridge
//
//  Created by The Narrator on 10/29/12.
//
//

#include "graphics/HGECCPixie.h"

#include "service/HGEDispatch.h"

#include "graphics/HGEGraphicsMacros.h"
#include "graphics/HGECCFabric.h"

USING_NS_CC;

NS_HGE_BEGIN

bool HGECCPixie::destroyJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox)
{
	bool didDestroy = 0;
	
	if (firstResponder) {
		HGEDeleteNull(this->cc.sprite);
		didDestroy = !0;
		firstResponder = 0;
	}
	
	return HGECCNexus::createJSON(json, firstResponder, toolbox) || didDestroy;
}

bool HGECCPixie::createJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox)
{
	bool didCreate = 0;
	
	if (json.IsObject()) {
		if (firstResponder) {
			HGEAssertC(!this->cc.sprite, "leaking");
			this->cc.sprite = new CCSprite();
			this->cc.sprite->init();
			didCreate = !0;
			firstResponder = 0;
		}
		
		bool implicit = !0;
		this->setBox(json, implicit);
		this->setPigment(json, implicit);
		this->setDensity(json, implicit);
		this->setFabric(json, implicit);
	}
	
	return HGECCNexus::createJSON(json, firstResponder, toolbox) || didCreate;
}

bool HGECCPixie::enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox)
{
	bool didEnact = 0;
	
	if (task.IsString()) {
		const char * taskStr = task.GetString();
		
		if (strcmp(taskStr, "setBox") == 0) {
			didEnact = this->setBox(json);
		} else if (strcmp(taskStr, "setPigment") == 0) {
			didEnact = this->setPigment(json);
		} else if (strcmp(taskStr, "setDensity") == 0) {
			didEnact = this->setDensity(json);
		} else if (strcmp(taskStr, "setFabric") == 0) {
			didEnact = this->setFabric(json);
		}
	}
	
	return didEnact || HGECCNexus::enactJSON(task, json, toolbox);
}




bool HGECCPixie::setBox(JSONValue const& json, bool implicit)
{
	JSONValue const * pointer = 0;
	
	pointer = &json["box"];
	if (!pointer->IsArray()) {
		HGEAssertC(implicit, "no array box");
		return 0;
	}
	
	JSONValue const * o = 0;
	JSONValue const * s = 0;
	JSONValue const * x = 0;
	JSONValue const * y = 0;
	JSONValue const * w = 0;
	JSONValue const * h = 0;
	
	VAPIDJSON_GET_ELEMENTS(*pointer, &o, &s);
	
	if (!o->IsArray() &&
		!s->IsArray()) {
		HGEAssertC(implicit, "no box origin or size");
		return 0;
	}
	
	if (o) {
		VAPIDJSON_GET_ELEMENTS(*o, &x, &y);
	}
	
	if (s) {
		VAPIDJSON_GET_ELEMENTS(*s, &w, &h);
	}
	
	if (!x->IsNumber() &&
		!y->IsNumber() &&
		!w->IsNumber() &&
		!h->IsNumber()) {
		HGEAssertC(implicit, "no box values set");
		return 0;
	}
	
	CCRect rect = this->cc.sprite->getTextureRect();
	
	if (this->cc.sprite->isFlipX()) {
		rect.origin.x += rect.size.width;
		rect.size.width *= -1;
	}
	
	if (this->cc.sprite->isFlipY()) {
		rect.origin.y += rect.size.height;
		rect.size.height *= -1;
	}
	
	return this->setBox((x	&& x->IsNumber() ? x->GetDouble() : rect.origin.x),
						(y	&& y->IsNumber() ? y->GetDouble() : rect.origin.y),
						(w	&& w->IsNumber() ? w->GetDouble() : rect.size.width),
						(h	&& h->IsNumber() ? h->GetDouble() : rect.size.height));
}

bool HGECCPixie::setBox(float x, float y, float w, float h)
{
	CCRect rect = CCRect(x, y, w, h);
	
	this->cc.sprite->setFlipX(rect.size.width < 0);
	
	if (this->cc.sprite->isFlipX()) {
		rect.origin.x += rect.size.width;
		rect.size.width *= -1;
	}
	
	this->cc.sprite->setFlipY(rect.size.height < 0);
	
	if (this->cc.sprite->isFlipY()) {
		rect.origin.y += rect.size.height;
		rect.size.height *= -1;
	}
	
	this->cc.sprite->setTextureRect(rect);
	return !0;
}

bool HGECCPixie::setPigment(JSONValue const& json, bool implicit)
{
	JSONValue const * pointer = 0;
	
	pointer = &json["pigment"];
	if (!pointer->IsArray()) {
		HGEAssertC(implicit, "no array pigment");
		return 0;
	}
	
	JSONValue const * r = 0;
	JSONValue const * g = 0;
	JSONValue const * b = 0;
	
	VAPIDJSON_GET_ELEMENTS(*pointer, &r, &g, &b);
	
	if (!r->IsNumber() &&
		!g->IsNumber() &&
		!b->IsNumber()) {
		HGEAssertC(implicit, "no pigment values set");
		return 0;
	}
	
	ccColor3B const& color = this->cc.sprite->getColor();
	
	return this->setPigment((r && r->IsNumber() ? r->GetDouble() : hgeb2f(color.r)),
							(g && g->IsNumber() ? g->GetDouble() : hgeb2f(color.g)),
							(b && b->IsNumber() ? b->GetDouble() : hgeb2f(color.b)));
}

bool HGECCPixie::setPigment(float r, float g, float b)
{
	this->cc.sprite->setColor(ccc3(hgef2b(r), hgef2b(g), hgef2b(b)));
	return !0;
}

bool HGECCPixie::setDensity(JSONValue const& json, bool implicit)
{
	JSONValue const * pointer = 0;
	
	float density = 0;
	pointer = &json["density"];
	if (pointer->IsNumber()) {
		density = pointer->GetDouble();
	} else {
		HGEAssertC(implicit, "no density set");
		return 0;
		density = this->cc.sprite->getOpacity() / 255.f;
	}
	
	return this->setDensity(density);
}

bool HGECCPixie::setDensity(float a)
{
	this->cc.sprite->setOpacity(hgef2b(a));
	return !0;
}

bool HGECCPixie::setFabric(JSONValue const& json, bool implicit)
{
	JSONValue const * pointer = 0;
	
	pointer = &json["fabricDn"];
	id_hge domain = pointer->IsNumber() ? pointer->GetUint64() : 0;
	
	pointer = &json["fabricId"];
	if (!pointer->IsNumber()) {
		HGEAssertC(implicit, "no numeric fabricId");
		return 0;
	}
	
	id_hge hgeuuid = pointer->GetUint64();
	if (!hgeuuid) {
		HGEAssertC(implicit, "fabricId was zero...");
		return 0;
	}
	
	HGECCFabric * fabric = 0;
	HGEEntity * entity = HGEDispatch::EntityWithId(domain, hgeuuid);
	if (entity &&
		entity->hgekindof(HGEKind<HGECCFabric>())) {
		fabric = (HGECCFabric *)entity;
	} else {
		HGEAssertC(implicit, "fabric could not be resolved from %li/%li (domain/id)", domain, hgeuuid);
		return 0;
	}
	
	return this->setFabric(fabric);
}

bool HGECCPixie::setFabric(HGECCFabric * fabric)
{
	HGECCFabric * other = this->fabricRef.arrow();
	
	if (fabric != other) {
		this->fabricRef.set(fabric);
	} else {
		this->fabricRef.clean();
	}
	
	CCTexture2D * previous = this->cc.sprite->getTexture();
	CCTexture2D * texture = fabric ? fabric->getTexture() : 0;
	
	if (texture != previous) {
		this->cc.sprite->setTexture(texture);
		CCRect rect = this->cc.sprite->getTextureRect();
		// WARNING: this condition may not be consistent with how HGECCFabric works
		// (particularly if it creates an empty CCTexture2D object by default)
		if (!previous &&
			rect.equals(CCRectZero)) {
			rect.size = texture->getContentSize();
			this->cc.sprite->setTextureRect(rect);
		}
	}
	
	return !0;
}

void HGECCPixie::autoCorrect(update_priority phase)
{
	if (this->fabricRef.dirty()) {
		this->setFabric(this->fabricRef.arrow());
		HGEAssert(!this->fabricRef.dirty(), "fabric reference was not cleaned...");
	}
}

NS_HGE_END
