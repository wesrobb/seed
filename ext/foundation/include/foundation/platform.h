/* platform.h  -  Foundation library  -  Public Domain  -  2013 Mattias Jansson / Rampant Pixels
 * 
 * This library provides a cross-platform foundation library in C11 providing basic support data types and
 * functions to write applications and games in a platform-independent fashion. The latest source code is
 * always available at
 * 
 * https://github.com/rampantpixels/foundation_lib
 * 
 * This library is put in the public domain; you can redistribute it and/or modify it without any restrictions.
 *
 */

#pragma once

/*! \file platform.h
    Platform abstractions
	
	This file tries to unify platform definitions and data types across supported platforms and compilers.
	If you need to override platform or architecture, predefine one of FOUNDATION_PLATFORM_[...] and FOUNDATION_ARCH_[...] to 1

	All preprocessor macros are designed to be always defined and used by value, i.e a check should be performed like
	"\#if FOUNDATION_PLATFORM_[...]" rather than "\#ifdef FOUNDATION_PLATFORM_[...]", since this solves the potential problem of typos in
	preprocessor checks (the \#if test will most likely catch the typo with an not defined error, whereas the
	\#ifdef macro will just resolve to false)

	This header also defines a bunch of preprocessor macros:
	FOUNDATION_PLATFORM_[...]
	FOUNDATION_ARCH_[...]
	FOUNDATION_COMPILER_[...]
	FOUNDATION_EXTERN
	FOUNDATION_API
	FOUNDATION_PREPROCESSOR_TOSTRING
	FOUNDATION_PREPROCESSOR_JOIN
	FOUNDATION_DECLARE_THREAD_LOCAL
	FOUNDATION_DECLARE_THREAD_LOCAL_ARRAY

	And some short-form type/function attributes as well:
	RESTRICT
	THREADLOCAL
	DEPRECATED
	FORCEINLINE
	NOINLINE
	PURE

	*/

//Lint warning inhibitors
/*lint -e717    We use do {} while(0) constructs in macros deliberately */

/*! \def FOUNDATION_COMPILE
    Set to 1 when compiling the foundation library, undefined (or zero) when using the library */

/*! \def FOUNDATION_EXTERN
    Declare a variable visible to users of the library */

/*! \def FOUNDATION_API
    Declare a function visible to users of the library */

/*! \def FOUNDATION_PLATFORM_ANDROID
    Defined to 1 if compiling for android platforms, 0 otherwise */

/*! \def FOUNDATION_PLATFORM_IOS
    Defined to 1 if compiling for iOS platforms (iPhone/iPad and simulators), 0 otherwise */

/*! \def FOUNDATION_PLATFORM_IOS_SIMULATOR
    Defined to 1 if compiling for iOS simulator (also has FOUNDATION_PLATFORM_IOS defined to 1), 0 otherwise */

/*! \def FOUNDATION_PLATFORM_MACOSX
    Defined to 1 if compiling for MacOS X, 0 otherwise */

/*! \def FOUNDATION_PLATFORM_LINUX
    Defined to 1 if compiling for Linux, 0 otherwise */

/*! \def FOUNDATION_PLATFORM_LINUX_RASPBERRYPI
    Defined to 1 if compiling for Raspberry Pi (also has FOUNDATION_PLATFORM_LINUX defined to 1), 0 otherwise */

/*! \def FOUNDATION_PLATFORM_BSD
    Defined to 1 if compiling for BSD, 0 otherwise */

/*! \def FOUNDATION_PLATFORM_WINDOWS
    Defined to 1 if compiling for Windows, 0 otherwise */

/*! \def FOUNDATION_ARCH_ARM
    Defined to 1 if compiling for ARM architectures, 0 otherwise */

/*! \def FOUNDATION_ARCH_ARM5
    Defined to 1 if compiling for ARMv5 architectures, 0 otherwise */

/*! \def FOUNDATION_ARCH_ARM6
    Defined to 1 if compiling for ARMv6 architectures, 0 otherwise */

/*! \def FOUNDATION_ARCH_ARM7
    Defined to 1 if compiling for ARMv7 architectures, 0 otherwise */

/*! \def FOUNDATION_ARCH_ARM8
    Defined to 1 if compiling for ARMv8 architectures, 0 otherwise */

/*! \def FOUNDATION_ARCH_ARM_64
    Defined to 1 if compiling for 64-bit ARM architectures, 0 otherwise */

/*! \def FOUNDATION_ARCH_ARM8_64
    Defined to 1 if compiling for 64-bit ARMv8 architectures, 0 otherwise */

/*! \def FOUNDATION_ARCH_X86
    Defined to 1 if compiling for x86 architectures, 0 otherwise */

/*! \def FOUNDATION_ARCH_X86_64
    Defined to 1 if compiling for x86-64 architectures, 0 otherwise */

/*! \def FOUNDATION_ARCH_PPC
    Defined to 1 if compiling for PPC architectures, 0 otherwise */

/*! \def FOUNDATION_ARCH_PPC_64
    Defined to 1 if compiling for 64-bit PPC architectures, 0 otherwise */

/*! \def FOUNDATION_ARCH_IA64
    Defined to 1 if compiling for IA64 architectures, 0 otherwise */

/*! \def FOUNDATION_ARCH_MIPS
    Defined to 1 if compiling for MIPS architectures, 0 otherwise */

/*! \def FOUNDATION_ARCH_SSE2
    Defined to 1 if compiling with SSE2 instruction set enabled, 0 otherwise */

/*! \def FOUNDATION_ARCH_SSE3
    Defined to 1 if compiling with SSE3 instruction set enabled, 0 otherwise */

/*! \def FOUNDATION_ARCH_SSE4
    Defined to 1 if compiling with SSE4 instruction set enabled, 0 otherwise */

/*! \def FOUNDATION_ARCH_SSE4_FMA3
    Defined to 1 if compiling with SSE4 instruction set (including FMA3 instruction) enabled, 0 otherwise */

/*! \def FOUNDATION_ARCH_NEON
    Defined to 1 if compiling with NEON instruction set enabled, 0 otherwise */

/*! \def FOUNDATION_ARCH_THUMB
    Defined to 1 if compiling for ARM THUMB instruction set, 0 otherwise */

/*! \def FOUNDATION_ARCH_ENDIAN_LITTLE
    Defined to 1 if compiling for little endian architectures, 0 otherwise */

/*! \def FOUNDATION_ARCH_ENDIAN_BIG
    Defined to 1 if compiling for big endian architectures, 0 otherwise */

/*! \def FOUNDATION_PLATFORM_APPLE
    Defined to 1 if compiling for Apple platforms (OSX, iOS), 0 otherwise */

/*! \def FOUNDATION_PLATFORM_POSIX
    Defined to 1 if compiling for POSIX platforms (Linux, BSD, OSX, iOS, Android), 0 otherwise */

/*! \def FOUNDATION_PLATFORM_FAMILY_MOBILE
    Defined to 1 if compiling for mobile platforms (iOS, Android), 0 otherwise */

