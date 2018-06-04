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
void fetch(int count, enviroment *env, instruction *irList, FILE *ofp);
void execute(enviroment *env,int *stack,int *halt, FILE *ofp, FILE *ofp2);
void opr(enviroment *env, int *stack);
void printStack(int printvalue,enviroment *env,int sp,int bp,int* stack,int l,FILE *ofp);

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
	int count, *stack, *halt, buff[3];
	FILE *ifp, *ofp, *ofp2, *ofp3;
	char ofpPrint;

	irList = malloc( MAX_CODE_LENGTH * sizeof(instruction));
	env = malloc(sizeof(enviroment));
	env->R = calloc(16,sizeof(int));
	stack = calloc(MAX_STACK_HEIGHT,sizeof(int));
	halt = malloc(sizeof(int));
	
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

	fprintf(ofp,"\nFactorial Op Printout:\n");
	fprintf(ofp2,"\nFactorial Stack Trace:\nPC OP Rg Lx Vl [PC BP SP]\n");
	fprintf(ofp3,"\nFactorial Output:\n");
	
	count = 0;
	while( fscanf(ifp, "%d %d %d %d",&buff[0],
					&buff[1],&buff[2],&buff[3]) != EOF){

		irList[count].op = buff[0];
		irList[count].r = buff[1];
		irList[count].l = buff[2];
		irList[count].m = buff[3];
		
		count++;
	}

	count = 0;
	while (*halt != 1 ){
		
		if(count == 0){
			//printf("Factorial Op Printout:\n");			
		}
		fetch(count, env, irList, ofp);
		execute(env, stack, halt,ofp2,ofp3);
		printStack(1,env,env->sp,env->bp,stack,env->ir.l,ofp2);
		printStack(2,env,env->sp,env->bp,stack,env->ir.l,ofp2);
		printStack(3,env,env->sp,env->bp,stack,env->ir.l,ofp2);
		count++;
	}
	fclose(ifp);
	fclose(ofp);
	fclose(ofp2);
	fclose(ofp3);
	
	ofp = fopen("factOpPrint.txt", "r");
	ofp2 = fopen("stackTracePrint.txt", "r");
	ofp3 = fopen("factPrint.txt", "r");
	
	ofpPrint = fgetc(ofp);
	while(ofpPrint != EOF){
		printf("%c",ofpPrint);
		ofpPrint = fgetc(ofp);		
	}
	ofpPrint = fgetc(ofp2);
	while(ofpPrint != EOF){
		printf("%c",ofpPrint);
		ofpPrint = fgetc(ofp2);		
	}
	ofpPrint = fgetc(ofp3);
	while(ofpPrint != EOF){
		printf("%c",ofpPrint);
		ofpPrint = fgetc(ofp3);		
	}
	
	fclose(ofp);
	fclose(ofp2);
	fclose(ofp3);
	
}

void fetch(int count,enviroment *env, instruction *irList, FILE *ofp){
	
	env->ir = irList[env->pc];
	
	/*printf("%d %s %d %d %d \n",env->pc,opCode[env->ir.op],env->ir.r,env->ir.l,
					env->ir.m);*/
					
	fprintf(ofp,"%d %s %d %d %d \n",count,opCode[env->ir.op],env->ir.r,env->ir.l,
					env->ir.m);
	
	env->pcPrev = env->pc;
	env->pc++;
	
}

