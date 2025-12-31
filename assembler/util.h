#pragma once

/**
 * strips comments from the .asm line ("NOT R7, R7 ; comments bla bla" --> "NOT R7, R7")
 * 
 * @param line to strip comments from
 */
void strip_comments(char* line);
/**
 * strips whitespace from the .asm line 
 * 
 * @param line to strip whitespace from
 */
void strip_whitespace(char* line);
/**
 * puts the cleaned up line into separate token arrays (i.e. toks[0] = 'ADD', toks[1] = 'R4',...)
 * 
 * @param line line to tokenize
 * @param toks array to store tokens
 * @param max_toks a max number of tokens as a safeguard.
 * 
 * @return number of tokens filled
 */
int tokenize(char* line, char toks[][64], int max_toks);
/**
 * given a token string like "R4", returns 4 as an integer
 * 
 * @param the token string
 * @returns the associated register if valid, else -1
 */
int parse_reg(char* token);
