/*

  The MIT License (MIT)

  Copyright (c) 2013 Matthew Stump

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

*/

#ifndef _GO_C_V8_
#define _GO_C_V8_

#ifdef __cplusplus
extern "C" {
#endif

#if !defined _WIN32_WCE
#include <errno.h>
#endif
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#ifndef __cplusplus
typedef unsigned char bool;
static const bool false = 0;
static const bool true = 1;
#endif

/*  Handle DSO symbol visibility */
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define GOCV8_EXPORT __attribute__ ((dllexport))
    #else
      #define GOCV8_EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define GOCV8_EXPORT __attribute__ ((dllimport))
    #else
      #define GOCV8_EXPORT __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
#else
  #if __GNUC__ >= 4
    #define GOCV8_EXPORT __attribute__ ((visibility ("default")))
  #else
    #define GOCV8_EXPORT
  #endif
#endif


GOCV8_EXPORT void*
gocv8_context_new();

GOCV8_EXPORT void
gocv8_context_free(
    void* context);

GOCV8_EXPORT bool
gocv8_init(
    void*  context,
    void** status);

bool
gocv8_process_event(
    void*       context,
    const char* event,
    size_t      event_size,
    const char* js,
    size_t      js_size,
    void**      status);


#undef GOCV8_EXPORT

#ifdef __cplusplus
}
#endif

#endif // _GO_C_V8_
