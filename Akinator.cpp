#include "Akinator.h"
#include "stdio.h"
#include "assert.h"
#include "string.h"
#include "stdlib.h"


namespace My_tree
{

// write prefix
int Akinator::Write_tree(Elem *ptr, FILE* output)
{
    assert(output != NULL);

    if (ptr == nullptr)
    {
        fprintf(output, "{nill}");
        return 0;
    }

    fprintf(output, "{%s", ptr->data);

    Akinator::Write_tree(ptr->left, output);
    Akinator::Write_tree(ptr->right, output);

    fprintf(output, "}");

    return 0;
}

// read the whole file in the buf
// then works with buf.
// prefix format of the tree
int Akinator::Read_tree(FILE* input)
{
    assert(input != NULL);

    fseek(input, 0, SEEK_END);
    size_t buf_size = ftell(input);
    rewind(input);

    char* buf = new char[buf_size + 1];
    memset(buf, '\0', buf_size);

    fread(buf, sizeof(buf[0]), buf_size + 1, input);

    char* ptr = buf;

    root = Make_tree(ptr);

    delete[] buf;

    return 0;
}

//uses buf to make tree
//recursively.
Elem* Akinator::Make_tree(char* &ptr)
{
    char string[STR_MAXSIZE] = "";

    //while reading we have to
    //move ptr.
    //so move tells us how much
    //we should move
    int move = 0;

    sscanf(ptr, "{%[^}{\n\r] %n", string, &move);
    //move ptr
    ptr += move;

    //if move is 0
    //smth is incorrect.
    assert(move != 0);

    if (strcmp(string, "nill") == 0)
    {
        ptr++; // for }
        return nullptr;
    }

    Elem* newelem = new Elem;
    size++;
    //newelem->data = string
    strcpy(newelem->data, string);

    //read left
    newelem->left = Make_tree(ptr);
    if (newelem->left != nullptr)
    {
        newelem->left->parent = newelem;
        newelem->left->side =true;
    }

    //read right
    newelem->right = Make_tree(ptr);
    if (newelem->right != nullptr)
        newelem->right->parent = newelem;
        //newelem->right->side = false by default

    ptr++; // for '}'

    return newelem;
}

//check if ptr is a leaf of the tree
bool is_leaf(Elem* ptr)
{
    if (ptr->left == nullptr && ptr->right == nullptr)
        return true;
    else
        return false;
}

//make new connections between old elems and a new one
int reconnect(Elem*& parentside, Elem* elem, Elem* new_item, Elem* difference)
{

    parentside = difference;
    difference->parent = elem->parent;


    difference->left = new_item;
    new_item->side = true;
    new_item->parent = difference;

    difference->right = elem;
    elem->side = false;
    elem->parent = difference;

    return 0;
}

//if the user wants to add new object (item)
int New_Item(Elem* elem)
{
    printf("sorry i don't know(\n"
           "can you tell me what it is?\n");

    //user tells new object
    scanf(" ");
    char str[STR_MAXSIZE + 1] = "";
    fgets(str, STR_MAXSIZE + 1, stdin);

    //to delete '\n' in the end of the string
    char* c = strchr(str,'\n');
    *c = '\0';


    Elem* new_item = new Elem;
    strcpy(new_item->data, str);

    printf("OK, how does %s differs from %s?\n", new_item->data, elem->data);

    //the leather bag with flesh and bones
    //tells the difference
    fgets(str, STR_MAXSIZE + 1, stdin);
    c = strchr(str, '\n');
    *c = '\0';

    Elem* difference = new Elem;
    strcpy(difference->data, str);

    if (elem->side)
    {
        difference->side = true;
        reconnect(elem->parent->left, elem, new_item, difference);

    }
    else
    {
        difference->side = false;
        reconnect(elem->parent->right, elem, new_item, difference);
    }

    return 0;
}

// the interactive game with a user
int Akinator::Try_to_guess()
{

const bool LEFT = true;
const bool RIGHT = false;

    Elem* ptr = root;
    char answer = {};

    while (!is_leaf(ptr))
    {
        //question
        printf("%s?\n", ptr->data);

        //answer
        scanf(" %c", &answer);

        if (answer == 'y')
        {
            assert(ptr->left != nullptr);
            ptr = ptr->left;
        }

        else
        if (answer == 'n')
        {
            assert(ptr->right != nullptr);
            ptr = ptr->right;
        }

        else
            printf("i don't understand, try to be more understandable, PLEASE \n");
    }

    //if ptr is leaf
    assert (ptr != nullptr);

    //ask the user
    printf("it is %s?\n",ptr->data);

    scanf(" %c", &answer);

    if (answer == 'y')
        printf("yeah! I'm super clever\n");

    else
    if (answer == 'n')
        //if wrong then add new item
        New_Item(ptr);

    else
        printf("oh, if so! I'll go away!\n");

    return 0;
}

// delete tree recursively
int Delete_rec(Elem* ptr)
{
    if (ptr->left != nullptr)
        Delete_rec(ptr->left);

    if (ptr->right != ptr->left)
        if (ptr->right != nullptr)
            Delete_rec(ptr->right);

    delete[] ptr;
    return 0;
}


Akinator::~Akinator()
{
     if (root != nullptr)
        Delete_rec(root);

     root = nullptr;
}

//to recursively write dot file
int Dotwrite_elems(Elem* elem, int* num, FILE* out)
{
#define YEScolor "red"
    assert(elem !=nullptr);

    int cur_num = *num;
    fprintf(out, "node%d[label=%c{%s|{<f1>YES|<f2>NO}}%c];\n", cur_num, '"', elem->data, '"');

    if (elem->left != nullptr)
    {
        ++*num;
        fprintf(out, "node%d:f1->node%d[color=%c" YEScolor "%c];\n", cur_num, *num,'"','"');

        Dotwrite_elems(elem->left, num, out);
    }

    if (elem->right != nullptr)
    {
        ++*num;
        fprintf(out, "node%d:f2->node%d;\n", cur_num, *num);
        Dotwrite_elems(elem->right, num, out);
    }

    return 0;
}

//write code in dot language
//to make graph
int Akinator::Write_DOT(FILE* out)
{
    assert(out != NULL);
    assert(root!=nullptr);

#define fillcolor "#FFEBCD"
    fprintf(out,"digraph Akinator {\nnode[shape=record style=%cfilled%c fillcolor=%c" fillcolor "%c]\n",
            '"', '"', '"', '"');

    int num = 0;
    Dotwrite_elems(root, &num, out);

    fprintf(out, "}");
    return 0;
}

//verificate recursively
int Check(Elem* elem, int length)
{
    if (elem == nullptr) return length;

    length++;

    length = Check(elem->left, length);
    length = Check(elem->right, length);

    return length;
}

int Akinator::Verificator()
{
    if (root == nullptr)
    {
        printf("root == nullptr\n");
        return 1;
    }

    int length = 0;
    length = Check(root, length);

    if (length != size)
    {
        printf("oops, the tree is broken!\n"
               "length(%d) != size(%d)\n", length, size);
        return 2;
    }

    return 0;
}

//find the chosen element
//dfs
//(i know, it is not the best way)
Elem* FindElem(char* ans, Elem* elem)
{
    if (is_leaf(elem))
        if (strcmp(elem->data, ans) == 0)
            return elem;
        else
            return nullptr;
    else
    {
        Elem* ptr = FindElem(ans, elem->left);
        if(ptr != nullptr)
            return ptr;
        else
            ptr = FindElem(ans, elem->right);
        return ptr;
    }

}

//gives the definition
//of any chosen object
int Akinator::Give_definition(char *ans)
{
    assert(strcmp(ans, ""));
    Elem* elem = FindElem(ans, root);

    if (elem == nullptr)
        printf("sorry(not), I don't know what it is.\n");
    else
    {
        printf("%s is ", elem->data);

        bool no = !elem->side;
        elem = elem->parent;

        while (elem != nullptr)
        {
            if (no)
                if (elem->parent == nullptr)
                    printf("not %s. ", elem->data);
                else
                    printf("not %s, ", elem->data);
            else
                if (elem->parent == nullptr)
                    printf("%s. ", elem->data);
                else
                    printf("%s, ", elem->data);

            no = !elem->side;
            elem = elem->parent;
        }
        printf("\n");
    }

    return 0;
}

} // My_tree