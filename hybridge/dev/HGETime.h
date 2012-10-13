//
//  HGETime.h
//  hybridge
//
//  Created by The Narrator on 8/21/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGETIME_H__
#define __HGETIME_H__

#include "dev/HGELanguage.h"

typedef double timeSD_hge;
typedef unsigned long long timeMSI_hge;

HGE_C_EXTERN()

/**
 get the time in seconds as a double-precision floating-point value
 */
timeSD_hge HGE_TIME_SD(void);

/**
 get the time in milliseconds as a long integer value
 */
timeMSI_hge HGE_TIME_MSL(void);

HGE_C_INTERN()

#endif
