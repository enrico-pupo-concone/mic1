// Emulador2.c
#include <stdio.h>

// Carrega o microprograma de controle:
void carregar_microprograma_controle(void);

// Carrega o nome do programa principal:
void carregar_programa(const char *nome_do_programa);

// Exibe os processos em execução:
void exibir_processos(void);

// Decodifica a microinstrução atual:
void decodificar_microinstrucao_atual(void);

// Atribui o valor ao barramento B:
void atribuir_barramento_B(void);

// Realiza a operação da Unidade Lógica e Aritmética:
void executar_operacao_ALU(void);

// Atribui o resultado ao barramento C:
void atribuir_barramento_C(void);

// Executa operações de leitura/escrita na memória:
void executar_operacao_leitura_escrita_memoria(void);

// Realiza um salto de instrução:
void executar_pulo_instrucao(void);

// Exibe valor em formato binário:
void exibir_valor_tipo_binario(void *valor, int tipo);
