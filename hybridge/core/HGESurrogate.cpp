//
//  HGESurrogate.cpp
//  hybridge
//
//  Created by The Narrator on 8/14/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#include "core/HGESurrogate.h"

NS_HGE_BEGIN

HGESurrogate::HGESurrogate(id_hge unique)
: HGECarrier(unique)
{
	
}

HGESurrogate::~HGESurrogate() {
	
}

bool HGESurrogate::destroyJSON(JSONValue& json, HGEToolbox * toolbox)
{
	return !0;
}

bool HGESurrogate::createJSON(JSONValue& json, HGEToolbox * toolbox)
{
	if (json.IsObject()) {
		JSONValue& substitution = json[JSON_SUBSTITUTION_DECLARATION];
		if (!substitution.IsUndefined())
		{
			this->carry(substitution, toolbox);
			
			return !0;
		}
	}
	
	return 0;
}

bool HGESurrogate::enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox)
{
	return !0;
}

NS_HGE_END
