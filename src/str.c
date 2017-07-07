#include "fatal_error.h"
#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t _DEFAULT_CHUNK_SIZE = 64; // size to grow buffer by when necessary

/**
 * create a new Str
 */
Str *
str_new(const char *value)
{
  Str *t = malloc(sizeof (Str));
  if (t == NULL) fatal_error("str_new: malloc error");

  t->chunk_size = _DEFAULT_CHUNK_SIZE;
  t->length = strlen(value);
  t->size = t->length + 1;

  t->buffer = malloc(t->size);
  if (t->buffer == NULL) fatal_error("str_new: malloc error");
  strcpy(t->buffer, value);
  
  return t;
}

/**
 * destroy a Str
 */
void
str_free(Str *t)
{
  free(t->buffer);
  free(t);
}

/** 
 * set a chunk size for a Str to override the _DEFAULT_CHUNK_SIZE
 */
void
str_set_chunk_size(Str *t, const size_t size)
{
  if (size < 1) fatal_error("str_set_chunk_size: size cannot be < 1");
  t->chunk_size = size;
}

/**
 * get the contents of a Str
 */ 
const char *
str_get(const Str *t)
{
  return t->buffer;
}

/**
 * get the length of a Str
 */
size_t
str_length(const Str *t)
{
  return t->length;
}

/**
 * get the memory buffer size of a Str
 */
size_t
str_size(const Str *t)
{
  return t->size;
}

/**
 * grow the memory buffer of a Str
 */
void
_str_grow_buffer(Str *t, const size_t new_length)
{
  while (t->size <= new_length) {
    t->size += t->chunk_size;
  }

  t->buffer = realloc(t->buffer, t->size);
  if (t->buffer == NULL) fatal_error("_str_grow_buffer: realloc error");
}

/**
 * append to a Str
 */
void
str_append(Str *t, const char *value)
{
  const size_t value_length = strlen(value);
  const size_t new_length = t->length + value_length;

  if (t->size <= new_length) {
    _str_grow_buffer(t, new_length);
  }

  strcpy(t->buffer + t->length, value);
  t->length = new_length;
}

/**
 * append a character to a Str
 */
void
str_append_char(Str *t, const char value)
{
  const char buffer[2] = { value };
  str_append(t, buffer);
}

/**
 * append an integer to a Str
 */
void
str_append_int(Str *t, const int value)
{
  char buffer[32];
  sprintf(buffer, "%d", value);
  str_append(t, buffer);
}

