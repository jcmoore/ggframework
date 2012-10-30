//
//  HGEDispatch.h
//  hybridge
//
//  Created by The Narrator on 8/14/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEDISPATCH_H__
#define __HGEDISPATCH_H__

#include "service/HGEWorker.h"
#include "util/HGEUtilString.h"
#include "core/HGEEntity.h"

#include <map>
#include <list>

NS_HGE_BEGIN

typedef HGEEntity * (*HGEGenerator)(id_hge);
typedef std::map< id_hge, HGEEntity * > HGEEntityMap;
typedef std::map< const char *, HGEGenerator, cmpstr > HGEGeneratorMap;

/**
 worker in charge of creating, destroying, and passing messages to
 uniquely identifiable entities
 */
class HGEDispatch : public HGEWorker {
	
public:
	HGEDispatch();
	virtual ~HGEDispatch();
	
	/**
	 prepare to provide service
	 */
	virtual bool initService(uint32_t argc, const char* argn[], const char* argv[], HGEToolbox * toolbox);
protected:
	/**
	 hge@: ({'hgeid':idNumber, 'task':taskJSON, ...}) passes taskJSON to the entity with id idNumber,
	 *	if no entity exists with the desired id and the taskJSON is JSON String, a new entity is generated
	 *	with the idNumber using the generator specified by the taskJSON String,
	 *	if the taskJSON is a JSON String prefixed with a '~' (tilde), any entity found with the id idNumber
	 *	is destroyed
	 */
	bool consumeJSON(JSONValue& json, HGEToolbox * toolbox);
	
public:
	std::string& aliasName() { static std::string instance(HGE_KEYTEXT_SERVICE_DISPATCH); return instance; }
	
private:
	
	JSONValue cleanupTasklist;
	
	/**
	 create a new entity with the given type, id, and construction json
	 */
	HGEEntity * generateEntity(const char* type, id_hge unique, JSONValue& json, HGEToolbox * toolbox);
	
	/**
	 get the generator collection
	 */
	static HGEGeneratorMap& GeneratorRoster() {
		static HGEGeneratorMap * instance = new HGEGeneratorMap();
		return *instance;
	}
	
	/**
	 get the entity collection
	 */
	static HGEEntityMap& EntityRoster() {
		static HGEEntityMap * instance = new HGEEntityMap();
		return *instance;
	}
	
public:
	/**
	 add to a set of tasks the dispatcher should hand out when it is destroyed (a cleanup mechanism) --
	 currently this feature is only used in unit testing to avoid memory leaks
	 */
	void tidyJSON (JSONValue& task, HGEToolbox * toolbox);
	
	/**
	 get the entity with the specified id
	 */
	static HGEEntity * EntityWithId(id_hge domain, id_hge hgeuuid); // a little worried about thread safety...
	
	/**
	 set a generator to be used for a specific entity type
	 */
	static void AssignGenerator(const char* type, HGEGenerator gen);
};

NS_HGE_END

#endif
