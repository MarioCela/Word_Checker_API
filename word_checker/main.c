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

int main() {
    int program_is_up = 1;

    printf("Welcome to WordChecker game. Follow initial instructions and then start playing.\n"
           "To start a new game insert +nuova_partita, followed by the word that has to be guessed,\n"
           "the number of attempts you have to guess it and then you can insert the attempts, \n"
           "each of them followed by the correspondent output\n\n");

    //reading length of the words and the words that will form the initial dictionary
    read_k();
    get_the_starting_dictionary();

    //printing actual dictionary
    print_dictionary();

    //managing a game
    while (program_is_up) {
        char* ins = malloc(15);

        do {
            printf("Start a new game inserting +nuova_partita\n");
            scanf("%s", ins);
        } while (strcmp(ins, "+nuova_partita") != 0);

        free(ins);

        new_game();

        ins = malloc(15);

        printf("Game is over, to start a new game insert +nuova_partita, to close the program insert +game_over: ");
        scanf("%s", ins);

        if (strcmp(ins, "+game_over") == 0) {
            program_is_up = 0;
        }

        free(ins);
    }
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
            Node *prev;
            while (act != NULL) {
                prev = act;
                act = act->next;
            }
            act = malloc(sizeof(Node));
            act->word = malloc(sizeof(char) * strlen(insertion));
            strcpy(act->word, insertion);
            prev->next = act;

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
