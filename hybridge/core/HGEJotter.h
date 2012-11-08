//
//  HGEJotter.h
//  hybridge
//
//  Created by The Narrator on 11/4/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEJOTTER_H__
#define __HGEJOTTER_H__

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
class HGEJotter : public HGEEntity {
	
protected:
	
	virtual bool beKind (MagicChip::Condition condition, RealChip ** result) {
		if (kind_hge(condition) == HGE_KINDOF( HGEJotter ) ||
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
	
	HGEJotter(HGEWorker * worker)
	: HGEEntity()
	, producer(worker) {}
	virtual ~HGEJotter();
	
	/**
	 send json to a worker
	 */
	bool produce(JSONValue& json, bool purify = 0);
	
private:
	HGEWorker * producer;
};

NS_HGE_END

#endif
