#include <iostream>
#include "Akinator.h"
#include "assert.h"
#include "string.h"

//game with the user
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

    //tree.txt is the file where
    //akinator's data is stored
    FILE* tree = fopen("tree.txt", "r");
    assert(tree != NULL);

    My_tree::Akinator akinator;

    akinator.Read_tree(tree);

    //interactive game with the user
    Play_and_write(&akinator, tree);
    fclose(tree);

    //write dotgraph file
    FILE* dot = fopen("dot/firstgraph.gv","w");
    akinator.Write_DOT(dot);

    fclose(dot);
    return 0;
}