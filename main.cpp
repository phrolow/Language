#include "Compiler/compiler.h"
#include "config.h"

int main(int argc, char **argv) {
    if(argc < 1) {
        printf("Too few arguments!");
        exit(1);
    }

    if(!checkfile(argv[1])) {
        printf("Invalid input file");
        exit(2);
    }

    language_compile(argv[1], ASM_PATH);
    system("start Y:/gthb/Language/SoftCPU/asm/asm.exe asm.asm bin.bin");
    system("start Y:/gthb/Language/SoftCPU/cpu/cpu.exe bin.bin");
    return 0;
}