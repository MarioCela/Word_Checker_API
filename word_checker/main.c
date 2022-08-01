#include <stdio.h>
#include <string.h>
#include <malloc.h>

//global variable
//length of the words that will be inserted by input
int k;




//structs
//structure for the nodes where dictionary's words will be saved
struct Dictionary_Node {
    char *word;
    char color;
    struct Dictionary_Node *father;
    struct Dictionary_Node *next_left;
    struct Dictionary_Node *next_right;
};

//structure used as node in a BST to save the values of chars of strings to elaborate the resulting output
//color is 0 if black, 1 if red
struct Char_Node {
    char key;
    char color;
    int occurrences_in_r;
    int in_the_correct_position_in_p;
    struct Char_Node *father;
    struct Char_Node *next_left;
    struct Char_Node *next_right;
};




//leaves
//dictionary node RB tree leaf
struct Dictionary_Node *dictionary_node_nil = NULL;

//char node RB tree leaf
struct Char_Node *char_node_nil = NULL;

//RB trees
//head of the list of nodes of the dictionary
struct Dictionary_Node *dictionary_head = NULL;

//head of the list of words that could be the reference word, that is printed when +stampa_filtrate is inserted
struct Dictionary_Node *possible_words = NULL;

//RB-tree
struct Char_Node *word_characters = NULL;




//functions signatures
//function to initialize leaves node for dictionary tree
void initialize_dictionary_node_nil();

//function to initialize leaves node for char tree
void initialize_char_node_nil();

//function to read from input the value of the length of the words that will be inserted
void read_k();

//function to initialize the dictionary at the beginning of the program
void get_the_starting_dictionary();

//function to insert words to the dictionary
void insert_words_to_the_dictionary(char exit_string[]);

//function to create a new node of the RB tree where a word will be saved
short int manage_new_dictionary_node(char exit_string[]);

//function to create a new dictionary node
struct Dictionary_Node *create_new_dictionary_node(char *word);

//function to insert a new node to the dictionary RB tree
void insert_dictionary_node(struct Dictionary_Node *head, struct Dictionary_Node *new_node);

//function called after a dictionary node insertion to keep respected RB tree's properties
void dictionary_node_insert_fix(struct Dictionary_Node *node);

//function that implements left rotation for dictionary nodes
void dictionary_head_left_rotation(struct Dictionary_Node *x);

//function that implements right rotation for dictionary nodes
void dictionary_head_right_rotation(struct Dictionary_Node *y);

//function to print all the words saved in the dictionary RB tree
void print_dictionary(struct Dictionary_Node *node);

//function that manages a game
void new_game();

//function that initializes the RB tree of possible words
void initialize_possible_words_list();

//function that elaborates the resulting output string when user tries to guess the reference word
short int compute_res(char r[], char p[], char res[]);

//function that checks if a word inserted by the user to try to guess the reference word belongs to the dictionary or not
short int check_if_belongs_to_the_dictionary(char *word);

//function to search a node in the dictionary RB tree
short int search_a_dictionary_node(struct Dictionary_Node *node, char *word);

//function that implements quicksort algorithm
void quicksort(char *A, int lo, int hi);

//function that implements partition Hoare algorithm used by quicksort algorithm
int partition_Hoare(char *A, int lo, int hi);

//function to create a new char node
struct Char_Node *create_new_char_node(char c, int occ);

//function to insert a new node to the char RB tree
void insert_new_char_node(struct Char_Node *new_node);

//function called after a char node insertion to keep respected RB tree's properties
void char_node_insert_fix(struct Char_Node *node);

//function that implements left rotation for char nodes
void char_node_left_rotation(struct Char_Node *x);

//function that implements right rotation for char nodes
void char_node_right_rotation(struct Char_Node *y);

//function to increment the field in_the_correct_position_in_p in a char node, useful to correctly generate the resulting output
void increment_correct_position_count(char c);

//function to get the difference between occurrences_in_r and in_the_correct_position_in_p
int get_ni_minus_ci(char c);

//function to empty the RB tree made by char nodes
void remove_all_chars(struct Char_Node *node);

