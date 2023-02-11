#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

#define ALPHABET_SIZE 64

short int k;
short int buffer_size = 18;

struct dictionary_node {
    char *word;
    struct dictionary_node *next_left;
    struct dictionary_node *next_right;
};

struct list_node {
    struct dictionary_node *node;
    struct list_node *next;
};

struct dictionary_node *dictionary_head = NULL;
struct dictionary_node *dictionary_node_nil = NULL;
struct list_node *list_head = NULL;
struct list_node *list_mid = NULL;
struct list_node *list_tail = NULL;

//support functions
int index_of_character(char c) {
    switch (c) {
        case '-':
            return 0;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return c - '0' + 1;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
            return c - 'A' + 11;
        case '_':
            return 37;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
            return c - 'a' + 38;
        default:
            return -1;
    }
}
bool respect_constraints(char *word, char yon[][k], short int mae[][2]) {
    bool respects = true;

    for (int i = 0; i < k; ++i) {
        if (yon[index_of_character(word[i])][i] == 'n') {
            return false;
        }
    }

    if (respects) {
        short int counts[ALPHABET_SIZE];
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            counts[i] = 0;
        }

        for (int i = 0; i < k; ++i) {
            ++counts[index_of_character(word[i])];
            if (mae[index_of_character(word[i])][1] != -1) {
                if (counts[index_of_character(word[i])] > mae[index_of_character(word[i])][1]) {
                    return false;
                }
            }
        }

        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (mae[i][1] != -1) {
                if (counts[i] != mae[i][1]) {
                    respects = false;
                    break;
                }
            } else {
                if (mae[i][0] != -1 && counts[i] < mae[i][0]) {
                    respects = false;
                    break;
                }
            }
        }
    }

    return respects;
}

//management of the Red and Black Tree (dictionary of allowed words)
struct dictionary_node *create_new_dictionary_node(char *word) {
    struct dictionary_node *new_node = malloc(sizeof(struct dictionary_node));

    new_node->word = malloc(sizeof(char) * (k + 1));
    strncpy(new_node->word, word, k);
    new_node->word[k] = '\0';
    new_node->next_left = dictionary_node_nil;
    new_node->next_right = dictionary_node_nil;

    return new_node;
}
void insert_dictionary_node(struct dictionary_node *head, struct dictionary_node *new_node) {
    struct dictionary_node *prev = NULL;
    struct dictionary_node *curr = head;
    char left_or_right = 'R';

    if (dictionary_head == NULL) {
        dictionary_head = new_node;
    } else {
        while (curr != dictionary_node_nil) {
            for (int i = 0; i < k; ++i) {
                if (new_node->word[i] < curr->word[i]) {
                    prev = curr;
                    curr = curr->next_left;
                    left_or_right = 'L';
                    i = k;
                } else if (new_node->word[i] > curr->word[i]) {
                    prev = curr;
                    curr = curr->next_right;
                    left_or_right = 'R';
                    i = k;
                }
            }
        }

        if (left_or_right == 'L') {
            prev->next_left = new_node;
        } else {
            prev->next_right = new_node;
        }
    }
}
void print_dictionary(struct dictionary_node *node) {
    if (node->next_left != dictionary_node_nil) {
        print_dictionary(node->next_left);
    }

    printf("%s\n", node->word);

    if (node->next_right != dictionary_node_nil) {
        print_dictionary(node->next_right);
    }
}
void remove_all_dictionary_words(struct dictionary_node *node) {
    if (node->next_left != dictionary_node_nil) {
        remove_all_dictionary_words(node->next_left);
    }

    if (node->next_right != dictionary_node_nil) {
        remove_all_dictionary_words(node->next_right);
    }

    free(node->word);
    node->word = NULL;

    free(node);
    node = NULL;
}

//management of the linked list (set of filtered words)
struct list_node *create_new_list_node(struct dictionary_node *n) {
    struct list_node *new_node = malloc(sizeof(struct list_node));

    new_node->node = n;
    new_node->next = NULL;

