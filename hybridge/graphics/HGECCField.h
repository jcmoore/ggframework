//
//  HGECCField.h
//  hybridge
//
//  Created by The Narrator on 10/28/12.
//
//

#ifndef __HGECCFIELD_H__
#define __HGECCFIELD_H__

#include "graphics/HGECCNexus.h"

NS_HGE_BEGIN

class HGECCField : public HGECCNexus {
	
public:
	
	virtual bool is(kind_hge concrete, MagicBlack::MagicDerived ** result) {
		if (HGE_KINDOF( HGECCField ) == concrete) {
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return this->HGECCNexus::is(concrete, result);
		}
	}
	
protected:
	
	virtual bool beKind (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
		if (kind_hge(condition) == HGE_KINDOF( HGECCField ) ||
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
	
	HGECCField(MagicOnline::NameServer * ns)
	: HGECCNexus(ns) {};
	
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
	
protected:
	
};

NS_HGE_END

#endif