//function launched when game ends, all the nodes of the RB tree of possible words are removed
void remove_all_possible_words();

//function launched every time that a new game could start after another one finishes
short int start_new_game();

//function that defines the behavior depending on the user's input. Cases are to start a new game,
//to close the program and to add new words to the dictionary
short int manage_input_new_game(short int *p);

//function to empty the RB tree containing the words that form the dictionary
void remove_all_dictionary_words(struct Dictionary_Node *node);




int main() {
    short int program_is_up;

    //initializing the leaves for RB trees
    initialize_dictionary_node_nil();
    initialize_char_node_nil();

    printf("Welcome to WordChecker game. Follow initial instructions and then start playing.\n"
           "To start a new game insert +nuova_partita, followed by the word that has to be guessed,\n"
           "the number of attempts you have to guess it and then you can insert the attempts, \n"
           "each of them followed by the correspondent output\n\n");

    //reading the lenght of the words
    read_k();
    //getting the starting dictionary
    get_the_starting_dictionary();

    //printing the dictionary
    printf("The dictionary is:\n");
    print_dictionary(dictionary_head);

    //launching a game and then, when the latter finishes, asking the user if he wants to close the program, to launch
    //a new game or to insert new words to the dictionary
    do {
        new_game();
        program_is_up = start_new_game();
    } while (program_is_up);

    //removing all the words contained in the dictionary RB tree
    remove_all_dictionary_words(dictionary_head);

    free(dictionary_node_nil);
    free(char_node_nil);
}

/**
 * The function initializes the leaf node for dictionary RB tree
 */
void initialize_dictionary_node_nil() {
    dictionary_node_nil = malloc(sizeof(struct Dictionary_Node));

    //all the leaves are black
    dictionary_node_nil->color = 'B';
    dictionary_node_nil->father = NULL;
    dictionary_node_nil->next_left = NULL;
    dictionary_node_nil->next_right = NULL;
}

/**
 * The function initializes the leaf node for chars RB tree
 */
void initialize_char_node_nil() {
    char_node_nil = malloc(sizeof(struct Char_Node));

    //all the leaves are black
    char_node_nil->color = 'B';
    char_node_nil->father = NULL;
    char_node_nil->next_left = NULL;
    char_node_nil->next_right = NULL;
}

/**
 * The function read from the user the length of the words
 */
void read_k() {
    int ins;

    printf("Insert the length of the words.\n"
           "Input: ");
    scanf("%d", &ins);

    k = ins + 1;
}

/**
 * The function is called when the program is run first and collects the words inserted by the user
 */
void get_the_starting_dictionary() {
    insert_words_to_the_dictionary("+nuova_partita");
}

/**
 * The function is called every time words have to be added to the dictionary
 */
void insert_words_to_the_dictionary(char exit_string[]) {
    short int check = 1;

    printf("\n");

    //goes on asking new words until ending loop input is inserted by user
    while (check) {
        check = manage_new_dictionary_node(exit_string);
    }
}

/**
 * The function ask the user to insert a word or the ending loop string. If a word is inserted, it is created a node
 * passed as parameter to the function that adds it to the dictionary RB tree
 *
 * @return 0 if the last input from user was +inserisci_fine, so there is no more need to ask for other words, 1 if not
 */
short int manage_new_dictionary_node(char exit_string[]) {
    //getting the input
    char *insertion = malloc(sizeof(char) * k);
    printf("Insert a new word to the dictionary. Insert %s instead if you do not want to insert any other word: ",
           exit_string);
    scanf("%s", insertion);

    //if it is not the ending loop string, the new node is inserted to the dictionary RB tree
    if (strcmp(insertion, exit_string) != 0) {
        insert_dictionary_node(dictionary_head, create_new_dictionary_node(insertion));
        free(insertion);
        return 1;
    } else {
        free(insertion);
        return 0;
    }
}

/**
 * The function creates a new dictionary node
 *
 * @param word is the string that will be saved into the node
 * @return the node
 */
