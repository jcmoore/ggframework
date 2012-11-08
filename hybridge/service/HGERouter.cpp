//
//  HGERouter.cpp
//  hybridge
//
//  Created by The Narrator on 8/14/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#include "service/HGERouter.h"

NS_HGE_BEGIN

HGERouter::HGERouter(const char * a,
					 HGERouter::TableInterface * t,
					 HGERouter::BottomLevelNameServerInterface * b,
					 HGERouter::SpawnerInterface * s)
: HGEWorker(a)
, table(t)
, bdns(b)
, spawner(s)
{
	//HGEAssertC(this->does(HGE_IMPOF( HGEChip )), "!HGEChip");
	//HGEAssertC(this->does(HGE_IMPOF( HGECircuit )), "!HGECircuit");
	//HGEAssertC(this->does(HGE_IMPOF( HGEOnline )), "!HGEOnline");
	
	HGEAssertC(this->table, "router needs a valid table");
	HGEAssertC(this->bdns, "router needs a valid bottom level name server");
	HGEAssertC(this->spawner, "router needs a valid spawner");
	this->cleanupTasklist.SetArray();
}

HGERouter::~HGERouter() {
	this->consumeJSON(this->cleanupTasklist);
	this->table->flip(this, this->bdns);
	HGEDeleteNull(this->spawner);
	HGEDeleteNull(this->table);
	HGEDeleteNull(this->bdns);
}


void HGERouter::tidyJSON (JSONValue& task) {
	this->cleanupTasklist.PushBack(task);
}

bool HGERouter::digestJSON(JSONValue& json) {
	
	JSONValue& port = json[HGE_KEYTEXT_PORT_NUMBER];
	if (!port.IsUint64()) {
		return 0;
	}
	
	JSONValue& task = json[HGE_KEYTEXT_TASK];
	if (task.IsUndefined()) {
		return 0;
	}
	
	JSONValue& bldn = json[HGE_KEYTEXT_BOTTOM_LEVEL_DOMAIN_NAME];
	HGEBottomLevelDomainName bldnString = bldn.IsString() ? bldn.GetString() : 0;
	
	uint64_t portNumber = port.GetUint64();
	
	HGEBottomLevelDomainName safeBLDN = 0;
	HGEPortMap * safeListing = 0;
	
	this->table->safeDomain(bldnString, &safeListing, &safeBLDN);
	
	if (!safeListing) {
		HGEAssertC(0, "unexpectedly found no domain listing . . . are you testing?");
		return 0;
	}
	
	HGEPortMap::iterator iter = (safeListing->insert(HGEPortMap::value_type(portNumber, 0))).first;
	
	HGEHandler *& handler = iter->second;
	
	const JSONKey * taskString = task.IsString() ? task.GetString() : 0;
	
	if (handler) {
		if (taskString &&
			taskString[0] == '~') {
			bool result = handler->destroyJSON(json, !0);
			HGEDeleteNull(handler);
			safeListing->erase(iter);
			return result;
		} else {
			return handler->enactJSON(task, json, !0);
		}
	} else if (taskString) {
		if (taskString[0] == '~') {
			safeListing->erase(iter);
			return !0;
		} else {
			handler = this->spawner->generate(taskString, json, safeBLDN, portNumber, this, this->bdns);
			if (handler) {
				return handler->createJSON(json, !0);
			}
			HGEAssertC(0, "failed to generate doer");
		}
	} else {
		HGEAssertC(0, "failed to resolve doer generation type");
	}
	
	return 0;
}

NS_HGE_END
