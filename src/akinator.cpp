#include "akinator.h"

int main()
{
    node* root = CreateTreeRoot ("Letherman");

    node* billy = InsertNode ("Billy", root);
    InsertNode ("Van", root, RIGHT);
    node* van = FindNode (root, "Van");
    InsertNode ("Darkholm", van);
    DumpTree (root);

    DestructNode (root);
}


node* CreateTreeRoot (char name[])
{
    node* root = CreateNewNode();

    root->name = name;

    return root;
}


node* DestructNode (node* root)
{
    if (root->left)  DestructNode (root->left);
    if (root->right) DestructNode (root->right);

    root->name = nullptr;
    
    free (root);
    root = nullptr;

    return root;
}


node* InsertNode (const char name[], node* parent, int position)
{
    node* new_node = CreateNewNode();
    
    new_node->name = name;
    new_node->parent = parent;

    if (position == LEFT)
    {
        if (parent->left == nullptr) parent->left = new_node;
        else
        {
            new_node->left = parent->left;
            parent->left->parent = new_node;
            parent->left = new_node;
        }
    }
    else if (position == RIGHT)
    {
        if (parent->right == nullptr) parent->right = new_node;
        else
        {
            new_node->left = parent->right;
            parent->right->parent = new_node;
            parent->right = new_node;
        }
    }

    return new_node;
}


node* FindNode (node* cur_node, const char name[])
{
    if (!cur_node) return nullptr;

    if (strcmp (cur_node->name, name) == 0) return cur_node;

    node* find_left = FindNode (cur_node->left, name);
    node* find_right = FindNode (cur_node->right, name);

    if (find_left) return find_left;
    if (find_right) return find_right;

    return nullptr;
}


void DumpTree (node* node)
{

    
    printf ("Ptr[%p]", node);
    printf ("\tNode %s: left %p, right %p, parent %p\n",
            node->name, node->left,
            node->right, node->parent);

    if (node->left)  DumpTree (node->left);
    if (node->right) DumpTree (node->right);

    return;
    
}


node* CreateNewNode ()
{
    node* new_node = (node*) calloc (1, sizeof (node));
    if (!new_node) return nullptr;

    new_node->left   = nullptr;
    new_node->right  = nullptr;
    new_node->parent = nullptr;

    return new_node;
}