struct Dictionary_Node *create_new_dictionary_node(char *word) {
    struct Dictionary_Node *new_node = malloc(sizeof(struct Dictionary_Node));
    new_node->word = malloc(sizeof(char) * strlen(word));

    strcpy(new_node->word, word);
    //new node's color is initialized as red
    new_node->color = 'R';
    new_node->father = NULL;
    new_node->next_left = dictionary_node_nil;
    new_node->next_right = dictionary_node_nil;

    return new_node;
}

/**
 * The function inserts a new node to the dictionary RB tree
 *
 * @param head is the head of the RB tree where the new node will be inserted
 * @param new_node is the new node that will be inserted
 */
void insert_dictionary_node(struct Dictionary_Node *head, struct Dictionary_Node *new_node) {
    struct Dictionary_Node *prev = NULL;
    struct Dictionary_Node *att = head;
    char left_or_right = 'R';

    //if the tree has no elements, new_node is the new head
    if (dictionary_head == NULL) {
        dictionary_head = new_node;
        dictionary_head->color = 'B';
    } else {
        //if not, it is found the correct position and new_node is inserted to the tree
        while (att != dictionary_node_nil) {
            for (int i = 0; i < k; ++i) {
                if (new_node->word[i] < att->word[i]) {
                    prev = att;
                    att = att->next_left;
                    left_or_right = 'L';
                    i = k;
                } else if (new_node->word[i] > att->word[i]) {
                    prev = att;
                    att = att->next_right;
                    left_or_right = 'R';
                    i = k;
                }
            }
        }

        if (left_or_right == 'L') {
            prev->next_left = new_node;
            new_node->father = prev;
        } else {
            prev->next_right = new_node;
            new_node->father = prev;
        }

        //after an insertion, RB tree's properties could have been violated: fixes could be needed
        dictionary_node_insert_fix(new_node);
    }
}

/**
 * The function fixes a RB tree formed by dictionary nodes recoloring or rotating the nodes of the tree
 * in order to keep respected RB tree's properties
 *
 * @param node is the node that has just been inserted into the tree
 */
void dictionary_node_insert_fix(struct Dictionary_Node *node) {
    while (node->father != NULL && node->father->color == 'R') {
        if (node->father == node->father->father->next_left) {
            struct Dictionary_Node *y = node->father->father->next_right;

            if (y->color == 'R') {
                node->father->color = 'B';
                y->color = 'B';
                node->father->father->color = 'R';
                node = node->father->father;
            }
            else {
                if (node == node->father->next_right) {
                    node = node->father;
                    dictionary_head_left_rotation(node);
                }

                node->father->color = 'B';
                node->father->father->color = 'R';
                dictionary_head_right_rotation(node->father->father);
            }

        } else {
            struct Dictionary_Node *y = node->father->father->next_left;

            if (y->color == 'R') {
                node->father->color = 'B';
                y->color = 'B';
                node->father->father->color = 'R';
                node = node->father->father;
            }
            else {
                if (node == node->father->next_left) {
                    node = node->father;
                    dictionary_head_right_rotation(node);
                }

                node->father->color = 'B';
                node->father->father->color = 'R';
                dictionary_head_left_rotation(node->father->father);
            }
        }
    }

    dictionary_head->color = 'B';
}

/**
 * The function implements left rotation for dictionary nodes
 *
 * @param x and x->next_right are the nodes that will be left rotated
 */
void dictionary_head_left_rotation(struct Dictionary_Node *x) {
    struct Dictionary_Node *y = x->next_right;

    if (y->next_left != dictionary_node_nil) {
        x->next_right = y->next_left;
        y->next_left->father = x;
    }

    if (x->father == NULL) {
        dictionary_head = y;
        y->father = NULL;
    } else if (x == x->father->next_left) {
        x->father->next_left = y;
        y->father = x->father;
    } else {
        x->father->next_right = y;
        y->father = x->father;
    }

    x->next_right = y->next_left;
    x->father = y;
    y->next_left = x;
}

/**
 * The function implements right rotation for dictionary nodes
 *
 * @param y and y->next_left are the nodes that will be right rotated
 */
