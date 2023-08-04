#include <iostream>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"

FILE* Asm::outfile;
int Asm::freeRegisters[4];
const char* Asm::registerList[4] = { "r8" , "r9" , "r10" , "r11" };

void Asm::init(char* filename) {

    outfile = fopen(("%s.nasm", filename), "w");
    if (outfile == nullptr) {
        fprintf(stderr, "cannot create outfile");
        exit(1);
    }
    freeAllRegisters();
    return;
}

void Asm::freeAllRegisters(void) {
    freeRegisters[0] = freeRegisters[1] = freeRegisters[2] = freeRegisters[3] = 1;
    return;
}

int Asm::allocateRegister(void) {
    for (int i = 0; i < 4; i++) {
        if (freeRegisters[i]) {
            freeRegisters[i] = 0;
            return i;
        }
    }
    fprintf(stderr, "Out of registers\n");
    exit(1);
}

void Asm::freeRegister(int r) {
    if (freeRegisters[r] != 0) {
        fprintf(stderr, "Register already free %d\n", r);
        exit(1);
    }

    freeRegisters[r] = 1;
}

void Asm::preamble(void) {
    fputs("\tglobal\t_start\n"
	"\textern\tprintf\n"
	"\tsection\t.text\n"
	"LC0:\tdb\t\"%d\",10,0\n"
	"printint:\n"
	"\tpush\trbp\n"
	"\tmov\trbp, rsp\n"
	"\tsub\trsp, 16\n"
	"\tmov\t[rbp-4], edi\n"
	"\tmov\teax, [rbp-4]\n"
	"\tmov\tesi, eax\n"
	"\tlea	rdi, [rel LC0]\n"
	"\tmov	eax, 0\n"
	"\tcall	printf\n"
	"\tnop\n"
	"\tleave\n"
	"\tret\n"
	"\n"
	"_start:\n" "\tpush\trbp\n" "\tmov	rbp, rsp\n", outfile);

}

void Asm::postamble(void) {
    fputs("\tmov	eax, 0\n" "\tpop	rbp\n" "\tret\n", outfile);
    fclose(outfile);
}

int Asm::loadInt(int v) {
    int r = allocateRegister();
    printf("INT: Loading register %i with value: %i\n",r ,v);

    fprintf(outfile, "\tmov\t%s, %d\n", registerList[r], v);
     return r;
}

int Asm::add(int r1, int r2) {

    printf("Adding reg:%i,reg: %i\n", r1, r2);

    fprintf(outfile, "\tadd\t%s, %s\n", registerList[r2], registerList[r1]);
    freeRegister(r1);
    return r2;
}

int Asm::subtract(int r1, int r2) {
    printf("Subtracting reg:%i,reg: %i\n", r1, r2);
    fprintf(outfile, "\tsub\ts%s\n", registerList[r1], registerList[r2]);
    freeRegister(r2);
    return r1;
}

int Asm::multiply(int r1, int r2) {
    printf("multiplying reg: %i,reg: %i\n", r1, r2);
    fprintf(outfile, "\timul\t%s, %s\n", registerList[r2], registerList[r1]);
    freeRegister(r1);
    return r2;
}

int Asm::divide(int r1, int r2) {
    printf("Dividing reg:%i, reg:%i\n", r1, r2);
    fprintf(outfile, "\tmov\trax, %s\n", registerList[r1]);
    fprintf(outfile, "\tcqo\n");
    fprintf(outfile, "\tidiv\t%s\n", registerList[r2]);
    fprintf(outfile, "\tmov\t%s, rax\n", registerList[r1]);
    freeRegister(r2);

    return r1;
}

void Asm::printInt(int r) {
    printf("INT: Printing reg:%i\n",r);
    fprintf(outfile, "\tmov\trdi, %s\n", registerList[r]);
    fprintf(outfile, "\tcall\tprintint\n");
    freeRegister(r);
}

int Asm::loadGlobalSymbol(char* s) {
    int r = allocateRegister();
    printf("Loading symbol: %s in register: %i", s, r);

    fprintf(outfile, "\tmov\t%s, [%s]\n", registerList[r], s);
    return r;
}

int Asm::storeGlobalSymbol(int r, char* s) {
    printf("storing global symbol: %s in reg:%i", s, r);
    fprintf(outfile, "\tmov\t[%s], %s\n", s, registerList[r]);
    return r;
}

void Asm::globalSymbol(char* s) {
    printf("creating global symbol:%s\n",s);
    fprintf(outfile, "\tcommon\t%s 8:8\n", s);
}