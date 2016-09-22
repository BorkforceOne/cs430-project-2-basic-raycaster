//
// Created by Brandon Garling on 9/21/2016.
//

#ifndef CS430_PROJECT_2_BASIC_RAYCASTER_JSON_H
#define CS430_PROJECT_2_BASIC_RAYCASTER_JSON_H

typedef enum JSONValueType_t {
	STRING_T,
	NUMBER_T,
	OBJECT_T,
	ARRAY_T,
	TRUE_T,
	FALSE_T,
	NULL_T
} JSONValueType_t;

typedef struct JSONObject JSONObject;
typedef struct JSONValue JSONValue;
typedef struct JSONElement JSONElement;
typedef struct JSONArray JSONArray;

typedef struct JSONValue {
	JSONValueType_t type;
	union {
		char *dataString;
		float dataNumber;
		JSONObject *dataObject;
		JSONArray *dataArray;
	} data;
} JSONValue;

typedef struct JSONElement {
	char *key;
	JSONValue *value;
} JSONElement;

typedef struct JSONObject {
	char **keys;
	JSONElement **values;
	int length;
} JSONObject;

typedef struct JSONArray {
	JSONValue **values;
	int length;
} JSONArray;

int read_json(char* fname, JSONValue *JSONRootRef);
int read_JSONValue(FILE *fp, JSONValue *JSONValueRef);
int read_JSONObject(FILE *fp, JSONObject *JSONObjectRef);
int read_JSONElement(FILE *fp, JSONElement *JSONElementRef);
int read_JSONArray(FILE *fp, JSONArray *JSONArrayRef);

#endif //CS430_PROJECT_2_BASIC_RAYCASTER_JSON_H