/*! \def FOUNDATION_PLATFORM_FAMILY_DESKTOP
    Defined to 1 if compiling for desktop platforms (Windows, OSX, Linux, BSD), 0 otherwise */

/*! \def FOUNDATION_PLATFORM_FAMILY_CONSOLE
    Defined to 1 if compiling for console platforms (iOS, Android), 0 otherwise */

/*! \def FOUNDATION_COMPILER_CLANG
    Defined to 1 if compiling with clang, 0 otherwise */

/*! \def FOUNDATION_COMPILER_GCC
    Defined to 1 if compiling with GCC, 0 otherwise */

/*! \def FOUNDATION_COMPILER_MSVC
    Defined to 1 if compiling with Microsoft compiler, 0 otherwise */

/*! \def FOUNDATION_COMPILER_INTEL
    Defined to 1 if compiling with Intel compiler, 0 otherwise */

/*! \def FOUNDATION_COMPILER_NAME
    A string naming the compiler used */

/*! \def FOUNDATION_COMPILER_DESCRIPTION
    A string with a more detailed description of the compiler used, name and version */

/*! \def RESTRICT
    Restrict attribute, defined to nothing if compiler does not support restrict */

/*! \def THREADLOCAL
    Thread local attribute, defined to nothing if the compiler/platform/architecture does not support thread local variables. For full platform support, use FOUNDATION_DECLARE_THREAD_LOCAL instead */

/*! \def DEPRECATED
    Deprecated attribute */

/*! \define FORCEINLINE
    Attribute to force function to be inlined */

/*! \def NOINLINE
    Attribute to prevent function from being inlined */

/*! \def PURECALL
    Attribute declaring function to be pure, meaning it has no effects except the return value and the return value depends only on the parameters and/or global variables */

/*! \def CONSTCALL
    Attribute declaring function to be const, meaning it does not examine any values except the arguments, and has no effects except the return value.
	Basically this is just slightly more strict class than the PURECALL attribute, since function is not allowed to read global memory.
	Note that a function that has pointer arguments and examines the data pointed to must not be declared const. Likewise, a function that calls a non-const
    function usually must not be const. It does not make sense for a const function to return void. */

/*! \def ALIGN
    Variable or type attribute declaring the variable/type to have the specified memory alignment
	\param x      Alignment */

/*! \typedef float32_t
    Floating point type guaranteed to be 32-bit in size */

/*! \typedef float64_t
    Floating point type guaranteed to be 64-bit in size */

/*! \typedef real
    Floating point type of the size chosen in build config (32 or 64 bit). See FOUNDATION_PLATFORM_REALSIZE for declaring size used. */

/*! \def FLOAT32_C
    Declare a 32-bit floating point constant. Use for automatic suffixing, for example FLOAT32_C(1.0)
	\param x      Constant value */

/*! \def FLOAT64_C
    Declare a 64-bit floating point constant. Use for automatic suffixing, for example FLOAT64_C(1.0)
	\param x      Constant value */

/*! \def REAL_C
    Declare a real constant. Use for automatic suffixing depending on floating point notation used, for example REAL_C(1.0)
	\param x      Constant value */

/*! \def FOUNDATION_PLATFORM_REALSIZE
    Declare the size of a real number, either 32 or 64 bit. Default to 32 bit */

/*! \def FOUNDATION_ARCH_POINTER_SIZE
    Defines the size of a pointer on the current architecture. Either 4 or 8 bytes for 32 and 64 bit architectures, respectively */

/*! \def FOUNDATION_WCHAR_SIZE
    Defines the size of the wchar_t type, depending on platform */

/*! \fn uint128_make
    Declare a 128-bit unsigned int value from low and high 64-bit components
	\param low     Low 64 bits
	\param high    High 64 bits
	\return        128-bit value */

/*! \fn uint128_equal
    Query if two 128-bit unsigned int values are equal
	\param u0      First value
	\param u1      Second value
	\return        true if values are equal, false if not */

/*! \fn uint128_null
    Declare a zero (null) 128-bit unsigned int value
	\return        Zero 128-bit value */

/*! \fn uint128_is_null
    Query if a 128-bit unsigned int value is zero (null)
	\param u0      Value
	\return        true if value is zero (null), false if not */

/*! \fn uint256_make
    Declare a 256-bit unsigned int value from four 64-bit components
	\param low     Low 64 bits
	\param high    High 64 bits */

/*! \fn uint256_equal
    Query if two 256-bit unsigned int values are equal
	\param u0      First value
	\param u1      Second value
	\return        true if values are equal, false if not */

/*! \fn uint256_null
    Declare a zero (null) 256-bit unsigned int value
	\return        Zero 256-bit value */

/*! \fn uint256_is_null
    Query if a 256-bit unsigned int value is zero (null)
	\param u0      Value
	\return        true if value is zero (null), false if not */

/*! \typedef atomic32_t
    Atomic 32-bit integer type. Use with functions declares in atomic.h */

/*! \typedef atomic64_t
    Atomic 64-bit integer type. Use with functions declares in atomic.h */

/*! \typedef atomicptr_t
    Atomic pointer type. Use with functions declares in atomic.h */

/*! \def pointer_offset
    Offset a non-const pointer the given number of bytes, disregarding type of pointer
	\param ptr     Pointer
	\param ofs     Offset in bytes (positive or negative) */

/*! \def pointer_offset_const
    Offset a const pointer the given number of bytes, disregarding type of pointer
	\param ptr     Pointer
	\param ofs     Offset in bytes (positive or negative) */

/*! \def pointer_diff
    Calculate the offset in bytes between two pointers (from first to second), disregarding type of pointer
	\param first   First pointer
	\param second  Second pointer */

/*! \def FOUNDATION_MAX_PATHLEN
    Defines the maximum length of a path */

/*! \def FOUNDATION_DECLARE_THREAD_LOCAL
    Declare a thread-local variable of the given type, name and initial value. This will also declare and implement
	two inlined functions to set and get value of the variable, called get_thread_[name] and set_thread_[name]. For example, to declare
	a thread-local integer and use the get/set functions:
	FOUNDATION_DECLARE_THREAD_LOCAL( int, myvar, 0 );
	set_thread_myvar( 1 );
	int currentval = get_thread_myvar();
	\param type    Data type
	\param name    Variable name
	\param init    Initial value */

/*! \def FOUNDATION_DECLARE_THREAD_LOCAL_ARRAY
    Declare a thread-local array of the given type, name and array size. This will also declare and implement
	one inlined function to get the array pointer value of the variable, called get_thread_[name]. For example, to declare
	a thread-local integer array and use the get/set functions:
	FOUNDATION_DECLARE_THREAD_LOCAL_ARRAY( int, myvar, 10 );
	int* currentarr = get_thread_myvar(); //Get thread-local array storage
	currentarr[2] = 10;
	int val = currentarr[2];
	\param type    Data type
	\param name    Variable name
	\param arrsize Size of array in number of elements */

/*! \def PRIREAL
    Printf-style format declaration for a real variable. Use like other standard PRI* format specifiers, like string_format( "Value: %" PRIREAL, realval ); */

