/* regex.h  -  Foundation library  -  Public Domain  -  2013 Mattias Jansson / Rampant Pixels
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

/*! \file regex.h
    Small regular expression implementation matching a subset of Perl regular expression syntax.
    ^        Match beginning of string
    $        Match end of string
    ()       Group and capture substring
    []       Match any character from the grouped set
    [^]      Match any character but ones from the grouped set
    \s       Match whitespace
    \S       Match non-whitespace
    \d       Match decimal digits
    \D       Match non-digits
    \n       Match newline
    \r       Match carriage return
    +        Match one or more times (longest possible match)
    +?       Match one or more times (shortest possible match)
    *        Match zero or more times (longest possible match)
    *?       Match zero or more times (shortest possible match)
    ?        Match zero or once
    \XX      Match byte with hex value 0xXX
    \meta    Match one of the meta characters ^$()[].*+?|\ */

#include <foundation/platform.h>


/*! Precompile a regular expression
    \param pattern                   Pattern string
    \return                          Compiled expression, null if error */
FOUNDATION_API regex_t*              regex_compile( const char* pattern );

/*! Match input string with regular expression
    \param regex                     Compiled expression
    \param input                     Input string
    \param inlength                  Input length (0 for input string length)
	\param captures                  Result capture array, null if not wanted
    \param maxcaptures               Maximum number of captures
    \return                          true if string matches expression, false if not */
FOUNDATION_API bool                  regex_match( regex_t* regex, const char* input, int inlength, regex_capture_t* captures, int maxcaptures );

/*! Free a compiled expression
    \param regex                     Compiled expression */
FOUNDATION_API void                  regex_free( regex_t* regex );

