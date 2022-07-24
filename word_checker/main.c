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

void new_game();

//TODO has to been modified to correctly elaborate res
int compute_res(char r[], char p[], char res[]);

int start_new_game();

int manage_input_new_game(int *p);

int main() {
    int program_is_up;

    printf("Welcome to WordChecker game. Follow initial instructions and then start playing.\n"
           "To start a new game insert +nuova_partita, followed by the word that has to be guessed,\n"
           "the number of attempts you have to guess it and then you can insert the attempts, \n"
           "each of them followed by the correspondent output\n\n");

    //reading length of the words and the words that will form the initial dictionary
    read_k();
    get_the_starting_dictionary();

    //printing actual dictionary
    print_dictionary();

    char* ins = malloc(15);

    do {
        printf("Start a new game inserting +nuova_partita\n");
        scanf("%s", ins);
    } while (strcmp(ins, "+nuova_partita") != 0);

    free(ins);

    do {
        new_game();
        program_is_up = start_new_game();
    } while (program_is_up);
}

void read_k() {
    int ins;
    printf("Insert the length of the words: ");
    scanf("%d", &ins);
    k = ins + 1;
}

void get_the_starting_dictionary() {
    insert_words_to_the_dictionary();
}

void insert_words_to_the_dictionary() {
    int check = 1;

    while (check) {
        check = create_node();
        printf("Dictionary head: %s\n", dictionary_head->word);
    }
}

int create_node() {
    char *insertion = malloc(sizeof(char) * k);
    printf("Insert a new word to the dictionary. Insert +inserisci_fine instead if you do not want to insert any other word: ");
    scanf("%s", insertion);

    if (strcmp(insertion, "+inserisci_fine") != 0) {
        Node *new_node = malloc(sizeof(Node));
        new_node->word = malloc(sizeof(char) * strlen(insertion));
        new_node->word = insertion;
        new_node->next = NULL;

        if (dictionary_head == NULL) {
            dictionary_head = new_node;
        }
        else {
            Node *act = dictionary_head;
            Node *prev = NULL;
            int has_to_be_ordered = 1;

            while (act != NULL && has_to_be_ordered) {
                for (int i = 0; i < k; ++i) {
                    if (new_node->word[i] < act->word[i]) {
                        new_node->next = act;
                        if (prev == NULL) {
                            dictionary_head = new_node;
                        } else {
                            prev->next = new_node;
                        }
                        i = k;
                        has_to_be_ordered = 0;
                    }
                    else if (new_node->word[i] > act->word[i]) {
                        i = k;
                    }
                }

                prev = act;
                act = act->next;
            }

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

void print_dictionary() {
    Node *tmp = dictionary_head;

    printf("\nPrinting the dictionary:\n");

    do {
        printf("%s\n", tmp->word);
        tmp = tmp->next;
    } while (tmp != NULL);

    printf("\n");
}

void new_game() {
    int attempts;

    printf("Insert the number of attempts: ");
    scanf("%d", &attempts);

    char *ref = malloc(k);

    printf("Insert the reference word, the one that has to be guessed: ");
    scanf("%s", ref);

    for (int i = 0; i < attempts; ++i) {
        char *word = malloc(k);
        char *result = malloc(k);

        printf("Try to guess the word (tentative n. %d): ", i + 1);
        scanf("%s", word);

        compute_res(ref, word, result);

        printf("Output: %s\n\n", result);

        free(word);
        free(result);
    }
}

/**
 * The method generates the output string depending on the string r and p received as params
 *
 * @param r is the reference word that has to be guessed
 * @param p is the word that tries to guess r
 * @param res is the resulting string
 * @return 1 if the word has been guessed, 0 if not
 */
int compute_res(char* r, char* p, char res[]) {
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
