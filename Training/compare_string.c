#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    char *s = get_string("s: ");
    char *t = get_string("t: ");

    // It will be compare address of each variable,
    // it will be not compare for each char
    if (s == t) {
        printf("Same\n");
    }
    else {
        printf("Different\n");
    }

    printf("Address s: %p\n", s);
    printf("Address t: %p\n", t);

    if (strcmp(s, t) == 0) {
        printf("Actual: Same\n");
    }
    else {
        printf("Actual: Different\n");
    }
}