void dictionary_head_right_rotation(struct Dictionary_Node *y) {
    struct Dictionary_Node *x = y->next_left;

    if (x->next_right != dictionary_node_nil) {
        y->next_left = x->next_right;
        x->next_right->father = y;
    }

    if (y->father == NULL) {
        dictionary_head = x;
        x->father = NULL;
    } else if (y == y->father->next_right) {
        y->father->next_right = x;
        x->father = y->father;
    } else {
        y->father->next_left = x;
        x->father = y->father;
    }

    y->next_left = x->next_right;
    y->father = x;
    x->next_right = y;
}

/**
 * The function prints in order the nodes of the RB tree made by dictionary nodes
 */
void print_dictionary(struct Dictionary_Node *node) {
    if (node->next_left != dictionary_node_nil) {
        print_dictionary(node->next_left);
    }
    printf("%s\n", node->word);
    if (node->next_right != dictionary_node_nil) {
        print_dictionary(node->next_right);
    }
}

/**
 * TODO +stampa_filtrate
 *
 * The function is launched when a new game starts.
 * They are asked: the reference word, the number of attempts to try to guess the reference word, and the effective attempts
 */
void new_game() {
    char *ref = malloc(k);

    //asking for the reference word
    printf("\nInsert the reference word, the one that has to be guessed: ");
    scanf("%s", ref);

    //initializing the RB tree containing the words that will be printed after +stampa_filtrate input
    initialize_possible_words_list();

    int attempts;

    //asking for the number of attempts
    printf("\nInsert the number of attempts: ");
    scanf("%d", &attempts);

    printf("\n");

    //asking for the words to guess the reference word
    for (int i = 0; i < attempts; ++i) {
        char *word = malloc(k);
        char *result = malloc(k);

        printf("Try to guess the word (tentative n. %d): ", i + 1);
        scanf("%s", word);

        short int r = compute_res(ref, word, result);

        //printing the output depending on the result: word belongs to the dictionary or not, the words has been guessed or not
        if (r == 0) {
            printf("Output: %s\n\n", result);
        } else if (r == 1) {
            printf("Output: ok\n\n");
            i = attempts;
        } else {
            printf("Output: not exist\n\n");
        }

        free(result);
        free(word);

        free(char_node_nil);

        //char_node_nil has to be reinitialized at every iteration because of the free function
        initialize_char_node_nil();
    }

    free(ref);

    //removing all the RB tree nodes containing the words that would be printed after +stampa_filtrate input
    remove_all_possible_words();
}

/**
 * TODO +stampa_filtrate
 */
void initialize_possible_words_list() {

}

/**
 * TODO +stampa_filtrate
 *
 * The function generates the output string depending on the string r and p received as params
 *
 * @param r is the reference word that has to be guessed
 * @param p is the word that tries to guess r
 * @param res is the resulting string
 * @return 1 if the word has been guessed, 0 if not, 2 if the word inserted is not in the dictionary
 */
