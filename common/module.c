#include <stdio.h>

#include "common.h"
#include "module.h"

#define out_name_module(m, v, n) [v] = n,

static char *s_module_name[] = 
{
    SDP_MODULE_TABLE(out_name_module)
};

char *module_name_get(SDP_MODULE_E module)
{
    if (SDP_MD_NUMBER <= module)
    {
        return "unkown";
    }

    return s_module_name[module];
}