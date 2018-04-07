// MIT License
//
// Copyright (c) 2018 Akhil Indurti
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef TEST_H
#define TEST_H

#include <stdarg.h>
#include <stdio.h>
#include "str.h"

#define SUCCESS 0
#define FAIL 1

typedef struct t_s Test;
struct t_s {
  const char* fname;
  string buf;
  int status;
  void (*fn)(Test* test);
};

Test NewTest(const char* fname, void (*fn)(Test* test)) {
  Test t = {fname, str.New(""), SUCCESS, fn};
  return t;
}

void Errorf(Test* t, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char* s;
  vasprintf(&s, fmt, args);
  t->buf = str.CSCat(t->buf, s);
  free(s);
  va_end(args);
  t->status = FAIL;
}

#define Fatalf(...)    \
  fatalf(__VA_ARGS__); \
  return;

void fatalf(Test* t, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char* s;
  vasprintf(&s, fmt, args);
  t->buf = str.CSCat(t->buf, s);
  free(s);
  va_end(args);
  t->status = FAIL;
}

void Run(Test* tests, int len) {
  int failed = 0;
  for (int i = 0; i < len; i++) {
    Test t = tests[i];
    t.fn(&t);
    if (t.status != SUCCESS) {
      failed = 1;
      printf("\"%s\" failed the following tests:\n", t.fname);
      printf("%s", t.buf);
      str.Free(t.buf);
    }
  }
  if (!failed) {
    printf("PASS\n");
  }
}

#endif