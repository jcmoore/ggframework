//
//  HGEModel.h
//  hybridge
//
//  Created by The Narrator on 8/21/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEMODEL_H__
#define __HGEMODEL_H__

#include "core/HGEEntity.h"
#include "core/HGEVersion.h"

NS_HGE_BEGIN

/**
 base class for versioned objects of a specific kind
 */
class HGEModel : public HGEEntity {
	
protected:
	
	virtual bool beKind (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
		if (kind_hge(condition) == HGE_KINDOF( HGEModel ) ||
			HGEEntity::beKind(condition, result)) {
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return 0;
		}
	}
	
	HGE_VERSIONED_WILL_INITIALIZE(ver);
	
public:
	HGEModel()
	: HGEEntity()
	, ver(HGE_VERSION_NONE())
	{
		this->revise(); // make sure it has a valid version
	}
	virtual ~HGEModel() {}
};

NS_HGE_END

#endif