/*! \def PRIfixPTR
    Printf-style format declaration for a pointer variable producing a fixed-size string (padding with zeroes). Use like other standard PRI* format specifiers, like string_format( "Value: %" PRIfixPTR, ptr ); */


#if !defined( FOUNDATION_COMPILE )
#  define FOUNDATION_COMPILE 0
#endif

#if FOUNDATION_COMPILE
#  ifdef __cplusplus
#  define FOUNDATION_EXTERN extern "C"
#  define FOUNDATION_API extern "C"
#  else
#  define FOUNDATION_EXTERN extern
#  define FOUNDATION_API extern
#  endif
#else
#  ifdef __cplusplus
#  define FOUNDATION_EXTERN extern "C"
#  define FOUNDATION_API extern "C"
#  else
#  define FOUNDATION_EXTERN extern
#  define FOUNDATION_API extern
#  endif
#endif

//Platforms
#ifndef FOUNDATION_PLATFORM_ANDROID
#  define FOUNDATION_PLATFORM_ANDROID 0
#endif
#ifndef FOUNDATION_PLATFORM_IOS
#  define FOUNDATION_PLATFORM_IOS 0
#endif
#ifndef FOUNDATION_PLATFORM_IOS_SIMULATOR
#  define FOUNDATION_PLATFORM_IOS_SIMULATOR 0
#endif
#ifndef FOUNDATION_PLATFORM_MACOSX
#  define FOUNDATION_PLATFORM_MACOSX 0
#endif
#ifndef FOUNDATION_PLATFORM_LINUX
# define FOUNDATION_PLATFORM_LINUX 0
#endif
#ifndef FOUNDATION_PLATFORM_LINUX_RASPBERRYPI
# define FOUNDATION_PLATFORM_LINUX_RASPBERRYPI 0
#endif
#ifndef FOUNDATION_PLATFORM_BSD
#  define FOUNDATION_PLATFORM_BSD 0
#endif
#ifndef FOUNDATION_PLATFORM_WINDOWS
#  define FOUNDATION_PLATFORM_WINDOWS 0
#endif

//Architectures
#ifndef  FOUNDATION_ARCH_ARM
#  define FOUNDATION_ARCH_ARM 0
#  define FOUNDATION_ARCH_ARM5 0
#  define FOUNDATION_ARCH_ARM6 0
#  define FOUNDATION_ARCH_ARM7 0
#  define FOUNDATION_ARCH_ARM8 0
#endif
#ifndef  FOUNDATION_ARCH_ARM_64
#  define FOUNDATION_ARCH_ARM_64 0
#  define FOUNDATION_ARCH_ARM8_64 0
#endif
#ifndef  FOUNDATION_ARCH_X86
#  define FOUNDATION_ARCH_X86 0
#endif
#ifndef  FOUNDATION_ARCH_X86_64
#  define FOUNDATION_ARCH_X86_64 0
#endif
#ifndef  FOUNDATION_ARCH_PPC
#  define FOUNDATION_ARCH_PPC 0
#endif
#ifndef  FOUNDATION_ARCH_PPC_64
#  define FOUNDATION_ARCH_PPC_64 0
#endif
#ifndef FOUNDATION_ARCH_IA64
#  define FOUNDATION_ARCH_IA64 0
#endif
#ifndef FOUNDATION_ARCH_MIPS
#  define FOUNDATION_ARCH_MIPS 0
#endif

//Architecture details
#ifndef FOUNDATION_ARCH_SSE2
#  define FOUNDATION_ARCH_SSE2 0
#endif
#ifndef FOUNDATION_ARCH_SSE3
#  define FOUNDATION_ARCH_SSE3 0
#endif
#ifndef FOUNDATION_ARCH_SSE4
#  define FOUNDATION_ARCH_SSE4 0
#endif
#ifndef FOUNDATION_ARCH_SSE4_FMA3
#  define FOUNDATION_ARCH_SSE4_FMA3 0
#endif
#ifndef FOUNDATION_ARCH_NEON
#  define FOUNDATION_ARCH_NEON 0
#endif
#ifndef FOUNDATION_ARCH_THUMB
#  define FOUNDATION_ARCH_THUMB 0
#endif
#define FOUNDATION_ARCH_ENDIAN_LITTLE 0
#define FOUNDATION_ARCH_ENDIAN_BIG 0
	
//Platform traits
#define FOUNDATION_PLATFORM_APPLE 0
#define FOUNDATION_PLATFORM_POSIX 0

#define FOUNDATION_PLATFORM_FAMILY_MOBILE 0
#define FOUNDATION_PLATFORM_FAMILY_DESKTOP 0
#define FOUNDATION_PLATFORM_FAMILY_CONSOLE 0

//Compilers
#define FOUNDATION_COMPILER_CLANG 0
#define FOUNDATION_COMPILER_GCC 0
#define FOUNDATION_COMPILER_MSVC 0
#define FOUNDATION_COMPILER_INTEL 0


//First, platforms and architectures

// Android
#if defined( __ANDROID__ ) || FOUNDATION_PLATFORM_ANDROID

#  undef  FOUNDATION_PLATFORM_ANDROID
#  define FOUNDATION_PLATFORM_ANDROID 1

// Compatibile platforms
#  undef  FOUNDATION_PLATFORM_POSIX
#  define FOUNDATION_PLATFORM_POSIX 1

#  define FOUNDATION_PLATFORM_NAME "Android"

// Architecture and detailed description
#  if defined( __arm__ ) || FOUNDATION_ARCH_ARM
#    undef  FOUNDATION_ARCH_ARM
#    define FOUNDATION_ARCH_ARM 1
#    ifdef __ARM_ARCH_7A__
#      undef  FOUNDATION_ARCH_ARM7
#      define FOUNDATION_ARCH_ARM7 1
#      define FOUNDATION_PLATFORM_DESCRIPTION "Android ARMv7"
#    elif defined(__ARM_ARCH_5TE__)
#      undef  FOUNDATION_ARCH_ARM5
#      define FOUNDATION_ARCH_ARM5 1
#      define FOUNDATION_PLATFORM_DESCRIPTION "Android ARMv5"
#    else
#      error Unsupported ARM architecture
#    endif
#  elif defined( __i386__ ) || FOUNDATION_ARCH_X86
#    undef  FOUNDATION_ARCH_X86
#    define FOUNDATION_ARCH_X86 1
#    define FOUNDATION_PLATFORM_DESCRIPTION "Android x86"
#  elif defined( __x86_64__ ) || FOUNDATION_ARCH_X86_64
#    undef  FOUNDATION_ARCH_X86_64
#    define FOUNDATION_ARCH_X86_64 1
#    define FOUNDATION_PLATFORM_DESCRIPTION "Android x86-64"
#  elif defined( __mips__ ) || FOUNDATION_ARCH_MIPS
#    undef  FOUNDATION_ARCH_MIPS
#    define FOUNDATION_ARCH_MIPS 1
#    define FOUNDATION_PLATFORM_DESCRIPTION "Android MIPS"
#    ifndef _MIPS_ISA
#      define _MIPS_ISA 6 /*_MIPS_ISA_MIPS32*/
#    endif
#  else
#    error Unknown architecture
#  endif

