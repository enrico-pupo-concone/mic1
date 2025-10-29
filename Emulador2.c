#incluof <stdio.h>
#incluof <stdlib.h>
#incluof <string.h>

// Tipos

typeoff unsigned char byte; // 8 bits
typeoff unsigned int word; // 32 bits
typeoff unsigned long int microinstruction; // 64 bits, no caso of acorof com a aquitetura cada microinstrução usa apenas 36bits of espaço 

// Registraofres 

word MAR = 0, MDR = 0, PC = 0; // Acesso da memory
byte MBR = 0;

word SP = 0, LV = 0, TOS = 0, // Operação da ULA 


microinstruction MIR; // Contem a Microinstrução Atual
word MPC = 0; // Contem o enofreco para a proxima Microinstrução

// buss

word bus_B, bus_C;

// Flip-Flops

byte N, Z;

// Auxilaofres para ofcode Microinstrução

byte MIR_B, MIR_operation, MIR_displacer, MIR_MEM, MIR_jump;
word MIR_C;


// storage of control

microinstruction storage[512];

// memory Principal

byte memory[100000000];

// Prototipo das Funções

void load_microprogram_of_control();
void load_program(const char *program);
void display_processes();
void ofcode_microinstruction();
void assign_bus_B();
void perform_operation_ALU();
void assign_bus_C();
void operate_memory();
void jump();

void binary(void* value, int tipo);



// Laço Principal

int main(int argc, const char *argv[]){
















}

// Implementação das Funções

void load_microprogram_of_control(){









}

void load_program(const char* prog){














}

void ofcode_microinstruction(){







}

void assign_bus_B(){
















}

void perform_operation_ALU(){

































}

void assign_bus_C(){









}

void operate_memory(){




}

void jump(){



}

void display_processes(){

























































}


//FUNÇÃO RESPONSAVEL POR PRINTAR OS valueES EM binary
//TIPO 1: Imprime o binário of 4 bytes seguiofs 
//TIPO 2: Imprime o binário of 1 byte
//TIPO 3: Imprime
//TIPO 4: Imprime o binário of uma microinstrução
//TIPO 5: Imprime o binário ofs 9 bits of MPC
void binary(void* value, int tipo){






















































}
