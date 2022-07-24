#include <stdio.h>
#include <string.h>
#include <malloc.h>

//structure for the nodes where dictionary's words will be saved
typedef struct {
    char *word;
    struct Node *next;
} Node;

//length of the words that will be inserted by input
int k;

//head of the list of nodes of the dictionary
Node *dictionary_head = NULL;

//method to read from input the value of the length of the words that will be inserted
void read_k();

//method to initialize the dictionary at the beginning of the program
void get_the_starting_dictionary();

//method to insert words to the dictionary
void insert_words_to_the_dictionary();

//method to create a new node of the list where a word will be saved
int create_node();

//method to print all the words saved in the list
void print_dictionary();

//method that manages a game
void new_game();

//TODO has to been modified to correctly elaborate res
int compute_res(char r[], char p[], char res[]);

//method launched every time that a new game could start after another one finishes
int start_new_game();

//method that defines the behavior depending on the user's input. Cases are to start a new game,
//to close the program and to add new words to the dictionary
int manage_input_new_game(int *p);

//method that checks if a word inserted by the user to try to guess the reference word belongs to the dictionary or not
int check_if_belongs_to_the_dictionary(char *word);

int main() {
    int program_is_up;

    printf("Welcome to WordChecker game. Follow initial instructions and then start playing.\n"
           "To start a new game insert +nuova_partita, followed by the word that has to be guessed,\n"
           "the number of attempts you have to guess it and then you can insert the attempts, \n"
           "each of them followed by the correspondent output\n\n");

    read_k();
    get_the_starting_dictionary();

    print_dictionary();

    //starting first game
    char* ins = malloc(15);

    do {
        printf("Start a new game inserting +nuova_partita\n");
        scanf("%s", ins);
    } while (strcmp(ins, "+nuova_partita") != 0);

    free(ins);

    //launching a game and then, when the latter finishes, asking the user if he wants to close the program, to launch
    //a new game or to insert new words to the dictionary
    do {
        new_game();
        program_is_up = start_new_game();
    } while (program_is_up);
}

/**
 * The method read from the user the length of the words
 */
void read_k() {
    int ins;

    printf("Insert the length of the words.\n"
           "Input: ");
    scanf("%d", &ins);

    printf("\n");

    k = ins + 1;
}

/**
 * The method is called when the program is run first and collects the words inserted by the user
 */
void get_the_starting_dictionary() {
    insert_words_to_the_dictionary();
}

/**
 * The method is called every time words have to be added to the dictionary
 */
void insert_words_to_the_dictionary() {
    int check = 1;

    while (check) {
        check = create_node();
    }
}

/**
 * The method creates a node in the list representing the dictionary if the user inserts a word and not the ending command.
 * Every word is added to the correct position so the list is already ordered
 *
 * @return 0 if the last input from user was +inserisci_fine, so there is no more need to ask for other words, 1 if not
 */
int create_node() {
    //getting the input
    char *insertion = malloc(sizeof(char) * k);
    printf("Insert a new word to the dictionary. Insert +inserisci_fine instead if you do not want to insert any other word: ");
    scanf("%s", insertion);

    if (strcmp(insertion, "+inserisci_fine") != 0) {
        //creating the new node where the inserted word will be saved
        Node *new_node = malloc(sizeof(Node));
        new_node->word = malloc(sizeof(char) * strlen(insertion));
        new_node->word = insertion;
        new_node->next = NULL;

        //if dictionary head is NULL it means that we are saving the first word of the dictionary
        if (dictionary_head == NULL) {
            dictionary_head = new_node;
        }
        else {
            Node *act = dictionary_head;
            Node *prev = NULL;
            int has_to_be_ordered = 1;

            //we start from the first node to control which is the correct position
            //of the new word to have the list still ordered
            while (act != NULL && has_to_be_ordered) {
                for (int i = 0; i < k; ++i) {
                    if (new_node->word[i] < act->word[i]) {
                        new_node->next = act;

                        //if prev is NULL it means that the new word will be the new head
                        if (prev == NULL) {
                            dictionary_head = new_node;
                        } else {
                            prev->next = new_node;
                        }
                        i = k;
                        has_to_be_ordered = 0;
                    }
                    //if this is not the correct position it is useless to check the remaining positions,
                    //so the for will end
                    else if (new_node->word[i] > act->word[i]) {
                        i = k;
                    }
                }

                //going on with the list
                prev = act;
                act = act->next;
            }

            //the condition will be true only if the new node will be the last node
            if (has_to_be_ordered) {
                act = malloc(sizeof(Node));
                act->word = malloc(sizeof(char) * strlen(insertion));
                strcpy(act->word, insertion);
                prev->next = act;
            }

            return 1;
        }
    }
    else {
        return 0;
    }
}

/**
 * The method prints the dictionary
 */
void print_dictionary() {
    Node *tmp = dictionary_head;

    printf("\nPrinting the dictionary:\n");

    do {
        printf("%s\n", tmp->word);
        tmp = tmp->next;
    } while (tmp != NULL);

    printf("\n");
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

        int r = compute_res(ref, word, result);

        if (r == 0) {
            printf("Output: %s\n\n", result);
        } else if (r == 1) {
            printf("Output: ok\n\n");
        } else {
            printf("Output: not exist\n\n");
        }

        free(word);
        free(result);
    }
}

/**
 * /TODO to be modified
 *
 * The method generates the output string depending on the string r and p received as params
 *
 * @param r is the reference word that has to be guessed
 * @param p is the word that tries to guess r
 * @param res is the resulting string
 * @return 1 if the word has been guessed, 0 if not, 2 if the word inserted is not in the dictionary
 */
int compute_res(char* r, char* p, char res[]) {
    if (!check_if_belongs_to_the_dictionary(p)) {
        return 2;
    } else {
        int i;
        int result = 1;

        for (i = 0; i < k - 1; ++i) {
            if (r[i] == p[i]) {
                res[i] = '+';
            } else {
                res[i] = '-';
                result = 0;
            }
        }

        res[i] = '\0';

        return result;
    }


}

/**
 * The method is called when previous game ended and the program needs to know if a new one has to be started or not
 * @return 1 if a new game has to be started, 0 if not
 */
int start_new_game() {
    int continue_asking_input = 1;
    int game_goes_on = 1;
    int *ptr = &game_goes_on;

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
int manage_input_new_game(int *p) {
    char *ins = malloc(18);

    printf("Start a new game inserting +nuova_partita;\n"
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
        insert_words_to_the_dictionary();
        free(ins);
        return 1;
    }
}

/**
 * The method checks if the param word belongs to the list that represent the dictionary or not
 *
 * @param word is the word inserted by user in input to try to guess the reference word
 * @return 1 if the word belongs to the dictionary, 0 if not
 */
int check_if_belongs_to_the_dictionary(char *word) {
    Node *tmp = dictionary_head;

    while(tmp != NULL) {
        if (strcmp(word, tmp->word) == 0) {
            return 1;
        }

        tmp = tmp->next;
    }

    return 0;
}