// Traits
#  undef  FOUNDATION_ARCH_ENDIAN_LITTLE
#  define FOUNDATION_ARCH_ENDIAN_LITTLE 1

#  undef  FOUNDATION_PLATFORM_FAMILY_MOBILE
#  define FOUNDATION_PLATFORM_FAMILY_MOBILE 1

#  undef  FOUNDATION_PLATFORM_FAMILY_CONSOLE
#  define FOUNDATION_PLATFORM_FAMILY_CONSOLE 1

// Workarounds for weird include dependencies in NDK headers
#  if !defined(__LP64__)
#    define __LP64__ 0
#    define FOUNDATION_PLATFORM_ANDROID_LP64_WORKAROUND
#  endif

// MacOS X and iOS
#elif ( defined( __APPLE__ ) && __APPLE__ ) || FOUNDATION_PLATFORM_IOS || FOUNDATION_PLATFORM_MACOSX

#  undef  FOUNDATION_PLATFORM_APPLE
#  define FOUNDATION_PLATFORM_APPLE 1

#  undef  FOUNDATION_PLATFORM_POSIX
#  define FOUNDATION_PLATFORM_POSIX 1

#  include <TargetConditionals.h>

#  if defined( __IPHONE__ ) || ( defined( TARGET_OS_IPHONE ) && TARGET_OS_IPHONE ) || ( defined( TARGET_IPHONE_SIMULATOR ) && TARGET_IPHONE_SIMULATOR ) || FOUNDATION_PLATFORM_IOS

#    undef  FOUNDATION_PLATFORM_IOS
#    define FOUNDATION_PLATFORM_IOS 1

#    define FOUNDATION_PLATFORM_NAME "iOS"

#    if defined( __arm__ ) || FOUNDATION_ARCH_ARM
#      undef  FOUNDATION_ARCH_ARM
#      define FOUNDATION_ARCH_ARM 1
#      if defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7S__)
#        undef  FOUNDATION_ARCH_ARM7
#        define FOUNDATION_ARCH_ARM7 1
#        define FOUNDATION_PLATFORM_DESCRIPTION "iOS ARMv7"
#        ifndef __ARM_NEON__
#          error Missing ARM NEON support
#        endif
#      elif defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6__)
#        undef  FOUNDATION_ARCH_ARM6
#        define FOUNDATION_ARCH_ARM6 1
#        define FOUNDATION_PLATFORM_DESCRIPTION "iOS ARMv6"
#      else
#        error Unrecognized ARM architecture
#      endif
#    elif defined( __arm64__ ) || FOUNDATION_ARCH_ARM_64
#      undef  FOUNDATION_ARCH_ARM
#      define FOUNDATION_ARCH_ARM 1
#      undef  FOUNDATION_ARCH_ARM_64
#      define FOUNDATION_ARCH_ARM_64 1
#      if defined( __ARM64_ARCH_8__ )
#        undef  FOUNDATION_ARCH_ARM8_64
#        define FOUNDATION_ARCH_ARM8_64 1
#        define FOUNDATION_PLATFORM_DESCRIPTION "iOS ARM64v8"
#      else
#        error Unrecognized ARM architecture
#      endif
#    elif defined( __i386__ ) || FOUNDATION_ARCH_X86
#      undef  FOUNDATION_PLATFORM_IOS_SIMULATOR
#      define FOUNDATION_PLATFORM_IOS_SIMULATOR 1
#      undef  FOUNDATION_ARCH_X86
#      define FOUNDATION_ARCH_X86 1
#      define FOUNDATION_PLATFORM_DESCRIPTION "iOS x86 (simulator)"
#    elif defined( __x86_64__ ) || FOUNDATION_ARCH_X86_64
#      undef  FOUNDATION_PLATFORM_IOS_SIMULATOR
#      define FOUNDATION_PLATFORM_IOS_SIMULATOR 1
#      undef  FOUNDATION_ARCH_X86_64
#      define FOUNDATION_ARCH_X86_64 1
#      define FOUNDATION_PLATFORM_DESCRIPTION "iOS x86_64 (simulator)"
#    else
#      error Unknown architecture
#    endif

#    undef  FOUNDATION_ARCH_ENDIAN_LITTLE
#    define FOUNDATION_ARCH_ENDIAN_LITTLE 1

#    undef  FOUNDATION_PLATFORM_FAMILY_MOBILE
#    define FOUNDATION_PLATFORM_FAMILY_MOBILE 1

#    undef  FOUNDATION_PLATFORM_FAMILY_CONSOLE
#    define FOUNDATION_PLATFORM_FAMILY_CONSOLE 1

#  elif defined( __MACH__ ) || FOUNDATION_PLATFORM_MACOSX

#    undef  FOUNDATION_PLATFORM_MACOSX
#    define FOUNDATION_PLATFORM_MACOSX 1

#    define FOUNDATION_PLATFORM_NAME "MacOSX"

#    if defined( __x86_64__ ) ||  defined( __x86_64 ) || defined( __amd64 ) || FOUNDATION_ARCH_X86_64
#      undef  FOUNDATION_ARCH_X86_64
#      define FOUNDATION_ARCH_X86_64 1
#      undef  FOUNDATION_ARCH_ENDIAN_LITTLE
#      define FOUNDATION_ARCH_ENDIAN_LITTLE 1
#      define FOUNDATION_PLATFORM_DESCRIPTION "MacOSX x86-64"
#    elif defined( __i386__ ) || defined( __intel__ ) || FOUNDATION_ARCH_X86
#      undef  FOUNDATION_ARCH_X86
#      define FOUNDATION_ARCH_X86 1
#      undef  FOUNDATION_ARCH_ENDIAN_LITTLE
#      define FOUNDATION_ARCH_ENDIAN_LITTLE 1
#      define FOUNDATION_PLATFORM_DESCRIPTION "MacOSX x86"

#    elif defined( __powerpc64__ ) || defined( __POWERPC64__ ) || FOUNDATION_ARCH_PPC_64
#      undef  FOUNDATION_ARCH_PPC_64
#      define FOUNDATION_ARCH_PPC_64 1
#      undef  FOUNDATION_ARCH_ENDIAN_BIG
#      define FOUNDATION_ARCH_ENDIAN_BIG 1
#      define FOUNDATION_PLATFORM_DESCRIPTION "MacOSX PPC64"
#    elif defined( __powerpc__ ) || defined( __POWERPC__ ) || FOUNDATION_ARCH_PPC
#      undef  FOUNDATION_ARCH_PPC
#      define FOUNDATION_ARCH_PPC 1
#      undef  FOUNDATION_ARCH_ENDIAN_BIG
#      define FOUNDATION_ARCH_ENDIAN_BIG 1
#      define FOUNDATION_PLATFORM_DESCRIPTION "MacOSX PPC"

#    else
#      error Unknown architecture
#    endif

#    undef  FOUNDATION_PLATFORM_FAMILY_DESKTOP
#    define FOUNDATION_PLATFORM_FAMILY_DESKTOP 1

