//
//  HGECCZone.h
//  hybridge
//
//  Created by The Narrator on 10/28/12.
//
//

#ifndef __HGECCZONE_H__
#define __HGECCZONE_H__

#include "graphics/HGECCNexus.h"
#include "core/can/HGECanIdentify.h"
#include "core/can/HGECanTask.h"

NS_HGE_BEGIN

class HGECCZone : public
HGECCNexus {
	
protected:
	
	typedef
	HGECanIdentify <
	HGECanImp <
	MagicImp::MagicDerived > >
	Identity;
	Identity identity;
	
	typedef
	HGECanTask <
	HGECanImp <
	MagicImp::MagicDerived > >
	Tasker;
	Tasker tasker;
	
public:
	
	/**
	 composite imps should impliment this
	 */
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result, HGECantImp * compositExclusion) {
		if ((HGECantImp *)(&this->tasker) != compositExclusion &&
			this->tasker.canYou(interface, result, this)) {
			return !0;
		} else if ((HGECantImp *)(&this->identity) != compositExclusion &&
				   this->identity.canYou(interface, result, this)){
			return !0;
		} else {
			return MagicParent::canYou(interface, result, compositExclusion);
		}
	}
	
	virtual bool areYou(kind_hge concrete, MagicImp::MagicDerived ** result) {
		if (HGE_KINDOF( HGECCZone ) == concrete) {
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
		if (kind_hge(condition) == HGE_KINDOF( HGECCZone ) ||
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
	
	HGECCZone(HGEBottomLevelDomainName bldn,
			   HGEPortNumber port,
			   Jotter::Publisher * p,
			   Connector::NameServer * ns)
	: HGECCNexus(p, ns)
	, identity(bldn, port, this)
	, tasker(this->jotter.feat(), this->identity.feat(), this)
	{};
	
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
