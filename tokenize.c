#include "9cc.h"

int is_alnum(char c)
{
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           ('0' <= c && c <= '9') ||
           (c == '_');
}

/*
tokenize 
*/
void tokenize(char *p, Vector *tokens)
{
    while (*p)
    {
        // Skip whitespace
        if (isspace(*p))
        {
            p++;
            continue;
        }

        if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6]))
        {
            Token *token = new_token();
            token->ty = TK_RETURN;
            token->input = p;
            vec_push(tokens, (void *)token);
            p += 6;
            continue;
        }

        if (strncmp(p, "if", 2) == 0 && !is_alnum(p[2])) 
        {
            Token *token = new_token();
            token->ty = TK_IF;
            token->input = p;
            vec_push(tokens, (void *)token);
            p += 2;
            continue;
        }

        if (strncmp(p, "else", 4) == 0 && !is_alnum(p[4]))
        {
            Token *token = new_token();
            token->ty = TK_ELSE;
            token->input = p;
            vec_push(tokens, (void *)token);
            p += 4;
            continue;
        }

        if (strncmp(p, "while", 5) == 0 && !is_alnum(p[5]))
        {
            Token *token = new_token();
            token->ty = TK_WHILE;
            token->input = p;
            vec_push(tokens, (void *)token);
            p += 5;
            continue;
        }

        if (strncmp(p, "for", 3) == 0 && !is_alnum(p[3]))
        {
            Token *token = new_token();
            token->ty = TK_FOR;
            token->input = p;
            vec_push(tokens, (void *)token);
            p += 3;
            continue;
        }

        if (strncmp(p, "==", 2) == 0)
        {
            Token *token = new_token();
            token->ty = TK_EQ;
            token->input = p;
            vec_push(tokens, (void *)token);
            p += 2;
            continue;
        }

        if (strncmp(p, "!=", 2) == 0)
        {
            Token *token = new_token();
            token->ty = TK_NE;
            token->input = p;
            vec_push(tokens, (void *)token);
            p += 2;
            continue;
        }

        if (strncmp(p, "<=", 2) == 0)
        {
            Token *token = new_token();
            token->ty = TK_LE;
            token->input = p;
            vec_push(tokens, (void *)token);
            p += 2;
            continue;
        }

        if (strncmp(p, ">=", 2) == 0)
        {
            Token *token = new_token();
            token->ty = TK_GE;
            token->input = p;
            vec_push(tokens, (void *)token);
            p += 2;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' || *p == '<' || *p == '>' || *p == ';' || *p == '=' || *p == '{' || *p == '}')
        {
            Token *token = new_token();
            token->ty = *p;
            token->input = p;
            vec_push(tokens, (void *)token);
            p++;
            continue;
        }

        if ('a' <= *p && *p <= 'z')
        {
            Token *token = new_token();
            token->ty = TK_IDENT;
            token->input = p++;
            int length = 1;
            while ('a' <= *p && *p <= 'z') {
                length++;
                p++;
            }
            token->len = length;
            vec_push(tokens, (void *)token);
            continue;
        }

        if (isdigit(*p))
        {
            Token *token = new_token();
            token->ty = TK_NUM;
            token->input = p;
            token->val = strtol(p, &p, 10);
            vec_push(tokens, (void *)token);
            continue;
        }

        fprintf(stderr, "Cannot tokenize: %s\n", p);
        exit(1);
    }

    Token *token = new_token();
    token->ty = TK_EOF;
    token->input = p;
    vec_push(tokens, (void *)token);
}