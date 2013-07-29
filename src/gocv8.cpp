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

#include "gocv8/gocv8.h"

struct gocv8_context_t {

};

void*
gocv8_context_new()
{
    return new gocv8_context_t();
}

void
gocv8_context_free(
    void* context)
{
    delete static_cast<gocv8_context_t*>(context);
}

bool
gocv8_init(
    void*  context,
    void** status)
{
    (void) context;
    (void) status;
    return false;
}

bool
gocv8_process_event(
    void*  context,
    char*  event,
    size_t event_size,
    char*  js,
    size_t js_size,
    void** status)
{
    (void) context;
    (void) event;
    (void) event_size;
    (void) js;
    (void) js_size;
    (void) status;
    return false;
}