#  else
#    error Unknown Apple Platform
#  endif

// Linux
#elif ( defined( __linux__ ) || defined( __linux ) ) || FOUNDATION_PLATFORM_LINUX

#  undef  FOUNDATION_PLATFORM_LINUX
#  define FOUNDATION_PLATFORM_LINUX 1

#  undef  FOUNDATION_PLATFORM_POSIX
#  define FOUNDATION_PLATFORM_POSIX 1

#  define FOUNDATION_PLATFORM_NAME "Linux"

#  if defined( __x86_64__ ) || defined( __x86_64 ) || defined( __amd64 ) || FOUNDATION_ARCH_X86_64
#    undef  FOUNDATION_ARCH_X86_64
#    define FOUNDATION_ARCH_X86_64 1
#    undef  FOUNDATION_ARCH_ENDIAN_LITTLE
#    define FOUNDATION_ARCH_ENDIAN_LITTLE 1
#    define FOUNDATION_PLATFORM_DESCRIPTION "Linux x86-64"
#  elif defined( __i386__ ) || defined( __intel__ ) || defined( _M_IX86 ) || FOUNDATION_ARCH_X86
#    undef  FOUNDATION_ARCH_X86
#    define FOUNDATION_ARCH_X86 1
#    undef  FOUNDATION_ARCH_ENDIAN_LITTLE
#    define FOUNDATION_ARCH_ENDIAN_LITTLE 1
#    define FOUNDATION_PLATFORM_DESCRIPTION "Linux x86"

#  elif defined( __powerpc64__ ) || defined( __POWERPC64__ ) || FOUNDATION_ARCH_PPC_64
#    undef  FOUNDATION_ARCH_PPC_64
#    define FOUNDATION_ARCH_PPC_64 1
#    undef  FOUNDATION_ARCH_ENDIAN_BIG
#    define FOUNDATION_ARCH_ENDIAN_BIG 1
#    define FOUNDATION_PLATFORM_DESCRIPTION "Linux PPC64"
#  elif defined( __powerpc__ ) || defined( __POWERPC__ ) || FOUNDATION_ARCH_PPC
#    undef  FOUNDATION_ARCH_PPC
#    define FOUNDATION_ARCH_PPC 1
#    undef  FOUNDATION_ARCH_ENDIAN_BIG
#    define FOUNDATION_ARCH_ENDIAN_BIG 1
#    define FOUNDATION_PLATFORM_DESCRIPTION "Linux PPC"
#  elif defined( __arm__ ) || FOUNDATION_ARCH_ARM
#    undef  FOUNDATION_ARCH_ARM
#    define FOUNDATION_ARCH_ARM 1
#    undef  FOUNDATION_ARCH_ENDIAN_LITTLE
#    define FOUNDATION_ARCH_ENDIAN_LITTLE 1
#    if defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7S__)
#      undef  FOUNDATION_ARCH_ARM7
#      define FOUNDATION_ARCH_ARM7 1
#      define FOUNDATION_PLATFORM_DESCRIPTION "Linux ARMv7"
#      ifndef __ARM_NEON__
#        error Missing ARM NEON support
#      endif
#    elif defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6__)
#      undef  FOUNDATION_ARCH_ARM6
#      define FOUNDATION_ARCH_ARM6 1
#      define FOUNDATION_PLATFORM_DESCRIPTION "Linux ARMv6"
#    else
#      error Unrecognized ARM architecture
#    endif
#  elif defined( __arm64__ ) || FOUNDATION_ARCH_ARM_64
#    undef  FOUNDATION_ARCH_ARM
#    define FOUNDATION_ARCH_ARM 1
#    undef  FOUNDATION_ARCH_ARM_64
#    define FOUNDATION_ARCH_ARM_64 1
#    undef  FOUNDATION_ARCH_ENDIAN_LITTLE
#    define FOUNDATION_ARCH_ENDIAN_LITTLE 1
#    if defined( __ARM64_ARCH_8__ )
#      undef  FOUNDATION_ARCH_ARM8_64
#      define FOUNDATION_ARCH_ARM8_64 1
#      define FOUNDATION_PLATFORM_DESCRIPTION "Linux ARM64v8"
#    else
#      error Unrecognized ARM architecture
#    endif
#  else
#    error Unknown architecture
#  endif

#  if defined( __raspberrypi__ )
#    undef  FOUNDATION_PLATFORM_LINUX_RASPBERRYPI
#    define FOUNDATION_PLATFORM_LINUX_RASPBERRYPI 1
#  endif

#  undef  FOUNDATION_PLATFORM_FAMILY_DESKTOP
#  define FOUNDATION_PLATFORM_FAMILY_DESKTOP 1

#  ifndef _GNU_SOURCE
#    define _GNU_SOURCE
#  endif

//BSD family
#elif ( defined( __BSD__ ) || defined( __FreeBSD__ ) ) || FOUNDATION_PLATFORM_BSD

#  undef  FOUNDATION_PLATFORM_BSD
#  define FOUNDATION_PLATFORM_BSD 1

#  undef  FOUNDATION_PLATFORM_POSIX
#  define FOUNDATION_PLATFORM_POSIX 1

#  define FOUNDATION_PLATFORM_NAME "BSD"

#  if defined( __x86_64__ ) || defined( __x86_64 ) || defined( __amd64 ) || FOUNDATION_ARCH_X86_64
#    undef  FOUNDATION_ARCH_X86_64
#    define FOUNDATION_ARCH_X86_64 1
#    undef  FOUNDATION_ARCH_ENDIAN_LITTLE
#    define FOUNDATION_ARCH_ENDIAN_LITTLE 1
#    define FOUNDATION_PLATFORM_DESCRIPTION "BSD x86-64"
#  elif defined( __i386__ ) || defined( __intel__ ) || defined( _M_IX86 ) || FOUNDATION_ARCH_X86
#    undef  FOUNDATION_ARCH_X86
#    define FOUNDATION_ARCH_X86 1
#    undef  FOUNDATION_ARCH_ENDIAN_LITTLE
#    define FOUNDATION_ARCH_ENDIAN_LITTLE 1
#    define FOUNDATION_PLATFORM_DESCRIPTION "BSD x86"

#  elif defined( __powerpc64__ ) || defined( __POWERPC64__ ) || FOUNDATION_ARCH_PPC_64
#    undef  FOUNDATION_ARCH_PPC_64
#    define FOUNDATION_ARCH_PPC_64 1
#    undef  FOUNDATION_ARCH_ENDIAN_BIG
#    define FOUNDATION_ARCH_ENDIAN_BIG 1
#    define FOUNDATION_PLATFORM_DESCRIPTION "BSD PPC64"
#  elif defined( __powerpc__ ) || defined( __POWERPC__ ) || FOUNDATION_ARCH_PPC
#    undef  FOUNDATION_ARCH_PPC
#    define FOUNDATION_ARCH_PPC 1
#    undef  FOUNDATION_ARCH_ENDIAN_BIG
#    define FOUNDATION_ARCH_ENDIAN_BIG 1
#    define FOUNDATION_PLATFORM_DESCRIPTION "BSD PPC"

