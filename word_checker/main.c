#include <stdio.h>
#include <string.h>
#include <malloc.h>

//structs
//structure for the nodes where dictionary's words will be saved
struct Dictionary_Node {
    char *word;
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

//global variable
//length of the words that will be inserted by input
int k;

//BSTes
//head of the list of nodes of the dictionary
struct Dictionary_Node *dictionary_head = NULL;

//head of the list of words that could be the reference word, that is printed when +stampa_filtrate is inserted
struct Dictionary_Node *possible_words = NULL;

//RB-tree
struct Char_Node *word_characters = NULL;

struct Char_Node *nil;

//TODO doc
void initialize_nil();

//TODO doc
struct Char_Node *create_new_char_node(char c);

//TODO doc
void insert_new_char_node(struct Char_Node *new_node);

//TODO doc
void insert_fix(struct Char_Node *node);

//TODO doc
void in_order_print_char_nodes(struct Char_Node *node);

//method to read from input the value of the length of the words that will be inserted
void read_k();

//method to initialize the dictionary at the beginning of the program
void get_the_starting_dictionary();

//method to insert words to the dictionary
void insert_words_to_the_dictionary(char exit_string[]);

//method to create a new node of the BST where a word will be saved
short int manage_new_dictionary_node(char exit_string[]);

//method to insert a new node to the dictionary BST
void insert_dictionary_node(struct Dictionary_Node *att, struct Dictionary_Node *prev, char *word, int left_or_right);

//method to create a new dictionary node
struct Dictionary_Node *create_dictionary_node(char *word);

//method to print all the words saved in the dictionary BST
void print_dictionary(struct Dictionary_Node *node);

//method that manages a game
void new_game();

//method that initializes the BST of possible words
void initialize_possible_words_list();

//method that elaborates the resulting output string when user tries to guess the reference word
short int compute_res(char r[], char p[], char res[]);

//method that checks if a word inserted by the user to try to guess the reference word belongs to the dictionary or not
short int check_if_belongs_to_the_dictionary(char *word);

//
void quicksort(char *A, int lo, int hi);

//
int partition_Hoare(char *A, int lo, int hi);

//method launched when game ends, all the nodes of the BST are removed
void remove_all_possible_words();

//method launched every time that a new game could start after another one finishes
short int start_new_game();

//method that defines the behavior depending on the user's input. Cases are to start a new game,
//to close the program and to add new words to the dictionary
short int manage_input_new_game(short int *p);

//method to create a new char node
struct Char_Node *create_char_node(char c);

//method to search a node in the dictionary BST
short int search_a_dictionary_node(struct Dictionary_Node *node, char *word);

//
void remove_all_dictionary_words(struct Dictionary_Node *node);


int main() {
    short int program_is_up;

    initialize_nil();

    printf("Welcome to WordChecker game. Follow initial instructions and then start playing.\n"
           "To start a new game insert +nuova_partita, followed by the word that has to be guessed,\n"
           "the number of attempts you have to guess it and then you can insert the attempts, \n"
           "each of them followed by the correspondent output\n\n");

    read_k();
    get_the_starting_dictionary();

    printf("The dictionary is:\n");
    print_dictionary(dictionary_head);

    //launching a game and then, when the latter finishes, asking the user if he wants to close the program, to launch
    //a new game or to insert new words to the dictionary
    do {
        new_game();
        program_is_up = start_new_game();
    } while (program_is_up);

    remove_all_dictionary_words(dictionary_head);
}

/**
 * The method read from the user the length of the words
 */
void read_k() {
    int ins;

    printf("Insert the length of the words.\n"
           "Input: ");
    scanf("%d", &ins);

    k = ins + 1;
}

/**
 * The method is called when the program is run first and collects the words inserted by the user
 */
void get_the_starting_dictionary() {
    insert_words_to_the_dictionary("+nuova_partita");
}

/**
 * The method is called every time words have to be added to the dictionary
 */
void insert_words_to_the_dictionary(char exit_string[]) {
    short int check = 1;

    printf("\n");

    while (check) {
        check = manage_new_dictionary_node(exit_string);
    }
}

/**
 * The method creates a node in the list representing the dictionary if the user inserts a word and not the ending command.
 * Every word is added following the rules of a BST
 *
 * @return 0 if the last input from user was +inserisci_fine, so there is no more need to ask for other words, 1 if not
 */
short int manage_new_dictionary_node(char exit_string[]) {
    //getting the input
    char *insertion = malloc(sizeof(char) * k);
    printf("Insert a new word to the dictionary. Insert %s instead if you do not want to insert any other word: ",
           exit_string);
    scanf("%s", insertion);

    if (strcmp(insertion, exit_string) != 0) {
        insert_dictionary_node(dictionary_head, NULL, insertion, 0);
        return 1;
    } else {
        return 0;
    }
}

/**
 * The method inserts a new node to the dictionary BST
 *
 * @param att is the actual node of BST
 * @param prev is the previous node of the BST where we were the previous iteration
 * @param word is the string containing the word to be added to the dictionary
 * @param left_or_right is 0 if act = prev->next_left, 1 if act = prev->next_right
 */
void insert_dictionary_node(struct Dictionary_Node *att, struct Dictionary_Node *prev, char *word, int left_or_right) {
    //if act is NULL it means that it is the root or a leaf
    if (att == NULL) {
        //if it is the root the new head will be the new node
        if (dictionary_head == NULL) {
            dictionary_head = create_dictionary_node(word);
        } else {
            //it is a leaf, it is created a new node that is linked to its father and its father is linked to it back
            att = create_dictionary_node(word);
            att->father = prev;

            if (left_or_right == 0) {
                prev->next_left = att;
            } else {
                prev->next_right = att;
            }
        }
    } else {
        //if it is not NULL, it means that it has to go down in the BST
        for (int i = 0; i < k; ++i) {
            if (word[i] < att->word[i]) {
                //the left tree is analyzed
                insert_dictionary_node(att->next_left, att, word, 0);
                i = k;
            } else if (word[i] > att->word[i]) {
                //the right tree is analyzed
                insert_dictionary_node(att->next_right, att, word, 1);
                i = k;
            }
        }
    }
}

/**
 * The method creates a new dictionary node
 *
 * @param word is the string that will be saved into the node
 * @return the node
 */
struct Dictionary_Node *create_dictionary_node(char *word) {
    struct Dictionary_Node *new_node = malloc(sizeof(struct Dictionary_Node));
    new_node->word = malloc(sizeof(char) * strlen(word));
    new_node->word = word;
    new_node->father = NULL;
    new_node->next_left = NULL;
    new_node->next_right = NULL;

