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

//TODO has to been modified to correctly elaborate res
void compute_res(char r[], char p[], char res[]);

int main() {
    read_k();
    get_the_starting_dictionary();

    print_dictionary();

    char *reference = dictionary_head->word;
    char word[k];
    char result[k];

    printf("Insert second word: ");
    scanf("%s", word);

    compute_res(reference, word, result);

    printf("%s\n", reference);
    printf("%s\n", word);
    printf("%s\n", result);
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

void compute_res(char* r, char* p, char res[]) {
    int i;

    for (i = 0; i < k - 1; ++i) {
        if (r[i] == p[i]) {
            res[i] = '+';
        } else {
            res[i] = '-';
        }
    }

    res[i] = '\0';
}
