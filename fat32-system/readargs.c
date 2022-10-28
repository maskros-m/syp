#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char partionName[20];
unsigned int partionSize;

void setDefaultValues () {
    strcpy(partionName, "myPart.bin");
    partionSize = 2000000;
}

int main(int argc, char** argv) {
    setDefaultValues();
    int i;

    for (i = 0; i < argc; i++) {
        printf("\n[%d] ==> [%s]", i, argv[i]);
    }
}