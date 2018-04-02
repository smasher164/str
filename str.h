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
#ifndef STR_H
#define STR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint_t;

// string points to a middle of a structure holding a length field,
// front offset, and an array of characters. It is created via
// str.New with an empty value represented by str.New(""). The order of struct
// fields allows it to be printed like a regular char* in printf.
typedef char* string;

struct string_lib {
  // New creates a string from a C string. An empty string is reflected by a
  // null pointer.
  string (*New)(const char* cs);
  // Copy creates a newly allocated string with the same contents and length of
  // src.
  string (*Copy)(string src);
  // Cat concatenates a string object to another string object.
  string (*Cat)(string s1, string s2);
  // CSCat concatenates a C string to a string object.
  string (*CSCat)(string src, const char* cs);
  // Slice returns a string that represents a portion of src starting
  // at beg (inclusive) and ending at end (exclusive).
  string (*Slice)(string src, uint_t beg, uint_t end);
  // SliceCopy creates a new string starting at beg (inclusive) and
  // ending at end (exclusive).
  string (*SliceCopy)(string src, uint_t beg, uint_t end);
  // Len returns the length of the string in constant time.
  uint_t (*Len)(string s);
  // Free deletes the stringheader along with its contents.
  void (*Free)(string s);
};

// make allocates a stringheader with the specified length.
static inline string make(const char* cs, uint_t len) {
  char* s = malloc((2 * sizeof(uint_t)) + (sizeof(char) * (len + 1)));
  memcpy(s + (2 * sizeof(uint_t)), cs, len + 1);
  (*(uint_t**)&s)[0] = len;
  (*(uint_t**)&s)[1] = sizeof(uint_t);
  return s + (2 * sizeof(uint_t));
}

// new creates a string from a c string. An empty string is reflected by a
// null pointer.
static inline string new (const char* cs) {
  uint_t len = (cs == NULL) ? 0 : strlen(cs);
  return make(cs, len);
}

// grow reallocates the string to a new length.
static inline string grow(string src, uint_t newlen) {
  uint_t offset = *(uint_t*)(src - sizeof(uint_t));
  char* s = realloc(src - sizeof(uint_t) - offset,
                    (2 * sizeof(uint_t)) + (sizeof(char) * (newlen + 1)));
  (*(uint_t**)&s)[0] = newlen;
  (*(uint_t**)&s)[1] = sizeof(uint_t);
  return s + (2 * sizeof(uint_t));
}

// len returns the length of the string in constant time.
static inline uint_t len(string s) {
  uint_t offset = *(uint_t*)(s - sizeof(uint_t));
  uint_t l = *(uint_t*)(s - sizeof(uint_t) - offset);
  return l;
}

// copy creates a newly allocated string with the same contents and length of
// src.
static inline string copy(string src) { return make(src, len(src)); }

// strfree deletes the stringheader along with its contents.
static inline void strfree(string s) {
  uint_t offset = *(uint_t*)(s - sizeof(uint_t));
  free(s - sizeof(uint_t) - offset);
}

// catcs concatenates a c string to a string object.
static inline string catcs(string src, const char* cs) {
  uint_t cslen = strlen(cs);
  uint_t slen = len(src);
  string s = grow(src, slen + cslen);
  memcpy(s + slen, cs, cslen + 1);
  return s;
}

// cat concatenates a string object to another string object.
static inline string cat(string s1, string s2) {
  uint_t ls1 = len(s1);
  uint_t ls2 = len(s2);
  string s = grow(s1, ls1 + ls2);
  memcpy(s + ls1, s2, ls2 + 1);
  return s;
}

// slice returns a string that represents a portion of src starting
// at beg (inclusive) and ending at end (exclusive).
static inline string slice(string src, uint_t beg, uint_t end) {
  uint_t newlen = end - beg;
  src += beg;
  string s = src - sizeof(uint_t);
  (*(uint_t**)&s)[0] += beg;
  uint_t offset = *(uint_t*)s;
  s -= offset;
  (*(uint_t**)&s)[0] = newlen;
  src[newlen] = '\0';
  return src;
}

// slicecopy creates a new string starting at beg (inclusive) and ending at end
// (exclusive).
static inline string slicecopy(string src, uint_t beg, uint_t end) {
  size_t newlen = end - beg;
  string s = make(src + beg, newlen);
  s[newlen] = '\0';
  return s;
}

struct string_lib const str = {new,   copy,      cat, catcs,
                               slice, slicecopy, len, strfree};

#endif