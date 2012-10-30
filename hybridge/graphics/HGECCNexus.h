//
//  HGECCNexus.h
//  hybridge
//
//  Created by The Narrator on 10/27/12.
//
//

#ifndef __HGECCNEXUS_H__
#define __HGECCNEXUS_H__

#include "core/HGEEntity.h"

#include "include/cocos2d.h"

NS_HGE_BEGIN

typedef int depth_nexus;
typedef int key_nexus;

class HGECCNexus : public HGEEntity {
	
	HGEClassifyKind(HGECCNexus, HGEEntity);
public:
	
	HGECCNexus(id_hge unique)
	: HGEEntity(unique)
	{
		this->cc.node = 0;
		this->ccelevation = 0;
	};
	
	virtual bool destroyJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox);
	
	virtual bool createJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox);
	
	virtual bool enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox);
	
	
	
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
	};
	cctype cc;
	depth_nexus ccelevation;
	
};

NS_HGE_END

#endif
