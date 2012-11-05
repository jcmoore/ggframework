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


#define HGE_VERSIONED_WILL_INITIALIZE(ver)		\
/**
update the version of the object -- to be called after the object is modified --
works particularly in conjunction with HGERefs, to make references "dirty"
when the referree object has changed in a significant way that must be taken
into account by the referrer
*/												\
public:											\
version_hge revise() {							\
	HGE_VERSION_UP(this->ver);					\
	return this->ver;							\
}												\
/**
 get the version number of the object
 */												\
version_hge vnumber() { return ver; }			\
												\
protected:										\
/**
 get the version number of the object by reference -- only to be used by subclasses
 */												\
version_hge& revision() { return ver; }			\
												\
private:										\
version_hge ver;


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
version_hge const& HGE_VERSION_NONE();

NS_HGE_END

#endif
