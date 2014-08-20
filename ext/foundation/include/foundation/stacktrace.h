/* stacktrace.h  -  Foundation library  -  Public Domain  -  2013 Mattias Jansson / Rampant Pixels
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

/*! \file stacktrace.h
    Stacktrace utilities */

#include <foundation/platform.h>
#include <foundation/types.h>

/*! Capture stack trace. If not supported, number of captured frames will be zero.
    \param trace               Stack trace buffer. Must be able to hold max_depth number of frame pointers.
	\param max_depth           Maximum call stack depth to capture
	\param skip_frames         Number of initial frames to skip before starting capture
	\return                    Number of stack frames captured */
FOUNDATION_API unsigned int    stacktrace_capture( void** trace, unsigned int max_depth, unsigned int skip_frames );

/*! Resolve a previously captured stack trace. Will allocate memory to hold the resolved string.
    \param trace               Stack trace buffer
	\param max_depth           Maximum call stack depth to resolve
	\param skip_frames         Number of initial frames to skip before starting resolve
	\return                    Resolved stack trace string, empty string if unable to resolve */
FOUNDATION_API char*           stacktrace_resolve( void** trace, unsigned int max_depth, unsigned int skip_frames );
