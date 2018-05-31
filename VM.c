#include <stdlib.h>
#include <stdio.h>

int MAX_STACK_HEIGHT = 2000;
int MAX_CODE_LENGTH = 500;
int MAX_LEXI_LEVELS = 3;

char *opCode[] = {"NULL", "lit", "lod", "sto", "cal", "inc", "jmp",
                    "jpc", "sio", "neg", "add", "sub", "mul", "div", "odd",
                    "mod", "eql", "neq", "lss", "leq", "gtr", "geq"};
typedef struct instruction{
                 int op; // opcode
                 int r; // reg
                 int l; // L
                 int m; // M
                }instruction;

typedef struct enviroment{
                 int pc; // opcode
                 int bp; // reg
                 int sp; // L
                 instruction ir; // M
               }enviroment;

main(){
    for(i=1; i < argc; i++)
    {
        fetch(argv[i]);
    }
    return 0;
}

int base(l, base) // l stand for L in the instruction format
{
  int b1; //find base L levels down
  b1 = base;   

  while (l > 0){
    b1 = stack[b1 + 1];
    l- -;
  }
    return b1;
}

fetch(char* fileName){

enviroment *env;
int *stack;


    FILE *ifp;
    FILE *ofp;

    ifp = fopen("fileName","r");
    ofp =fopen("factOpPrint","w");
    ofp2 =

    for (i=0;i<MAX_STACK_HEIGHT;i++){

          stack[i] = malloc(sizeof(int));
          stack[i] = 0;
        }


    while( fscanf(ifp, "%d,%d,%d",env->ir.op,env->ir.r,env->ir.l
                  env->ir.m)) != EOF ){

      fprintf(ofp,"%s %d %d %d",opCode[env->ir.op],env->ir.r,env->ir.l,
              env->ir.m);
      execute(env);
    }
    fclose(ifp);
    fclose(ofp);
}

execute(enviroment *env){

  if(env->ir.op > 10){
      OPR(env);
      }

switch (env->ir.op) {
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
  }

}

OPR(){

  switch(env->ir.op){
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
