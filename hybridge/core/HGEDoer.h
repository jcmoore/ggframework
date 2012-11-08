//
//  HGEDoer.h
//  hybridge
//
//  Created by The Narrator on 11/4/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEDOER_H__
#define __HGEDOER_H__

#include "core/HGEEntity.h"
#include "service/HGEJSON.h"

NS_HGE_BEGIN

// (does entity have to extend model? doubt it . . .)

// entity (has nothing)
// producer (has an HGEWorker *)
// identity (has an HGEBottomLevelDomainName and an HGEPortNumber)
// adapter (has a version -- like HGEModel)
// talker (has an HGERouter::BDNS)

// how can this be accomplished in a composition friendly (i.e. non-inheritance-y) way

typedef int update_priority;

/**
 base class of all uniquely identifiable instance-based sub-services that can turn JSON into actions
 */
class HGEDoer : public HGEEntity {
	
protected:
	
	virtual bool beKind (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
		if (kind_hge(condition) == HGE_KINDOF( HGEDoer ) ||
			HGEEntity::beKind(condition, result)) {
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return 0;
		}
	}
	
public:
	
	/**
	 using JSON as input, destroy the entity
	 */
	virtual bool destroyJSON(JSONValue& json, bool firstResponder) = 0;
	
	/**
	 using JSON as input, create the entity
	 */
	virtual bool createJSON(JSONValue& json, bool firstResponder) = 0;
	
	/**
	 using JSON as input, take some action
	 */
	virtual bool enactJSON(JSONValue& task, JSONValue& json, bool firstResponder) = 0;
	
	/**
	 using JSON as input, take some action
	 */
	virtual void autoCorrect(update_priority phase) {};
	
};

NS_HGE_END

#endif
