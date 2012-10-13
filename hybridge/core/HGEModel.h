//
//  HGEModel.h
//  hybridge
//
//  Created by The Narrator on 8/21/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEMODEL_H__
#define __HGEMODEL_H__

#include "core/HGEKind.h"
#include "core/HGEVersion.h"

NS_HGE_BEGIN

/**
 base class for versioned objects of a specific kind
 */
class HGEModel {
	
public:
	HGEModel();
	virtual ~HGEModel();
	
	// component support for kind classification
	HGEClassifyBaseKind(HGEModel);
	
	/**
	 update the version of the object -- to be called after the object is modified -- 
	 works particularly in conjunction with HGERefs, to make references "dirty"
	 when the referree object has changed in a significant way that must be taken
	 into account by the referrer
	 */
	version_hge hgerevise();
	/**
	 get the version number of the object
	 */
	version_hge hgeversion() { return version; }
	
protected:
	/**
	 get the version number of the object by reference -- only to be used by subclasses
	 */
	version_hge& hgerevision() { return version; }
	
private:
	version_hge version;
};

NS_HGE_END

#endif
