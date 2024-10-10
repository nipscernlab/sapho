#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

typedef struct Node
{
    int type;
    int ival;
    struct Node *left;
    struct Node *right;
    struct Node *back;
    struct Node *forth;
} Node;

Node* create(int type, int ival, Node *back, Node *left, Node *right, Node *forth);

void xuxa(Node *N);

#endif // AST_H_INCLUDED
