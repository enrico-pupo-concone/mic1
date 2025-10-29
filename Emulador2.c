#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tipos

typedef unsigned char byte; // 8 bits
typedef unsigned int word; // 32 bits
typedef unsigned long int microinstrucao; // 64 bits, no caso de acordo com a aquitetura cada microinstrução usa apenas 36bits de espaço 

// Registradores 

word MAR = 0, MDR = 0, PC = 0; // Acesso da Memoria
byte MBR = 0;		       // Acesso da Memoria

word SP = 0, LV = 0, TOS = 0, // Operação da ULA 
		OPC = 0, CPP = 0, H = 0; // Operação da ULA 

microinstruction MIR; // Contem a Microinstrução Atual
word MPC = 0; // Contem o endereco para a proxima Microinstrução

// Barramentos

word bus_B, bus_C;

// Flip-Flops

byte N, Z;

// Auxiladores para Decodificar Microinstrução

byte MIR_B, MIR_operation, MIR_displacer, MIR_MEM, MIR_jump;
word MIR_C;


// Armazenamento de Controle

microinstruction Armazenamento[512];

// Memoria Principal

byte memory [100000000];

// Prototipo das Funções

void load_microprogrm_of_control();
void load_program(const char *programa);
void display_processes();
void decode_microinstruction();
void assign_bus_B();
void perform_operation_ALU();
void perform_bus_C();
void operate_memory();
void jump();

void binary(void* valor, int tipo);



// Laço Principal

int main(int argc, const char *argv[]){
	load_microprogram_of_control();
	load_program(argv[1]);
	while(1){
		display_processes();
		MIR = storage[MPC];

		
		decode_microinstruction();
		assign_bus_B();
		perform_operation_ALU();
		assign_bus_C();
		operate_memory();
		jump();
	}

	return 0;
}

// Implementação das Funções

void load_microprogram_of_control(){
	FILE* microprogram;
	microprogram = fopen("microprog.rom", "rb");

	if(microprogram != NULL){
		fread(storage, sizeof(microinstruction), 512, microprogram);
		
		fclose(microprogram);
	}

}

void load_program(const char* prog){
	FILE* program;
	word size;
	byte tamanho_temp[4];
	program = fopen(prog, "rb");

	if(program != NULL){
		fread(size_temp, sizeof(byte), 4, program); // Lendo o tamanho em bytes do Programa
		memcpy(&size, size_temp, 4);

		fread(memory, sizeof(byte), 20, program); // Lendo os 20 bytes de Inicialização

		fread(&memory[0x0401], sizeof(byte), size - 20, program); // Lendo o Programa

	}
}

void decode_microinstruction(){
	MIR_B = (MIR) & 0b1111;
	MIR_MEM = (MIR >> 4) & 0b111;
	MIR_C = (MIR >> 7) & 0b111111111;
	MIR_operation = (MIR >> 16) & 0b111111;
	MIR_displacer = (MIR >> 22) & 0b11;
	MIR_jump = (MIR >> 24) & 0b111;
	MPC = (MIR >> 27) & 0b111111111;
}

void assign_bus_B(){
	switch(MIR_B){
		case 0: bus_B = MDR;				break;
		case 1: bus_B = PC;				break;
		//O caso 2 carrega o MBR com sinal fazendo a extensão de sinal, ou seja, copia-se o bit mais significativo do MBR para as 24 posições mais significativas do barramento B.
		case 2: bus_B = MBR;
			if(MBR & (0b10000000))
					bus_B = bus_B | (0b111111111111111111111111 << 8);
										break;
		case 3: bus_B = MBR;				break;
		case 4: bus_B = SP;				break;
		case 5: bus_B = LV;				break;
		case 6: bus_B = CPP;				break;
		case 7: bus_B = TOS;				break;
		case 8: bus_B = OPC;				break;
		default: bus_B = -1;				break; 
	}
}

void perform_operation_ALU(){
	switch(MIR_operate){
		//Cada operação da ULA é representado pela sequencia dos bits de operação. Cada operação útil foi convertida para inteiro para facilitar a escrita do switch
		case 12: bus_C = H & bus_B;		break;
		case 17: bus_C = 1;			break;
		case 18: bus_C = -1;			break;
		case 20: bus_C = bus_B;			break;
		case 24: bus_C = H;			break;
		case 26: bus_C = ~H;			break;
		case 28: bus_C = H | bus_B;		break;
		case 44: bus_C = ~bus_B;			break;
		case 53: bus_C = bus_B + 1;		break;
		case 54: bus_C = bus_B - 1;		break;
		case 57: bus_C = H + 1;		break;
		case 59: bus_C = -H;			break;
		case 60: bus_C = H + bus_B;		break;
		case 61: bus_C = H + bus_B + 1;	break;
		case 63: bus_C = bus_B - H;		break;

		default: break;
	}

	if(bus_C){
		N = 0;
		Z = 1;
	} else{
		N = 1;
		Z = 0;
	}

	switch(MIR_displacer){
		case 1: bus_C = bus_C << 8; break;
		case 2: bus_C = bus_C >> 1; break;		
	}
}

