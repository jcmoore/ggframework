//
//  HGEMacros.h
//  hybridge
//
//  Created by The Narrator on 11/20/11.
//  Copyright (c) 2011 Stardu. All rights reserved.
//

#ifndef __HGEMACROS_H__
#define __HGEMACROS_H__

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <unistd.h>

#include "dev/HGELanguage.h"
#include "dev/HGETime.h"

#ifdef DEBUG
#	define HGE_DEBUG	1
#else
#	define HGE_DEBUG	0
#endif

#if HGE_DEBUG
#	define HGE_TEST		0
#else
#	define HGE_TEST		0
#endif

typedef signed char bool_hge;

#define HGEPrintln                          HGEPrint("\n");  HGEPrint
#define HGEPrint							printf
//#define HGEPrint                            hge_not_print

#define HGEVPrint							vprintf

static inline void hge_not_print(const char * format, ...) {}

#define HGEQuote(x)                         (#x)

#if HGE_DEBUG
#	if __IPHONE_OS_VERSION_MAX_ALLOWED
#		if defined (__arm__)
#			define HGEBreak(x) if(!(x)){__asm__ volatile ("trap");}
#		else   // !ARM - assume INTEL. Everything else will break
#			define HGEBreak(x) if(!(x)){__asm__ volatile ("int3");}
#		endif
#	else
#		define HGEBreak(x)
#	endif
#else
#	define HGEBreak(x)
#endif

#define HGEAlarm                            hge_alarm
#define HGEVAlarm                           hge_valarm

static inline void hge_not_alarm(const unsigned int value, const char * expr, const char * format, ...) {}
static inline void hge_alarm(const unsigned int value, const char * expr, const char * format, ...) {
    if (!value)
    {
        va_list args;
        va_start (args, format);
        HGEVPrint (format, args);
        va_end (args);
        HGEPrint("\n%s\n", expr);
    }
}

static inline void hge_not_valarm(const unsigned int value, const char * expr, const char * format, va_list args) {}
static inline void hge_valarm(const unsigned int value, const char * expr, const char * format, va_list args) {
    if (!value)
    {
        HGEVPrint (format, args);
        HGEPrint("\n%s\n", expr);
    }
}

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wno-variadic-macros"
//	left as a reference, did not work for nacl because AS
//#pragma GCC diagnostic pop

#define HGEAssertC(x, ...)                   HGEAlarm((const unsigned int)(x), #x, __VA_ARGS__); HGEBreak(x)
#define HGEVAssert(x, f, va_l)              HGEVAlarm((const unsigned int)(x), #x, f, va_l); HGEBreak(x)

#define HGENameFunction()                       (__FUNCTION__)

#define HGEAssertC(x, ...)                  if (!(x)) { HGEPrint("%s(): ", HGENameFunction()); } HGEAlarm((const unsigned int)(void *)(x), #x, __VA_ARGS__); HGEBreak(x)
#define HGEVAssertC(x, f, va_l)             if (!(x)) { HGEPrint("%s(): ", HGENameFunction()); } HGEVAlarm((const unsigned int)(void *)(x), #x, f, va_l); HGEBreak(x)

#define HGECAT_(a, b) a ## b
#define HGECAT(a, b) HGECAT_(a, b)
#define HGEAssertStatic(cond, ...) typedef int HGECAT(HGEAssertS, __LINE__)[(cond) ? 1 : -1]



#ifdef HGE_OBJC_IS

#define HGELog                              NSLog
#define HGELogv                             NSLogv
//#define HGELog                              hge_not_log

static inline void hge_not_log(NSString * format, ...) {}

#define HGEError(e)                         if (e) { HGELog(@"%s:%i > %@", __FILE__, __LINE__, [e localizedDescription]);  HGEBreak(0); }
#define HGEErrorAndClear(e)                 HGEError(e) (e = nil)

#define HGERelease(x)                       [x release]
#define HGEReleaseNil(x)                    HGERelease(x);  x = nil

#define HGENameSelector()                   (sel_getName(_cmd))
#define HGENameClass(x)                     (object_getClassName(x))
#define HGEClassMatch(x, c)                 ([x isKindOfClass:c])
#define HGEClassMatchOrNil(x, c)            (!x || HGEClassMatch(x, c))
#define HGETypeMatch(x, t)                  ([x isKindOfClass:[t class]])
#define HGETypeMatchOrNil(x, t)             (!x || HGETypeMatch(x, t))
#define HGESetToIfClass(x, y, c)            ([y isKindOfClass:c] ? (x = y) || YES : NO)

#define HGEAlarmObjC                        hge_alarm_objc
#define HGEVAlarmObjC                       hge_valarm_objc

static inline void hge_not_alarm_objc(const void * value, const char * expr, NSString * format, ...) {}
static inline void hge_alarm_objc(const void * value, const char * expr, NSString * format, ...) {
    if (!value)
    {
        va_list args;
        va_start (args, format);
        HGELogv(format, args);
        va_end (args);
        HGELog(@"%s", expr);
    }
};

static inline void hge_not_valarm_objc(const void * value, const char * expr, NSString * format, va_list args) {}
static inline void hge_valarm_objc(const void * value, const char * expr, NSString * format, va_list args) {
    if (!value)
    {
        HGELogv(format, args);
        HGELog(@"%s", expr);
    }
};

#define HGEAssertObjC(x, ...)               if (!(x)) { HGELog(@"%s ~~ ", HGENameFunction()); } HGEAlarmObjC(x, #x, __VA_ARGS__); HGEBreak(x)
#define HGEVAssertObjC(x, f, va_l)          if (!(x)) { HGELog(@"%s ~~ ", HGENameFunction()); } HGEVAlarmObjC(x, #x, f, va_l); HGEBreak(x)

#endif





#ifdef HGE_CPP_IS

#define HGEDelete(x)                        delete x
#define HGEDeleteNull(x)                    HGEDelete(x);  x = 0



#endif

#define HGEFree(x)							free(x)
#define HGEFreeNull(x)						HGEFree(x);  x = 0




#endif
