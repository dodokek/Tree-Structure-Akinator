#define TX_USE_SPEAK  // May the god save us.

#include "TXLib.h"

#include "akinator.h"

int main()
{
    node* root = GetTreeRoot();

    StartGame (root);

    SaveProgress (root);

    DestructTree (root);
}


node* GetTreeRoot ()
{
    FILE* tree_data = get_file ("data/tree.txt", "rb");
    
    node* root = BuildTree (tree_data);

    DumpTree (root);
    
    fclose (tree_data);

    return root;
}


node* InitTreeRoot (char name[])
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


node* DestructTree (node* root)
{
    if (root->left)  DestructTree (root->left);
    if (root->right) DestructTree (root->right);

    root->name = nullptr;
    
    free (root);
    root = nullptr;

    return root;
}


// Positions 
node* InsertNode (char name[], node* parent, Positions position)
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

//------------------------Tree builder--------------------

// node* BuildTree (FILE* tree_info)
// {
//     char* buffer = GetTextBuffer (tree_info);
//     char* buffer_begin = buffer;

//     buffer += OFFSET;  // skipping '{ '

//     node* root = CreateNewNode();
//     sscanf (buffer, "%s", root->name);

//     printf ("Root: %s\n", root->name);

//     buffer += strlen (root->name) + 1; // skipping name and space
    
//     if (*buffer == '}') return root;
    
//     root->left  = RecBuildNode (&buffer);
//     root->left->parent = root;

//     root->right = RecBuildNode (&buffer);
//     root->right->parent = root;

//     free (buffer_begin);

//     return root;
// }

node* BuildTree (FILE* tree_info)
{
    Text input = {};
    GetTreeObjects (&input, tree_info);
    int obj_counter = 1;

    for (int i = 0; i < input.lines_amount; i++)
    {
        printf ("Got line %s \n ", input.objects[i].begin);
    }

    node* root = CreateNewNode();
    root->name = input.objects[obj_counter].begin;
    printf ("Root: %s \n ", root->name);
    obj_counter++;

    if (*input.objects[obj_counter].begin == '}') return root;

    obj_counter++;
    root->left  = RecBuildNode (&input, &obj_counter);
    root->left->parent = root;

    root->right = RecBuildNode (&input, &obj_counter);
    root->right->parent = root;

    return root;
}


node* RecBuildNode (Text* input, int* obj_counter)
{
    while (*input->objects[*obj_counter].begin == '{' || *input->objects[*obj_counter].begin == '}')
    {
        (*obj_counter)++; // skips all unneeded brackets
    }
    
    node* new_node = CreateNewNode();
    new_node->name = input->objects[*obj_counter].begin;

    printf ("Got node %s \n ", new_node->name);

    (*obj_counter)++;

    if (*input->objects[*obj_counter].begin == '}')
    {
        (*obj_counter)++; // skips '}'
        return new_node;
    }

    new_node->left = RecBuildNode (input, obj_counter);
    new_node->left->parent = new_node;

    new_node->right = RecBuildNode (input, obj_counter);
    new_node->right->parent = new_node;

    return new_node;
}


// node* RecBuildNode (char** buffer)
// {
//     while (**buffer == '{' || **buffer == '}') *buffer += OFFSET;

//     node* new_node = CreateNewNode();
//     sscanf (*buffer, "%s", new_node->name);

//     // printf ("Got node %s, cur sign %c\n", new_node->name, **buffer);

//     *buffer += strlen (new_node->name) + 1;

//     if (**buffer == '}')
//     {
//         *buffer += OFFSET; // Skipping ' { '

//         return new_node;
//     }

//     new_node->left  = RecBuildNode (buffer);
//     new_node->left->parent = new_node;

//     new_node->right = RecBuildNode (buffer);
//     new_node->right->parent = new_node;

//     return new_node;
// }

//------------------------Tree builder--------------------


//-----------------------Choosing the mode-------------------

