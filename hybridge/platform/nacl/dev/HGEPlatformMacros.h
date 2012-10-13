//
//  HGEPlatformMacros.h
//  hybridge
//
//  Created by The Narrator on 10/4/12.
//
//

#ifndef __HGEPLATFORMMACROS_H__
#define __HGEPLATFORMMACROS_H__

#ifdef __cplusplus
#define NS_HGE_BEGIN                     namespace hybridge {
#define NS_HGE_END                       }
#define USING_NS_HGE                     using namespace hybridge
#else
#define NS_HGE_BEGIN
#define NS_HGE_END
#define USING_NS_HGE
#endif

#endif
