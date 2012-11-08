//
//  HGEIdentity.h
//  hybridge
//
//  Created by The Narrator on 11/3/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEIDENTITY_H__
#define __HGEIDENTITY_H__

#include "core/HGEEntity.h"
#include "core/HGEID.h"
#include "service/HGERouter.h"

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
class HGEIdentity : public HGEEntity {
	
protected:
	
	virtual bool beKind (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
		if (kind_hge(condition) == HGE_KINDOF( HGEIdentity ) ||
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
	
	HGEIdentity(HGEBottomLevelDomainName bldn, HGEPortNumber pn)
	: HGEEntity()
	, port(pn)
	, domain(bldn) {}
	virtual ~HGEIdentity();
	
	HGEBottomLevelDomainName dname() { return domain; }
	/**
	 get the uuid for the entity
	 */
	HGEPortNumber pnumber() { return port; }
private:
	HGEPortNumber port;
	HGEBottomLevelDomainName domain;
};

NS_HGE_END

#endif
