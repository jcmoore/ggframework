//
//  HGECCFabric.h
//  hybridge
//
//  Created by The Narrator on 10/29/12.
//
//

#ifndef __HGECCFABRIC_H__
#define __HGECCFABRIC_H__

#include "core/can/HGECanRout.h"
#include "core/can/HGECanJott.h"
#include "core/can/HGECanIdentify.h"
#include "core/can/HGECanTask.h"

#include "include/cocos2d.h"

NS_HGE_BEGIN

class HGECCFabric : public
HGECanRout < HGECCFabric,
HGECanImp < HGECCFabric,
HGEEntity > > {
	
protected:
	
	typedef
	HGECanJott <
	HGECanImp < 
	HGEEntity > >
	Jotter;
	Jotter jotter;
	
	typedef
	HGECanIdentify <
	HGECanImp <
	HGEEntity > >
	Identity;
	Identity identity;
	
	typedef 
	HGECanTask <
	HGECanImp <
	HGEEntity > >
	Tasker;
	Tasker tasker;
	
public:
	
protected:
	
	virtual bool beKind (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
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
	
	HGECCFabric(HGEBottomLevelDomainName bldn,
				HGEPortNumber port,
				Jotter::Publisher * p)
	: jotter(p)
	, identity(bldn, port)
	, tasker(this->jotter.feat(), this->identity.feat())
	{
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
private:
	
};

NS_HGE_END

#endif
