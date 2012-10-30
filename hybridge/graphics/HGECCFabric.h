//
//  HGECCFabric.h
//  hybridge
//
//  Created by The Narrator on 10/29/12.
//
//

#ifndef __HGECCFABRIC_H__
#define __HGECCFABRIC_H__

#include "core/HGEEntity.h"

#include "include/cocos2d.h"

NS_HGE_BEGIN

class HGECCFabric : public HGEEntity {
	
	HGEClassifyKind(HGECCFabric, HGEEntity);
public:
	
	HGECCFabric(id_hge unique)
	: HGEEntity(unique)
	{
		this->cc.texture = 0;
		this->cccahced = 0;
	};
	
	virtual bool destroyJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox);
	
	virtual bool createJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox);
	
	virtual bool enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox);
	
	
	
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
