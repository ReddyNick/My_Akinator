#include <iostream>
#include "Akinator.h"
#include "assert.h"
#include "string.h"
// todo assert from empty file;

int Play_and_write(My_tree::Akinator* aki, FILE* tree)
{
    char c = {};
    while (true)
    {
        printf("Do you want to play? Or do you want to get defenition?\n"
               "Enter 'p' or 'd'. And any key to exit\n");

        scanf(" %c", &c);

        if (c == 'p')
        {
            aki->Try_to_guess();
            continue;
        }
        else
        if (c == 'd')
        {
            printf("Whose definition do you want to know?\n");

            char ans[STR_MAXSIZE] = "";
            scanf(" %[^\n]", ans);

            aki->Give_definition(ans);
            continue;
        }
        else
          break;
    }

    fclose(tree);

    tree = fopen("tree.txt","w");
    aki->Write_tree(aki->root, tree);

    return 0;
}

int main() {

    FILE* tree = fopen("tree.txt", "r");
    assert(tree != NULL);

    My_tree::Akinator akinator;

    akinator.Read_tree(tree);


    Play_and_write(&akinator, tree);
    fclose(tree);

//    akinator.root->left = akinator.root->right;
//    if(!akinator.Verificator())
//        printf("tree is OK\n");


    FILE* dot = fopen("dot/firstgraph.gv","w");
    akinator.Write_DOT(dot);
    fclose(dot);

    return 0;
}