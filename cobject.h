typedef enum ObjectKind snek_object_kind_t;
typedef struct Vector snek_vector_t;
typedef union ObjectData snek_object_data_t;
typedef struct Object snek_object_t;

enum ObjectKind{
	INTEGER,
	FLOAT,
	STRING,
	VECTOR3
};

struct Vector{
	snek_object_t *x;
	snek_object_t *y;
	snek_object_t *z;
};

union ObjectData{
	int v_int;
	float v_float;
	char *v_string;
	snek_vector_t v_vector3;
};

struct Object{
	snek_object_kind_t kind;
	snek_object_data_t data;
};


snek_object_t *new_snek_integer(int value);
snek_object_t *new_snek_float(float value);
snek_object_t *new_snek_string(char *value);
snek_object_t *new_snek_vector3(snek_object_t *x, snek_object_t *y, snek_object_t *z);