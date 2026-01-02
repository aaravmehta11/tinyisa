#include "util.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>



void strip_comments(char* line);
void strip_whitespace(char* line);
int tokenize(char* line, char toks[][64], int max_toks);
int parse_reg(char* token);

void strip_comments(char* line) {
    char* p = strchr(line, ';');
    if (p) {
        *p = '\0';
    }
}

void strip_whitespace(char* line) {
    char* start = line;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    if (start != line) {
        memmove(line, start, strlen(start)+1);
    }

    size_t i = strlen(line);
    while (i > 0 && isspace((unsigned char)line[i - 1])) {
        i--;
    }
    line[i] = '\0';
}

int tokenize(char* line, char toks[][64], int max_toks) {
    if (line == NULL || max_toks <= 0) {
        return 0;
    }

    int i = 0; //tokens array ptr
    int j = 0; //line idx ptr


    while (*line && i < max_toks) {
        
        while (*line && (isspace(*line) || *line == ',')) {
            line++;
        }

        if (!*line) {
            break;
        }

        while (*line && !isspace(*line) && *line != ',' && j < 63) {
            toks[i][j] = *line;
            j++;
            line++;
        }
        toks[i][j] = '\0';
        j = 0;
        i++;
        }
    
    return i;
}

int parse_reg(char* token) {
    if (token == NULL || strlen(token) != 2) {
        return -1;
    } 
    if (token[0] != 'r' && token[0] != 'R') {
        return -1;
    }
    if (token[1] < '0' || token[1] > '7') {
        return -1;
    }

    return token[1] - '0';
}

