#include <stdlib.h>
#include <stdio.h>

int MAX_STACK_HEIGHT = 2000;
int MAX_CODE_LENGTH = 500;
int MAX_LEXI_LEVELS = 3;

char *opCode[] = {"NULL", "LIT", "RTN", "LOD", "STO", "CAL", "INC", "JMP","JPC", "SIO",
					"NEG", "ADD", "SUB", "MUL", "DIV", "ODD","MOD", "EQL", "NEQ",
					"LSS", "LEQ", "GTR", "GEQ"};

typedef struct instruction{
							int op; // opcode
							int r; // reg
							int l; // L
							int m; // M
						}instruction;

typedef struct enviroment{
							int pc; // pc
							int pcPrev; //previous pc
							int bp; // base pointer
							int sp; // stack pointer
							int *R;   //Register
							instruction ir; // current instruction
							
					}enviroment;
					
int base(int l, int base,int *stack);
void vm (char* fileName);
void fetch(enviroment *env, instruction *irList, FILE *ofp);
void execute(enviroment *env,int *stack,int *halt);
void opr(enviroment *env, int *stack);
void printStack(enviroment *env, int *stack);


int main(int argc, char **argv){
	int i;
	for(i=1; i < argc; i++)
	{
			vm(argv[i]);
	}
	return 0;
}

void vm (char *fileName){

	
	instruction *irList;
	enviroment *env;
	int *stack,i = 0, buff[3],*halt;
	FILE *ifp, *ofp, *ofp2, *ofp3;

	irList = malloc( MAX_CODE_LENGTH * sizeof(instruction));
	env = malloc(sizeof(enviroment));
	env->R = calloc(16,sizeof(int));
	stack = calloc(MAX_STACK_HEIGHT,sizeof(int));
	halt = malloc(sizeof(int));
	
	
	env->R[]
	env->sp = 0;
	env->bp = 1;
	env->pc = 0;
	*halt = 0;
	
	ifp = fopen(fileName, "r");
	ofp = fopen("factOpPrint.txt", "w");
	ofp2 = fopen("stackTracePrint.txt", "w");
	ofp3 = fopen("factPrint.txt", "w");
	
	
	if(ifp == NULL || ofp == NULL)
	{
		printf("Invalid file pointer");
		return;
	}

	fprintf(ofp,"Factorial Op Printout:\n");
	fprintf(ofp2,"Factorial Stack Trace:\n");
	fprintf(ofp3,"Factorial Output:\n");
	
	while( fscanf(ifp, "%d %d %d %d",&buff[0],
					&buff[1],&buff[2],&buff[3]) != EOF){

		irList[i].op = buff[0];
		irList[i].r = buff[1];
		irList[i].l = buff[2];
		irList[i].m = buff[3];
		
		i++;
	}

	while (*halt != 1 ){
		
		fetch(env, irList, ofp);
		execute(env, stack, halt);
		printf("%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t",env->pcPrev,opCode[env->ir.op],env->ir.r,env->ir.l,
					env->ir.m, env->pc, env->bp, env->sp);
		printStackFrame(env, stack);
		//printStackFrame(stack, env, ofp2);
	}
	
	fclose(ifp);
	fclose(ofp);
	fclose(ofp2);
	fclose(ofp3);
	
}

void fetch(enviroment *env, instruction *irList, FILE *ofp){

	env->ir = irList[env->pc];
	
	//fprintf(ofp,"%d %s %d %d %d \n",env->pc,opCode[env->ir.op],env->ir.r,env->ir.l,
					//env->ir.m);
	printf("%d %s %d %d %d \n",env->pc,opCode[env->ir.op],env->ir.r,env->ir.l,
					env->ir.m);
	
	env->pcPrev = env->pc;
	env->pc++;
	
}

