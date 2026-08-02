#include "cobject.h"
#include <stdlib.h>

snek_object_t *new_snek_integer(int value) {
	snek_object_t *int_object = malloc(sizeof(snek_object_t));
	if(!int_object) return NULL;

	int_object->kind = INTEGER;
	int_object->data.v_int = value;
	return int_object;
}
