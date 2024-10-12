#include "text.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* makeColorful(char *str, Color color) {
    char *ret = calloc(strlen(str) + strlen(color) + strlen(end), sizeof(char));
    sprintf(ret, "%s%s%s", color, str, end);
    return ret;
}