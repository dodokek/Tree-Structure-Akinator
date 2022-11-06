#ifndef AKINATOR_H
#define AKINATOR

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <Mmsystem.h>
#include <string.h>

#include "fileUtils.h"
#include "stack.h"

//-----------------------------------------------

enum Positions
{
    LEFT,
    RIGHT,
};


enum Sizes
{
    MAX_IMG_SRC_LEN = 100,
    MAX_BUFFER_SIZE = 1000,
    OFFSET = 2,
    STACK_INIT_CAP = 5,
    MAX_NAME_LEN = 100,
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

node* CreateTreeRoot (char name[]);

int GuessTheNode (node* node);

char* GetInput (char* buffer);

void AppendNewObject (node* cur_node);

void AddAncestor (node* cur_node, Stack* ancestors);

void DumpTree (node* root);

void DrawTree (node* root);

int PrintPreOrder (node* node, FILE* tree_data);

int PrintPostOrder (node* node, FILE* tree_data);

void PrintObject (node* node_to_print);

Stack BuildAncestorsStack (node* cur_node);

void AddAncestor (node* cur_node, Stack* ancestors);

void InitGraphvisNode (node* node, FILE* dot_file);

void RecursDrawConnections (node* node, FILE* dot_file);

node* FindNode (node* cur_node, const char name[]);

node* BuildTree (FILE* tree_info);

node* RecBuildNode (char** buffer);

node* DestructNode (node* root);

void ClearBuffer();


#endif