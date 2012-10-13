//
//  HGEConfig.h
//  hybridge
//
//  Created by The Narrator on 10/6/12.
//
//

#ifndef __HGECONFIG_H__
#define __HGECONFIG_H__

#define HGE_PLATFORM_UNKNOWN			0
#define HGE_PLATFORM_IOS				1
#define HGE_PLATFORM_ANDROID			2
#define HGE_PLATFORM_WIN32				3
#define HGE_PLATFORM_MARMALADE			4
#define HGE_PLATFORM_LINUX				5
#define HGE_PLATFORM_BADA				6
#define HGE_PLATFORM_BLACKBERRY			7
#define HGE_PLATFORM_MAC				8
#define HGE_PLATFORM_NACL				9

#define HGE_TARGET_PLATFORM             HGE_PLATFORM_UNKNOWN

// iphone
#if ! HGE_TARGET_PLATFORM && (defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR))
#undef  HGE_TARGET_PLATFORM
#define HGE_TARGET_PLATFORM         HGE_PLATFORM_IOS
#define HGE_SUPPORT_PVRTC
#endif

// android
#if ! HGE_TARGET_PLATFORM && defined(ANDROID)
#undef  HGE_TARGET_PLATFORM
#define HGE_TARGET_PLATFORM         HGE_PLATFORM_ANDROID
#endif

// win32
#if ! HGE_TARGET_PLATFORM && (defined(WIN32) && defined(_WINDOWS))
#undef  HGE_TARGET_PLATFORM
#define HGE_TARGET_PLATFORM         HGE_PLATFORM_WIN32
#endif

// linux
#if ! HGE_TARGET_PLATFORM && defined(LINUX)
#undef  HGE_TARGET_PLATFORM
#define HGE_TARGET_PLATFORM         HGE_PLATFORM_LINUX
#endif

// marmalade
#if ! HGE_TARGET_PLATFORM && defined(MARMALADE)
#undef  HGE_TARGET_PLATFORM
#define HGE_TARGET_PLATFORM         HGE_PLATFORM_MARMALADE
#endif

// bada
#if ! HGE_TARGET_PLATFORM && defined(SHP)
#undef  HGE_TARGET_PLATFORM
#define HGE_TARGET_PLATFORM         HGE_PLATFORM_BADA
#endif

// qnx
#if ! HGE_TARGET_PLATFORM && defined(__QNX__)
#undef  HGE_TARGET_PLATFORM
#define HGE_TARGET_PLATFORM     HGE_PLATFORM_BLACKBERRY
#endif

// mac
#if ! HGE_TARGET_PLATFORM && defined(TARGET_OS_MAC)
#undef  HGE_TARGET_PLATFORM
#define HGE_TARGET_PLATFORM         HGE_PLATFORM_MAC
#endif

// nacl
#if ! HGE_TARGET_PLATFORM && defined(__native_client__)
#undef  HGE_TARGET_PLATFORM
#define HGE_TARGET_PLATFORM     HGE_PLATFORM_NACL
#endif

// validate
#if ! HGE_TARGET_PLATFORM
#error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif

#if (HGE_TARGET_PLATFORM == HGE_PLATFORM_WIN32)
#pragma warning (disable:4127)
#endif  // HGE_PLATFORM_WIN32

#endif