void execute(enviroment *env,int *stack, int *halt){

	if(env->ir.op >= 10){
		opr(env,stack);
		return;
	}

	switch (env->ir.op) {
		case 1: //LIT
			env->sp++;
			stack[env->sp] = env->ir.m;
			break;
		case 2: //RTN
			env->sp = env->bp - 1;
			env->bp = stack[env->sp + 3];
			env->pc = stack[env->sp + 4];
			env->ir.l++;			
			break;
		case 3: //LOD
			env->sp++;
			stack[env->sp] = stack[base(env->ir.l, env->bp, stack) + env->ir.m];
			break;
		case 4: //STO
			stack[base(env->ir.l, env->bp, stack) + env->ir.m] = stack[env->sp];
			env->sp--;
			break;
		case 5: //CAL
			stack[env->sp + 1] = 0; //env->space to return value
			stack[env->sp + 2] = base(env->ir.l, env->bp, stack); //static link (SL)
			stack[env->sp + 3] = env->bp; //dynamic link (DL)
			stack[env->sp + 4] = env->pc; //return address (RA)
			env->bp = env->sp + 1;
			env->sp = env->sp + 4;
			env->pc = env->ir.m;
			env->ir.l++;			
			break;
		case 6: //INC
			env->sp = env->sp + env->ir.m;
			break;
		case 7: //JMP
			env->pc = env->ir.m;
			break;
		case 8: //JPC
			if (stack[env->sp] == 0) {
				env->pc = env->ir.m;
			}
			env->sp--;
			break;
		case 9: //SIO
			switch (env->ir.m) {
				case 1://SIO1
					printf("%d\n", stack[env->sp]);
					env->sp--;
					break;
				case 2://SIO2
					env->sp++;
					scanf("%d", &stack[env->sp]);
					break;
				case 3://SIO3
					*halt = 1;
					printf("PROGRAM HALTED\n");
					break;
				default:
				printf("Invalid m for SIO\n");
			}
			break;
		default:
			printf(" 1 Invalid op!\n");
	}
}

void opr(enviroment *env, int *stack){

	switch(env->ir.op){
		case 10: //NEG
			stack[env->sp] = -stack[env->sp];
			break;
		case 11: //ADD
			env->sp--;
			stack[env->sp] = stack[env->sp] + stack[env->sp + 1];
			break;
		case 12: //SUB
			env->sp--;
			stack[env->sp] = stack[env->sp] - stack[env->sp + 1];
			break;
		case 13: //MUL
			env->sp--;
			stack[env->sp] = stack[env->sp] * stack[env->sp + 1];
			break;
		case 14: //DIV
			env->sp--;
			stack[env->sp] = stack[env->sp] / stack[env->sp + 1];
			break;
		case 15: //ODD
			stack[env->sp] = stack[env->sp] % 2;
			break;
		case 16: //MOD
			env->sp--;
			stack[env->sp] = stack[env->sp] % stack[env->sp + 1];
			break;
		case 17: //EQL
			env->sp--;
			stack[env->sp] = stack[env->sp] == stack[env->sp + 1];
			break;
		case 18: //NEQ
			env->sp--;
			stack[env->sp] = stack[env->sp] != stack[env->sp + 1];
			break;
		case 19: //LSS
			env->sp--;
			stack[env->sp] = stack[env->sp] < stack[env->sp + 1];
			break;
		case 20: //LEQ
			env->sp--;
			stack[env->sp] = stack[env->sp] <= stack[env->sp + 1];
			break;
		case 21: //GTR
			env->sp--;
			stack[env->sp] = stack[env->sp] > stack[env->sp + 1];
			break;
		case 22: //GEQ
			env->sp--;
			stack[env->sp] = stack[env->sp] >= stack[env->sp + 1];
			break;
		default:
			printf("2 Invalid op");
		}
}

int base(int l, int base,int *stack) // l stand for L in the instruction format
{
	int b1 = base;; //find base L levels down
	
	while (l > 0){
			b1 = stack[b1 + 1];
			l--;
	}
			return b1;
}

void printStack(enviroment *env,int* stack){
     int i;
	 
     if (env->bp == 1) {
     	if (env->ir.l > 0) {
	   printf("|");
	   }
     }	   
     else {
     	  //Print the lesser lexical level
     	  printStack(env->bp-1, stack[env->bp + 2], stack, env->ir.l-1);
	  printf("|");
     }
     //Print the stack contents - at the current level
     for (i = env->bp; i <= env->sp; i++) {
     	 printf("%3d ", stack[i]);	
     }
}
	
	
