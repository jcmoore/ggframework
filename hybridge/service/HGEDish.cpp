//
//  HGEDish.cpp
//  hybridge
//
//  Created by The Narrator on 11/14/12.
//
//

#include "service/HGEDish.h"

NS_HGE_BEGIN

bool HGEDish::digestJSON(JSONValue& json) {
	return 0;
}

bool HGEDish::broadcastShake() {
	
	JSONValue json(vapidjson::kObjectType);
	
	json.AddMember(HGE_KEYTEXT_TASK, "shake");
	json.AddMember(HGE_KEYTEXT_ARGS, true);
	
	this->produceJSON(json);
	
	return !0;
}

NS_HGE_END
