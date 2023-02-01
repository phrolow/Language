#include "Compiler/compiler.h"
#include "config.h"

int main(int argc, char **argv) {
    if(argc < 1) {
        printf("Too few arguments!");
        exit(1);
    }

    //system("Y:/gthb/Language/Compiler.exe code.txt");
    language_compile(argv[1], ASM_PATH);
    system("Y:/gthb/Language/Exes/asm.exe ../asm.asm ../bin.bin");
    system("Y:/gthb/Language/Exes/cpu.exe ../bin.bin");

    return 0;
}