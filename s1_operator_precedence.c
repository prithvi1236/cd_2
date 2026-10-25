/* 1. OPERATOR PRECEDENCE PARSER        Grammar:  E -> E+E | E*E | (E) | i */
#include <stdio.h>
#include <string.h>

char t[] = "+*i()$";                  /* order of rows and columns in the table */
char M[6][6] = {                      /* M[stack top][input] */
/*        +    *    i    (    )    $   */
/* + */ {'>', '<', '<', '<', '>', '>'},
/* * */ {'>', '>', '<', '<', '>', '>'},
/* i */ {'>', '>', 'e', 'e', '>', '>'},
/* ( */ {'<', '<', '<', '<', '=', 'e'},
/* ) */ {'>', '>', 'e', 'e', '>', '>'},
/* $ */ {'<', '<', '<', '<', 'e', 'a'}};

char *h[] = {"i", "E+E", "E*E", "(E)"};   /* handles = right sides */
char s[50] = "$", in[50];                 /* stack, input */

char topTerminal() {                  /* topmost stack symbol that is not E */
    int i = strlen(s) - 1;
    while (s[i] == 'E') i--;
    return s[i];
}

int reduce() {                        /* replace a handle on top of the stack by E */
    int k, l, n = strlen(s);
    for (k = 0; k < 4; k++) {
        l = strlen(h[k]);
        if (n >= l && strcmp(s + n - l, h[k]) == 0) {
            s[n - l] = 'E';
            s[n - l + 1] = '\0';
            printf("Reduce E->%s\n", h[k]);
            return 1;
        }
    }
    return 0;                         /* no handle found */
}

int main() {
    int i, n, ip = 0;
    char a, b, r;
    printf("Enter string (i for id): ");
    scanf("%s", in);
    strcat(in, "$");
    for (i = 0; in[i]; i++)
        if (!strchr(t, in[i])) { printf("Invalid symbol\n"); return 1; }

    printf("%-10s %-10s %s\n", "STACK", "INPUT", "ACTION");
    while (1) {
        a = topTerminal();            /* a = terminal on stack, b = next input */
        b = in[ip];
        r = M[strchr(t, a) - t][strchr(t, b) - t];
        printf("%-10s %-10s ", s, in + ip);
        if (r == '<' || r == '=') {                   /* SHIFT */
            n = strlen(s);
            s[n] = b;
            s[n + 1] = '\0';
            ip++;
            printf("Shift %c\n", b);
        } else if (r == '>') {                        /* REDUCE */
            if (!reduce()) { printf("Error\n"); break; }
        } else if (r == 'a' && strcmp(s, "$E") == 0) {
            printf("Accept\n\nValid string\n");
            return 0;
        } else {
            printf("Error\n");
            break;
        }
    }
    printf("\nInvalid string\n");
    return 1;
}
