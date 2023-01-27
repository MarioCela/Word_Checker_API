#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

#define ALPHABET_SIZE 64

int k;

struct dictionary_node {
    char *word;
    char color;
    struct dictionary_node *father;
    struct dictionary_node *next_left;
    struct dictionary_node *next_right;
};

struct list_node {
    char *word;
    struct list_node *next;
};

struct dictionary_node *dictionary_head = NULL;
struct dictionary_node *dictionary_node_nil = NULL;
struct list_node *list_head = NULL;
struct list_node *list_tail = NULL;

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

struct dictionary_node *create_new_dictionary_node(char *word) {
    struct dictionary_node *new_node = malloc(sizeof(struct dictionary_node));

    new_node->word = malloc(sizeof(char) * (k + 1));
    strcpy(new_node->word, word);
    new_node->color = 'R';
    new_node->father = NULL;
    new_node->next_left = dictionary_node_nil;
    new_node->next_right = dictionary_node_nil;

    return new_node;
}

void left_rotation(struct dictionary_node *x) {
    struct dictionary_node *y = x->next_right;

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

void right_rotation(struct dictionary_node *y) {
    struct dictionary_node *x = y->next_left;

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

void insert_dictionary_node(struct dictionary_node *head, struct dictionary_node *new_node) {
    struct dictionary_node *prev = NULL;
    struct dictionary_node *curr = head;
    char left_or_right = 'R';

    if (dictionary_head == NULL) {
        dictionary_head = new_node;
        dictionary_head->color = 'B';
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
            new_node->father = prev;
        } else {
            prev->next_right = new_node;
            new_node->father = prev;
        }

        while (new_node->father != NULL && new_node->father->color == 'R') {
            if (new_node->father == new_node->father->father->next_left) {
                struct dictionary_node *y = new_node->father->father->next_right;

                if (y->color == 'R') {
                    new_node->father->color = 'B';
                    y->color = 'B';
                    new_node->father->father->color = 'R';
                    new_node = new_node->father->father;
                } else {
                    if (new_node == new_node->father->next_right) {
                        new_node = new_node->father;
                        left_rotation(new_node);
                    }

                    new_node->father->color = 'B';
                    new_node->father->father->color = 'R';
                    right_rotation(new_node->father->father);
                }

            } else {
                struct dictionary_node *y = new_node->father->father->next_left;

                if (y->color == 'R') {
                    new_node->father->color = 'B';
                    y->color = 'B';
                    new_node->father->father->color = 'R';
                    new_node = new_node->father->father;
                } else {
                    if (new_node == new_node->father->next_left) {
                        new_node = new_node->father;
                        right_rotation(new_node);
                    }

                    new_node->father->color = 'B';
                    new_node->father->father->color = 'R';
                    left_rotation(new_node->father->father);
                }
            }
        }

        dictionary_head->color = 'B';
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

struct list_node *create_new_list_node(char *word) {
    struct list_node *new_node = malloc(sizeof(struct list_node));

    new_node->word = malloc(sizeof(char) * (k + 1));
    strcpy(new_node->word, word);
    new_node->next = NULL;

    return new_node;
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

void print_list(struct list_node *head) {
    struct list_node *curr = head;

    while (curr != NULL) {
        printf("%s\n", curr->word);
        curr = curr->next;
    }
}

void free_list(struct list_node *head) {
    struct list_node *curr = head;

    while (curr != NULL) {
        struct list_node *next = curr->next;

        free(curr->word);
        curr->word = NULL;

        free(curr);
        curr = NULL;

        curr = next;
    }

    list_head = NULL;
    list_tail = NULL;
}

void copy_dictionary_to_list(struct dictionary_node *root) {
    struct dictionary_node *node = root;

    if (node->next_left != dictionary_node_nil)
        copy_dictionary_to_list(node->next_left);

    insert_list_node_on_tail(create_new_list_node(node->word));

    if (node->next_right != dictionary_node_nil)
        copy_dictionary_to_list(node->next_right);
}

void quicksort(char *A, int lo, int hi) {
    if (lo < hi) {
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
        int p = i + 1;

        quicksort(A, lo, p - 1);
        quicksort(A, p + 1, hi);
    }
}

void filter(const char *p, const char *res, short int mae[][2], int *pointer) {
    struct list_node *prev = NULL;
    struct list_node *curr = list_head;

    do {
        bool removed = false;

        for (int i = 0; i < k; ++i) {
            if ((res[i] == '+' && p[i] != curr->word[i]) ||
                ((res[i] == '|' || res[i] == '/') && p[i] == curr->word[i])) {
                if (prev == NULL) {
                    list_head = curr->next;

                    free(curr->word);
                    curr->word = NULL;
                    curr->next = NULL;
                    free(curr);

                    curr = list_head;
                } else {
                    prev->next = curr->next;

                    free(curr->word);
                    curr->word = NULL;
                    curr->next = NULL;
                    free(curr);

                    curr = prev->next;
                }

                i = k;
                removed = true;
            }
        }
        if (!removed) {
            char ordered_word[k];
            strcpy(ordered_word, curr->word);
            quicksort(ordered_word, 0, k - 1);

            short int counts[ALPHABET_SIZE];
            for (int i = 0; i < ALPHABET_SIZE; ++i) {
                counts[i] = 0;
            }

            for (int i = 0; i < k; ++i) {
                short int count = 1;

                while (i != 4 && ordered_word[i + 1] == ordered_word[i]) {
                    ++count;
                    ++i;
                }

                counts[index_of_character(ordered_word[i])] = count;
            }

            for (int i = 0; i < ALPHABET_SIZE; ++i) {
                if (mae[i][1] != -1) {
                    if (counts[i] != mae[i][1]) {
                        removed = true;
                        break;
                    }
                } else {
                    if (mae[i][0] != -1 && counts[i] < mae[i][0]) {
                        removed = true;
                        break;
                    }
                }
            }

            if (removed) {
                if (prev == NULL) {
                    list_head = curr->next;

                    free(curr->word);
                    curr->word = NULL;
                    curr->next = NULL;
                    free(curr);

                    curr = list_head;
                } else {
                    prev->next = curr->next;

                    free(curr->word);
                    curr->word = NULL;
                    curr->next = NULL;
                    free(curr);

                    curr = prev->next;
                }
            } else {
                prev = curr;
                curr = curr->next;
                (*pointer)++;
            }
        }
    } while (curr != NULL);
}

short int
compute_res(char *r, char *p, char res[], char yon[][k], short int mae[][2], short int oar[][2], int *pointer) {
    struct dictionary_node *node = dictionary_head;

    bool go_on = false;

    while (node != dictionary_node_nil) {
        if (!strcmp(node->word, p)) {
            go_on = true;
            break;
        }

        for (int i = 0; i < k; ++i) {
            if (p[i] < node->word[i]) {
                node = node->next_left;
                i = k;
            } else if (p[i] > node->word[i]) {
                node = node->next_right;
                i = k;
            }
        }
    }

    if (go_on) {
        short int mae_temp[ALPHABET_SIZE][2];
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            mae_temp[i][0] = -1;
            mae_temp[i][1] = -1;
        }

        int i = 0;

        short int result = 1;

        char r_ordered[k];
        char p_ordered[k];
        strcpy(r_ordered, r);
        strcpy(p_ordered, p);
        quicksort(r_ordered, 0, k - 1);
        quicksort(p_ordered, 0, k - 1);

        int r_index = 0;
        int p_index = 0;

        short int occur_in_r;

        for (p_index = 0; p_index < k; ++p_index) {

            occur_in_r = 0;

            int old_r_index = r_index;

            if (p_index == 0 || p_ordered[p_index] != p_ordered[p_index - 1]) {

                for (; r_ordered[r_index] != p_ordered[p_index] && r_index < k; ++r_index) {

                }

                if (r_ordered[r_index] != p_ordered[p_index]) {
                    r_index = old_r_index;
                } else {
                    for (; r_ordered[r_index] == p_ordered[p_index] && r_index < k; ++r_index) {
                        ++occur_in_r;
                    }
                }

                oar[index_of_character(p_ordered[p_index])][0] = occur_in_r;
            }
        }

        int done = 0;

        for (i = 0; i < k; ++i) {
            if (r[i] == p[i]) {
                res[i] = '+';

                ++done;
                ++oar[index_of_character(p[i])][1];

                if (mae_temp[index_of_character(p[i])][0] == -1)
                    mae_temp[index_of_character(p[i])][0] = 1;
                else
                    ++mae_temp[index_of_character(p[i])][0];

                for (int j = 0; j < k; ++j) {
                    yon[index_of_character(p[i])][i] = 'n';
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

        for (i = 0; i < k; ++i) {
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
        }

        for (int j = 0; j < k; ++j) {
            if (mae_temp[index_of_character(p[j])][0] > mae[index_of_character(p[j])][0]) {
                mae[index_of_character(p[j])][0] = mae_temp[index_of_character(p[j])][0];
            }

            if (mae_temp[index_of_character(p[j])][1] > mae[index_of_character(p[j])][1]) {
                mae[index_of_character(p[j])][1] = mae_temp[index_of_character(p[j])][1];
            }
        }

        filter(p, res, mae, pointer);

        res[i] = '\0';

        return result;
    } else {
        return 2;
    }
}

void new_game(char *r, int tries) {
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

    copy_dictionary_to_list(dictionary_head);

    char *p = NULL;
    char *res = malloc(sizeof(char) * (k + 1));

    short int result = 0;

    for (int i = 0; i < tries; ++i) {
        if (scanf("%ms", &p) != 1)
            return;

        if (!strcmp(p, "+inserisci_inizio")) {
            free(p);
            p = NULL;

            if (scanf("%ms", &p) != 1)
                return;

            while (strcmp(p, "+inserisci_fine")) {
                insert_dictionary_node(dictionary_head, create_new_dictionary_node(p));

                bool insert_to_list = true;

                for (int j = 0; j < k; ++j) {
                    if (yon[index_of_character(p[j])][j] == 'n') {
                        insert_to_list = false;
                        break;
                    }
                }

                if (insert_to_list) {
                    char ordered_word[k];
                    strcpy(ordered_word, p);
                    quicksort(ordered_word, 0, k - 1);

                    short int counts[ALPHABET_SIZE];

                    for (int j = 0; j < ALPHABET_SIZE; ++j) {
                        counts[j] = 0;
                    }

                    for (int j = 0; j < k; ++j) {
                        short int count = 1;
                        while (j != 4 && ordered_word[j + 1] == ordered_word[j]) {
                            ++count;
                            ++j;
                        }
                        counts[index_of_character(ordered_word[j])] = count;
                    }

                    for (int j = 0; j < ALPHABET_SIZE; ++j) {
                        if (mae[j][1] != -1) {
                            if (counts[j] != mae[j][1]) {
                                insert_to_list = false;
                                break;
                            }
                        } else {
                            if (mae[j][0] != -1 && counts[j] < mae[j][0]) {
                                insert_to_list = false;
                                break;
                            }
                        }
                    }
                }
                if (insert_to_list) {
                    struct list_node *prev = NULL;
                    struct list_node *curr = list_head;

                    while (curr != NULL && strcmp(curr->word, p) < 0) {
                        prev = curr;
                        curr = curr->next;
                    }

                    struct list_node *new_node = create_new_list_node(p);

                    if (prev == NULL) {
                        new_node->next = list_head;
                        list_head = new_node;
                    } else {
                        new_node->next = curr;
                        prev->next = new_node;
                    }
                }

                free(p);
                p = NULL;

                if (scanf("%ms", &p) != 1)
                    return;
            }

            i--;

            free(p);
            p = NULL;
        } else if (!strcmp(p, "+stampa_filtrate")) {
            print_list(list_head);

            i--;

            free(p);
            p = NULL;
        } else {
            short int oar[ALPHABET_SIZE][2];
            for (int j = 0; j < ALPHABET_SIZE; ++j) {
                oar[j][0] = 0;
                oar[j][1] = 0;
            }

            int count = 0;
            int *pointer = &count;

            result = compute_res(r, p, res, yon, mae, oar, pointer);

            switch (result) {
                case 0:
                    printf("%s\n%d\n", res, count);
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

            pointer = NULL;

            free(p);
            p = NULL;
        }
    }
    if (result != 1) {
        printf("ko\n");
    }

    free(res);
    res = NULL;
}

int main() {
    dictionary_node_nil = malloc(sizeof(struct dictionary_node));

    dictionary_node_nil->color = 'B';
    dictionary_node_nil->father = NULL;
    dictionary_node_nil->next_left = NULL;
    dictionary_node_nil->next_right = NULL;

    if (scanf("%d", &k) != 1)
        return 0;

    char *r = NULL;
    bool first_time = true;

    do {
        if (!first_time) {
            free(r);
            r = NULL;
        } else
            first_time = false;

        if (scanf("%ms", &r) != 1)
            return 0;

        if (strcmp(r, "+nuova_partita") != 0)
            insert_dictionary_node(dictionary_head, create_new_dictionary_node(r));

    } while (strcmp(r, "+nuova_partita") != 0);

    free(r);
    r = NULL;

    if (scanf("%ms", &r) != 1)
        return 0;

    int tries = 0;
    if (scanf("%d", &tries) != 1)
        return 0;

    new_game(r, tries);

    bool exit = false;

    free(r);
    r = NULL;

    do {
        if (!feof(stdin)) {
            if (scanf("%ms", &r) != 1)
                return 0;

            if (!strcmp(r, "+inserisci_inizio")) {
                free(r);
                r = NULL;

                if (scanf("%ms", &r) != 1)
                    return 0;

                while (strcmp(r, "+inserisci_fine")) {
                    insert_dictionary_node(dictionary_head, create_new_dictionary_node(r));

                    free(r);
                    r = NULL;

                    if (scanf("%ms", &r) != 1)
                        return 0;
                }
            } else if (!strcmp(r, "+nuova_partita")) {
                free(r);
                r = NULL;

                free_list(list_head);

                if (scanf("%ms", &r) != 1)
                    return 0;

                if (scanf("%d", &tries) != 1)
                    return 0;

                new_game(r, tries);

                free(r);
                r = NULL;
            }
        } else
            exit = true;
    } while (!exit);

    free(r);
    r = NULL;

    free_list(list_head);
    list_head = NULL;

    remove_all_dictionary_words(dictionary_head);
    free(dictionary_node_nil);
    dictionary_head = NULL;
    dictionary_node_nil = NULL;
}
