/* 4. SHIFT REDUCE PARSER        Grammar:  E -> E+E | E*E | (E) | i
   Loop: try to REDUCE; if you cannot, SHIFT; at the end the stack must be E. */
#include <stdio.h>
#include <string.h>

char *h[] = {"E*E", "E+E", "(E)", "i"};     /* handles = right sides */
char s[50] = "", in[50];                    /* stack, input */

int reduce(char next) {
    int k, l, n = strlen(s);
    for (k = 0; k < 4; k++) {
        if (k == 1 && next == '*') continue;     /* conflict: * is stronger, so E+E must wait */
        l = strlen(h[k]);
        if (n >= l && strcmp(s + n - l, h[k]) == 0) {
            s[n - l] = 'E';
            s[n - l + 1] = '\0';
            printf("Reduce E->%s\n", h[k]);
            return 1;
        }
    }
    return 0;
}

int main() {
    int ip = 0, n;
    printf("Enter string (i for id): ");
    scanf("%s", in);
    strcat(in, "$");

    printf("%-10s %-10s %s\n", "STACK", "INPUT", "ACTION");
    while (1) {
        printf("%-10s %-10s ", s, in + ip);
        if (reduce(in[ip])) continue;            /* 1. reduce if possible */
        if (in[ip] == '$') break;                /* 3. input finished */
        n = strlen(s);                           /* 2. otherwise shift */
        s[n] = in[ip];
        s[n + 1] = '\0';
        printf("Shift %c\n", in[ip++]);
    }
    if (strcmp(s, "E") == 0) printf("Accept\n\nValid string\n");
    else                     printf("Error\n\nInvalid string\n");
    return 0;
}
