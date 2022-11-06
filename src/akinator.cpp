#include "akinator.h"

int main()
{
    FILE* tree_data = get_file ("data/tree.txt", "r");
    node* root = BuildTree (tree_data);
    fclose (tree_data);

    // DumpTree (root);

    node* ya = FindNode (root, "ya");
    PrintObject (ya);

    node* Sanya = FindNode (root, "Arcadiy");
    PrintObject (Sanya);

    CompareObjects (ya, Sanya);
    //-----------------------------------

    // GuessTheNode (root);

    //-----------------------------------
    tree_data = get_file ("data/tree.txt", "w+");
    PrintPreOrder (root, tree_data);
    fclose (tree_data);

    // DumpTree (root);
    DrawTree (root);


    DestructNode (root);
}


node* CreateTreeRoot (char name[])
{
    node* root = CreateNewNode();

    root->name = name;

    return root;
}


node* CreateNewNode ()
{
    node* new_node = (node*) calloc (1, sizeof (node));
    if (!new_node) return nullptr;

    new_node->left   = nullptr;
    new_node->right  = nullptr;
    new_node->parent = nullptr;

    new_node->name = (char*) calloc (MAX_NAME_LEN, sizeof (char)); 

    return new_node;
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


node* InsertNode (char name[], node* parent, int position)
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

//-----------------------Choosing the mode-------------------


void StartGame (node* root)
{
    printf ("Welcome to Akinator, choose of of the following game modes\n",
            "Guessing game - 1\n",
            "Object listing - 2\n",
            "Objects comparison - 3\n");
    
    while (true)
    {
        printf ("I am choosing: ");
        char ans = -1;

        ans = getchar();

        switch (ans)
        {
        case :
            /* code */
            break;
        
        default:
            break;
        }
    }
}


//-----------------------Choosing the mode. End--------------

//------------------------Guessing mode-----------------------

int GuessTheNode (node* cur_node)
{
    int ans = -1;
    
    if (!cur_node->left && !cur_node->right) 
    {
        printf ("Ben, you guessed %s?\n", cur_node->name);
        scanf ("%d", &ans);

        if (ans == 1) printf ("Fuck yea\n");
        else
        {
            AppendNewObject (cur_node);
        }
    }
    else 
    {
        printf ("Does you character %s?\n", cur_node->name);
        scanf ("%d", &ans);

        if (ans == 1) GuessTheNode (cur_node->left);
        else
        {
            GuessTheNode (cur_node->right); 
        }
    }

    return 0;
}


void AppendNewObject (node* cur_node)
{
    node* new_obj = CreateNewNode();
    node* new_question = CreateNewNode();

    printf ("Who it was?\n");
    // GetInput (new_obj->name);
    scanf ("%s", new_obj->name);

    printf ("What's the difference between %s and %s?\n", new_obj->name, cur_node->name);
    // GetInput (new_question->name);
    scanf ("%s", new_question->name);
    
    node* top_node = cur_node->parent;
    cur_node->parent = new_question;

    if (top_node->left == cur_node) top_node->left = new_question;
    else { top_node->right = new_question; }
    
    new_question->parent = top_node;
    new_question->left   = new_obj;
    new_question->right  = cur_node;

    new_obj->parent = new_question;

    printf ("Objectf %s was added to base\n", new_obj->name);
}


char* GetInput (char* buffer)
{
    char* check_input = fgets(buffer, MAX_NAME_LEN, stdin);
    if (check_input == NULL) 
    {
        printf ("Bad input");
        return NULL;
    }

    buffer[strlen(buffer) - 1] = '\0';

    while (strlen(buffer) == 0) 
    {
        printf("\n Invalid name, write it again. \n\n");

        check_input = fgets(buffer, MAX_NAME_LEN, stdin);
        if (check_input == NULL) return NULL;

        buffer[strlen(buffer)] = '\0';
    }

    return buffer;
}

//------------------------Guessing mode. End-----------------------


//------------------------Object find mode----------------

void PrintObject (node* node_to_print)
{
    printf ("Characteristics of object %s:", node_to_print->name);
    
    Stack ancestors = BuildAncestorsStack (node_to_print);    

    node* tmp_node = nullptr;

    while (ancestors.size != 1)
    {
        tmp_node = (node*) StackPop (&ancestors);
        printf ("%s->", tmp_node->name);
    }

    tmp_node = (node*) StackPop (&ancestors);
    printf ("%s.\n", tmp_node->name);

    return;
}


Stack BuildAncestorsStack (node* cur_node)
{
    Stack ancestors = {};
    StackCtor (&ancestors, 10);
    StackPush (&ancestors, cur_node);
    
    AddAncestor (cur_node->parent, &ancestors);
    
    return ancestors;
}


void AddAncestor (node* cur_node, Stack* ancestors)
{
    StackPush (ancestors, cur_node);

    if (cur_node->parent) AddAncestor (cur_node->parent, ancestors);

    return;
}

//------------------------Object find mode. End-----------

//------------------------Object Comparison---------------

void CompareObjects (node* obj1, node* obj2)
{
    if (obj1 == obj2)
    { 
        printf ("Error. Comparison of the same object.");
        return;
    }

    Stack FirstAnc  = BuildAncestorsStack (obj1);
    Stack SecondAnc = BuildAncestorsStack (obj2);

    while (FirstAnc.size != 1 && SecondAnc.size != 1)
    {
        node* trait1 = (node*) StackPop (&FirstAnc);
        node* trait2 = (node*) StackPop (&SecondAnc);
        if (trait1 == trait2)
        {
            printf ("Objects have in common trait: %s\n", trait1->name);
        }
        else
        {
            printf ("Objects have difference in traits: Node %s has %s, when Node %s has %s.\n",
                    obj1->name, trait1->name, obj2->name, trait2->name);
            return;
        }
    }

    if (FirstAnc.size == 1 && SecondAnc.size == 1) printf ("Objects are exactly the same!\n");
    else if (FirstAnc.size > SecondAnc.size)
    {
        node* additional_trait = (node*) StackPop (&FirstAnc);
        printf ("Object %s has additional trait %s\n", obj1->name, additional_trait->name);
    }
    else if (FirstAnc.size < SecondAnc.size)
    {
        node* additional_trait = (node*) StackPop (&SecondAnc);
        printf ("Object %s has additional trait %s\n", obj2->name, additional_trait->name);
    }
    
    return;
}

//-----------------Object Comparison. End-----------------


//------------------------Dump----------------------------

void DumpTree (node* node)
{
    assert (node);

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
    fprintf (tree_data, "} ");

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
    _print ("Node%p[shape=rectangle, color=\"red\", width=0.2, style=\"filled\","
            "fillcolor=\"lightblue\", label=\"%s\"] \n \n",
            node, node->name);
    
    if (node->left) InitGraphvisNode (node->left, dot_file);
    if (node->right) InitGraphvisNode (node->right, dot_file);

    return;
}


void RecursDrawConnections (node* node, FILE* dot_file)
{
    if (node->left)
    {
        _print ("Node%p->Node%p\n", node, node->left);
        RecursDrawConnections (node->left, dot_file);
    } 
    if (node->right)
    {
        _print ("Node%p->Node%p\n", node, node->right);
        RecursDrawConnections (node->right, dot_file);
    } 

    return;
}


#undef _print

//------------------------Dump----------------------------

//------------------------Tree builder--------------------

node* BuildTree (FILE* tree_info)
{
    char* buffer = (char*) calloc (MAX_BUFFER_SIZE, sizeof (char));
    char* buffer_begin = buffer;

    fgets (buffer, MAX_BUFFER_SIZE, tree_info);

    buffer += OFFSET;  // skipping '{ '

    node* root = CreateNewNode();
    sscanf (buffer, "%s", root->name);
    // printf ("Root: %s \n", root->name);

    buffer += strlen (root->name) + 1; // skipping name and space
    if (*buffer == '}') return root;
    else 
    {
        root->left  = RecBuildNode (&buffer);
        root->left->parent = root;

        root->right = RecBuildNode (&buffer);
        root->right->parent = root;
    }

    free (buffer_begin);
    // printf ("\nexiting");

    return root;
}


node* RecBuildNode (char** buffer)
{
    while (**buffer == '{' || **buffer == '}') *buffer += OFFSET;

    node* new_node = CreateNewNode();
    sscanf (*buffer, "%s", new_node->name);

    // printf (new_node->name);

    *buffer += strlen (new_node->name) + 1;

    if (**buffer == '}')
    {
        *buffer += OFFSET; // Skipping ' { '
        return new_node;
    }
    else
    {
        new_node->left  = RecBuildNode (buffer);
        new_node->left->parent = new_node;

        new_node->right = RecBuildNode (buffer);
        new_node->right->parent = new_node;
    }

    return new_node;
}


//------------------------Tree builder--------------------


