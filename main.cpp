#include "SoftCPU/asm/asm.h"
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
    run_asm(ASM_PATH, BINPATH);
    system("Y:/gthb/Language/SoftCPU/cpu/cpu.exe ../../bin.bin");
    return 0;
}