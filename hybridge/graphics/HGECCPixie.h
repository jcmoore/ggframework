//
//  HGECCPixie.h
//  hybridge
//
//  Created by The Narrator on 10/29/12.
//
//

#ifndef __HGECCPIXIE_H__
#define __HGECCPIXIE_H__

#include "graphics/HGECCNexus.h"
#include "core/HGERef.h"

NS_HGE_BEGIN

class HGECCFabric;

class HGECCPixie : public
HGECCNexus {
	
	typedef HGECCNexus Parent;
	
public:
	
	virtual bool areYou(kind_hge concrete, MagicImp::MagicDerived ** result) {
		if (HGE_KINDOF( HGECCPixie ) == concrete) {
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return this->Parent::areYou(concrete, result);
		}
	}
	
protected:
	
	virtual bool beKind (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
		if (kind_hge(condition) == HGE_KINDOF( HGECCPixie ) ||
			this->Parent::beKind(condition, result)) {
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return 0;
		}
	}
public:
	
	HGECCPixie(Jotter::Publisher * p, Connector::NameServer * ns)
	: HGECCNexus(p, ns)
	, fabricRef() {};
	
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
	
	virtual void autoCorrect(update_priority phase);
	
	
	
	virtual bool setBox(JSONValue const& json, bool implicit = 0);
	virtual bool setBox(float x, float y, float w, float h);
	
	virtual bool setPigment(JSONValue const& json, bool implicit = 0);
	virtual bool setPigment(float r, float g, float b);
	
	virtual bool setDensity(JSONValue const& json, bool implicit = 0);
	virtual bool setDensity(float a);
	
	virtual bool setFabric(JSONValue const& json, bool implicit = 0);
	virtual bool setFabric(HGECCFabric * fabric);
	
protected:
	
	HGERef fabricRef;
	
};

NS_HGE_END

#endif