    return new_node;
}
void insert_list_node(struct list_node *node) {
    if (list_head == NULL || strcmp(list_head->node->word, node->node->word) >= 0) {
        node->next = list_head;
        list_head = node;
        return;
    }

    struct list_node *curr = list_head;
    while (curr->next != NULL && strcmp(curr->next->node->word, node->node->word) < 0) {
        curr = curr->next;
    }

    node->next = curr->next;
    curr->next = node;
}
void insert_list_node_in_the_middle(struct list_node *node) {
    struct list_node *curr = list_mid;
    while (curr->next != NULL && strcmp(curr->next->node->word, node->node->word) < 0) {
        curr = curr->next;
    }

    node->next = curr->next;
    curr->next = node;

    if (curr->next == NULL)
        list_tail = curr;
}
void insert_list_node_on_tail(struct list_node *node) {
    if (list_head == NULL) {
        list_head = node;
        list_tail = node;
    } else {
        struct list_node *curr = list_tail;
        curr->next = node;
        list_tail = node;
    }
}
void copy_dictionary_to_list(struct dictionary_node *root, char yon[][k], short int mae[][2], int *lc) {
    if (root->next_left != dictionary_node_nil)
        copy_dictionary_to_list(root->next_left, yon, mae, lc);

    if (respect_constraints(root->word, yon, mae)) {
        insert_list_node_on_tail(create_new_list_node(root));
        ++(*lc);
    }

    if (root->next_right != dictionary_node_nil)
        copy_dictionary_to_list(root->next_right, yon, mae, lc);
}
void print_list(struct list_node *head) {
    struct list_node *curr = head;

    while (curr != NULL) {
        printf("%s\n", curr->node->word);
        curr = curr->next;
    }
}
void free_list(struct list_node *head) {
    struct list_node *curr = head;

    while (curr != NULL) {
        struct list_node *next = curr->next;

        curr->node = NULL;

        free(curr);
        curr = NULL;

        curr = next;
    }

    list_head = NULL;
    list_mid = NULL;
    list_tail = NULL;
}

