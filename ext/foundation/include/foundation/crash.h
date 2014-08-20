/* crash.h  -  Foundation library  -  Public Domain  -  2013 Mattias Jansson / Rampant Pixels
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

/*! \file crash.h
    Crash guards and dump utilities */

#include <foundation/platform.h>
#include <foundation/types.h>


/*! Guard the given method and generate a crash dump if the function generates
	an abnormal program termination.
	\param fn                              Function to wrap
	\param data                            Argument
    \param callback                        Function to call on crash dump
    \param name                            Dump name prefix
	\return                                Function return value, CRASH_DUMP_GENERATED if dump was generated */
FOUNDATION_API int                         crash_guard( crash_guard_fn fn, void* data, crash_dump_callback_fn callback, const char* name );

/*! Set the global crash guard callback and dump name prefix. This will be used for all new threads created.
    \param callback                        Function to call on crash dump
    \param name                            Dump name prefix */
FOUNDATION_API void                        crash_guard_set( crash_dump_callback_fn callback, const char* name );

/*! Get current global dump name prefix
    \return                                Dump name prefix */
FOUNDATION_API const char*                 crash_guard_name( void );

/*! Get current global crash dump callback
    \return                                Crash dump callback */
FOUNDATION_API crash_dump_callback_fn      crash_guard_callback( void );

/*! Cause a debug break that will be caught by any attached debugger */
FOUNDATION_API void                        crash_debug_break( void );

/*! Cause a core dump (illegal instruction) that will hard crash the process */
FOUNDATION_API void                        crash_dump( void );

