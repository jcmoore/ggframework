//
//  HGECCNexus.h
//  hybridge
//
//  Created by The Narrator on 10/27/12.
//
//

#ifndef __HGECCNEXUS_H__
#define __HGECCNEXUS_H__

#include "core/HGEDoer.h"
#include "core/can/HGECanConnect.h"
#include "core/can/HGECanJott.h"

#include "include/cocos2d.h"

NS_HGE_BEGIN

typedef int depth_nexus;
typedef int key_nexus;

class HGECCNexus : public
HGECanRout < HGECCNexus,
HGECanJott < HGECCNexus,
HGECanConnect < HGECCNexus,
HGECanImp < HGECCNexus,
HGEEntity > > > > {
	
public:
	
protected:
	
	virtual bool beKind (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
		if (kind_hge(condition) == HGE_KINDOF( HGECCNexus ) ||
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
	
	HGECCNexus(MagicOnline::NameServer * ns, MagicJotter::Producer * p)
	//: HGEEntity()
	{
		this->MagicJotter::producer = p;
		this->MagicOnline::bdns = ns;
		this->cc.node = 0;
		this->ccelevation = 0;
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
	
	
	
	virtual bool addLeaf(JSONValue const& json, bool implicit = 0);
	virtual bool addLeaf(HGECCNexus * leaf, depth_nexus ele, key_nexus code);
	
	virtual bool setRoot(JSONValue const& json, bool implicit = 0);
	virtual bool setRoot(float x, float y, float z);
	
	virtual bool setLocation(JSONValue const& json, bool implicit = 0);
	virtual bool setLocation(float x, float y, float z);
	
	virtual bool setMagnitude(JSONValue const& json, bool implicit = 0);
	virtual bool setMagnitude(float x, float y, float z);
	
	virtual bool setElevation(JSONValue const& json, bool implicit = 0);
	virtual bool setElevation(depth_nexus elevation);
	
	virtual bool setOrientation(JSONValue const& json, bool implicit = 0);
	virtual bool setOrientation(float orient);
	
protected:
	
	union cctype {
		cocos2d::CCNode * node;
		cocos2d::CCSprite * sprite;
		cocos2d::CCLayer * layer;
		cocos2d::CCScene * scene;
		cocos2d::CCSpriteBatchNode * batch;
	};
	cctype cc;
	depth_nexus ccelevation;
	
};

NS_HGE_END

#endif
