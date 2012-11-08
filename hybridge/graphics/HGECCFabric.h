//
//  HGECCFabric.h
//  hybridge
//
//  Created by The Narrator on 10/29/12.
//
//

#ifndef __HGECCFABRIC_H__
#define __HGECCFABRIC_H__

#include "core/HGEDoer.h"
#include "service/HGERouter.h"

#include "include/cocos2d.h"

NS_HGE_BEGIN

class HGECCFabric : public
HGEOnline <
HGEWhite <
HGEVirtualPublic <
HGECCFabric, HGEEntity > > > {
	
public:
	
	virtual bool is(kind_hge concrete, HGEEntity ** result) {
		if (HGE_KINDOF( HGECCFabric ) == concrete) {
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return this->HGEEntity::is(concrete, result);
		}
	}
	
protected:
	
	virtual bool beKind (MagicChip::Condition condition, RealChip ** result) {
		if (kind_hge(condition) == HGE_KINDOF( HGECCFabric ) ||
			HGEEntity::beKind(condition, result)) {
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return 0;
		}
	}
	
public:
	
	HGECCFabric(MagicOnline::NameServer * ns)
	: HGEEntity()
	{
		this->MagicOnline::bdns = ns;
		this->cc.texture = 0;
		this->cccahced = 0;
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
	
	virtual bool cleanup(bool firstResponder);
	
	cocos2d::CCTexture2D * getTexture() { return this->cc.texture; }
	
protected:
	union cctype {
		cocos2d::CCTexture2D * texture;
	};
	
	cctype cc;
	unsigned char cccahced;
};

NS_HGE_END

#endif
