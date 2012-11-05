//
//  HGEVersion.cpp
//  hybridge
//
//  Created by The Narrator on 8/21/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#include "core/HGEVersion.h"

NS_HGE_BEGIN

version_hge HGE_VERSION_UP(version_hge& current, bool autoassign)
{
#if HGE_VERSION_BY_TIMESTAMP
	return HGE_TIME_SD();
#else
	return autoassign ? ++current : current + 1;
#endif
}

version_hge const& HGE_VERSION_NONE()
{
	static version_hge const instance = 0;
	
	return instance;
}

NS_HGE_END