#  else
#    error Unknown architecture
#  endif

#  undef  FOUNDATION_PLATFORM_FAMILY_DESKTOP
#  define FOUNDATION_PLATFORM_FAMILY_DESKTOP 1

// Windows
#elif defined( _WIN32 ) || defined( __WIN32__ ) || defined( _WIN64 ) || FOUNDATION_PLATFORM_WINDOWS

#  undef  FOUNDATION_PLATFORM_WINDOWS
#  define FOUNDATION_PLATFORM_WINDOWS 1

#  define FOUNDATION_PLATFORM_NAME "Windows"

#  if defined( __x86_64__ ) || defined( _M_AMD64 ) || defined( _AMD64_ ) || FOUNDATION_ARCH_X86_64
#    undef  FOUNDATION_ARCH_X86_64
#    define FOUNDATION_ARCH_X86_64 1
#    define FOUNDATION_PLATFORM_DESCRIPTION "Windows x86-64"
#  elif defined( __x86__ ) || defined( _M_IX86 ) || defined( _X86_ ) || FOUNDATION_ARCH_X86
#    undef  FOUNDATION_ARCH_X86
#    define FOUNDATION_ARCH_X86 1
#    define FOUNDATION_PLATFORM_DESCRIPTION "Windows x86"
#  elif defined( __ia64__ ) || defined( _M_IA64 ) || defined( _IA64_ ) || FOUNDATION_ARCH_IA64
#    undef  FOUNDATION_ARCH_IA64
#    define FOUNDATION_ARCH_IA64 1
#    define FOUNDATION_PLATFORM_DESCRIPTION "Windows IA-64"

#  else
#    error Unknown architecture
#  endif

#  undef  FOUNDATION_ARCH_ENDIAN_LITTLE
#  define FOUNDATION_ARCH_ENDIAN_LITTLE 1

#  undef  FOUNDATION_PLATFORM_FAMILY_DESKTOP
#  define FOUNDATION_PLATFORM_FAMILY_DESKTOP 1

#  if defined( FOUNDATION_COMPILE ) && FOUNDATION_COMPILE && !defined( _CRT_SECURE_NO_WARNINGS )
#    define _CRT_SECURE_NO_WARNINGS 1
#  endif

#else
#  error Unknown platform
#endif

#if FOUNDATION_ARCH_ARM
#  if defined(__thumb__)
#    undef  FOUNDATION_ARCH_THUMB
#    define FOUNDATION_ARCH_THUMB 1
#  endif
#endif


//Utility macros
#define FOUNDATION_PREPROCESSOR_TOSTRING( x )          _FOUNDATION_PREPROCESSOR_TOSTRING(x)
#define _FOUNDATION_PREPROCESSOR_TOSTRING( x )         #x

#define FOUNDATION_PREPROCESSOR_JOIN( a, b )           _FOUNDATION_PREPROCESSOR_JOIN( a, b )
#define _FOUNDATION_PREPROCESSOR_JOIN( a, b )          _FOUNDATION_PREPROCESSOR_JOIN_INTERNAL( a, b )
#define _FOUNDATION_PREPROCESSOR_JOIN_INTERNAL( a, b ) a##b

#define FOUNDATION_PREPROCESSOR_NARGS( ... )           _FOUNDATION_PREPROCESSOR_NARGS( __VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 )
#define _FOUNDATION_PREPROCESSOR_NARGS( _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _, ... ) _


//Architecture details
#ifdef __SSE2__
#  undef  FOUNDATION_ARCH_SSE2
#  define FOUNDATION_ARCH_SSE2 1
#endif

#ifdef __SSE3__
#  undef  FOUNDATION_ARCH_SSE3
#  define FOUNDATION_ARCH_SSE3 1
#endif

#ifdef __SSE4_1__
#  undef  FOUNDATION_ARCH_SSE4
#  define FOUNDATION_ARCH_SSE4 1
#endif

#ifdef __ARM_NEON__
#  undef  FOUNDATION_ARCH_NEON
#  define FOUNDATION_ARCH_NEON 1
#endif


//Compilers

// CLang
#if defined( __clang__ )

#  undef  FOUNDATION_COMPILER_CLANG
#  define FOUNDATION_COMPILER_CLANG 1

#  define FOUNDATION_COMPILER_NAME "clang"
#  define FOUNDATION_COMPILER_DESCRIPTION FOUNDATION_COMPILER_NAME " " __clang_version__

#  define RESTRICT restrict
#  if FOUNDATION_PLATFORM_WINDOWS
#    define THREADLOCAL
#  else
#    define THREADLOCAL __thread
#  endif

#  define ATTRIBUTE(x) __attribute__((__##x##__))
#  define ATTRIBUTE2(x,y) __attribute__((__##x##__(y)))
#  define ATTRIBUTE3(x,y,z) __attribute__((__##x##__(y,z)))

#  define DEPRECATED ATTRIBUTE(deprecated)
#  define FORCEINLINE inline ATTRIBUTE(always_inline)
#  define NOINLINE ATTRIBUTE(noinline)
#  define PURECALL ATTRIBUTE(pure)
#  define CONSTCALL ATTRIBUTE(const)
#  define ALIGN(x) ATTRIBUTE2(aligned,x)

#  if FOUNDATION_PLATFORM_WINDOWS
#    define STDCALL
#    ifndef __USE_MINGW_ANSI_STDIO
#      define __USE_MINGW_ANSI_STDIO 1
#    endif
#    ifndef _CRT_SECURE_NO_WARNINGS
#      define _CRT_SECURE_NO_WARNINGS 1
#    endif
#    ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#      define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 0
#    endif
#    ifndef _MSC_VER
#      define _MSC_VER 1300
#    endif
#    define USE_NO_MINGW_SETJMP_TWO_ARGS 1
#  endif

#  include <stdbool.h>
#  include <stdarg.h>

#  include <wchar.h>

// GCC
#elif defined( __GNUC__ )

#  undef  FOUNDATION_COMPILER_GCC
#  define FOUNDATION_COMPILER_GCC 1

#  define FOUNDATION_COMPILER_NAME "gcc"
#  define FOUNDATION_COMPILER_DESCRIPTION FOUNDATION_COMPILER_NAME " v" FOUNDATION_PREPROCESSOR_TOSTRING( __GNUC__ ) "." FOUNDATION_PREPROCESSOR_TOSTRING( __GNUC_MINOR__ )

#  define RESTRICT __restrict
#  define THREADLOCAL __thread

#  define ATTRIBUTE(x) __attribute__((__##x##__))
#  define ATTRIBUTE2(x,y) __attribute__((__##x##__(y)))
#  define ATTRIBUTE3(x,y,z) __attribute__((__##x##__(y,z)))

#  define DEPRECATED ATTRIBUTE(deprecated)
#  define FORCEINLINE inline ATTRIBUTE(always_inline)
#  define NOINLINE ATTRIBUTE(noinline)
#  define PURECALL ATTRIBUTE(pure)
#  define CONSTCALL ATTRIBUTE(const)
#  define ALIGN(x) ATTRIBUTE2(aligned,x)

