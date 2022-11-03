#ifndef AKINATOR_H
#define AKINATOR

#include <stdlib.h>
#include <stdio.h>

#include "fileUtils.h"




//-----------------------------------------------

enum Positions
{
    LEFT,
    RIGHT,
};


enum MaxLens
{
    MAX_IMG_SRC_LEN = 100,
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

void DrawTree (node* root);

int PrintPreOrder (node* node, FILE* tree_data);

int PrintPostOrder (node* node, FILE* tree_data);

void InitGraphvisNode (node* node, FILE* dot_file);

void RecursDrawConnections (node* node, FILE* dot_file);

node* FindNode (node* cur_node, const char name[]);

node* CreateTreeRoot (char name[]);

node* DestructNode (node* root);


#endif