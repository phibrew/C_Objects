#include "cobject.h"
#include <stdlib.h>
#include <string.h>
#include "bootlib.h"

snek_object_t *new_snek_integer(int value) {
	snek_object_t *int_object = malloc(sizeof(snek_object_t));
	if(!int_object) return NULL;

	int_object->kind = INTEGER;
	int_object->data.v_int = value;
	return int_object;
}

snek_object_t *new_snek_float(float value){
	snek_object_t *float_object = malloc(sizeof(snek_object_t));
	if(!float_object) return NULL;

	float_object->kind = FLOAT;
	float_object->data.v_float = value;
	return float_object;
}	

snek_object_t *new_snek_string(char *value){
	snek_object_t *string_object = malloc(sizeof(snek_object_t));
	if(!string_object) return NULL;

	size_t len = strlen(value);
	char* new_val = malloc(len + 1);
	if(!new_val){	
		free(string_object);
		return NULL;
	}

	strcpy(new_val, value);
	string_object->kind = STRING;
	string_object->data.v_string = new_val;

	return string_object;
}

snek_object_t *new_snek_vector3(snek_object_t *x, snek_object_t *y, snek_object_t *z){
	if(!x || !y || !z) return NULL;

	snek_object_t *vector_object = malloc(sizeof(snek_object_t));
	if(!vector_object) return NULL;

	vector_object->kind = VECTOR3;
	vector_object->data.v_vector3 = (snek_vector_t){.x=x, .y=y, .z=z};

	return vector_object;
}

snek_object_t *new_snek_array(size_t size){
	snek_object_t *array_object = malloc(sizeof(snek_object_t));
	if(array_object == NULL) return NULL;

	snek_object_t **arr = calloc(size, sizeof(snek_object_t*));
	if(!arr){
		free(array_object);
		return NULL;
	}

	array_object->kind = ARRAY;
	array_object->data.v_array = (snek_array_t){.size = size, .elements = arr};

	return array_object;
}

bool snek_array_set(snek_object_t *array, size_t index, snek_object_t *value){
	if(!array || !value) return false;
	if(array->kind != ARRAY) return false;
	if(index >= array->data.v_array.size) return false;

	array->data.v_array.elements[index] = value;
	return true;
}

snek_object_t *snek_array_get(snek_object_t *array, size_t index){
	if(!array) return NULL;
	if(array->kind != ARRAY) return NULL;
	if(index >= array->data.v_array.size) return NULL;

	return array->data.v_array.elements[index];
}

int snek_length(snek_object_t *obj){
	if(!obj) return -1;
	if(obj->kind == INTEGER || obj->kind == FLOAT) return 1;
	if(obj->kind == STRING) return strlen(obj->data.v_string);
	if(obj->kind == VECTOR3) return 3;
	if(obj->kind == ARRAY) return obj->data.v_array.size;

	return -1;
}	
