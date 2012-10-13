//
//  HGEUtilMutex.h
//  hybridge
//
//  Created by The Narrator on 10/7/12.
//
//

// source
// Gulyan
// http://www.codeproject.com/Articles/25569/Cross-Platform-Mutex

#ifndef __HGEUTILMUTEX_H__
#define __HGEUTILMUTEX_H__

#include "dev/HGEPlatformMacros.h"

#include "dev/HGEConfig.h"

#define HGE_MUTEX_WITH_PTHREAD		((HGE_TARGET_PLATFORM != HGE_PLATFORM_WIN32))

#if HGE_MUTEX_WITH_PTHREAD
#include <pthread.h>
#define MUTEX pthread_mutex_t
#else
#include <windows.h>
#include <process.h>
#define MUTEX HANDLE
#endif

NS_HGE_BEGIN


/**
 wrapper class for (cross-platform?) mutexes
 */
class HGEMutex
{
	typedef MUTEX mutual_exclusion_hge;
	
	mutual_exclusion_hge mutual_exclusion;
	
	friend class HGELock;
	
	int init(mutual_exclusion_hge *mutex);
	
public:
	
	/**
	 init mutex only in constructor
	 */
	HGEMutex ()
	{
		this->init(&this->mutual_exclusion);
	}
	
	/**
	 "exception/scope-safe" mutex lock 
	 */
	class Lock
	{
		HGEMutex * mutex;
		
		int lockMutex(mutual_exclusion_hge * mutex);
		int unlockMutex(mutual_exclusion_hge * mutex);
		
	public:
		
		/**
		 lock mutex only in constructor
		 */
		Lock (HGEMutex& m)
		: mutex(&m)
		{
			lockMutex(&this->mutex->mutual_exclusion);
		}
		
		/**
		 unlock mutex only in destructor
		 */
		~Lock ()
		{
			unlockMutex(&this->mutex->mutual_exclusion);
		}
	};
};


NS_HGE_END

#endif
