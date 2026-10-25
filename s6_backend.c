/* 6. BACK END : three-address code  ->  8086 assembly
   Input : the number of statements, then the statements (one per line, words
           separated by spaces), for example:
               5
               t1 = c * d
               t2 = b + t1
               a = t2
               if a > 10 goto L1
               L1:
   Forms : x = y          x = y op z  (op is + - * /)
           L1:            goto L1       if x relop y goto L1  (< > <= >= == !=)
   Run   : ./a.out < code.txt > out.asm                                       */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum { LABEL, GOTO, IF, COPY, ARITH, BAD };

char L[100][60];                /* the statements */
char w[6][12];                  /* the words of the current statement */
char v[60][12];                 /* every variable and temporary */
int n, nv = 0;

int split(char *s) {            /* words of a statement go to w[0..5]; returns how many */
    int m = sscanf(s, "%11s %11s %11s %11s %11s %11s", w[0], w[1], w[2], w[3], w[4], w[5]);
    return m < 0 ? 0 : m;
}

char *jump(char *rel) {         /* conditional jump for a relational operator */
    if (!strcmp(rel, "<"))  return "JL";
    if (!strcmp(rel, ">"))  return "JG";
    if (!strcmp(rel, "<=")) return "JLE";
    if (!strcmp(rel, ">=")) return "JGE";
    if (!strcmp(rel, "==")) return "JE";
    if (!strcmp(rel, "!=")) return "JNE";
    return NULL;                /* not a relational operator */
}

int kind(int m) {               /* which of the five forms is this statement? */
    if (m == 1 && w[0][strlen(w[0]) - 1] == ':')   return LABEL;
    if (m == 2 && !strcmp(w[0], "goto"))           return GOTO;
    if (m == 6 && !strcmp(w[0], "if") && jump(w[2]) && !strcmp(w[4], "goto")) return IF;
    if (m == 3 && !strcmp(w[1], "="))              return COPY;
    if (m == 5 && !strcmp(w[1], "=") && strlen(w[3]) == 1 && strchr("+-*/", w[3][0]))
        return ARITH;
    return BAD;
}

void declare(char *s) {         /* remember a variable once; constants are skipped */
    int i;
    if (isdigit(s[0])) return;
    for (i = 0; i < nv; i++)
        if (strcmp(v[i], s) == 0) return;
    strcpy(v[nv++], s);
}

int main() {
    int i, k;
    char rest[60];

    /* steps 2-3: read the count and the statements */
    scanf("%d", &n);
    fgets(rest, sizeof rest, stdin);                       /* rest of the count line */
    for (i = 0; i < n; i++) {
        fgets(L[i], sizeof L[i], stdin);
        L[i][strcspn(L[i], "\n")] = '\0';
    }

    /* step 5: collect every variable and temporary */
    for (i = 0; i < n; i++) {
        k = kind(split(L[i]));
        if (k == BAD) {
            fprintf(stderr, "Statement %d not understood: %s\n", i + 1, L[i]);
            return 1;
        }
        if (k == IF)    { declare(w[1]); declare(w[3]); }
        if (k == COPY)  { declare(w[0]); declare(w[2]); }
        if (k == ARITH) { declare(w[0]); declare(w[2]); declare(w[4]); }
    }

    /* step 6: DATA SEGMENT, then CODE SEGMENT */
    printf("DATA SEGMENT\n");
    for (i = 0; i < nv; i++) printf("  %s DW 0\n", v[i]);
    printf("DATA ENDS\n\nCODE SEGMENT\nASSUME CS:CODE, DS:DATA\nSTART:\n");
    printf("  MOV AX, DATA\n  MOV DS, AX\n");

    /* step 4: translate each statement */
    for (i = 0; i < n; i++) {
        k = kind(split(L[i]));
        printf("  ; %s\n", L[i]);                          /* original statement as a comment */

        if (k == LABEL)                                    /* L:  */
            printf("%s\n", w[0]);
        else if (k == GOTO)                                /* goto L */
            printf("  JMP %s\n", w[1]);
        else if (k == IF)                                  /* if x relop y goto L */
            printf("  MOV AX, %s\n  CMP AX, %s\n  %s %s\n", w[1], w[3], jump(w[2]), w[5]);
        else {                                             /* x = y   or   x = y op z */
            printf("  MOV AX, %s\n", w[2]);
            if (k == ARITH) {
                if (w[3][0] == '+') printf("  ADD AX, %s\n", w[4]);
                if (w[3][0] == '-') printf("  SUB AX, %s\n", w[4]);
                if (w[3][0] == '*') printf("  MOV BX, %s\n  MUL BX\n", w[4]);
                if (w[3][0] == '/') printf("  MOV DX, 0\n  MOV BX, %s\n  DIV BX\n", w[4]);
            }
            printf("  MOV %s, AX\n", w[0]);
        }
    }
    printf("  MOV AH, 4CH\n  INT 21H\nCODE ENDS\nEND START\n");
    return 0;
}
