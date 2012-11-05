//
//  HGECCSurface.h
//  hybridge
//
//  Created by The Narrator on 10/28/12.
//
//

#ifndef __HGECCSURFACE_H__
#define __HGECCSURFACE_H__

#include "graphics/HGECCNexus.h"

NS_HGE_BEGIN

class HGECCSurface : public HGECCNexus {
	
protected:
	
	virtual bool beKind (ImpChip::Condition condition, RealChip ** result) {
		if (kind_hge(condition) == HGEKind<HGECCSurface>() ||
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
	
	HGECCSurface(ImpOnline::NameServer * ns)
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
	
	
	
	virtual bool run(JSONValue const& json, bool implicit = 0);
	virtual bool run();
	
protected:
	
};

NS_HGE_END

#endif
