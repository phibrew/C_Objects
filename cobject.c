#include "cobject.h"
#include <stdlib.h>
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