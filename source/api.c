#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <parser.h>
#include <io.h>
#include <api.h>
#include <math.h>

struct json_object_t *json_to_object(char *file_name) {
  char *buff=0;
  size_t read_bytes=read_from_file(file_name, &buff);
  struct json_object_t *json=parse(buff, read_bytes);
  free(buff);
  return json;
}

size_t has_key(struct json_object_t *object, char *key) {
  for(int i=0; i<object->size; ++i) {
    if(!strcmp(object->keys[i]->literal, key)) return i;
  }
  return -1;
}

double json_number_to_number(struct json_number_t *json_number) {
  double digits=atof(json_number->digits);
  double fraction=0;
  double expo=0;
  long power=1;
  if(json_number->fraction) {
    for(int i=0; i<strlen(json_number->fraction); ++i) power*=10;
    fraction=atof(json_number->fraction)/power;
  }
  if(json_number->exp) {
    expo=exp(atof(json_number->exp));
  }
  return (digits+fraction)*expo;
}

struct json_value_t *get(struct json_object_t *object, char *key) {
  size_t index=has_key(object, key);
  if(index==-1) return 0;
  return object->values[index];
}

double get_number(struct json_object_t *object, char *key) {
  struct json_value_t *value=get(object, key);
  assert(value != 0);
  assert(value->type == J_NUMBER);
  return json_number_to_number((struct json_number_t *)(value->data));
}

bool get_bool(struct json_object_t *object, char *key) {
  struct json_value_t *value=get(object, key);
  assert(value != 0);
  assert(value->type == J_BOOL);
  return VALUE_TO_BOOL(value);
}

char *get_string(struct json_object_t *object, char *key) {
  struct json_value_t *value=get(object, key);
  assert(value != 0);
  assert(value->type == J_STRING);
  return VALUE_TO_STRING(value);
}

struct json_object_t *get_object(struct json_object_t *object, char *key) {
  struct json_value_t *value=get(object, key);
  assert(value != 0);
  assert(value->type == J_OBJECT);
  return VALUE_TO_OBJECT(value);
}

struct json_array_t *get_array(struct json_object_t *object, char *key) {
  struct json_value_t *value=get(object, key);
  assert(value != 0);
  assert(value->type == J_ARRAY);
  return VALUE_TO_ARRAY(value);
}

double get_array_number_element(struct json_object_t *object, char *key, uint8_t idx) {
  struct json_array_t *arr=get_array(object, key);
  assert(idx < arr->size && idx >= 0);
  assert(arr->elements[idx]->type == J_NUMBER);
  return json_number_to_number((struct json_number_t *)(arr->elements[idx]->data));
}

struct json_object_t *get_array_object_element(struct json_object_t *object, char *key, uint8_t idx) {
  struct json_array_t *arr=get_array(object, key);
  assert(idx < arr->size && idx >= 0);
  assert(arr->elements[idx]->type == J_OBJECT);
  return VALUE_TO_OBJECT(arr->elements[idx]);
}


struct json_array_t *get_array_array_element(struct json_object_t *object, char *key, uint8_t idx) {
  struct json_array_t *arr=get_array(object, key);
  assert(idx < arr->size && idx >= 0);
  assert(arr->elements[idx]->type == J_ARRAY);
  return VALUE_TO_ARRAY(arr->elements[idx]);
}
