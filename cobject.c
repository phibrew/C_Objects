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
	vector_object->data.v_vector3.x = x;
	vector_object->data.v_vector3.y = y;
	vector_object->data.v_vector3.z = z;

	return vector_object;
}