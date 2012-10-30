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

bool HGESurrogate::destroyJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox)
{
	bool didDestroy = 0;
	
	if (firstResponder)
	{
		didDestroy = !0;
		firstResponder = 0;
	}
	
	return didDestroy;
}

bool HGESurrogate::createJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox)
{
	bool didCreate = 0;
	
	if (json.IsObject()) {
		if (firstResponder) {
			JSONValue& substitution = json[JSON_SUBSTITUTION_DECLARATION];
			if (!substitution.IsUndefined())
			{
				this->carry(substitution, toolbox);
				didCreate = !0;
				firstResponder = 0;
			}
		}
		
		
	}
	
	return didCreate;
}

bool HGESurrogate::enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox)
{
	bool didEnact = 0;
	
	return didEnact;
}

NS_HGE_END
