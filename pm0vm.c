// Name: Steve Louis
// Class: COP 3402 summer 2018
// Assignment: Homework #1 (P-Machine)

/*
README

	# How to run

	- compile using gcc pm0vm.c

	- run the program with the input file by entering ( ./a.out "name of input file".txt )
		
	- The program will print the outputs to the screen as well as within three separate txt files
	 
		file named "factOpPrint.txt" has the instructions
		file named "stackTracePrint.txt" has the stack trace and registers
		file named "factPrint.txt" has the factorial output
*/

#include <stdlib.h>
#include <stdio.h>

int MAX_STACK_HEIGHT = 2000;
int MAX_CODE_LENGTH = 500;
int MAX_LEXI_LEVELS = 3;

// Array of strings with the indexes corresponding to the name of the op codes.
char *opCode[] = {"NULL", "LIT", "RTN", "LOD", "STO", "CAL", "INC", "JMP","JPC", "SIO",
					"NEG", "ADD", "SUB", "MUL", "DIV", "ODD","MOD", "EQL", "NEQ",
					"LSS", "LEQ", "GTR", "GEQ"};

// Struct named instructions that holds the  the content of the instructions
typedef struct instruction{
							int op; // opcode
							int r; // reg
							int l; // L
							int m; // M
						}instruction;

// Struct named enviroment that holds the information relevant to the instruction being currently execute.
typedef struct enviroment{
							int pc; // pc
							int pcPrev; //previous pc
							int bp; // base pointer
							int sp; // stack pointer
							int *R;   //Register
							instruction ir; // current instruction
							
					}enviroment;

// Function Prototypes for the program.					
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
	
	// Initializing file pointers to read the input file and files to output.
	ifp = fopen(fileName, "r");
	ofp = fopen("factOpPrint.txt", "w");
	ofp2 = fopen("stackTracePrint.txt", "w");
	ofp3 = fopen("factPrint.txt", "w");
	
	
	if(ifp == NULL || ofp == NULL || ofp2 == NULL || ofp3 == NULL)
	{
		printf("Invalid file pointer");
		return;
	}

	// Prints the headers to the files.
	fprintf(ofp,"\nFactorial Op Printout:\n");
	fprintf(ofp2,"\nFactorial Stack Trace:\nPC OP Rg Lx Vl [PC BP SP]\n");
	fprintf(ofp3,"\nFactorial Output:\n");
	
	count = 0;
	
	// Reads the input file one instruction at a time and stores it the array of instructions. 
	while( fscanf(ifp, "%d %d %d %d",&buff[0],
					&buff[1],&buff[2],&buff[3]) != EOF){

		irList[count].op = buff[0];
		irList[count].r = buff[1];
		irList[count].l = buff[2];
		irList[count].m = buff[3];
		
		count++;
	}

	count = 0;
	
	// Runs fetch then execute,then prints the calls printStack contents into 3 files.
	while (*halt != 1 ){
		
		fetch(count, env, irList, ofp);
		execute(env, stack, halt,ofp2,ofp3);
		printStack(1,env,env->sp,env->bp,stack,env->ir.l,ofp2);
		printStack(2,env,env->sp,env->bp,stack,env->ir.l,ofp2);
		printStack(3,env,env->sp,env->bp,stack,env->ir.l,ofp2);
		count++;
	}
	
	// Closes the files since we are done writing to them.
	fclose(ifp);
	fclose(ofp);
	fclose(ofp2);
	fclose(ofp3);
	
	// Initializing file pointers to be read and printed to the screen.
	ofp = fopen("factOpPrint.txt", "r");
	ofp2 = fopen("stackTracePrint.txt", "r");
	ofp3 = fopen("factPrint.txt", "r");
	
	// Gets the contents of the 3 output files and prints them to the screen.
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
	
	// Closes the file pointers since we are done printing them to the screen.
	fclose(ofp);
	fclose(ofp2);
	fclose(ofp3);
	
}

void fetch(int count,enviroment *env, instruction *irList, FILE *ofp){
	
	// Takes the instruction code corresponding to the pc and puts it in the environment struct.
	env->ir = irList[env->pc];
			
	// Prints the contents of the current instruction a file.
	fprintf(ofp,"%d %s %d %d %d \n",count,opCode[env->ir.op],env->ir.r,env->ir.l,
					env->ir.m);
	
	// Stores the pc before it is incremented.
	env->pcPrev = env->pc;
	
	// PC is incremented to be able to detch the next instruction.
	env->pc++;
	
}

void execute(enviroment *env,int *stack, int *halt,FILE *ofp, FILE *ofp2){

	// Takes care of the math operations and sends them to a another function.
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
			stack[env->sp + 1] = 0;
			stack[env->sp + 2] = base(env->ir.l, env->bp, stack);
			stack[env->sp + 3] = env->bp; 
			stack[env->sp + 4] = env->pc; 
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
					fprintf(ofp,"%d\n", env->R[env->ir.r]);
					fprintf(ofp2,"%d\n", env->R[env->ir.r]);
					break;
				case 2://SIO2
					scanf("%d", &stack[env->sp]);
					break;
				case 3://SIO3
					*halt = 1;
					break;
				default:
				printf("Invalid m for SIO\n");
			}
			break;
		default:
			printf("Invalid OP!\n");
	}
}

void opr(enviroment *env, int *stack){

	// Switch case that executes the math functions depending on the OP code.
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
			printf("Invalid OP!\n");
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
			// Prints the first 8 outputs of stack trace to an output file.
			fprintf(ofp,"%d %s %d %d %d    [%d %d %d]    ",env->pcPrev,opCode[env->ir.op],env->ir.r,env->ir.l,
						env->ir.m, env->pc, env->bp, env->sp);
						
			break;
		case 2:
			// Recursively prints the contents of the stack to an iput file.
			if (bp == 1) {
				if (l > 0) {
					fprintf(ofp,"|");
				}
			 }	   
			else {
				//Print the lesser lexical level
				printStack(2,env,bp-1, stack[bp + 2], stack, l-1,ofp);
				fprintf(ofp,"|");
			}
				//Print the stack contents - at the current level
			for (i = bp; i <= sp; i++){
				fprintf(ofp,"%3d ", stack[i]);	
			}
			break;
		case 3:
			// Points the registers to an output file.
			fprintf(ofp,"\n\tRegisters:[");
			
			for(i=0;i<8;i++){
				fprintf(ofp,"%d ",env->R[i]);
			}
			fprintf(ofp,"]\n\n");
			break;
	}
}	
