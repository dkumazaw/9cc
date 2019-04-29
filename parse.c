#include "9cc.h"

/*
consume
*/
int consume(Vector *tokens, int ty)
{
    if (((Token *)tokens->data[pos])->ty != ty)
        return 0;
    pos++;
    return 1;
}

/*
equality: relational
equality: equality "==" relational
equality: equality "!=" relational
*/
Node *equality(Vector *tokens)
{
    Node *node = relational(tokens);

    for (;;)
    {
        if (consume(tokens, TK_EQ))
            node = new_node(ND_EQ, node, relational(tokens));
        else if (consume(tokens, TK_NE))
            node = new_node(ND_NE, node, relational(tokens));
        else
            return node;
    }
}

/*
relational: add
relational: relational "<"  add
relational: relational "<=" add
relational: relational ">"  add
relational: relational ">=" add
*/
Node *relational(Vector *tokens)
{
    Node *node = add(tokens);

    for (;;)
    {
        if (consume(tokens, '<'))
            node = new_node('<', node, add(tokens));
        else if (consume(tokens, TK_LE))
            node = new_node(ND_LE, node, add(tokens));
        else if (consume(tokens, '>'))
            node = new_node('<', add(tokens), node); // Flip to represent <
        else if (consume(tokens, TK_GE))
            node = new_node(ND_LE, add(tokens), node); // Flip to represent <=
        else
            return node;
    }
}

/*
add: mul
add: mul "+" mul
add: mul "-" mul
*/
Node *add(Vector *tokens)
{
    Node *node = mul(tokens);

    for (;;)
    {
        if (consume(tokens, '+'))
            node = new_node('+', node, mul(tokens));
        else if (consume(tokens, '-'))
            node = new_node('-', node, mul(tokens));
        else
            return node;
    }
}

/*
mul: unary
mul: mul "*" unary
mul: mul "/" unary
*/
Node *mul(Vector *tokens)
{
    Node *node = unary(tokens);

    for (;;)
    {
        if (consume(tokens, '*'))
            node = new_node('*', node, unary(tokens));
        else if (consume(tokens, '/'))
            node = new_node('/', node, unary(tokens));
        else
            return node;
    }
}

/*
unary: term
unary: "+" term
unary: "-" term
*/
Node *unary(Vector *tokens)
{
    if (consume(tokens, '+'))
        return term(tokens);
    if (consume(tokens, '-'))
        return new_node('-', new_node_num(0), term(tokens));
    return term(tokens);
}

/*
term: num
term: "(" equality ")"
*/
Node *term(Vector *tokens)
{
    if (consume(tokens, '('))
    {
        Node *node = equality(tokens);
        if (!consume(tokens, ')'))
        {
            fprintf(stderr, "No closing parenthesis: %s", ((Token *)tokens->data[pos])->input);
            exit(1);
        }
        return node;
    }

    if (((Token *)tokens->data[pos])->ty == TK_NUM)
        return new_node_num(((Token *)tokens->data[pos++])->val);

    fprintf(stderr, "Token is neither an integer or parenthesis: %s", ((Token *)tokens->data[pos])->input);
    exit(1);
}