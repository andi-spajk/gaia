#ifndef PARSER_H
#define PARSER_H

#define PARSER_SUCCESS 1

int parse_instr_tree(struct Token **seq, int index);
int parse_label_tree(struct Token **seq, int index);
int parse_token_sequence(struct Lexer *lexer);

#endif