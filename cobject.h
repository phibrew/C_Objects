typedef enum ObjectKind{
	INTEGER,
	FLOAT
} snek_object_kind_t;

typedef union ObjectData{
	int v_int;
	float v_float;
} snek_object_data_t;

typedef struct ObjectType{
	snek_object_kind_t kind;
	snek_object_data_t data;
} snek_object_t;

snek_object_t *new_snek_integer(int value);
snek_object_t *new_snek_float(float value);