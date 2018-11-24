#include "Akinator.h"
#include "stdio.h"
#include "assert.h"
#include "string.h"
#include "stdlib.h"


namespace My_tree
{

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

    return 1;
}

Elem* Akinator::Read_tree(FILE* input)
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
}

Elem* Akinator::Make_tree(char* &ptr)
{
    char string[STR_MAXSIZE] = "";
    int move = 0;

    sscanf(ptr, "{%[^}{\n\r] %n", string, &move);
    ptr += move;

    assert(move != 0);

    if (strcmp(string, "nill") == 0)
    {
        ptr++; // for }
        return nullptr;
    }

    Elem* newelem = new Elem;
    size++;
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

    ptr++; // for '}'

    return newelem;
}

bool is_leaf(Elem* ptr)
{
    if (ptr->left == nullptr && ptr->right == nullptr)
        return true;
    else
        return false;
}

int Answer(Elem*& ptr, Elem* where, bool side)
{
    if (where != nullptr)
        ptr = where;
    else
    {
        printf("\ni dunno, tell me please what it is.\n");

        char str[STR_MAXSIZE]= "";
        scanf(" %s", str);

        where = new Elem;
        strcpy(where->data, str);

        if (side)
            ptr->left = where;
        else
            ptr->right = where;

        printf("thanks, now i'm a bit more intelligent\n");
    }
}

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

int New_Item(Elem* elem)
{
    printf("sorry i don't know(\n"
           "can you tell me what it is?\n");

    scanf(" ");
    char str[STR_MAXSIZE + 1] = "";
    fgets(str, STR_MAXSIZE + 1, stdin);


    char* c = strchr(str,'\n');
    *c = '\0';

    Elem* new_item = new Elem;
    strcpy(new_item->data, str);

    printf("OK, how does %s differs from %s?\n", new_item->data, elem->data);

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
            // ptr = ptr->left
            Answer(ptr, ptr->left, LEFT);

        else
        if (answer == 'n')
            // ptr = ptr->right
            Answer(ptr, ptr->right, RIGHT);

        else
            printf("i don't understand, try to be more understandable, PLEASE \n");
    }

    assert (ptr != nullptr);

    printf("it is %s?\n",ptr->data);

    scanf(" %c", &answer);

    if (answer == 'y')
        printf("yeah! I'm super clever\n");

    else
    if (answer == 'n')
        New_Item(ptr);

    else
        printf("oh, if so! I'll go away!\n");

    return 0;
}


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