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