void execute(enviroment *env,int *stack, int *halt,FILE *ofp, FILE *ofp2){

	if(env->ir.op >= 10){
		opr(env,stack);
		return;
	}

	switch (env->ir.op) {
		case 1: //LIT
			env->R[env->ir.r] = env->ir.m;
			break;
		case 2: //RTN
			env->sp = env->bp - 1;
			env->bp = stack[env->sp + 3];
			env->pc = stack[env->sp + 4];			
			break;
		case 3: //LOD
			env->R[env->ir.r] = stack[base(env->ir.l, env->bp, stack) + env->ir.m];
			break;
		case 4: //STO
			stack[base(env->ir.l, env->bp, stack) + env->ir.m] = env->R[env->ir.r];
			break;
		case 5: //CAL
			stack[env->sp + 1] = 0; //env->space to return value
			stack[env->sp + 2] = base(env->ir.l, env->bp, stack); //static link (SL)
			stack[env->sp + 3] = env->bp; //dynamic link (DL)
			stack[env->sp + 4] = env->pc; //return address (RA)
			env->bp = env->sp + 1;
			env->pc = env->ir.m;	
			env->sp = env->sp + 4;
			break;
		case 6: //INC
			env->sp = env->sp + env->ir.m;
			break;
		case 7: //JMP
			env->pc = env->ir.m;
			break;
		case 8: //JPC
			if (env->R[env->ir.r] == 0) {
				env->pc = env->ir.m;
			}
			break;
		case 9: //SIO
			switch (env->ir.m) {
				case 1://SIO1
					//printf("%d\n", env->R[env->ir.r]);
					fprintf(ofp,"%d\n", env->R[env->ir.r]);
					fprintf(ofp2,"%d\n", env->R[env->ir.r]);
					break;
				case 2://SIO2
					scanf("%d", &stack[env->sp]);
					break;
				case 3://SIO3
					*halt = 1;
					//printf("PROGRAM HALTED\n");
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
			env->R[env->ir.r] = -env->R[env->ir.r];
			break;
		case 11: //ADD
			env->R[env->ir.r] = env->R[env->ir.l] + env->R[env->ir.m];
			break;
		case 12: //SUB
			env->R[env->ir.r] = env->R[env->ir.l] - env->R[env->ir.m];
			break;
		case 13: //MUL
			env->R[env->ir.r] = env->R[env->ir.l] * env->R[env->ir.m];
			break;
		case 14: //DIV
			env->R[env->ir.r] = env->R[env->ir.l] / env->R[env->ir.m];
			break;
		case 15: //ODD
			env->R[env->ir.r] = env->R[env->ir.r] % 2;
			break;
		case 16: //MOD
			env->R[env->ir.r] = env->R[env->ir.l] % env->R[env->ir.m];
			break;
		case 17: //EQL
			env->R[env->ir.r] = env->R[env->ir.l] == env->R[env->ir.m];
			break;
		case 18: //NEQ
			env->R[env->ir.r] = env->R[env->ir.l] != env->R[env->ir.m];
			break;
		case 19: //LSS
			env->R[env->ir.r] = env->R[env->ir.l] < env->R[env->ir.m];
			break;
		case 20: //LEQ
			env->R[env->ir.r] = env->R[env->ir.l] <= env->R[env->ir.m];
			break;
		case 21: //GTR
			env->R[env->ir.r] = env->R[env->ir.l] > env->R[env->ir.m];
			break;
		case 22: //GEQ
			env->R[env->ir.r] = env->R[env->ir.l] >= env->R[env->ir.m];
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

void printStack(int printValue,enviroment *env,int sp,int bp,int* stack,int l,FILE *ofp){
     int i;
	 	 
	switch(printValue){
		
		case 1:
			/*printf("%d %s %d %d %d\t %d %d %d",env->pcPrev,opCode[env->ir.op],env->ir.r,env->ir.l,
						env->ir.m, env->pc, env->bp, env->sp);*/
			
			fprintf(ofp,"%d %s %d %d %d    [%d %d %d]    ",env->pcPrev,opCode[env->ir.op],env->ir.r,env->ir.l,
						env->ir.m, env->pc, env->bp, env->sp);
						
			break;
		case 2:
			if (bp == 1) {
				if (l > 0) {
					//printf("|");
					fprintf(ofp,"|");
				}
			 }	   
			else {
				//Print the lesser lexical level
				printStack(2,env,bp-1, stack[bp + 2], stack, l-1,ofp);
				//printf("|");
				fprintf(ofp,"|");
			}
				//Print the stack contents - at the current level
			for (i = bp; i <= sp; i++){
				//printf("%3d ", stack[i]);
				fprintf(ofp,"%3d ", stack[i]);	
			}
			break;
		case 3:
			//printf("\n\tR[");
			fprintf(ofp,"\n\tRegisters:[");
			
			for(i=0;i<8;i++){
				//printf("%d ",env->R[i]);
				fprintf(ofp,"%d ",env->R[i]);
			}
			//printf("]\n");
			fprintf(ofp,"]\n\n");
			break;
	}
}	
