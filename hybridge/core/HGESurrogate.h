//
//  HGESurrogate.h
//  hybridge
//
//  Created by The Narrator on 8/14/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGESURROGATE_H__
#define __HGESURROGATE_H__

#include "core/HGEPreserve.h"

NS_HGE_BEGIN

/**
 class acts as a reference for a json value, particularly used in the context of HGEJSONRef fusions
 */
class HGESurrogate : public HGEPreserve {
	
public:
	HGESurrogate();
	virtual ~HGESurrogate();
	
	/**
	 destruction: () none
	 */
	virtual bool destroyJSON(JSONValue& json, bool firstResponder);
	
	/**
	 construction: ({'$$':substituteJSON, ...}) carry part of the provided json and act as a substitute for it through HGEJSONRef fusions
	 */
	virtual bool createJSON(JSONValue& json, bool firstResponder);
	
	/**
	 */
	virtual bool enactJSON(JSONValue& task, JSONValue& json, bool firstResponder);
	
	friend class HGEJSONRef;
};

NS_HGE_END

#endif
