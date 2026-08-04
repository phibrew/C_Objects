#include "cobject.h"
#include <stdlib.h>
#include <string.h>
#include "bootlib.h"

void refcount_dec(snek_object_t *obj) {
	if(obj == NULL) return;
	obj->refcount -=1;
	if(obj->refcount == 0) refcount_free(obj);
}

void refcount_inc(snek_object_t *obj) {
	if(obj == NULL) return;
	obj->refcount +=1;
}

void refcount_free(snek_object_t *obj){
	switch(obj->kind){
	case INTEGER:
		break;
	case FLOAT:
		break;

	case STRING:
		free(obj->data.v_string);
		break;

	case VECTOR3:{
		refcount_dec(obj->data.v_vector3.x);
		refcount_dec(obj->data.v_vector3.y);
		refcount_dec(obj->data.v_vector3.z);
		break;		
	}
	default:
		assert(false);
	}
	free(obj);
}

snek_object_t *_new_snek_object() {
	snek_object_t *new_object = calloc(1, sizeof(snek_object_t));
	if(!new_object) return NULL;

	new_object->refcount = 1;
	return new_object;
}

snek_object_t *new_snek_integer(int value) {
	snek_object_t *int_object = _new_snek_object();
	if(!int_object) return NULL;

	int_object->kind = INTEGER;
	int_object->data.v_int = value;
	return int_object;
}

snek_object_t *new_snek_float(float value){
	snek_object_t *float_object = _new_snek_object();
	if(!float_object) return NULL;

	float_object->kind = FLOAT;
	float_object->data.v_float = value;
	return float_object;
}	

snek_object_t *new_snek_string(char *value){
	snek_object_t *string_object = _new_snek_object();
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

	snek_object_t *vector_object = _new_snek_object();
	if(!vector_object) return NULL;

	vector_object->kind = VECTOR3;
	vector_object->data.v_vector3 = (snek_vector_t){.x=x, .y=y, .z=z};
	refcount_inc(x); refcount_inc(y); refcount_inc(z);
	return vector_object;
}

snek_object_t *new_snek_array(size_t size){
	snek_object_t *array_object = _new_snek_object();
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

snek_object_t *snek_add(snek_object_t *a, snek_object_t *b){
	if(!a || !b) return NULL;

	switch(a->kind){
	case INTEGER:{
		if(b->kind == INTEGER){
			snek_object_t *new_integer = new_snek_integer(a->data.v_int + b->data.v_int);
			return new_integer;
		}
		if(b->kind == FLOAT){
			snek_object_t *new_float = new_snek_float( a->data.v_int + b->data.v_float);
			return new_float;
		}
		return NULL;
	}

	case FLOAT:{
		if(b->kind == FLOAT || b->kind == INTEGER){
			snek_object_t *new_float = new_snek_float(a->data.v_float);
			
			if(b->kind == FLOAT){
				new_float->data.v_float += b->data.v_float;
			} else {
				new_float->data.v_float += b->data.v_int;
			}

			return new_float;
		}
		return NULL;
	}

	case STRING:{
		if(b->kind != STRING){
			return NULL;
		}

		size_t size = strlen(a->data.v_string) + strlen(b->data.v_string) + 1;

		char *temp_string = (char*)calloc(size, sizeof(char));
		if(!temp_string) return NULL;

		strcat(temp_string, a->data.v_string);
		strcat(temp_string, b->data.v_string);

		snek_object_t *new_string = new_snek_string(temp_string);
		free(temp_string);
		return new_string;
	}

	case VECTOR3:{
		if(b->kind != VECTOR3) return NULL;
		snek_object_t *new_vector = new_snek_vector3(
			snek_add(a->data.v_vector3.x, b->data.v_vector3.x), 
			snek_add(a->data.v_vector3.y, b->data.v_vector3.y),
			snek_add(a->data.v_vector3.z, b->data.v_vector3.z));

		return new_vector;
	}

	case ARRAY:{
		if(b->kind != ARRAY) return NULL;

		snek_object_t *new_array = new_snek_array(a->data.v_array.size + b->data.v_array.size);
		for(size_t i = 0; i<a->data.v_array.size; ++i){
			bool res = snek_array_set(new_array, i, snek_array_get(a, i));
			if(res == false){
				free(new_array);
				return NULL;
			}
		}
		for(size_t i = 0, j = a->data.v_array.size; 
			i<b->data.v_array.size && j<new_array->data.v_array.size ; ++i, ++j){
			bool res = snek_array_set(new_array, j, snek_array_get(b, i));
			if(res == false){
				free(new_array);
				return NULL;
			}
		}
		return new_array;
	}
	default:
		break;
	}
	return NULL;
}