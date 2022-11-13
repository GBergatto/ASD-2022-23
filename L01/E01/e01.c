#include <ctype.h>
#include <stdio.h>
#include <string.h>

char *cercaRegexp(char *src, char *r);
int lenRegex(char *regex);
void checkMatch(int condition, char **pp, int *pi, int *pj, char *address);
int contains(int len, char list[], char c);

enum mode { normal,
            slash,    // following \\ expecting A or a
            any,      // following .
            include,  // following [
            exclude,  // following [^
} typedef Mode;

int main(int argc, char const *argv[]) {
    char string[50] = "aaAfAbQAbcnRauAbcnRadSad";
    char re[50] = "A[^f]\\anR.d";

    int len = lenRegex(re);
    char *p = cercaRegexp(string, re);

    if (p != NULL) {
        printf("%.*s\n", len, p);
    } else {
        printf("Regex non trovata\n");
    }

    return 0;
}

// calcola il numero di char della stringa rappresentata dalla regex
int lenRegex(char *regex) {
    int i = 0, len = 0, insideBracket = 0;

    while (regex[i] != 0) {
        if ((isalpha(regex[i]) || regex[i] == '.') && insideBracket == 0) {
            len++;
        } else {
            if (regex[i] == '[') {
                insideBracket = 1;
            } else if (regex[i] == ']') {
                len++;
                insideBracket = 0;
            }
        }
        i++;
    }
    return len;
}

void checkMatch(int condition, char **pp, int *pi, int *pj, char *address) {
    if (condition) {
        if (*pp == NULL)
            *pp = address;
        (*pi)++;
        (*pj)++;
    } else {
        if (*pp == NULL)
            (*pj)++;
        else
            *pp = NULL;
        *pi = 0;
    }
}

int contains(int len, char list[], char c) {
    for (int i = 0; i < len; i++) {
        if (list[i] == c) {
            return 1;
        }
    }
    return 0;
}

char *cercaRegexp(char *src, char *r) {
    char *p = NULL;
    int i = 0, j = 0;
    char toInclude[30];
    char toExclude[30];
    int len = 0;  // len of toInclude/toExclude
    Mode mode = normal;

    char ri;
    char scrrj;

    while (src[j] != '\0') {
        if (i == strlen(r)) {
            printf("%p\n", p);
            return p;
        }

        if (isalpha(r[i])) {  // lettera
            switch (mode) {
                case normal:
                    checkMatch((r[i] == src[j]), &p, &i, &j, &src[j]);
                    break;
                case include:
                    if (contains(len, toInclude, r[i]) == 0) {
                        toInclude[len] = r[i];
                        len++;
                    }
                    i++;
                    break;
                case exclude:
                    if (contains(len, toExclude, r[i]) == 0) {
                        toExclude[len] = r[i];
                        len++;
                    }
                    i++;
                    break;
                case slash:
                    mode = normal;
                    if (r[i] == 'A') {  // match any upper letter
                        checkMatch(isupper(src[j]), &p, &i, &j, &src[j]);
                    } else if (r[i] == 'a') {  // match any upper letter
                        checkMatch(islower(src[j]), &p, &i, &j, &src[j]);
                    } else {
                        printf("Errore: '%c' dopo \\\n", r[i]);
                        return NULL;
                    }
                    break;
            }
        } else {  // metacarattere
            switch (r[i]) {
                case '[':
                    mode = include;
                    i++;
                    break;
                case '.':
                    checkMatch(isalpha(src[j]), &p, &i, &j, &src[j]);
                    break;
                case '^':
                    if (mode == include) {
                        mode = exclude;
                        i++;
                    } else {
                        printf("Errore: ^ non preceduto da [");
                        return NULL;
                    }
                    break;
                case ']':
                    if (mode == include) {
                        checkMatch(contains(len, toInclude, src[j]), &p, &i, &j, &src[j]);
                        mode = normal;
                        len = 0;
                    } else if (mode == exclude) {
                        checkMatch((contains(len, toExclude, src[j]) - 1), &p, &i, &j, &src[j]);
                        mode = normal;
                        len = 0;
                    } else {
                        printf("Errore: ] non preceduto da [");
                        return NULL;
                    }
                    break;
                case '\\':
                    mode = slash;
                    i++;
                    break;
                default: {
                    printf("Errore: carattere non valido '%c'\n", r[i]);
                    return NULL;
                }
            }
        }
    }
    // ho finito la sorgente senza match
    return NULL;
}