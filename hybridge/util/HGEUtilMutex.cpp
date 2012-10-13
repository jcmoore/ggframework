//
//  HGEUtilMutex.cpp
//  hybridge
//
//  Created by The Narrator on 10/7/12.
//
//

#include "util/HGEUtilMutex.h"

NS_HGE_BEGIN

int HGEMutex::init(mutual_exclusion_hge *mutex)
{
#if HGE_MUTEX_WITH_PTHREAD
	return pthread_mutex_init (mutex, NULL);;
#else
	*mutex = CreateMutex(0, FALSE, 0);;
	return (*mutex==0);
#endif
    return -1;
}

int HGEMutex::Lock::lockMutex(mutual_exclusion_hge *mutex)
{
#if HGE_MUTEX_WITH_PTHREAD
	return pthread_mutex_lock( mutex );
#else
	return (WaitForSingleObject(*mutex, INFINITE)==WAIT_FAILED?1:0);
#endif
    return -1;
}

int HGEMutex::Lock::unlockMutex(mutual_exclusion_hge *mutex)
{
#if HGE_MUTEX_WITH_PTHREAD
	return pthread_mutex_unlock( mutex );
#else
	return (ReleaseMutex(*mutex)==0);
#endif
    return -1;
}

NS_HGE_END
