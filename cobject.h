typedef enum ObjectKind{
	INTEGER
} snek_object_kind_t;

typedef union ObjectData{
	int v_int;
} snek_object_data_t;

typedef struct ObjectType{
	snek_object_kind_t kind;
	snek_object_data_t data;
} snek_object_t;