void StartGame (node* root)
{
    char greetings[] = "Welcome to Akinator, choose of of the following game modes\n\
                    \tGuessing game - 1\n\    
                     \tObject listing - 2\n\
                     \tObjects comparison - 3\n\
                     \tType 0 to exit.\n";
    
    printf (greetings);
    
    bool is_exit = false;

    while (!is_exit)
    {
        printf ("I am choosing: ");
        int ans = -1;

        fflush (stdin);
        ans = getchar();

        node* tmp_node_1 = nullptr;
        node* tmp_node_2 = nullptr;

        int cvt_to_num = 48;

        switch (ans - cvt_to_num)
        {
            case GUESS:
                GuessNode (root);
                break;
            
            case LISTING:
                tmp_node_1 = GetNodeFromUser (root);

                if (tmp_node_1) PrintObject (tmp_node_1);
            
                break;

            case COMPARISON:
                tmp_node_1 = GetNodeFromUser (root);
                tmp_node_2 = GetNodeFromUser (root);

                if (tmp_node_1 && tmp_node_2)
                    CompareObjects (tmp_node_1, tmp_node_2);
                break;

            case EXIT:
                SayWords ("Thanks for playing\n");
                is_exit = true;
                break;

            default:
                SayWords ("Unknown command %d, try again.\n", ans - cvt_to_num);
                break;
        }
    }
}


node* GetNodeFromUser (node* root)
{
    char tmp_name[MAX_NAME_LEN] = "";

    SayWords ("Enter the name of the node: ");
    scanf ("%s", tmp_name);

    node* tmp_node = FindNode (root, tmp_name);

    if (!tmp_node) SayWords ("There is no such node with name %s in the base!\n", tmp_name);
    
    return tmp_node;
}

//-----------------------Choosing the mode. End--------------

//------------------------Guessing mode-----------------------

void GuessNode (node* cur_node)
{
    int ans = -1;
    
    if (!cur_node->left && !cur_node->right) 
    {
        SayWords ("Ben, you guessed %s?\n", cur_node->name);
        scanf ("%d", &ans);

        if (GetAnswer() == YES) SayWords ("Fuck yea\n");
        else                    AddNode (cur_node);
    }
    else 
    {
        SayWords ("Does you character %s?\n", cur_node->name);
        scanf ("%d", &ans);

        if (GetAnswer() == YES) GuessNode (cur_node->left);
        else                    GuessNode (cur_node->right); 
    }
}


int GetAnswer ()
{
    char ans = -1;
    scanf ("%c", &ans);

    while (true)
    {
        if      (ans == 'Y' || ans == 'y') return 1;
        else if (ans == 'N' || ans == 'n') return 0;
        else
        {
            printf ("I can get answers only \"Y,y or N,n\"\n");
            ClearBuffer ('\n');
            scanf ("%c", &ans);
        }
    }
}


