//
//  HGECCPrintedFabric.h
//  hybridge
//
//  Created by The Narrator on 11/13/12.
//
//

#ifndef __HGECCPRINTEDFABRIC_H__
#define __HGECCPRINTEDFABRIC_H__

#include "graphics/HGECCFabric.h"

NS_HGE_BEGIN

class HGECCPrintedFabric : public
HGECCFabric {
	
public:
	
	virtual bool areYou(kind_hge concrete, MagicImp::MagicDerived ** result) {
		if (HGE_KINDOF( HGECCPrintedFabric ) == concrete) {
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return this->HGECCFabric::areYou(concrete, result);
		}
	}
	
protected:
	
public:
	
protected:
	
	virtual bool beKind (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
		if (kind_hge(condition) == HGE_KINDOF( HGECCPrintedFabric ) ||
			HGECCFabric::beKind(condition, result)) {
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return 0;
		}
	}
	
public:
	
	HGECCPrintedFabric(HGEBottomLevelDomainName bldn,
				HGEPortNumber port,
				Jotter::Publisher * p)
	: HGECCFabric(bldn, port, p)
	{
	};
	
	/**
	 using JSON as input, destroy the entity
	 */
	virtual bool destroyJSON(JSONValue& json, bool firstResponder);
	
	/**
	 using JSON as input, create the entity
	 */
	virtual bool createJSON(JSONValue& json, bool firstResponder);
	
	/**
	 using JSON as input, take some action
	 */
	virtual bool enactJSON(JSONValue& task, JSONValue& json, bool firstResponder);
	
	
	
	virtual bool setURI(JSONValue const& json, bool implicit = 0);
	virtual bool setURI(const char * uri);
	
	virtual bool setSize(JSONValue const& json, bool implicit = 0);
	virtual bool setSize(unsigned int width, unsigned int height);
	
	bool start(JSONValue const& json, bool implicit = 0);
	bool start(bool clearColor = 0, float r = 0, float g = 0, float b = 0, float a = 0,
			   bool clearDepth = 0, float depth = 0,
			   bool clearStencil = 0, int stencil = 0);
	
	bool finish(JSONValue const& json, bool implicit = 0);
	bool finish();
	
	virtual bool cleanup(bool firstResponder);
	
	virtual cocos2d::CCTexture2D * getTexture() {
		return
		this->cc.renderTexture ?
		this->cc.renderTexture->getSprite() ?
		this->cc.renderTexture->getSprite()->getTexture() : 0 : 0;
	}
	
	int getWidth() {
		return
		this->cc.renderTexture ?
		this->cc.renderTexture->getSprite() ?
		this->cc.renderTexture->getSprite()->getTexture() ?
		this->cc.renderTexture->getSprite()->getTexture()->getPixelsWide() : 0 : 0 : 0;
	}
	
	int getHeight() {
		return
		this->cc.renderTexture ?
		this->cc.renderTexture->getSprite() ?
		this->cc.renderTexture->getSprite()->getTexture() ?
		this->cc.renderTexture->getSprite()->getTexture()->getPixelsHigh() : 0 : 0 : 0;
	}
	
protected:
	
private:
	
};

NS_HGE_END

#endif
