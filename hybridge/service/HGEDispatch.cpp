//
//  HGEDispatch.cpp
//  hybridge
//
//  Created by The Narrator on 8/14/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#include "service/HGEDispatch.h"

#include "service/HGEInterface.h"

#include "core/HGERef.h"

#include "dev/HGEKeycode.h"

NS_HGE_BEGIN

void HGEDispatch::AssignGenerator(const char* type, HGEGenerator gen) {
	if (type &&
		type[0] == '~') {
		HGEAssertC(0, "Generators must not be prefixed with a tilde (this is a pattern reserved for deconstruction): %s", type);
		return;
	}
	HGEGenerator & g = HGEDispatch::GeneratorRoster()[type];
	
	if (!g) {
		g = gen;
	} else {
		HGEAssertC(!g, "Already registered generator for type: %s", type);
	}
}

HGEDispatch::HGEDispatch() {
	this->cleanupTasklist.SetArray();
}

HGEDispatch::~HGEDispatch() {
	this->receiveJSON(this->cleanupTasklist, 0);
}


void HGEDispatch::tidyJSON (JSONValue& task, HGEToolbox * toolbox) {
	this->cleanupTasklist.PushBack(task);
}

bool HGEDispatch::initService(uint32_t argc, const char* argn[], const char* argv[], HGEToolbox * toolbox) {
	return true;
}

bool HGEDispatch::consumeJSON(JSONValue& json, HGEToolbox * toolbox) {
	
	JSONValue& hgeuuid = json[HGE_KEYTEXT_UUID];
	if (!hgeuuid.IsUint64()) {
		return false;
	}
	
	JSONValue& task = json[HGE_KEYTEXT_TASK];
	if (task.IsUndefined()) {
		return false;
	}
	
	uint64_t unique = hgeuuid.GetUint64();
	
	HGEEntity *& entity = HGEDispatch::EntityRoster()[unique];
	
	const JSONKey * taskString = task.IsString() ? task.GetString() : 0;
	
	if (entity) {
		if (taskString &&
			taskString[0] == '~') {
			bool result = entity->destroyJSON(json, toolbox);
			delete entity;
			entity = 0;
			HGEDispatch::EntityRoster().erase(unique);
			return result;
		} else {
			return entity->enactJSON(task, json, toolbox);
		}
	} else if (taskString &&
			   taskString[0] != '~') {
		entity = this->generateEntity(taskString, unique, json, toolbox);
		if (entity)
		{
			return entity->createJSON(json, toolbox);
		}
	} else {
		HGEDispatch::EntityRoster().erase(unique);
		return true;
	}
	
	return false;
}

HGEEntity * HGEDispatch::generateEntity(const char* type, id_hge unique, JSONValue& json, HGEToolbox * toolbox) {
	
	HGEGeneratorMap::iterator iter = HGEDispatch::GeneratorRoster().find(type);
	
	if (iter == HGEDispatch::GeneratorRoster().end()) {
		return 0;
	}
	
	HGEGenerator & gen = (*iter).second;
	
	if (!gen) {
		return 0;
	}
	
	//HGERef<> ref;
	
	HGEEntity * entity = gen(unique);
	
	//if (entity) {
	//	entity->hgerevise();
	//}
	//
	//ref.set(entity);
	//
	//if (entity) {
	//	entity->hgerevise();
	//}
	//
	//if (ref.dirty()) {
	//	//HGEToolboxLog(toolbox, "sodirty");
	//}
	
	return entity;
}

HGEEntity * HGEDispatch::EntityWithId(id_hge hgeuuid) {
	
	HGEEntityMap::iterator iter = HGEDispatch::EntityRoster().find(hgeuuid);
	
	if (iter == HGEDispatch::EntityRoster().end()) {
		return 0;
	}
	
	HGEEntity *& entity = (*iter).second;
	
	return entity;
}

NS_HGE_END
