# str

str is a lightweight, header-only, string-handling library for C.

str defines a data type named `string` and the following functions under a package namespace called `str`.
```C
// New creates a string from a C string. An empty string
// is reflected by a null pointer.
string New(const char* cs);

// Copy creates a newly allocated string with the same
// contents and length of src.
string Copy(string src);

// Cat concatenates a string object to another string object.
string Cat(string s1, string s2);

// CSCat concatenates a C string to a string object.
string CSCat(string src, const char* cs);

// Slice returns a string that represents a portion of src starting
// at beg (inclusive) and ending at end (exclusive).
string Slice(string src, size_t beg, size_t end);

// Slice creates a new string starting at beg (inclusive) and
// ending at end (exclusive).
string SliceCopy(string src, size_t beg, size_t end);

// Len returns the length of the string in constant time.
size_t Len(string s);

// Free deletes the stringheader along with its contents.
void Free(string s);
```

The following example demonstrates simple usage of this library:
```C
#include <stdio.h>
#include "str.h"

int main() {
  string s = str.New("hello");
  s = str.CSCat(s, " world!");
  string s2 = str.New("world");
  s = str.Cat(s, s2);
  s = str.Slice(s, 0, 12);
  printf("%s\n", s);
  str.Free(s);
  str.Free(s2);
}
```