short int compute_res(char *r, char *p, char res[]) {
    if (check_if_belongs_to_the_dictionary(p)) {
        int i;
        short int result = 1;

        //r and p are ordered using quicksort algorithm
        char r_ordered[k];
        char p_ordered[k];
        strcpy(r_ordered, r);
        strcpy(p_ordered, p);

        quicksort(r_ordered, 0, k - 2);
        quicksort(p_ordered, 0, k - 2);

        int r_index = 0;
        int p_index;

        int occur_in_r;

        //looping on the chars of p_ordered
        for (p_index = 0; p_index < k - 1; ++p_index) {

            //count of the occurrences in r of the char read in p_ordered[p_index]
            occur_in_r = 0;

            //used when it is tried to find p_ordered[i] in r, but it is not contained in it,
            //so it is needed to return to the old index
            int old_r_index = r_index;

            //if in the ordered string of p there are adjacent chars with the same content, the following ones are ignored
            if (p_index == 0 || p_ordered[p_index] != p_ordered[p_index - 1]) {

                //r_index is incremented until we find the same char it is being watched in p_ordered[p_index],
                //or until the string finishes
                for (; r_ordered[r_index] != p_ordered[p_index] && r_index < k - 1; ++r_index) {

                }

                //if it has been reached the end of the string, it means that there is no occurrences in r of p_ordered[p_index]
                //r_index is set again to its old value, because there could be other char to be analyzed,
                //and the p_ordered[p_index] is inserted into the RB tree
                if (r_ordered[r_index] != p_ordered[p_index]) {
                    r_index = old_r_index;
                } else {
                    //otherwise, it is counted the number of occurrences
                    for (; r_ordered[r_index] == p_ordered[p_index] && r_index < k - 1; ++r_index) {
                        ++occur_in_r;
                    }
                }


                //p_ordered[p_index] is inserted into the RB tree of char nodes
                insert_new_char_node(create_new_char_node(p_ordered[p_index], occur_in_r));
            }
        }

        //all the initial string is read in order to set + when r[i] and p[i] match
        for (i = 0; i < k - 1; ++i) {
            if (r[i] == p[i]) {
                res[i] = '+';
                increment_correct_position_count(p[i]);
            } else {
                res[i] = '*';
                result = 0;
            }
        }

        //positions where has not been set a + are managed
        for (i = 0; i < k - 1; ++i) {
            if (res[i] == '*') {
                int count = 0;
                //starting for current position - 1, it is counted the number of char equals to p[i], useful to find out
                //if a / or a | has to be set
                for (int j = i - 1; j >= 0; --j) {
                    if (p[j] == p[i] && p[j] != r[j]) {
                        ++count;
                    }
                }

                int diff = get_ni_minus_ci(p[i]);

                //correct output is set at position i
                if (count >= diff) {
                    res[i] = '/';
                } else {
                    res[i] = '|';
                }
            }
        }

        //the RB tree containing char nodes is emptied
        remove_all_chars(word_characters);
        word_characters = NULL;

        res[i] = '\0';

        return result;
    } else {
        return 2;
    }
}

/**
 * The function checks if the param word belongs to the dictionary RB tree
 *
 * @param word is the word inserted by user in input to try to guess the reference word
 * @return 1 if the word belongs to the dictionary, 0 if not
 */
short int check_if_belongs_to_the_dictionary(char *word) {
    return search_a_dictionary_node(dictionary_head, word);
}

/**
 * The function searches a word in the dictionary and tells to the caller if it is in it or not.
 * It is a recursive function, calling itself to the next left and right nodes
 *
 * @param node is the actual node is being analyzed to see if the word is saved here or not
 * @param word is the work is being looked for
 * @return 1 if the word is in the subtree, 0 if not
 */
short int search_a_dictionary_node(struct Dictionary_Node *node, char *word) {
    if (node != dictionary_node_nil) {
        if (strcmp(word, node->word) == 0) {
            return 1;
        } else {
            if (search_a_dictionary_node(node->next_left, word) == 1) {
                return 1;
            }
            if (search_a_dictionary_node(node->next_right, word) == 1) {
                return 1;
            }
        }
    }

    return 0;
}

/**
 * The function implements quicksort algorithm
 *
 * @param A is the string that has to be ordered
 * @param lo is low index
 * @param hi is high index
 */
void quicksort(char *A, int lo, int hi) {
    if (lo < hi) {
        int p = partition_Hoare(A, lo, hi);
        quicksort(A, lo, p - 1);
        quicksort(A, p + 1, hi);
    }
}

/**
 * The function implements the Partition Hoare algorithm used by quicksort algorithm
 *
 * @param A is the string that has to be ordered
 * @param lo is low index
 * @param hi is high index
 */
int partition_Hoare(char *A, int lo, int hi) {
    char pivot = A[hi];
    int i = lo - 1;

    for (int j = lo; j <= hi - 1; ++j) {
        if (A[j] < pivot) {
            i++;

            char tmp = A[i];
            A[i] = A[j];
            A[j] = tmp;
        }
    }

    char tmp = A[i + 1];
    A[i + 1] = A[hi];
    A[hi] = tmp;

    return i + 1;
}

