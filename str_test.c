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
#include "str.h"
#include "string.h"
#include "test.h"

void copyHelper(Test* t, string s, string s2) {
  string copy = str.Copy(s);
  if (strcmp(s, copy)) {
    Errorf(t, "Expected: '%s', got: '%s'", s, copy);
  }
  str.Free(s);
  if (s2 != NULL) {
    str.Free(s2);
  }
  str.Free(copy);
}

void TestCopy(Test* t) {
  char* cases[5] = {
      "a",
      "abc",
      "copy the concatenation of the second string to this",
      "the second \"should\" should be destructively sliced in",
      "the second \"should\" should be copy sliced in",
  };
  string s, copy, s2;
  s = copy = s2 = NULL;
  // copy "a"
  s = str.New(cases[0]);
  copyHelper(t, s, s2);

  // copy "abc"
  s = str.New(cases[1]);
  copyHelper(t, s, s2);

  // concatenate "abc" to third string as c string
  s = str.New(cases[2]);
  s = str.CSCat(s, cases[1]);
  copyHelper(t, s, s2);

  // concatenate "abc" to third string as string object
  s2 = str.New(cases[1]);
  s = str.New(cases[2]);
  s = str.Cat(s, s2);
  copyHelper(t, s, s2);
  s2 = NULL;

  // Slice out second "should"
  s = str.New(cases[3]);
  s = str.Slice(s, 12, 18);
  copyHelper(t, s, s2);

  // SliceCopy out second "should"
  s2 = str.New(cases[4]);
  s = str.SliceCopy(s2, 12, 18);
  copyHelper(t, s, s2);
}

void TestCat(Test* t) {
  typedef struct {
    string in1;
    string in2;
    char* want;
  } case_t;
  case_t cases[] = {{str.New(""), str.New(""), ""},
                    {str.New(""), str.New("b"), "b"},
                    {str.New("a"), str.New(""), "a"},
                    {str.New("aa"), str.New("bb"), "aabb"}};
  for (int i = 0; i < 4; i++) {
    cases[i].in1 = str.Cat(cases[i].in1, cases[i].in2);
    if (strcmp(cases[i].in1, cases[i].want)) {
      Errorf(t, "Expected \"%s\", got \"%s\"", cases[i].want, cases[i].in1);
    }
    str.Free(cases[i].in1);
    str.Free(cases[i].in2);
  }
}
void TestCSCat(Test* t) {
  typedef struct {
    string in1;
    char* in2;
    char* want;
  } case_t;
  case_t cases[] = {{str.New(""), "", ""},
                    {str.New(""), "b", "b"},
                    {str.New("a"), "", "a"},
                    {str.New("aa"), "bb", "aabb"}};
  for (int i = 0; i < 4; i++) {
    cases[i].in1 = str.CSCat(cases[i].in1, cases[i].in2);
    if (strcmp(cases[i].in1, cases[i].want)) {
      Errorf(t, "Expected \"%s\", got \"%s\"", cases[i].want, cases[i].in1);
    }
    str.Free(cases[i].in1);
  }
}
void TestSlice(Test* t) {
  typedef struct {
    char* in;
    size_t beg;
    size_t end;
    char* want;
  } case_t;
  case_t cases[] = {
      {"", 0, 0, ""},                // slicing empty string
      {"foreplay", 4, 8, "play"},    // slicing beginning
      {"background", 0, 4, "back"},  // slicing from end
      {"hello this world is mine for the taking", 6, 24,
       "this world is mine"},  // slicing to make shorter
      {"", 5, 10, "world"},    // reslicing, use previous def of s
      {"", 2, 2, ""},          // slice to empty, use previous def of s
  };
  string s;
  int len;
  for (int i = 0; i < 6; i++) {
    len = strlen(cases[i].want);
    if (i < 4) {
      s = str.New(cases[i].in);
    }
    s = str.Slice(s, cases[i].beg, cases[i].end);
    if (strcmp(s, cases[i].want) || str.Len(s) != len) {
      Errorf(t, "case %d: Expected \"%s\", got \"%s\"\n", i, cases[i].want, s);
    }
    if (i < 3) {
      str.Free(s);
    }
  }
  str.Free(s);
}
void TestSliceCopy(Test* t) {
  typedef struct {
    char* in;
    size_t beg;
    size_t end;
    char* want;
  } case_t;
  case_t cases[] = {
      {"", 0, 0, ""},                // slicing empty string
      {"foreplay", 4, 8, "play"},    // slicing beginning
      {"background", 0, 4, "back"},  // slicing from end
      {"hello this world is mine for the taking", 6, 24,
       "this world is mine"},  // slicing to make shorter
      {"", 5, 10, "world"},    // reslicing, use previous def of s
      {"", 2, 2, ""},          // slice to empty, use previous def of s
  };
  string s, s2;
  int len, len2;
  for (int i = 0; i < 6; i++) {
    len = strlen(cases[i].want);
    len2 = strlen(cases[i].in);
    if (i < 4) {
      s = str.New(cases[i].in);
    }
    s2 = str.SliceCopy(s, cases[i].beg, cases[i].end);
    if (i < 4) {
      if (strcmp(s2, cases[i].want) || str.Len(s2) != len ||
          strcmp(s, cases[i].in) || str.Len(s) != len2) {
        Errorf(
            t,
            "case %d: Expected s2: \"%s\", got \"%s\", Expected s: \"%s\", got "
            "\"%s\"\n",
            i, cases[i].want, s2, cases[i].in, s);
      }
    } else {
      if (strcmp(s2, cases[i].want) || str.Len(s2) != len) {
        Errorf(t, "case %d: Expected s2: \"%s\", got \"%s\"", i, cases[i].want);
      }
    }
    if (i < 3) {
      str.Free(s);
      str.Free(s2);
    }
    s = s2;
  }
  str.Free(s);
}

int main() {
  Test tests[] = {NewTest("TestCopy", TestCopy), NewTest("TestCat", TestCat),
                  NewTest("TestCSCat", TestCSCat),
                  NewTest("TestSlice", TestSlice),
                  NewTest("TestSliceCopy", TestSliceCopy)};
  Run(tests, (sizeof(tests) / sizeof((tests)[0])));
}