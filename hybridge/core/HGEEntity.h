//
//  HGEEntity.h
//  hybridge
//
//  Created by The Narrator on 8/14/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEENTITY_H__
#define __HGEENTITY_H__

#include "core/HGEModel.h"
#include "core/HGEID.h"
#include "service/HGEJSON.h"

#include "dev/HGEKeycode.h"

NS_HGE_BEGIN

class HGEToolbox;

/**
 base class of all uniquely identifiable instance-based sub-services that can turn JSON into actions
 */
class HGEEntity : public HGEModel {
	HGEClassifyKind(HGEEntity, HGEModel);
	
public:
	HGEEntity(id_hge unique);
	virtual ~HGEEntity();
	
	static JSONValue& TaskConstruction() { return HGEJSON::Null(); }
	static bool IsTaskConstruction(JSONValue& task) { return task.IsNull(); }
	
	static JSONValue& TaskDestruction() { return HGEJSON::Undefined(); }
	static bool IsTaskDestruction(JSONValue& task) { return task.IsUndefined(); }
	
	/**
	 using JSON as input, take some action
	 */
	virtual bool enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox) = 0;
	
	/**
	 get the uuid for the entity
	 */
	id_hge eid() { return hgeuuid; }
private:
	id_hge hgeuuid;
};

NS_HGE_END

#endif