/**
 * The function creates a new char node
 *
 * @param c is the key value of the node, the char that has to be saved
 * @return the node that has been allocated and initialized
 */
struct Char_Node *create_new_char_node(char c, int occ) {
    struct Char_Node *new_node = malloc(sizeof(struct Char_Node));
    new_node->key = c;
    //a new node is always initialized as a red node
    new_node->color = 'R';
    new_node->occurrences_in_r = occ;
    new_node->in_the_correct_position_in_p = 0;
    new_node->father = NULL;
    //the leaves are NIL
    new_node->next_left = char_node_nil;
    new_node->next_right = char_node_nil;
}

/**
 * The function inserts a new char node into the RB tree of chars
 *
 * @param new_node is the new node that has to be inserted
 */
void insert_new_char_node(struct Char_Node *new_node) {
    struct Char_Node *prev = NULL;
    struct Char_Node *att = word_characters;

    //if the tree has no elements, new_node is the new head
    if (word_characters == NULL) {
        word_characters = new_node;
        word_characters->color = 'B';
    } else {
        //if not, it is found the correct position and new_node is inserted to the tree
        while (att != char_node_nil) {
            if (new_node->key < att->key) {
                prev = att;
                att = att->next_left;
            } else {
                prev = att;
                att = att->next_right;
            }
        }

        if (new_node->key < prev->key) {
            prev->next_left = new_node;
            new_node->father = prev;
        } else {
            prev->next_right = new_node;
            new_node->father = prev;
        }

        //after an insertion, RB tree's properties could have been violated: fixes could be needed
        char_node_insert_fix(new_node);
    }
}

/**
 * The function is called when a new node is inserted but that could violate RB tree properties,
 * so fixes could be needed
 *
 * @param node is the new inserted node that could activate fix algorithms
 */
void char_node_insert_fix(struct Char_Node *node) {
    while (node->father != NULL && node->father->color == 'R') {
        if (node->father == node->father->father->next_left) {
            struct Char_Node *y = node->father->father->next_right;

            if (y->color == 'R') {
                node->father->color = 'B';
                y->color = 'B';
                node->father->father->color = 'R';
                node = node->father->father;
            }
            else {
                if (node == node->father->next_right) {
                    node = node->father;
                    char_node_left_rotation(node);
                }

                node->father->color = 'B';
                node->father->father->color = 'R';
                char_node_right_rotation(node->father->father);
            }

        } else {
            struct Char_Node *y = node->father->father->next_left;

            if (y->color == 'R') {
                node->father->color = 'B';
                y->color = 'B';
                node->father->father->color = 'R';
                node = node->father->father;
            }
            else {
                if (node == node->father->next_left) {
                    node = node->father;
                    char_node_right_rotation(node);
                }

                node->father->color = 'B';
                node->father->father->color = 'R';
                char_node_left_rotation(node->father->father);
            }
        }
    }

    word_characters->color = 'B';
}

/**
 * The function implements the left rotation algorithm in a RB tree
 *
 * @param x and its successor x->next_right are the node that will be left rotated
 */
void char_node_left_rotation(struct Char_Node *x) {
    struct Char_Node *y = x->next_right;

    if (y->next_left != char_node_nil) {
        x->next_right = y->next_left;
        y->next_left->father = x;
    }

    if (x->father == NULL) {
        word_characters = y;
        y->father = NULL;
    } else if (x == x->father->next_left) {
        x->father->next_left = y;
        y->father = x->father;
    } else {
        x->father->next_right = y;
        y->father = x->father;
    }

    x->next_right = y->next_left;
    x->father = y;
    y->next_left = x;
}

/**
 * The function implements the right rotation algorithm in a RB tree
 *
 * @param y and its successor y->next_left are the node that will be right rotated
 */
void char_node_right_rotation(struct Char_Node *y) {
    struct Char_Node *x = y->next_left;

    if (x->next_right != char_node_nil) {
        y->next_left = x->next_right;
        x->next_right->father = y;
    }

    if (y->father == NULL) {
        word_characters = x;
        x->father = NULL;
    } else if (y == y->father->next_right) {
        y->father->next_right = x;
        x->father = y->father;
    } else {
        y->father->next_left = x;
        x->father = y->father;
    }

    y->next_left = x->next_right;
    y->father = x;
    x->next_right = y;
}

