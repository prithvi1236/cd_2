/* 5. INTERMEDIATE CODE GENERATION  (three-address code + quadruples)
   Input: an assignment with single-letter operands, e.g.   a=b+c*d
       E -> T { (+|-) T }      T -> F { (*|/) F }      F -> ( E ) | letter
   Every function returns, in r, the name that holds its value. */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char *p;                                   /* current input symbol */
int n = 0;                                 /* temporaries used so far */
char O[20], A[20][8], B[20][8], R[20][8];  /* quadruple i :  O  A  B  R */

void E(char *r);

void bad() { printf("Invalid expression\n"); exit(0); }

void gen(char *r, char op, char *b) {      /* emit   t = r op b   then r := t */
    n++;
    O[n] = op;
    strcpy(A[n], r);
    strcpy(B[n], b);
    sprintf(R[n], "t%d", n);
    strcpy(r, R[n]);
}

void F(char *r) {
    if (*p == '(') { p++; E(r); if (*p != ')') bad(); p++; }
    else if (isalnum(*p)) { r[0] = *p++; r[1] = '\0'; }
    else bad();
}

void T(char *r) {
    char b[8], op;
    F(r);
    while (*p == '*' || *p == '/') { op = *p++; F(b); gen(r, op, b); }
}

void E(char *r) {
    char b[8], op;
    T(r);
    while (*p == '+' || *p == '-') { op = *p++; T(b); gen(r, op, b); }
}

int main() {
    char in[50], r[8];
    int i;
    printf("Enter assignment (e.g. a=b+c*d): ");
    scanf("%s", in);
    p = in + 2;                            /* skip "a=" */
    E(r);
    if (*p != '\0') bad();

    printf("\nThree-address code:\n");
    for (i = 1; i <= n; i++) printf("%s = %s %c %s\n", R[i], A[i], O[i], B[i]);
    printf("%c = %s\n", in[0], r);

    printf("\nQuadruples:\n%-4s %-5s %-5s %s\n", "op", "arg1", "arg2", "result");
    for (i = 1; i <= n; i++) printf("%-4c %-5s %-5s %s\n", O[i], A[i], B[i], R[i]);
    printf("%-4c %-5s %-5s %c\n", '=', r, "-", in[0]);
    return 0;
}
