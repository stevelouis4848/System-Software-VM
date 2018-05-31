#include <stdlib.h>
#include <stdio.h>

typedef struct instruction{
                 int op; // opcode
                 int r; // reg
                 int l; // L
                 int m; // M
                }instruction;

main(){
    for(i=1; i < argc; i++)
    {
        fetch(argv[i]);
    }
    return 0;
}

fetch(char* fileName){

    instruction *newInstruction;
    int* buff;
    FILE *fp;

    fp = fopen("fileName","r");


    while( fscanf(fp, "%s", buff) != EOF ){

        newInstruction.op = buff;

        fscanf(fp, "%d", buff);
        newInstruction.r = buff;

        fscanf(fp, "%d", buff);
        newInstruction.l = buff;

        fscanf(fp, "%d", buff);
        newInstruction.m = buff;

        execute(newInstruction);
        operations();
        PCUpdate();
    }
    fclose(fp);
}

execute(instruction *newInstruction){

switch (newInstruction.op) {
    case 01:
        break;
    case 02:
        break;
    case 03:
        break;
    case 04:
        break;
    case 05:
        break;
    case 06:
        break;
    case 07:
        break;
    case 08:
        break;
    case 09:
        break;
    case 10:
        break;
    case 11:
        break;
    case 12:
        break;
    case 13:
        break;
    case 14:
        break;
    case 15:
        break;
    case 16:
        break;
    case 17:
        break;
    case 18:
        break;
    case 19:
        break;
    case 20:
        break;
    case 21:
        break;
    case 22:
        break;
    }
}

operations(){

}

writeMemory(){

}

readMemory(){

}

readRegister(){

}

pcUpdate(){

}

Stack(){

}
