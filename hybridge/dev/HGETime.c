//
//  HGETime.c
//  hybridge
//
//  Created by The Narrator on 8/21/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#include "dev/HGETime.h"

#include <sys/time.h>



timeSD_hge HGE_TIME_SD(void)
{
	static struct timeval instance;
	
	gettimeofday(&instance, 0);
	
	return ((timeSD_hge)instance.tv_usec) * (0.000001);
}

timeMSI_hge HGE_TIME_MSL(void)
{
	static struct timeval instance;
	
	gettimeofday(&instance, 0);
	
	return instance.tv_usec / 1000;
}