#  if FOUNDATION_PLATFORM_WINDOWS
#    define STDCALL
#    ifndef __USE_MINGW_ANSI_STDIO
#      define __USE_MINGW_ANSI_STDIO 1
#    endif
#    ifndef _CRT_SECURE_NO_WARNINGS
#      define _CRT_SECURE_NO_WARNINGS 1
#    endif
#    ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#      define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 0
#    endif
#    ifndef _MSC_VER
#      define _MSC_VER 1300
#    endif
#  endif

#  include <stdbool.h>
#  include <stdarg.h>

#  include <wchar.h>

// Intel
#elif defined( __ICL ) || defined( __ICC ) || defined( __INTEL_COMPILER )

#  undef  FOUNDATION_COMPILER_INTEL
#  define FOUNDATION_COMPILER_INTEL 1

#  define FOUNDATION_COMPILER_NAME "intel"
#  if defined( __ICL )
#    define FOUNDATION_COMPILER_DESCRIPTION FOUNDATION_COMPILER_NAME " v" FOUNDATION_PREPROCESSOR_TOSTRING( __ICL )
#  elif defined( __ICC )
#    define FOUNDATION_COMPILER_DESCRIPTION FOUNDATION_COMPILER_NAME " v" FOUNDATION_PREPROCESSOR_TOSTRING( __ICC )
#  endif

#  define RESTRICT __restrict
#  define THREADLOCAL __declspec(thread)

#  define ATTRIBUTE(x)
#  define ATTRIBUTE2(x,y)
#  define ATTRIBUTE3(x,y,z)

#  define DEPRECATED 
#  define FORCEINLINE __forceinline
#  define NOINLINE __declspec(noinline)
#  define PURECALL 
#  define CONSTCALL
#  define ALIGN(x) __declspec(align(x))

#  if FOUNDATION_PLATFORM_WINDOWS
#    define STDCALL __stdcall
#    define va_copy(d,s) ((d)=(s))
#  endif

#  include <intrin.h>

#  define bool _Bool
#  define true 1
#  define false 0
#  define __bool_true_false_are_defined 1

// Microsoft
#elif defined( _MSC_VER )

#  undef  FOUNDATION_COMPILER_MSVC
#  define FOUNDATION_COMPILER_MSVC 1

#  define FOUNDATION_COMPILER_NAME "msvc"
#  define FOUNDATION_COMPILER_DESCRIPTION FOUNDATION_COMPILER_NAME " v" FOUNDATION_PREPROCESSOR_TOSTRING( _MSC_VER )

#  define RESTRICT __restrict
#  define THREADLOCAL __declspec(thread)

#  define DEPRECATED __declspec(deprecated)
#  define FORCEINLINE __forceinline
#  define NOINLINE __declspec(noinline)
#  define PURECALL
#  define CONSTCALL
#  define ALIGN(x) __declspec(align(x))

#  if FOUNDATION_PLATFORM_WINDOWS
#    define STDCALL __stdcall
#  endif

#  ifndef __cplusplus
typedef enum
{
	false = 0,
	true  = 1
} bool;
#  endif

#if _MSC_VER < 1800
#  define va_copy(d,s) ((d)=(s))
#endif

#else
#  error Unknown compiler
#endif


//Base data types
#include <stdint.h>
#include <stddef.h>
#include <float.h>
#include <limits.h>

typedef float          float32_t;
typedef double         float64_t;

/*! 128-bit Unsigned int type */
typedef struct {
	uint64_t word[2];
} uint128_t;

/*! 256-bit unsigned int type */
typedef struct {
	uint64_t word[4];
} uint256_t;

#define FLOAT32_C(x)   (x##f)
#define FLOAT64_C(x)   (x)

#if !defined( FOUNDATION_PLATFORM_REALSIZE )
#  define FOUNDATION_PLATFORM_REALSIZE 32
#endif

#if defined( FOUNDATION_PLATFORM_REALSIZE ) && ( FOUNDATION_PLATFORM_REALSIZE > 32 )
typedef   float64_t         real;
#  define REAL_C(x)         FLOAT64_C(x)
#  undef  FOUNDATION_PLATFORM_REALSIZE
#  define FOUNDATION_PLATFORM_REALSIZE 64
#else
typedef   float32_t         real;
#  define REAL_C(x)         FLOAT32_C(x)
#  undef  FOUNDATION_PLATFORM_REALSIZE
#  define FOUNDATION_PLATFORM_REALSIZE 32
#endif

//Pointer size
#if FOUNDATION_ARCH_ARM_64 || FOUNDATION_ARCH_X86_64 || FOUNDATION_ARCH_PPC_64 || FOUNDATION_ARCH_IA64
#  define FOUNDATION_ARCH_POINTER_SIZE 8
#else
#  define FOUNDATION_ARCH_POINTER_SIZE 4
#endif

//wchar_t size
#if FOUNDATION_PLATFORM_LINUX_RASPBERRYPI
#  define FOUNDATION_WCHAR_SIZE 32
#else
#  if WCHAR_MAX > 0xffff
#    define FOUNDATION_WCHAR_SIZE 32
#  else
#    define FOUNDATION_WCHAR_SIZE 16
#  endif
#endif

static FORCEINLINE CONSTCALL uint128_t uint128_make( const uint64_t low, const uint64_t high ) { uint128_t u = { low, high }; return u; }
static FORCEINLINE CONSTCALL bool      uint128_equal( const uint128_t u0, const uint128_t u1 ) { return u0.word[0] == u1.word[0] && u0.word[1] == u1.word[1]; }
static FORCEINLINE CONSTCALL uint128_t uint128_null( void ) { return uint128_make( 0, 0 ); }
static FORCEINLINE CONSTCALL bool      uint128_is_null( const uint128_t u0 ) { return !u0.word[0] && !u0.word[1]; }

static FORCEINLINE CONSTCALL uint256_t uint256_make( const uint64_t w0, const uint64_t w1, const uint64_t w2, const uint64_t w3 ) { uint256_t u = { w0, w1, w2, w3 }; return u; }
static FORCEINLINE CONSTCALL bool      uint256_equal( const uint256_t u0, const uint256_t u1 ) { return u0.word[0] == u1.word[0] && u0.word[1] == u1.word[1] && u0.word[2] == u1.word[2] && u0.word[3] == u1.word[3]; }
static FORCEINLINE CONSTCALL uint256_t uint256_null( void ) { return uint256_make( 0, 0, 0, 0 ); }
static FORCEINLINE CONSTCALL bool      uint256_is_null( const uint256_t u0 ) { return !u0.word[0] && !u0.word[1] && !u0.word[2] && !u0.word[3]; }


// Atomic types
typedef ALIGN(4) struct {
	uint32_t nonatomic;
} atomic32_t;

typedef ALIGN(8) struct {
	uint64_t nonatomic;
} atomic64_t;

typedef ALIGN(FOUNDATION_ARCH_POINTER_SIZE) struct {
	void* nonatomic;
} atomicptr_t;