void assign_bus_C(){
	if(MIR_C & 0b000000001)   MAR = bus_C;
	if(MIR_C & 0b000000010)   MDR = bus_C;
	if(MIR_C & 0b000000100)   PC  = bus_C;
	if(MIR_C & 0b000001000)   SP  = bus_C;
	if(MIR_C & 0b000010000)   LV  = bus_C;
	if(MIR_C & 0b000100000)   CPP = bus_C;
	if(MIR_C & 0b001000000)   TOS = bus_C;
	if(MIR_C & 0b010000000)   OPC = bus_C;
	if(MIR_C & 0b100000000)   H   = bus_C;
}

void operate_memory(){
	// Multiplicação por 4 é necessaria, pois as portas MAR e MDR leem palavras consecutivas na memoria
	if(MIR_MEM & 0b001) MBR = memory[PC]; 
	if(MIR_MEM & 0b010) memcpy(&MDR, &Memoria[MAR*4], 4);
	if(MIR_MEM & 0b100) memcpy(&Memoria[MAR*4], &MDR, 4);
}

void jump(){
	if(MIR_jump & 0b001) MPC = MPC | (N << 8);
	if(MIR_jump & 0b010) MPC = MPC | (Z << 8);
	if(MIR_jump & 0b100) MPC = MPC | (MBR);
}

void display_proceses(){

	if(LV && SP){
		printf("\t\t  stack of operands\n");
		printf("========================================\n");
		printf("     END");
		printf("\t   value binary");
		printf(" \t\tVALOR\n");
		for(int i = SP ; i >= LV ; i--){
			word value;
			memcpy(&value, &memory[i*4],4);			
			
			if(i == SP) printf("SP ->");
			else if(i == LV) printf("LV ->");
			else printf("     ");

			printf("%X ",i);
			binary(&value, 1); printf(" ");			
			printf("%d\n", value);
		}

		printf("========================================\n");
	}

	if(PC >= 0x0401) {
		printf("\n\t\t\tprogram area\n");
		printf("========================================\n");
		printf("\t\binary");
		printf("\t HEX");
		printf("  END DE BYTE\n");
		for(int i = PC - 2; i <= PC + 3 ; i++){
			if(i == PC) printf("in execution >>  ");
			else printf("\t\t");
			binary(&memory[i], 2);
			printf(" 0x%02X ", memory[i]);
			printf("\t%X\n", i);
		}
		printf("========================================\n\n");
	}

	printf("\t\tregistrars\n");
	printf("\tbinary");
	printf("\t\t\t\tHEX\n");
	printf("MAR: "); binary(&MAR, 3); printf("\t%x\n", MAR);
	printf("MDR: "); binary(&MDR, 3); printf("\t%x\n", MDR);
	printf("PC:  "); binary(&PC, 3); printf("\t%x\n", PC);
	printf("MBR: \t\t"); binary(&MBR, 2); printf("\t\t%x\n", MBR);
	printf("SP:  "); binary(&SP,3); printf("\t%x\n", SP);
	printf("LV:  "); binary(&LV,3); printf("\t%x\n", LV);
	printf("CPP: "); binary(&CPP,3); printf("\t%x\n", CPP);
	printf("TOS: "); binary(&TOS,3); printf("\t%x\n", TOS);
	printf("OPC: "); binary(&OPC, 3); printf("\t%x\n", OPC);
	printf("H:   "); binary(&H, 3); printf("\t%x\n", H);

	printf("MPC: \t\t"); binary(&MPC, 5); printf("\t%x\n", MPC);
	printf("MIR: "); binary(&MIR, 4); printf("\n");

	getchar();
}


//FUNÇÃO RESPONSAVEL POR PRINTAR OS VALORES EM BINARIO
//TIPO 1: Imprime o binário de 4 bytes seguidos 
//TIPO 2: Imprime o binário de 1 byte
//TIPO 3: Imprime	o binario de uma palavra 
//TIPO 4: Imprime o binário de uma microinstrução
//TIPO 5: Imprime o binário dos 9 bits do MPC
void binary(void* value, int type){
	switch(type){
		case 1: {
			printf(" ");
			byte aux;
			byte* valueAux = (byte*)value;
				
			for(int i = 3; i >= 0; i--){
				aux = *(valueAux + i);
				for(int j = 0; j < 8; j++){
					printf("%d", (aux >> 7) & 0b1);
					aux = aux << 1;
				}
				printf(" ");
			}		
		} break;
		case 2:{
			byte aux;
			
			aux = *((byte*)(value));
			for(int j = 0; j < 8; j++){
				printf("%d", (aux >> 7) & 0b1);
				aux = aux << 1;
			}
		} break;

		case 3: {
			word aux;			
			aux = *((word*)(value));
			for(int j = 0; j < 32; j++){
				printf("%d", (aux >> 31) & 0b1);
				aux = aux << 1;
			}
		} break;

		case 4: {
			microinstruction aux;		
			aux = *((microinstruction*)(value));
			for(int j = 0; j < 36; j++){
				if ( j == 9 || j == 12 || j == 20 || j == 29 || j == 32) printf(" ");

				printf("%ld", (aux >> 35) & 0b1);
				aux = aux << 1;
			}
		} break;
		case 5: {
			word aux;
		
			aux = *((word*)(value)) << 23;
			for(int j = 0; j < 9; j++){
				printf("%d", (aux >> 31) & 0b1);
				aux = aux << 1;
			}
		} break;
	}
}
