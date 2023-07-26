#include <string>
#include <fstream>
#include <stdio.h>

#include "atel.hpp"
#include "globals.hpp"

using namespace std;
char currentChar;
char prevChar;
int currentLine;


FILE *srcFile;
int main(int arc, char *argv[]) {
    if (!argv[1]) {
        printf("Include source file as arg0\n");
        exit(1);
    }

    srcFile = fopen(argv[1], "r");
    if (srcFile == NULL) {
        printf("Could not open file");
        exit(1);
    }


    while (1) {
        currentChar = nextChar();
        printf("%c", currentChar);
        if (currentChar == EOF)
        {   
            printf("hit EOF");
            fclose(srcFile);
            exit(0);
        }
    }
}

void run() {

}