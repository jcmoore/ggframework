//
//  HGELanguage.h
//  hybridge
//
//  Created by The Narrator on 2/19/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGELANGUAGE_H__
#define __HGELANGUAGE_H__

#ifdef __OBJC__
#   ifndef HGE_OBJC_IS
#       define HGE_OBJC_IS
#   endif
#else
#   ifdef HGE_OBJC_IS
#       undef HGE_OBJC_IS
#   endif
#endif

#ifdef __cplusplus
#   ifndef HGE_CPP_IS
#       define HGE_CPP_IS
#   endif
#else
#   ifdef HGE_CPP_IS
#       undef HGE_CPP_IS
#   endif
#endif

#ifdef HGE_CPP_IS
#   define HGE_C_EXTERN() extern "C" {
#   define HGE_C_INTERN() }
#else
#   define HGE_C_EXTERN()
#   define HGE_C_INTERN()
#endif

#if defined (HGE_CPP_IS) && defined (HGE_OBJC_IS)
#   ifndef HGE_OBJCPP_IS
#       define HGE_OBJCPP_IS
#   endif
#else
#   ifdef HGE_OBJCPP_IS
#       undef HGE_OBJCPP_IS
#   endif
#endif

#endif
