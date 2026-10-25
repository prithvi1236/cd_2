/* 2. FIRST and FOLLOW
   Input : n, then n productions, ONE ALTERNATIVE PER LINE, e.g.
               5
               E->TA
               A->+TA
               A->#          (# = epsilon)
               ...
   UPPERCASE = non-terminal, first production = start symbol. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int n, changed;
int  L[20];                     /* production i is   L[i] -> R[i]   */
char R[20][20];
char F[128][20], Fo[128][20];   /* FIRST and FOLLOW of each symbol, kept as strings */

void add(char *set, char c) {   /* put c in the set; remember if the set grew */
    int l = strlen(set);
    if (!strchr(set, c)) { set[l] = c; set[l + 1] = '\0'; changed = 1; }
}

void show(char *s) {            /* print a set like  { a, b, c } */
    printf("{ ");
    for (; *s; s++) printf("%c%s", *s, s[1] ? ", " : "");
    printf(" }\n");
}

int main() {
    int i, j, k;
    char s[30], *p;
    int X, B;

    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%s", s);
        L[i] = s[0];
        strcpy(R[i], s + 3);            /* skip "E->" */
    }

    /* ---- FIRST : repeat until nothing changes ---- */
    do {
        changed = 0;
        for (i = 0; i < n; i++) {
            for (j = 0; R[i][j]; j++) {
                X = R[i][j];
                if (!isupper(X)) { add(F[L[i]], X); break; }      /* terminal or # */
                for (p = F[X]; *p; p++)
                    if (*p != '#') add(F[L[i]], *p);              /* FIRST(X) minus # */
                if (!strchr(F[X], '#')) break;                    /* X can't vanish: stop */
            }
            if (R[i][j] == '\0') add(F[L[i]], '#');               /* every symbol can vanish */
        }
    } while (changed);

    /* ---- FOLLOW : repeat until nothing changes ---- */
    add(Fo[L[0]], '$');                                           /* rule 1 */
    do {
        changed = 0;
        for (i = 0; i < n; i++)
            for (j = 0; R[i][j]; j++) {
                B = R[i][j];
                if (!isupper(B)) continue;
                for (k = j + 1; R[i][k]; k++) {                   /* rule 2: what comes after B */
                    X = R[i][k];
                    if (!isupper(X)) { add(Fo[B], X); break; }
                    for (p = F[X]; *p; p++)
                        if (*p != '#') add(Fo[B], *p);
                    if (!strchr(F[X], '#')) break;
                }
                if (R[i][k] == '\0')                              /* rule 3: B is at the end */
                    for (p = Fo[L[i]]; *p; p++) add(Fo[B], *p);
            }
    } while (changed);

    printf("\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < i; j++) if (L[j] == L[i]) break;
        if (j < i) continue;                                      /* already printed */
        printf("FIRST(%c)  = ", L[i]);
        show(F[L[i]]);
    }
    printf("\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < i; j++) if (L[j] == L[i]) break;
        if (j < i) continue;
        printf("FOLLOW(%c) = ", L[i]);
        show(Fo[L[i]]);
    }
    return 0;
}
