#include "simpleJson.h"
#include <errno.h>   /* errno, ERANGE */
#include <math.h>    /* HUGE_VAL */
#if DEBUG
#include <assert.h>  /* assert() */
#endif
#include <stdlib.h>  /* NULL */


#if DEBUG
#define EXPECT(c,ch)    do { assert(*c->json == (ch)); c->json++; } while(0)
#endif

#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')

typedef struct 
{
    char *json;
}smon_context;


static void smon_PARSE_whitespace(smon_context* c)
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

static int smon_PARSE_literal(smon_context* c,smon_value* v,char* whichType,int whichPARSEType)
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
    v->type = whichPARSEType;
    return SMON_PARSE_OK;
}

/*
number = [ "-" ] int [ frac ] [ exp ]
int = "0" / digit1-9 *digit
frac = "." 1*digit
exp = ("e" / "E") ["-" / "+"] 1*digit
*/

static int smon_PARSE_number(smon_context* c,smon_value* v)
{
    char *p = c->json;
    if(*p == '-') p++;
    if(*p == '0') p++;
    else
    {
        if(!ISDIGIT(*p)) return SMON_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    if(*p == '.')
    {
        p++;
        if(!ISDIGIT(*p)) return SMON_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    if(*p == 'e' || *p == 'E')
    {
        p++;
        if(*p == '+' || *p == '-') p++;
        if(!ISDIGIT(*p)) return SMON_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    errno = 0;
    v->n = strtod(c->json,NULL);
    if (errno == ERANGE && (v->n == HUGE_VAL || v->n == -HUGE_VAL))
        return SMON_PARSE_NUMBER_TOO_BIG;
    v->type =SMON_NUMBER;
    c->json = p;
    return SMON_PARSE_OK;
}

static int smon_PARSE_value(smon_context* c,smon_value* v)
{
    switch (*c->json)
    {
    case 't':return smon_PARSE_literal(c,v,"true",SMON_TRUE);
    case 'f':return smon_PARSE_literal(c,v,"false",SMON_FALSE);
    case 'n':return smon_PARSE_literal(c,v,"null",SMON_NULL);
    case '\0':return SMON_PARSE_NOT_VALUE;
    default: return smon_PARSE_number(c,v);
    }
}


int smon_PARSE(smon_value* v, char* json)
{
    smon_context c;
    int ret;
    c.json = json;

    #if DEBUG
    assert(v != NULL);
    #endif

    v->type = SMON_NULL;
    smon_PARSE_whitespace(&c);
    if((ret = smon_PARSE_value(&c,v)) == SMON_PARSE_OK)
    {
        smon_PARSE_whitespace(&c);
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

double smon_get_number(smon_value* v)
{
    #if DEBUG
    assert(v != NULL && v->type ==SMON_NUMBER);
    #endif

    return v->n;
}