    return new_node;
}

/**
 * The method prints the dictionary in the "in order" way
 */
void print_dictionary(struct Dictionary_Node *node) {
    if (node->next_left != NULL) {
        print_dictionary(node->next_left);
    }
    printf("%s\n", node->word);
    if (node->next_right != NULL) {
        print_dictionary(node->next_right);
    }
}

/**
 * The method is launched when a new game starts.
 * Are asked: the reference word, the number of attempts to try to guess the reference word, and the effective attempts
 */
void new_game() {
    char *ref = malloc(k);

    //asking for the reference word
    printf("\nInsert the reference word, the one that has to be guessed: ");
    scanf("%s", ref);

    initialize_possible_words_list();

    int attempts;

    //asking for the number of attempts
    printf("\nInsert the number of attempts: ");
    scanf("%d", &attempts);

    printf("\n");

    //asking for the words to guess the word
    for (int i = 0; i < attempts; ++i) {
        char *word = malloc(k);
        char *result = malloc(k);

        printf("Try to guess the word (tentative n. %d): ", i + 1);
        scanf("%s", word);

        short int r = compute_res(ref, word, result);

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
    }

    free(ref);

    remove_all_possible_words();
}

/**
 * TODO +stampa_filtrate
 */
void initialize_possible_words_list() {

}

/**
 * /TODO complete the output
 *
 * The method generates the output string depending on the string r and p received as params
 *
 * @param r is the reference word that has to be guessed
 * @param p is the word that tries to guess r
 * @param res is the resulting string
 * @return 1 if the word has been guessed, 0 if not, 2 if the word inserted is not in the dictionary
 */
short int compute_res(char *r, char *p, char res[]) {
    if (!check_if_belongs_to_the_dictionary(p)) {
        return 2;
    } else {
        int i;
        short int result = 1;

        char r_ordered[k];
        char p_ordered[k];
        strcpy(r_ordered, r);
        strcpy(p_ordered, p);

        quicksort(r_ordered, 0, k - 2);
        quicksort(p_ordered, 0, k - 2);

        int r_index = 0;
        int p_index = 0;

        int occur_in_r;

        for (p_index = 0; p_index < k; ++p_index) {
            occur_in_r = 0;
            if (!(p_index != 0 && p_ordered[p_index] == p_ordered[p_index - 1])) {
                for (; r_ordered[r_index] == p_ordered[p_index]; ++r_index) {
                    ++occur_in_r;
                }

                //TODO add_new_char_to_RB_tree(p_ordered[p_index], occur_in_r);
            }
        }

        for (i = 0; i < k - 1; ++i) {
            if (r[i] == p[i]) {
                res[i] = '+';
                //TODO increment_correct_position_count(p[i]);
            } else {
                res[i] = '*';
                result = 0;
            }
        }

        for (i = 0; i < k - 1; ++i) {
            if (res[i] == '*') {
                int count = 0;
                for (int j = i - 1; j >= 0; --j) {
                    if (p[j] == p[i] && p[j] != r[j]) {
                        ++count;
                    }
                }

                int diff; //TODO = get_ni_minus_ci(p[i]);

                if (count >= diff) {
                    res[i] = '/';
                } else {
                    res[i] = '|';
                }
            }
        }

        res[i] = '\0';

        return result;
    }
}

/**
 * The method checks if the param word belongs to the list that represent the dictionary or not
 *
 * @param word is the word inserted by user in input to try to guess the reference word
 * @return 1 if the word belongs to the dictionary, 0 if not
 */
short int check_if_belongs_to_the_dictionary(char *word) {
    return search_a_dictionary_node(dictionary_head, word);
}

/**
 * The method searches a word in the dictionary and tells to the caller if it is in it or not
 *
 * @param node is the actual node is being analyzed to see if the word is saved here or not
 * @param word is the work is being looked for
 * @return 1 if the word is in the dictionary, 0 if not
 */
short int search_a_dictionary_node(struct Dictionary_Node *node, char *word) {
    if (node != NULL) {
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
 * TODO doc
 *
 * @param A
 * @param lo
 * @param hi
 */
void quicksort(char *A, int lo, int hi) {
    if (lo < hi) {
        int p = partition_Hoare(A, lo, hi);
        quicksort(A, lo, p - 1);
        quicksort(A, p + 1, hi);
    }
}

/**
 * TODO doc
 *
 * @param A
 * @param lo
 * @param hi
 * @return
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
 * The method creates a new char node
 *
 * @param c is the char that will be saved into the char node
 * @return the node
 */
struct Char_Node *create_char_node(char c) {
    struct Char_Node *new_node = malloc(sizeof(struct Char_Node));
    new_node->key = c;
    new_node->in_the_correct_position_in_p = 0;
    new_node->occurrences_in_r = 0;
    new_node->father = NULL;
    new_node->next_left = NULL;
    new_node->next_right = NULL;

    return new_node;
}

/**
 * TODO +stampa_filtrate
 */
void remove_all_possible_words() {

}

/**
 * The method is called when previous game ended and the program needs to know if a new one has to be started or not
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
 * The method manages the input by the player, that can choose to start a new game, to close the program, or to
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
 * The method removes recursively all the words saved in the dictionary
 */
void remove_all_dictionary_words(struct Dictionary_Node *node) {
    if (node->next_left != NULL) {
        remove_all_dictionary_words(node->next_left);
    }
    if (node->next_right != NULL) {
        remove_all_dictionary_words(node->next_right);
    }
    free(node);
}

/**
 *
 * @param c
 * @return
 */
struct Char_Node *create_new_char_node(char c) {
    struct Char_Node *new_node = malloc(sizeof(struct Char_Node));
    new_node->key = c;
    new_node->color = 'R';
    new_node->occurrences_in_r = 0;
    new_node->in_the_correct_position_in_p = 0;
    new_node->father = NULL;
    new_node->next_left = nil;
    new_node->next_right = nil;
}

/**
 *
 * @param new_node
 */
void insert_new_char_node(struct Char_Node *new_node) {
    struct Char_Node *prev = NULL;
    struct Char_Node *att = word_characters;

    if (word_characters == NULL) {
        word_characters = new_node;
        word_characters->color = 'B';
    } else {
        while (att != nil) {
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

        insert_fix(new_node);
    }
}

/**
 *
 * @param x
 */
void left_rotation(struct Char_Node *x) {
    struct Char_Node *y = x->next_right;

    if (y->next_left != nil) {
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
 *
 * @param y
 */
void right_rotation(struct Char_Node *y) {
    struct Char_Node *x = y->next_left;

    if (x->next_right != nil) {
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
 *
 * @param node
 */
void insert_fix(struct Char_Node *node) {
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
                    left_rotation(node);
                }

                node->father->color = 'B';
                node->father->father->color = 'R';
                right_rotation(node->father->father);
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
                    right_rotation(node);
                }

                node->father->color = 'B';
                node->father->father->color = 'R';
                left_rotation(node->father->father);
            }
        }
    }

    word_characters->color = 'B';
}

/**
 *
 * @param node
 */
void in_order_print_char_nodes(struct Char_Node *node) {
    if (node->next_left != nil) {
        in_order_print_char_nodes(node->next_left);
    }
    printf("%c, %c\n", node->key, node->color);
    if (node->next_right != nil) {
        in_order_print_char_nodes(node->next_right);
    }
}

void initialize_nil() {
    nil = malloc(sizeof(struct Char_Node));

    nil->key;
    nil->color = 'B';
    nil->father = NULL;
    nil->next_left = NULL;
    nil->next_right = NULL;
}