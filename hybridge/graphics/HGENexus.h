//
//  HGENexus.h
//  hybridge
//
//  Created by The Narrator on 10/7/12.
//
//

#ifndef __HGENEXUS_H__
#define __HGENEXUS_H__

#include "core/HGEEntity.h"

NS_HGE_BEGIN

class HGENexus : public HGEEntity {
	
public:
	
	HGENexus(id_hge unique) : HGEEntity(unique) {};
	
	virtual bool destroyJSON(JSONValue& json, HGEToolbox * toolbox);
	
	virtual bool createJSON(JSONValue& json, HGEToolbox * toolbox);
	
	virtual bool enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox);
	
};

NS_HGE_END

#endif
