//
//  HGEPreserve.h
//  hybridge
//
//  Created by The Narrator on 8/23/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEPRESERVE_H__
#define __HGEPRESERVE_H__

#include "core/HGEDoer.h"
#include "service/HGEJSON.h"

NS_HGE_BEGIN

/**
 specialized entity that stores JSON
 */
class HGEPreserve : public HGEDoer {
	
protected:
	
	virtual bool beKind (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
		if (kind_hge(condition) == HGE_KINDOF( HGEPreserve ) ||
			HGEDoer::beKind(condition, result)) {
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return 0;
		}
	}
	
	/**
	 get the contained data -- intended to be used only by subclasses
	 */
	HGEJSON& contents () { return this->data; }
	
	/**
	 hold the indicated JSON value
	 */
	void carry (JSONValue& json) { this->data.mimic(json); }
	
protected:
	HGEJSON data;
};

NS_HGE_END

#endif
