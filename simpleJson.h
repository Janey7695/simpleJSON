#ifndef SIMPLEJSON_H__
#define SIMPLEJSON_H__

#define DEBUG 1

typedef enum {
    SMON_NULL = 0,
    SMON_TRUE,
    SMON_FALSE,
}smon_type;

enum {
    SMON_PRASE_OK = 0, //successful prasing return SMON_PRASE_OK
    SMON_PARSE_NOT_VALUE, //if not give value(a null json) will return SMON_PARSE_NOT_VALUE
    SMON_PARSE_INVALID_VALUE, // if give a invalid value will return SMON_PARSE_INVALID_VALUE
    SMON_PARSE_ROOT_NOT_SINGULAR // If there are other characters after the value, an error `SMON_PARSE_ROOT_NOT_SINGULAR` is returned  
};

typedef struct
{
    smon_type type;
}smon_value;


int smon_prase(smon_value* v, char* json);
smon_type smon_get_type(smon_value* v);

#endif // !SIMPLEJSON_H__
