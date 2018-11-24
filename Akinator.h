#ifndef MY_AKINATOR_TREE_H
#define MY_AKINATOR_TREE_H

#include "stdio.h"

const int STR_MAXSIZE = 100;

namespace My_tree
{

struct Elem
{
    Elem* left = nullptr;
    Elem* right = nullptr;
    Elem* parent = nullptr;

    // if left, side == true
    // if right, side == false
    bool side = false;

    char data[STR_MAXSIZE] =  "";
};

class Akinator
{
public:
    Elem* root = nullptr;
    size_t size = 0;

    // read prefix
    Elem* Read_tree(FILE* input);
    Elem* Make_tree(char*& ptr);

    // write prefix
    int Write_tree(Elem* ptr,FILE* output);

    int Try_to_guess();

    int Verificator();
    int Write_DOT(FILE* out);

    int Give_definition(char* ans);

    ~Akinator();
};

int Delete_rec(Elem* ptr);

}


#endif //MY_AKINATOR_TREE_H
