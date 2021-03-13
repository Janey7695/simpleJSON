#include "simpleJson.h"

#if DEBUG
#include <assert.h>  /* assert() */
#endif
#include <stdlib.h>  /* NULL */


#if DEBUG
#define EXPECT(c,ch)    do { assert(*c->json == (ch)); c->json++; } while(0)
#endif

typedef struct 
{
    char *json;
}smon_context;


static void smon_prase_whitespace(smon_context* c)
{
    char *p = c->json;
    while(*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

/*
JSON-text = ws value ws
ws = *(%x20 / %x09 / %x0A / %x0D)
value = null / false / true 
null  = "null"
false = "false"
true  = "true"
*/

static int smon_prase_literal(smon_context* c,smon_value* v,char* whichType,int whichPraseType)
{
    size_t i;

    #if DEBUG
    EXPECT(c, whichType[0]);
    #endif

    #if !DEBUG
    c->json++;
    #endif

    for (i = 0; whichType[i + 1]; i++)
        if (c->json[i] != whichType[i + 1])
            return SMON_PARSE_INVALID_VALUE;
    c->json += i;
    v->type = whichPraseType;
    return SMON_PRASE_OK;
}


static int smon_prase_value(smon_context* c,smon_value* v)
{
    switch (*c->json)
    {
    case 't':return smon_prase_literal(c,v,"true",SMON_TRUE);
    case 'f':return smon_prase_literal(c,v,"false",SMON_FALSE);
    case 'n':return smon_prase_literal(c,v,"null",SMON_NULL);
    default: return SMON_PARSE_INVALID_VALUE;
    }
}


int smon_prase(smon_value* v, char* json)
{
    smon_context c;
    int ret;
    c.json = json;

    #if DEBUG
    assert(v != NULL);
    #endif

    v->type = SMON_NULL;
    smon_prase_whitespace(&c);
    if((ret = smon_prase_value(&c,v)) == SMON_PRASE_OK)
    {
        smon_prase_whitespace(&c);
        if(*c.json != '\0')
            ret = SMON_PARSE_ROOT_NOT_SINGULAR;
    }
    return ret;
}

smon_type smon_get_type(smon_value* v)
{
    #if DEBUG
    assert(v != NULL);
    #endif

    return v->type;
}