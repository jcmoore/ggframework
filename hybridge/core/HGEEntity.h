//
//  HGEEntity.h
//  hybridge
//
//  Created by The Narrator on 8/14/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEENTITY_H__
#define __HGEENTITY_H__

#include "core/can/HGECanImp.h"
#include "core/can/HGECanChip.h"
#include "service/HGEJSON.h"
#include "core/HGEKind.h"

NS_HGE_BEGIN

// (does entity have to extend model? doubt it . . .)

// entity (has nothing)
// producer (has an HGEWorker *)
// identity (has an HGEBottomLevelDomainName and an HGEPortNumber)
// adapter (has a version -- like HGEModel)
// talker (has an HGERouter::BDNS)

// how can this be accomplished in a composition friendly (i.e. non-inheritance-y) way

/**
 base class of all uniquely identifiable instance-based sub-services that can turn JSON into actions
 */
class HGEEntity : public
HGECanChip <
HGECanImp <
HGEPublic <
HGEEntity, void > > > {
	
public:
	
	virtual bool is(kind_hge concrete, MagicBlack::MagicDerived ** result) {
		if (HGE_KINDOF( HGEEntity ) == concrete) {
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return this->MagicParent::is(concrete, result);
		}
	}
	
protected:
	
	virtual bool beKind (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
		if (kind_hge(condition) == HGE_KINDOF( HGEEntity )) {
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return 0;
		}
	}
	
	virtual bool myKind (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
		return this->beKind(condition, result);
	}
	
private:
	
	bool beKindNonVirtual (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
		return this->beKind(condition, result);
	}
	
	bool myKindNonVirtual (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
		return this->myKind(condition, result);
	}
	
public:
	
	bool knownKind (kind_hge condition, HGEEntity ** result) { // TODO: do something smarter with this...
		return this->MagicChip::known(&HGEEntity::myKindNonVirtual, MagicChip::Condition(condition), result);
	}
	
	bool integratedKind (kind_hge condition, HGEEntity ** result) {
		HGEAssertC(0, "is this ever called?");
		return 0;
		//return this->ImpCircuit::integrated(&HGEEntity::beKindNonVirtual, ImpCircuit::Condition(condition), result);
	}
	
	bool unknownKind (kind_hge condition, HGEEntity ** result) {
		HGEAssertC(0, "is this ever called?");
		return 0;
		//return this->ImpCircuit::unknown(&HGEEntity::beKindNonVirtual, ImpCircuit::Condition(condition), result);
	}
	
	template <typename T>
	inline T* asKind () {
		// WARNING: all these options seem suboptimal 
		return dynamic_cast<T*>(this);
		//return reinterpret_cast<T*>(this);
		//return (T*)(void*)this;
	}
	
	template <typename T>
	T * toKind(HGECanImp<>::Magic<> * from) {
		return from ? from->with< T, MagicDerived >() : 0;
	}
};

NS_HGE_END

#endif
