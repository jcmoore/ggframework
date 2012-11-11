//
//  HGECCScape.h
//  hybridge
//
//  Created by The Narrator on 10/28/12.
//
//

#ifndef __HGECCSCAPE_H__
#define __HGECCSCAPE_H__

#include "graphics/HGECCNexus.h"

NS_HGE_BEGIN

class HGECCScape : public HGECCNexus {
	
public:
	
	virtual bool areYou(kind_hge concrete, MagicImp::MagicDerived ** result) {
		if (HGE_KINDOF( HGECCScape ) == concrete) {
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return this->HGECCNexus::areYou(concrete, result);
		}
	}
	
protected:
	
	virtual bool beKind (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
		if (kind_hge(condition) == HGE_KINDOF( HGECCScape ) ||
			HGECCNexus::beKind(condition, result)) {
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return 0;
		}
	}
public:
	
	HGECCScape(Jotter::Publisher * p, Online::NameServer * ns)
	: HGECCNexus(p, ns) {};
	
	/**
	 using JSON as input, destroy the entity
	 */
	virtual bool destroyJSON(JSONValue& json, bool firstResponder);
	
	/**
	 using JSON as input, create the entity
	 */
	virtual bool createJSON(JSONValue& json, bool firstResponder);
	
	/**
	 using JSON as input, take some action
	 */
	virtual bool enactJSON(JSONValue& task, JSONValue& json, bool firstResponder);
	
	
	
	virtual bool run(JSONValue const& json, bool implicit = 0);
	virtual bool run();
	
protected:
	
};

NS_HGE_END

#endif