//game functions
void filter(char yon[][k], short int mae[][2], int *lc) {
    struct list_node *prev = NULL;
    struct list_node *curr = list_head;

    do {
        if (!respect_constraints(curr->node->word, yon, mae)) {
            bool it_is_mid = false;

            if (curr == list_mid) {
                it_is_mid = true;
            }

            if (prev == NULL) {
                list_head = curr->next;

                curr->node = NULL;
                curr->next = NULL;
                free(curr);

                curr = list_head;
                if (it_is_mid)
                    list_mid = list_head;
            } else {
                if (curr == list_tail)
                    list_tail = prev;

                prev->next = curr->next;

                curr->node = NULL;
                curr->next = NULL;
                free(curr);

                curr = prev->next;

                if (it_is_mid) {
                    if (curr != NULL)
                        list_mid = curr->next;
                    else if (list_head->next != NULL)
                        list_mid = list_head->next;
                    else
                        list_mid = list_head;
                }
            }
            (*lc)--;
        } else {
            prev = curr;
            curr = curr->next;
        }
    } while (curr != NULL);
}
short int compute_res(char *r, char *p, char res[], char yon[][k], short int mae[][2], short int oar[][2]) {
    struct dictionary_node *node = dictionary_head;

    bool go_on = false;

    while (node != dictionary_node_nil) {
        if (!strcmp(node->word, p)) {
            go_on = true;
            break;
        }

        if (strcmp(p, node->word) < 0)
            node = node->next_left;
        else
            node = node->next_right;
    }

    if (go_on) {
        short int mae_temp[ALPHABET_SIZE][2];
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            mae_temp[i][0] = -1;
            mae_temp[i][1] = -1;
        }

        short int result = 1;

        for (int i = 0; i < k; ++i) {
            ++oar[index_of_character(r[i])][0];
        }

        short int done = 0;

        for (int i = 0; i < k; ++i) {
            if (r[i] == p[i]) {
                res[i] = '+';

                ++done;
                ++oar[index_of_character(p[i])][1];

                if (mae_temp[index_of_character(p[i])][0] == -1)
                    mae_temp[index_of_character(p[i])][0] = 1;
                else
                    ++mae_temp[index_of_character(p[i])][0];

                for (int j = 0; j < ALPHABET_SIZE; ++j) {
                    yon[j][i] = 'n';
                }

                yon[index_of_character(p[i])][i] = 'y';
            } else {
                yon[index_of_character(p[i])][i] = 'n';
                res[i] = '*';
                result = 0;
            }
        }
        if (done == k)
            return 1;

        for (int i = 0; i < k; ++i) {
            if (res[i] == '*') {
                int count = 0;

                for (int j = i - 1; j >= 0; --j) {
                    if (p[j] == p[i] && p[j] != r[j]) {
                        ++count;
                    }
                }

                int diff = oar[index_of_character(p[i])][0] - oar[index_of_character(p[i])][1];

                if (count >= diff) {
                    res[i] = '/';

                    if (oar[index_of_character(p[i])][0] == 0) {
                        for (int j = 0; j < k; ++j) {
                            yon[index_of_character(p[i])][j] = 'n';
                        }
                    }

                    if (mae_temp[index_of_character(p[i])][0] == -1) {
                        mae_temp[index_of_character(p[i])][1] = 0;
                        mae_temp[index_of_character(p[i])][0] = 0;
                    } else {
                        mae_temp[index_of_character(p[i])][1] = mae_temp[index_of_character(p[i])][0];
                    }
                } else {
                    res[i] = '|';

                    if (mae_temp[index_of_character(p[i])][0] == -1)
                        mae_temp[index_of_character(p[i])][0] = 1;
                    else
                        ++mae_temp[index_of_character(p[i])][0];
                }
            }
            if (mae_temp[index_of_character(p[i])][0] > mae[index_of_character(p[i])][0]) {
                mae[index_of_character(p[i])][0] = mae_temp[index_of_character(p[i])][0];
            }

            if (mae_temp[index_of_character(p[i])][1] > mae[index_of_character(p[i])][1]) {
                mae[index_of_character(p[i])][1] = mae_temp[index_of_character(p[i])][1];
            }
        }

        res[k] = '\0';

        return result;
    } else {
        return 2;
    }
}
void new_game(char *r, int tries, int *gc, int *lc) {
    char yon[ALPHABET_SIZE][k];
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        for (int j = 0; j < k; ++j) {
            yon[i][j] = 'd';
        }
    }

    short int mae[ALPHABET_SIZE][2];
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        mae[i][0] = -1;
        mae[i][1] = -1;
    }

    bool first_try = true;

    char p[buffer_size];
    char res[k + 1];

    short int result = 0;

    for (int i = 0; i < tries; ++i) {
        if (scanf("%s", p) != 1)
            return;

        if (!strncmp(p, "+inserisci_inizio", 17)) {

            if (scanf("%s", p) != 1)
                return;

            while (strncmp(p, "+inserisci_fine", 15)) {
                p[k] = '\0';
                struct dictionary_node *n = create_new_dictionary_node(p);
                insert_dictionary_node(dictionary_head, n);
                ++(*gc);

                if (!first_try) {
                    if (respect_constraints(n->word, yon, mae)) {
                        if (strcmp(n->word, list_mid->node->word) >= 0)
                            insert_list_node_in_the_middle(create_new_list_node(n));
                        else
                            insert_list_node(create_new_list_node(n));
                        ++(*lc);
                    }
                }

                if (scanf("%s", p) != 1)
                    return;
            }

            i--;

        } else if (!strncmp(p, "+stampa_filtrate", 16)) {
            if (first_try)
                print_dictionary(dictionary_head);
            else
                print_list(list_head);

            i--;

        } else {
            short int oar[ALPHABET_SIZE][2];
            for (int j = 0; j < ALPHABET_SIZE; ++j) {
                oar[j][0] = 0;
                oar[j][1] = 0;
            }

            char try[k + 1];
            strncpy(try, p, k);
            try[k] = '\0';

            result = compute_res(r, try, res, yon, mae, oar);

            if (!first_try) {
                if (result == 0) {
                    filter(yon, mae, lc);
                }
            } else {
                if (result == 0) {
                    copy_dictionary_to_list(dictionary_head, yon, mae, lc);
                    struct list_node *curr = list_head;
                    for (int j = 0; j < (*lc) / 2; ++j) {
                        curr = curr->next;
                    }
                    list_mid = curr;
                    first_try = false;
                }
            }

            switch (result) {
                case 0:
                    printf("%s\n%d\n", res, (*lc));
                    break;
                case 1:
                    printf("ok\n");
                    break;
                case 2:
                    printf("not_exists\n");
                    i--;
                    break;
                default:
                    break;
            }

            if (result == 1)
                i = tries;
        }
    }
    if (result != 1) {
        printf("ko\n");
    }
}
int main() {
    dictionary_node_nil = malloc(sizeof(struct dictionary_node));

    dictionary_node_nil->next_left = NULL;
    dictionary_node_nil->next_right = NULL;

    int global_count = 0;
    int *gc = &global_count;
    int local_count = 0;
    int *lc = &local_count;

    if (scanf("%hd", &k) != 1)
        return 0;

    if (k > 18)
        buffer_size = k + 1;

    char r[buffer_size];

    do {
        if (scanf("%s", r) != 1)
            return 0;

        if (!strncmp(r, "+stampa_filtrate", 16)) {
            if (dictionary_head != NULL)
                print_dictionary(dictionary_head);
        } else if (!strncmp(r, "+inserisci_inizio", 17)) {
            if (scanf("%s", r) != 1)
                return 0;

            while (strncmp(r, "+inserisci_fine", 15)) {
                r[k] = '\0';
                insert_dictionary_node(dictionary_head, create_new_dictionary_node(r));
                ++global_count;

                if (scanf("%s", r) != 1)
                    return 0;
            }
        } else if (strncmp(r, "+nuova_partita", 14) != 0) {
            r[k] = '\0';
            insert_dictionary_node(dictionary_head, create_new_dictionary_node(r));
            ++global_count;
        }

    } while (strncmp(r, "+nuova_partita", 14) != 0);

    bool go_on = true;

    do {
        if (scanf("%s", r) != 1)
            return 0;

        if (!strncmp(r, "+stampa_filtrate", 16)) {
            if (dictionary_head != NULL)
                print_dictionary(dictionary_head);
        } else if (!strncmp(r, "+inserisci_inizio", 17)) {
            if (scanf("%s", r) != 1)
                return 0;

            while (strncmp(r, "+inserisci_fine", 15)) {
                r[k] = '\0';
                insert_dictionary_node(dictionary_head, create_new_dictionary_node(r));
                ++global_count;

                if (scanf("%s", r) != 1)
                    return 0;
            }
        } else
            go_on = false;
    } while (go_on);

    char reference[k + 1];
    strncpy(reference, r, k);
    reference[k] = '\0';

    short int tries = 0;
    if (scanf("%hd", &tries) != 1)
        return 0;

    new_game(reference, tries, gc, lc);

    bool exit = false;

    do {
        if (!feof(stdin)) {
            if (scanf("%s", r) != 1)
                return 0;

            if (!strncmp(r, "+stampa_filtrate", 16)) {
                if (dictionary_head != NULL)
                    print_dictionary(dictionary_head);
            } else if (!strncmp(r, "+inserisci_inizio", 17)) {

                if (scanf("%s", r) != 1)
                    return 0;

                while (strncmp(r, "+inserisci_fine", 15)) {
                    r[k] = '\0';
                    insert_dictionary_node(dictionary_head, create_new_dictionary_node(r));
                    ++global_count;

                    if (scanf("%s", r) != 1)
                        return 0;
                }
            } else if (!strncmp(r, "+nuova_partita", 14)) {

                free_list(list_head);

                if (scanf("%s", r) != 1)
                    return 0;

                strncpy(reference, r, k);
                reference[k] = '\0';

                if (scanf("%hd", &tries) != 1)
                    return 0;

                local_count = 0;
                new_game(reference, tries, gc, lc);
            }
        } else
            exit = true;
    } while (!exit);

    free_list(list_head);
    list_head = NULL;

    remove_all_dictionary_words(dictionary_head);
    free(dictionary_node_nil);
    dictionary_head = NULL;
    dictionary_node_nil = NULL;

    gc = NULL;
    lc = NULL;
}
