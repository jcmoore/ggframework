//
//  HGEWorker.cpp
//  hybridge
//
//  Created by The Narrator on 7/4/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#include "service/HGEWorker.h"

#include "service/HGESuperior.h"

NS_HGE_BEGIN

bool HGEWorker::consumeJSON(JSONValue& json) {
	
	bool result = !0;
	
	if (json.IsObject()) {
		result = this->digestJSON(json) && result;
	} else if (json.IsArray()) {
		for (JSONValue::ValueIterator iter = json.Begin();
			 iter != json.End(); iter++) {
			JSONValue& value = *(*iter);
			if (value.IsObject()) {
				result = this->digestJSON(value) && result;
			} else {
				result = 0;
			}
		}
	} else {
		result = 0;
	}
	
	return result;
}

bool HGEWorker::produceJSON(JSONValue& json, bool purify) {
	if (this->superior) {
		
		JSONValue product;
		
		this->markJSON(product, json, purify);
		
		this->superior->produceJSON(product, 0);
		
		return !0;
	}
	
	return 0;
}

void HGEWorker::markJSON(JSONValue& result, JSONValue& json, bool purify) {
	
	if (purify) {
		JSONValue dupe;
		JSONDoc doc;
		doc.Reproduce(json, dupe);
		// TODO: fix code duplications
		if (this->getSuperior()) {
			result.SetObject();
			result.AddMember(this->alias.c_str(), dupe);
		} else {
			result = dupe;
		}
	} else {
		// TODO: fix code duplications
		if (this->getSuperior()) {
			result.SetObject();
			result.AddMember(this->alias.c_str(), json);
		} else {
			result = json;
		}
	}
}

bool HGEWorker::reportJSON(JSONValue& result) {
	return 0;
	//result.SetUndefined();
}

NS_HGE_END