void AddNode (node* cur_node)
{
    node* new_obj = CreateNewNode();
    node* new_question = CreateNewNode();

    printf ("Who it was?\n");
    scanf ("%s", new_obj->name);

    printf ("What's the difference between %s and %s?\n", new_obj->name, cur_node->name);
    scanf ("%s", new_question->name);
    
    node* top_node = cur_node->parent;
    cur_node->parent = new_question;

    if (top_node->left == cur_node) top_node->left = new_question;
    else                            top_node->right = new_question;
    
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


void SayWords (char* temp, ...)
{
    char message[MAX_MESSAGE_LEN] = "";
    char message_tmp[MAX_MESSAGE_LEN] = "";

    va_list params;
    va_start (params, temp);


    vsprintf (message, temp, params);
    printf ("%s\n", message);

    sprintf (message_tmp, "<speak version=\"1.0\" xmlns=\"http://www.w3.org/2001/10/synthesis\" xml:lang=\"en-US\"> \
                    <voice name=\"en-US-TonyNeural\">                         \
                    %s                                                                                 \
                    </voice>                                                               \
                    </speak>", message); 

    txSpeak (message_tmp);

    va_end (params);

}

//------------------------Guessing mode. End-----------------------


//------------------------Object find mode----------------

void PrintObject (node* node_to_print)
{
    printf ("Characteristics of object %s:", node_to_print->name);
    
    Stack ancestors = BuildAncestorsStack (node_to_print);    

    node* tmp_node = nullptr;

    while (ancestors.size != 0)
    {
        tmp_node = (node*) StackPop (&ancestors);
        if (isNegativeAns (tmp_node)) SayWords ("NOT %s - ", tmp_node->name);
        else   
        {                    
            printf ("%s - ", tmp_node->name);
        }
    }

    // tmp_node = (node*) StackPop (&ancestors);
    // SayWords ("%s.\n", tmp_node->name);
}

bool isNegativeAns (node* cur_node)
{
    if (!(cur_node->parent)) return false;

    // printf ("I am %s, %p My parent is %p he calls %p\n",
    //         cur_node->name, cur_node, cur_node->parent, cur_node->parent->left);

    if (cur_node->parent->right == cur_node) return true;

    return false;
}


Stack BuildAncestorsStack (node* cur_node)
{
    Stack ancestors = {0};
    StackCtor (&ancestors, 10);
    StackPush (&ancestors, cur_node);
    
    AddAncestor (cur_node->parent, &ancestors);
    
    return ancestors;
}


void AddAncestor (node* cur_node, Stack* ancestors)
{
    StackPush (ancestors, cur_node);

    if (cur_node->parent) AddAncestor (cur_node->parent, ancestors);
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

    node* trait1 = nullptr;
    node* trait2 = nullptr;

    while (FirstAnc.size != 1 && SecondAnc.size != 1)
    {
        trait1 = (node*) StackPop (&FirstAnc);
        trait2 = (node*) StackPop (&SecondAnc);

        if (trait1 == trait2)
        {
            printf ("They are same in: %s\n", trait1->name);
        }
        else
        {
            printf ("Oh, they are different: Node %s has %s, when Node %s has %s.\n",
                    obj1->name, trait1->name, obj2->name, trait2->name);
            return;
        }
    }

    if (FirstAnc.size == 1 && SecondAnc.size == 1) 
    {
        printf ("Objects are different in trait %s!\n", trait1->parent->name);
    }
    else if (FirstAnc.size > SecondAnc.size)
    {
        node* additional_trait = (node*) StackPop (&FirstAnc);
        printf ("Object %s has additional trait %s\n", 
                 obj1->name, additional_trait->name);
    }
    else if (FirstAnc.size < SecondAnc.size)
    {
        node* additional_trait = (node*) StackPop (&SecondAnc);
        printf ("Object %s has additional trait %s\n", 
                 obj2->name, additional_trait->name);
    }   
}

//-----------------Object Comparison. End-----------------


//------------------------Dump----------------------------

void SaveProgress (node* root)
{
    FILE* tree_data = get_file ("data/tree.txt", "w+");

    PrintPreOrder (root, tree_data);
    fclose (tree_data);
    DrawTree (root);
}


void DumpTree (node* node)
{
    assert (node);

    printf ("Ptr[%p] : \n", node);
    printf ("\t Node %s: left %p, right %p, parent %p, %s\n",
            node->name, node->left,
            node->right, node->parent, node->name);

    if (node->left)  DumpTree (node->left);
    if (node->right) DumpTree (node->right);
}


void PrintPreOrder (node* node, FILE* tree_data)
{
    fprintf (tree_data, "{\n%s", node->name);
    if (node->left)  PrintPreOrder (node->left,  tree_data);
    if (node->right) PrintPreOrder (node->right, tree_data);
    fprintf (tree_data, "}\n");
}


void PrintPostOrder (node* node, FILE* tree_data)
{
    if (node->left)  PrintPreOrder (node->left,  tree_data);
    if (node->right) PrintPreOrder (node->right, tree_data);
    fprintf (tree_data, "{\n%s", node->name);
    fprintf (tree_data, "}\n");
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



