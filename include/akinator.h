#ifndef AKINATOR_H
#define AKINATOR

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <Mmsystem.h>

#include "fileUtils.h"

#pragma comment(lib, "Winmm.lib")


//-----------------------------------------------

enum Positions
{
    LEFT,
    RIGHT,
};


enum Sizes
{
    MAX_IMG_SRC_LEN = 100,
    STACK_INIT_CAP = 5,
    MAX_NODE_NAME_LEN = 30,
};


//-----------------------------------------------


struct node
{
    node* parent;

    node* left;
    node* right;

    char* name;
};


#define $YES                                                                        \
    mciSendString("open \"data/yes.mp3\" type mpegvideo alias mp3", NULL, 0, NULL); \
    mciSendString("play mp3", NULL, 0, NULL);                                       


node* InsertNode (char name[], node* parent, int position = LEFT);

node* CreateNewNode ();

int GuessTheNode (node* node);

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