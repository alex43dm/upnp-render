#include <string.h>
#include <stdlib.h>

#include "Xml.h"

Xml::Xml() {
    //ctor
}

Xml::~Xml() {
    //dtor
}

void Xml::escape_real(const char *str, char *target, int *length,  int attribute) {
    if (target != NULL) {
        int len = 0;

        for (/**/; *str; str++) {
            if (*str == '<') {
                memcpy(target + len, "&lt;", 4);
                len += 4;
            }
            else if (attribute && (*str == '"')) {
                memcpy(target + len, "%22", 3);
                len += 3;
            }
            else if (*str == '>') {
                memcpy(target + len, "&gt;", 4);
                len += 4;
            }
            else if (*str == '&') {
                memcpy(target + len, "&amp;", 5);
                len += 5;
            }
            else {
                target[len++] = *str;
            }
        }
        target[len] = '\0';

        if (length != NULL)
            *length = len;
    }
    else if (length != NULL) {
        int len = 0;

        for (/**/; *str; str++) {
            if (*str == '<') {
                len += 4;
            }
            else if (attribute && (*str == '"')) {
                len += 3;
            }
            else if (*str == '>') {
                len += 4;
            }
            else if (*str == '&') {
                len += 5;
            }
            else {
                len++;
            }
        }

        *length = len;
    }
}

char *Xml::escape(const char *str, int attribute) {
    int len;
    char *out;

    escape_real(str, NULL, &len, attribute);
    out = (char *)malloc(len + 1);
    escape_real(str, out, NULL, attribute);
    return out;
}
