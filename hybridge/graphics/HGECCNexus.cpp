//
//  HGECCNexus.cpp
//  hybridge
//
//  Created by The Narrator on 10/7/12.
//
//

#include "graphics/HGENexus.h"

NS_HGE_BEGIN

bool HGENexus::destroyJSON(JSONValue& json, HGEToolbox * toolbox)
{
	return !0;
}

bool HGENexus::createJSON(JSONValue& json, HGEToolbox * toolbox)
{
	if (json.IsObject()) {
		
		
		return !0;
	}
	
	return 0;
}

bool HGENexus::enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox)
{
	return 0;
}

NS_HGE_END
