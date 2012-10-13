//
//  HGEWorker.cpp
//  hybridge
//
//  Created by The Narrator on 7/4/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#include "service/HGEWorker.h"

#include "service/HGEInterface.h"

NS_HGE_BEGIN

bool HGEWorker::receiveJSON(JSONValue& json, HGEToolbox * toolbox) {
	
	if (json.IsObject()) {
		if (!this->consumeJSON(json, toolbox)) {
			if (toolbox->interface) {
				toolbox->interface->bounceJSON(json, this, toolbox);
			}
		}
	} else if (json.IsArray()) {
		for (JSONValue::ValueIterator iter = json.Begin();
			 iter != json.End(); iter++) {
			JSONValue& value = *(*iter);
			if (value.IsObject()) {
				if (!this->consumeJSON(value, toolbox)) {
					if (toolbox->interface) {
						toolbox->interface->bounceJSON(value, this, toolbox);
					}
				}
			}
		}
	} else {
		return false;
	}
	
	return true;
}

NS_HGE_END
