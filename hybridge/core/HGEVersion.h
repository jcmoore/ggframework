//
//  HGEVersion.h
//  hybridge
//
//  Created by The Narrator on 8/21/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEVERSION_H__
#define __HGEVERSION_H__

#define HGE_VERSION_BY_TIMESTAMP		0

#if HGE_VERSION_BY_TIMESTAMP
#include "dev/HGETime.h"
#endif

#include "dev/HGEPlatformMacros.h"

NS_HGE_BEGIN

// type for hybrid version uuids
#if HGE_VERSION_BY_TIMESTAMP
typedef timeSD_hge version_hge;
#else
typedef unsigned long long version_hge;
#endif


/** 
 returns a new version uuid (using the current version as input)
 can optionally update the input current version as a side-effect
 */
version_hge HGE_VERSION_UP(version_hge& current, bool autoassign = !0);

/**
 returns special version uuid for the zero (initial) version
 */
version_hge const& HGE_VERSION_ZERO();

NS_HGE_END

#endif
