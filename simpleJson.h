#ifndef SIMPLEJSON_H__
#define SIMPLEJSON_H__

#define DEBUG 1

typedef enum {
    SMON_NULL = 0,
    SMON_TRUE,
    SMON_FALSE,
    SMON_NUMBER
}smon_type;

enum {
    SMON_PARSE_OK = 0, //successful prasing return SMON_PARSE_OK
    SMON_PARSE_NOT_VALUE, //if not give value(a null json) will return SMON_PARSE_NOT_VALUE
    SMON_PARSE_INVALID_VALUE, // if give a invalid value will return SMON_PARSE_INVALID_VALUE
    SMON_PARSE_ROOT_NOT_SINGULAR, // If there are other characters after the value, an error `SMON_PARSE_ROOT_NOT_SINGULAR` is returned  
    SMON_PARSE_NUMBER_TOO_BIG,
};

typedef struct
{
    double n;
    smon_type type;
}smon_value;


int smon_PARSE(smon_value* v, char* json);// PARSE function
smon_type smon_get_type(smon_value* v); // Get the type of a value
double smon_get_number(smon_value* v);// Get the number of a value

#endif // !SIMPLEJSON_H__
