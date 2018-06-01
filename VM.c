#include <stdlib.h>
#include <stdio.h>

int MAX_STACK_HEIGHT = 2000;
int MAX_CODE_LENGTH = 500;
int MAX_LEXI_LEVELS = 3;

char *opCode[] = {"NULL", "lit", "lod", "sto", "cal", "inc", "jmp","jpc", "sio",
                  "neg", "add", "sub", "mul", "div", "odd","mod", "eql", "neq",
                  "lss", "leq", "gtr", "geq"};

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

int base(l, base) // l stand for L in the instruction format
{
 int b1; //find base L levels down
 b1 = base;

 while (l > 0){
   b1 = stack[b1 + 1];
   l--;
 }
   return b1;
}

void vm (char* fileName){

 instruction *irList;
 enviroment *env;
 int *stack, i = 0, buff, buff2, buff3, buff4;
 FILE *ifp,*ofp;

 irList = malloc( * sizeof(instruction))
 env = malloc(sizeof(enviroment));

 env->sp = 0;
 env->bp = 1;
 env->pc = 0;

 ifp = fopen("fileName", "r");
 ofp = fopen("factOpPrint", "w");

 for (i=0; i<MAX_STACK_HEIGHT; i++){

        stack[i] = malloc(sizeof(int));
        stack[i] = 0;
  }

  while( fscanf(ifp, "%d,%d,%d,%d",buff, buff1,
                buff2, buff3,buff4)) != EOF){

            irList[i] = malloc(sizeof(instruction));

            irList[i]->op = buff;
            irList[i]->r = buff2;
            irList[i]->l = buff3;
            irList[i]->m = buff4;
            i++;
        }

        fetch(env, irList);
        execute(env,stack);

    execute(env,stack);

 fclose(ifp);
 fclose(ofp);
}

void fetch(enviroment *env, instruction *irList){

    env->ir = irList[env->pc];

    fprintf(ofp,"%s %d %d %d \n",opCode[env->ir.op],env->ir.r,env->ir.l,
            env->ir.m);
}
void execute(enviroment *env,int *stack){

  if(env->ir.op > 10){
      opr(env,stack);
      return;
     }

  switch (env->ir.op) {
    case 1: //LIT
          env->sp=env->sp+1;
          stack[env->sp]=env->ir.m;
          break;
      case 2: //OPR function
          OPR(sp, bp, pc, stack, env->ir);
          break;
      case 3: //LOD
          env->sp=env->sp+1;
          stack[env->sp]=stack[base(bp, env->ir, stack)+env->ir.m];
          break;
      case 4: //STO
          stack[base(bp, env->ir, stack)+env->ir->m]=stack[env->sp];
          env->sp=env->sp-1;
          break;
      case 5: //CAL
          stack[env->sp+1]=0; //space to return value
          stack[env->sp+2]=base(bp, env->ir, stack); //static link (SL)
          stack[env->sp+3]=env->bp; //dynamic link (DL)
          stack[env->sp+4]=env->pc; //return address (RA)
          env->bp=env->sp+1;
          env->pc=env->ir.m;
          break;
      case 6: //INC
          env->sp=env->sp+env->ir.m;
          break;
      case 7: //JMP
          env->pc=env->ir.m;
          break;
      case 8: //JPC
          if (stack[env->sp]==0) {
              env->pc=env->ir.m;
          }
          env->sp=env->sp-1;
          break;
      case 9: //SIO1
          printf("%d\n", stack[env->sp]);
          env->sp=env->sp-1;

          if(env->ir.m)//SIO2
            env->sp=env->sp+1;
            scanf("%d", &stack[env->sp]);

          if(env->ir.m)//SIO3
            env->sp=env->sp+1;
            scanf("%d", &stack[env->sp]);
          break;

      default:
          printf(" Not Allowed!\n");
    }
}

void opr(enviroment *env, int *stack){

  switch(env->ir.op){
    case 10: //NEG
        stack[env->sp]=-stack[env->sp];
        break;
    case 11: //ADD
        env->sp=env->sp-1;
        stack[env->sp]=stack[env->sp]+stack[env->sp+1];
        break;
    case 12: //SUB
        env->sp=env->sp-1;
        stack[env->sp]=stack[env->sp]-stack[env->sp+1];
        break;
    case 13: //MUL
        env->sp=env->sp-1;
        stack[env->sp]=stack[env->sp]*stack[env->sp+1];
        break;
    case 14: //DIV
        env->sp=env->sp-1;
        stack[env->sp]=stack[env->sp]/stack[env->sp+1];
        break;
    case 15: //ODD
        stack[env->sp]=stack[env->sp]%2;
        break;
    case 16: //MOD
        env->sp=env->sp-1;
        stack[env->sp]=stack[env->sp]%stack[env->sp+1];
        break;
    case 17: //EQL
        env->sp=env->sp-1;
        stack[env->sp]=stack[env->sp]==stack[env->sp+1];
        break;
    case 18: //NEQ
        env->sp=env->sp-1;
        stack[env->sp]=stack[env->sp]!=stack[env->sp+1];
        break;
    case 119: //LSS
        env->sp=env->sp-1;
        stack[env->sp]=stack[env->sp]<stack[env->sp+1];
        break;
    case 20: //LEQ
        env->sp=env->sp-1;
        stack[env->sp]=stack[env->sp]<=stack[env->sp+1];
        break;
    case 21: //GTR
        env->sp=env->sp-1;
        stack[env->sp]=stack[env->sp]>stack[env->sp+1];
        break;
    case 22: //GEQ
        env->sp=env->sp-1;
        stack[env->sp]=stack[env->sp]>=stack[env->sp+1];
        break;
    }
}
main(){
  for(i=1; i < argc; i++)
  {
    vm(argv[i]);
  }
  return 0;
}
