#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct {
    char *word;
    struct Node *next;
} Node;

int k;
Node *dictionary_head;

void read_k();

void get_the_dictionary();

void create_node(char insert[]);

void compute_res(char r[], char p[], char res[]);

int main() {
    read_k();
    get_the_dictionary();

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

void get_the_dictionary() {
    char *insertion = malloc(sizeof(char) * k);

    printf("Insert a new word to the dictionary. Insert /quit instead if you do not want to insert any other word: ");
    scanf("%s", insertion);

    char quit[6];
    strcpy(quit, "/quit");

    while (strcmp(insertion, quit) != 0) {
        create_node(insertion);
        printf("Insert a new word to the dictionary. Insert /quit instead if you do not want to insert any other word: ");
        scanf("%s", insertion);
    }
}

void create_node(char insert[]) {
    if (dictionary_head == NULL) {
        dictionary_head = malloc(sizeof(Node));
        dictionary_head->word = malloc(sizeof(char) * strlen(insert));
        strcpy(dictionary_head->word, insert);
        dictionary_head->next = NULL;
    }
    else {
        Node *tmp = dictionary_head;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = malloc(sizeof(Node));
        tmp->word = malloc(sizeof(char) * strlen(insert));
        strcpy(tmp->word, insert);
        tmp->next = NULL;
    }
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