/**
 * The function increment by 1 the value of int field in_the_correct_position_in_p of a char node.
 * This value is used to generate the resulting output when the user tries to guess the word
 *
 * @param c is the char, the key value of the node
 */
void increment_correct_position_count(char c) {
    struct Char_Node *tmp = word_characters;

    while (tmp != char_node_nil) {
        if (tmp->key == c) {
            ++tmp->in_the_correct_position_in_p;
            tmp = char_node_nil;
        } else {
            if (c < tmp->key) {
                tmp = tmp->next_left;
            } else {
                tmp = tmp->next_right;
            }
        }
    }
}

/**
 * The function is used to get the difference between the fields indicated in the return section.
 * The value is used to generate the resulting output when the user tries to guess the word
 *
 * @param c is the char, the key value of the node
 * @return the difference between occurrences_in_r and in_the_correct_position_in_p
 */
int get_ni_minus_ci(char c) {
    int ni;
    int ci;
    struct Char_Node *tmp = word_characters;

    while (tmp != char_node_nil) {
        if (c == tmp->key) {
            ni = tmp->occurrences_in_r;
            ci = tmp->in_the_correct_position_in_p;
            tmp = char_node_nil;
        } else if (c < tmp->key) {
            tmp = tmp->next_left;
        } else {
            tmp = tmp->next_right;
        }
    }

    return (ni - ci);
}

/**
 * The function deallocates all the memory allocated for char nodes saved into the RB tree
 *
 * @param node is the node from which the function will be called to its left and right subtrees and that then will be freed
 */
void remove_all_chars(struct Char_Node *node) {
    if (node->next_left != char_node_nil) {
        remove_all_chars(node->next_left);
    }
    if (node->next_right != char_node_nil) {
        remove_all_chars(node->next_right);
    }
    free(node);
}

/**
 * TODO +stampa_filtrate
 */
void remove_all_possible_words() {

}

/**
 * The function is called when previous game ended and the program needs to know if a new one has to be started or not
 * @return 1 if a new game has to be started, 0 if not
 */
short int start_new_game() {
    short int continue_asking_input = 1;
    short int game_goes_on = 1;
    short int *ptr = &game_goes_on;

    while (continue_asking_input) {
        continue_asking_input = manage_input_new_game(ptr);
    }

    return game_goes_on;
}

/**
 * The function manages the input by the player, that can choose to start a new game, to close the program, or to
 * insert new words to the dictionary
 *
 * @param p is a pointer to game_goes_on variable in start_new_game method
 * @return 1 if this method has to be called again, 0 if not
 */
short int manage_input_new_game(short int *p) {
    char *ins = malloc(18);

    printf("\nStart a new game inserting +nuova_partita;\n"
           "Close the program inserting +game_over;\n"
           "Add new words to the dictionary inserting +inserisci_inizio.\n"
           "Input: ");
    scanf("%s", ins);

    if (strcmp(ins, "+nuova_partita") == 0) {
        *p = 1;
        free(ins);
        return 0;
    } else if (strcmp(ins, "+game_over") == 0) {
        *p = 0;
        free(ins);
        return 0;
    } else if (strcmp(ins, "+inserisci_inizio") == 0) {
        insert_words_to_the_dictionary("+inserisci_fine");
        free(ins);
        return 1;
    }
}

/**
 * The function deallocates all the memory allocated for dictionary nodes saved into the RB tree
 *
 * @param node is the node from which the function will be called to its left and right subtrees and that then will be freed
 */
void remove_all_dictionary_words(struct Dictionary_Node *node) {
    if (node->next_left != dictionary_node_nil) {
        remove_all_dictionary_words(node->next_left);
    }
    if (node->next_right != dictionary_node_nil) {
        remove_all_dictionary_words(node->next_right);
    }
    free(node->word);
    free(node);
}
