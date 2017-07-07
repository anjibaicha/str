#ifndef str_h
#define str_h

#include <stdlib.h>

typedef struct Str {
  char *buffer;      // actual character data
  size_t size;       // size of buffer in memory
  size_t length;     // length of actual data
  size_t chunk_size; // amount to grow buffer by when necessary
} Str;

Str *str_new(const char *value);
void str_free(Str *t);
void str_set_chunk_size(Str *t, const size_t size);
const char *str_get(const Str *t);
size_t str_length(const Str *t);
size_t str_size(const Str *t);
void str_append(Str *t, const char *value);
void str_append_char(Str *t, const char value);
void str_append_int(Str *t, const int value);

#endif

