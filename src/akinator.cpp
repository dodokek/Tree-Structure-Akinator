#include "akinator.h"

int main()
{
    FILE* tree_data = get_file ("data/tree.txt", "rw+");

    node* root = CreateTreeRoot ("5");

    node* n_2 = InsertNode ("2", root);
    InsertNode ("8", root, RIGHT);
    
    node* n_8 = FindNode (root, "8");
    InsertNode ("10", n_8, RIGHT);
    InsertNode ("6", n_8);
    DumpTree (root);

    PrintPreOrder (root, tree_data);
    fprintf (tree_data, "\n");
    PrintPostOrder (root, tree_data);
    fprintf (tree_data, "\n");

    DrawTree (root);

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


int PrintPreOrder (node* node, FILE* tree_data)
{
    fprintf (tree_data, "{ %s ", node->name);
    if (node->left)  PrintPreOrder (node->left,  tree_data);
    if (node->right) PrintPreOrder (node->right, tree_data);
    fprintf (tree_data, "}");

    return 1;
}


int PrintPostOrder (node* node, FILE* tree_data)
{
    if (node->left)  PrintPreOrder (node->left,  tree_data);
    if (node->right) PrintPreOrder (node->right, tree_data);
    fprintf (tree_data, "{ %s ", node->name);
    fprintf (tree_data, "}");

    return 1;
} 


#define _print(...) fprintf (dot_file, __VA_ARGS__)

void DrawTree (node* root)
{
    FILE* dot_file = get_file ("data/graph.dot", "w+");
    
    // Writing header info
    const char header[] = R"(
    digraph g {
        dpi      = 200;
        fontname = "Comic Sans MS";
        fontsize = 20;
        rankdir   =  TB;
        edge [color = darkgrey, arrowhead = onormal, arrowsize = 1, penwidth = 1.2]
        graph[fillcolor = lightgreen, ranksep = 1.3, nodesep = 0.5,
        style = "rounded, filled",color = green, penwidth = 2]

    )";
    
    _print (header);

    InitGraphvisNode (root, dot_file);

    RecursDrawConnections (root, dot_file);

    _print ("}\n");
    
    // Executing dotfile and printing an image

    fclose (dot_file);

    system ("dot -Tpng data/graph.dot -o data/pretty_tree.png");

    return;
}


void InitGraphvisNode (node* node, FILE* dot_file)   // Recursivly initialises every node 
{
    _print ("Node%s[shape=rectangle, color=\"red\", width=0.2, style=\"filled\","
            "fillcolor=\"lightblue\", label=\"%s\"] \n \n",
            node->name, node->name);
    
    if (node->left) InitGraphvisNode (node->left, dot_file);
    if (node->right) InitGraphvisNode (node->right, dot_file);

    return;
}


void RecursDrawConnections (node* node, FILE* dot_file)
{
    

    if (node->left)
    {
        _print ("Node%s->Node%s\n", node->name, node->left->name);
        RecursDrawConnections (node->left, dot_file);
    } 
    if (node->right)
    {
        _print ("Node%s->Node%s\n", node->name, node->right->name);
        RecursDrawConnections (node->right, dot_file);
    } 

    return;
}


#undef _print


node* CreateNewNode ()
{
    node* new_node = (node*) calloc (1, sizeof (node));
    if (!new_node) return nullptr;

    new_node->left   = nullptr;
    new_node->right  = nullptr;
    new_node->parent = nullptr;

    return new_node;
}