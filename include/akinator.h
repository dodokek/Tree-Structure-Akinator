#ifndef AKINATOR_H
#define AKINATOR

#include "fileUtils.h"




//-----------------------------------------------

enum Positions
{
    LEFT,
    RIGHT,
};

//-----------------------------------------------


struct node
{
    node* parent;

    node* left;
    node* right;

    const char* name;
};


node* InsertNode (const char name[], node* parent, int position = LEFT);

node* CreateNewNode ();

void DumpTree (node* root);

node* FindNode (node* cur_node, const char name[]);

node* CreateTreeRoot (char name[]);

node* DestructNode (node* root);


#endif