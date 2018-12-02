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

    // read the whole file in the buf
    // then works with buf.
    // prefix format of the tree
    int Read_tree(FILE* input);

    //uses buf to make tree
    //recursively.
    Elem* Make_tree(char*& ptr);

    // write prefix
    int Write_tree(Elem* ptr,FILE* output);

    // the interactive game with a user
    int Try_to_guess();

    int Verificator();

    //write code in dot language
    //to make graph
    int Write_DOT(FILE* out);

    //gives the definition
    //of any chosen object
    int Give_definition(char* ans);

    ~Akinator();
};

// delete tree recursively
int Delete_rec(Elem* ptr);

//check if ptr is a leaf of the tree
bool is_leaf(Elem* ptr);

//make new connections between old elems and a new one
int reconnect(Elem*& parentside, Elem* elem, Elem* new_item, Elem* difference);

//if the user wants to add new object (item)
int New_Item(Elem* elem);

//to recursively write dot file
int Dotwrite_elems(Elem* elem, int* num, FILE* out);

//find the chosen element
Elem* FindElem(char* ans, Elem* elem);
}


#endif //MY_AKINATOR_TREE_H
