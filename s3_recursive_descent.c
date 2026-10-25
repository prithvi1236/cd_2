/* 3. RECURSIVE DESCENT PARSER (with trace)
       E  -> T E'          E' -> + T E' | epsilon
       T  -> F T'          T' -> * F T' | epsilon
       F  -> ( E ) | i
   Rule: one function per non-terminal.  Epsilon = do nothing.
   Each function prints the production it uses, in the order it is chosen. */
#include <stdio.h>

char *p;                 /* points at the current input symbol */
int err = 0;

void E(), Ep(), T(), Tp(), F();

void E()  { printf("E  -> T E'\n"); T(); Ep(); }
void Ep() {
    if (*p == '+') { printf("E' -> + T E'\n"); p++; T(); Ep(); }
    else printf("E' -> epsilon\n");
}
void T()  { printf("T  -> F T'\n"); F(); Tp(); }
void Tp() {
    if (*p == '*') { printf("T' -> * F T'\n"); p++; F(); Tp(); }
    else printf("T' -> epsilon\n");
}
void F() {
    if (*p == '(') {
        printf("F  -> ( E )\n");
        p++;
        E();
        if (*p == ')') p++; else err = 1;
    }
    else if (*p == 'i') { printf("F  -> i\n"); p++; }
    else err = 1;
}

int main() {
    char in[50];
    printf("Enter string (i for id): ");
    scanf("%s", in);
    p = in;
    printf("\n");
    E();                                    /* start symbol */
    if (*p == '\0' && !err) printf("\nValid\n");
    else                    printf("\nInvalid\n");
    return 0;
}