#define pointer_offset( ptr, ofs ) (void*)((char*)(ptr) + (ptrdiff_t)(ofs))
#define pointer_offset_const( ptr, ofs ) (const void*)((const char*)(ptr) + (ptrdiff_t)(ofs))
#define pointer_diff( first, second ) (ptrdiff_t)((const char*)(first) - (const char*)(second))

#include <string.h>


// Base limits

#define FOUNDATION_MAX_PATHLEN    512


// Wrappers for platforms that not yet support thread-local storage declarations
// For maximum portability use wrapper macros FOUNDATION_DECLARE_THREAD_LOCAL / FOUNDATION_DECLARE_THREAD_LOCAL_ARRAY
// Only works for types that can be safely cast through a uintptr_t (integers, pointers...)
//   FOUNDATION_DECLARE_THREAD_LOCAL( int, profile_block, 0 );
//   FOUNDATION_DECLARE_THREAD_LOCAL_ARRAY( void*, profile_free, 32 );
//   set_thread_profile_block( 1 ); // Assigns 1 to thread-local variable "profile_block"
//   get_thread_profile_free()[0] = some_ptr; // Assigns some_ptr to slot 0 in thread-local array "profile_free"
#if FOUNDATION_PLATFORM_APPLE || FOUNDATION_PLATFORM_ANDROID

// Forward declarations of various system APIs
#  if FOUNDATION_PLATFORM_ANDROID
typedef int _pthread_key_t;
#  else
typedef __darwin_pthread_key_t _pthread_key_t;
#  endif
FOUNDATION_EXTERN int pthread_key_create( _pthread_key_t*, void (*)(void*) );
FOUNDATION_EXTERN int pthread_setspecific( _pthread_key_t, const void* );
FOUNDATION_EXTERN void* pthread_getspecific( _pthread_key_t );

FOUNDATION_API void* _allocate_thread_local_block( unsigned int size );

#define FOUNDATION_DECLARE_THREAD_LOCAL( type, name, init ) \
static _pthread_key_t _##name##_key = 0; \
static FORCEINLINE _pthread_key_t get_##name##_key( void ) { if( !_##name##_key ) { pthread_key_create( &_##name##_key, 0 ); pthread_setspecific( _##name##_key, (init) ); } return _##name##_key; } \
static FORCEINLINE type get_thread_##name( void ) { return (type)((uintptr_t)pthread_getspecific( get_##name##_key() )); } \
static FORCEINLINE void set_thread_##name( type val ) { pthread_setspecific( get_##name##_key(), (const void*)(uintptr_t)val ); }

#define FOUNDATION_DECLARE_THREAD_LOCAL_ARRAY( type, name, arrsize ) \
static _pthread_key_t _##name##_key = 0; \
static FORCEINLINE _pthread_key_t get_##name##_key( void ) { if( !_##name##_key ) pthread_key_create( &_##name##_key, 0 ); return _##name##_key; } \
static FORCEINLINE type* get_thread_##name( void ) { _pthread_key_t key = get_##name##_key(); type* arr = (type*)pthread_getspecific( key ); if( !arr ) { arr = _allocate_thread_local_block( sizeof( type ) * arrsize ); pthread_setspecific( key, arr ); } return arr; }

#elif FOUNDATION_PLATFORM_WINDOWS && FOUNDATION_COMPILER_CLANG

__declspec(dllimport) unsigned long STDCALL TlsAlloc();
__declspec(dllimport) void* STDCALL TlsGetValue( unsigned long );
__declspec(dllimport) int STDCALL TlsSetValue( unsigned long, void* );

FOUNDATION_API void* _allocate_thread_local_block( unsigned int size );

#define FOUNDATION_DECLARE_THREAD_LOCAL( type, name, init ) \
static unsigned long _##name##_key = 0; \
static FORCEINLINE unsigned long get_##name##_key( void ) { if( !_##name##_key ) { _##name##_key = TlsAlloc(); TlsSetValue( _##name##_key, init ); } return _##name##_key; } \
static FORCEINLINE type get_thread_##name( void ) { return (type)((uintptr_t)TlsGetValue( get_##name##_key() )); } \
static FORCEINLINE void set_thread_##name( type val ) { TlsSetValue( get_##name##_key(), (void*)((uintptr_t)val) ); }

#define FOUNDATION_DECLARE_THREAD_LOCAL_ARRAY( type, name, arrsize ) \
static unsigned long _##name##_key = 0; \
static FORCEINLINE unsigned long get_##name##_key( void ) { if( !_##name##_key ) _##name##_key = TlsAlloc(); return _##name##_key; } \
static FORCEINLINE type* get_thread_##name( void ) { unsigned long key = get_##name##_key(); type* arr = (type*)TlsGetValue( key ); if( !arr ) { arr = _allocate_thread_local_block( sizeof( type ) * arrsize ); TlsSetValue( key, arr ); } return arr; }

#else

#define FOUNDATION_DECLARE_THREAD_LOCAL( type, name, init ) \
static THREADLOCAL type _thread_##name = init; \
static FORCEINLINE void set_thread_##name( type val ) { _thread_##name = val; } \
static FORCEINLINE type get_thread_##name( void ) { return _thread_##name; }

#define FOUNDATION_DECLARE_THREAD_LOCAL_ARRAY( type, name, arrsize ) \
static THREADLOCAL type _thread_##name [arrsize] = {0}; \
static FORCEINLINE type* get_thread_##name( void ) { return _thread_##name; }

#endif


//Format specifiers for 64bit and pointers

#if defined( _MSC_VER )
#  define PRId64       "I64d"
#  define PRIi64       "I64i"
#  define PRIdPTR      "Id"
#  define PRIiPTR      "Ii"
#  define PRIo64       "I64o"
#  define PRIu64       "I64u"
#  define PRIx64       "I64x"
#  define PRIX64       "I64X"
#  define PRIoPTR      "Io"
#  define PRIuPTR      "Iu"
#  define PRIxPTR      "Ix"
#  define PRIXPTR      "IX"
#else
#  ifndef __STDC_FORMAT_MACROS
#    define __STDC_FORMAT_MACROS
#  endif
#  include <inttypes.h>
#endif

#if FOUNDATION_PLATFORM_REALSIZE == 64
#  define PRIREAL      "llf"
#else
#  define PRIREAL      "f"
#endif

#if FOUNDATION_PLATFORM_WINDOWS
#  if FOUNDATION_ARCH_POINTER_SIZE == 8
#    define PRIfixPTR  "016I64X"
#  else
#    define PRIfixPTR  "08IX"
#  endif
#else
#  if FOUNDATION_ARCH_POINTER_SIZE == 8
#    define PRIfixPTR  "016llX"
#  else
#    define PRIfixPTR  "08X"
#  endif
#endif


#if FOUNDATION_PLATFORM_ANDROID && defined(FOUNDATION_PLATFORM_ANDROID_LP64_WORKAROUND)
#  undef __LP64__
#  undef FOUNDATION_PLATFORM_ANDROID_LP64_WORKAROUND
#endif
