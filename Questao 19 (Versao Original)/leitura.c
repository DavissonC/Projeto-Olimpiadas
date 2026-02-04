#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "leitura.h"


// === CONFIGURAÇÃO DAS COLUNAS DOS .CSV ===

// Arquivo de ATLETAS ('bios.csv'):
// Col 1: Sex | Col 3: Name | Col 4: Born | Col 7: ID
#define A_COL_GENERO 1
#define A_COL_NOME 3
#define A_COL_NASCIMENTO 4
#define A_COL_ID 7

// Arquivo de RESULTADOS ('results.csv'):
// Col 0: Games | Col 4: Medal | Col 6: ID | Col 7: NOC | Col 8: Discipline
#define R_COL_EDICAO 0
#define R_COL_MEDALHA 4
#define R_COL_ID 6
#define R_COL_NOC 7
#define R_COL_ESPORTE 8

#define BUFFER_SIZE 4096           // Aumentado para garantir a leitura de linhas longas.




// === FUNÇÕES AUXILIARES (PRIVADAS) ===

// Função para remover aspas da string:
void limpar_string ( char* string ) {
    if ( !string ) return;
    
    int length = strlen(string);
    if ( length == 0 ) return;
    
    // remove aspas do início:
    if ( string[0] == '"' ) {
        memmove(string, string + 1, length);
        length--;
    }

    // remove aspas do fim:
    if ( length > 0 && string[length - 1] == '"' ) {
        string[length - 1] = '\0';
    }
}

// Função para extrair o primeiro ano (4 dígitos) encontrados em uma string:
int extrair_ano_string ( char* string ) {
    if ( !string ) return 0;

    int length = strlen(string);
    for ( int i = 0 ; i < length - 3 ; i++ ) {
        // verifica se encontrou 4 dígitos seguidos
        if ( isdigit(string[i]) && isdigit(string[i+1]) && isdigit(string[i+2]) && isdigit(string[i+3]) ) {
            char ano_buf[5];
            strncpy(ano_buf, &string[i], 4);
            ano_buf[4] = '\0';
            return atoi(ano_buf);
        }
    }
    return 0;           // retorna 0 se não achar um ano válido.
}

// Função para obter o campo específico de uma linha CSV:
char* get_csv_field( char* line, int number ) {
    char* dup = strdup(line);
    char* result = NULL;
    char* ptr = dup;

    // Avançar N ('number') vezes pelas virgulas do arquivo .csv.
    for ( int i = 0 ; i < number ; i++ ) {
        ptr = strchr(ptr, ',');
        if ( !ptr ) {
            // Se a String acabar antes de chegar na coluna desejada:
            free(dup);
            return NULL;
        }
        ptr++;                                                                          // Pular a vírgula p/ ir ao próximo campo.
    }

    // 'ptr' aponta para o ínicio do campo. Para achar onde o campo termina (próx. ',' ou final da linha):
    char* end = strchr(ptr, ',');

    if ( end ) {
        *end = '\0';                                                                    // Corta a string na vírgula.

    } else {
        // Se não houver mais vírgula, remove o '\n' do final.
        end = strchr(ptr, '\n');
        if ( end ) *end = '\0';                                                         // Troca a quebra de linha por 'vazio'.
    }

    // Se o campo estiver vazio (,,), ptr já aponta para \0. strdup cria a string vazia
    result = strdup(ptr);

    free(dup);
    return result;
}




// === FUNÇÕES PRINCIPAIS (DO HEADER) ===

// Função para carregar os dados dos atletas:
int carregar_atletas ( const char* caminho_arquivo, Atleta *banco_dados ) {
    FILE* f = fopen(caminho_arquivo, "r");                                               // Abrir arquivo no modo leitura.

        if ( !f ) {
            printf( "Erro ao abrir arquivo de atletas: %s\n", caminho_arquivo );
            return -1;
        }

        char linha[BUFFER_SIZE];
        int contador = 0;

        // Pular cabeçalho do aquivo.
        fgets(linha, BUFFER_SIZE, f);

        // Loop de leitura ("Parsing").
        while ( fgets(linha, BUFFER_SIZE, f) ) {
            char* s_id = get_csv_field(linha, A_COL_ID);
            char* s_nome = get_csv_field(linha, A_COL_NOME);
            char* s_genero = get_csv_field(linha, A_COL_GENERO);
            char* s_nascimento = get_csv_field(linha, A_COL_NASCIMENTO);

            if ( s_id && s_nome && s_genero ) {
                int id = atoi(s_id);

                // Verificar se o ID é válido para usar como índice.
                if ( id > 0 ) {
                    banco_dados[id].id = id;

                    limpar_string(s_nome);
                    strncpy(banco_dados[id].nome, s_nome, MAX_NOME - 1);
                    banco_dados[id].nome[MAX_NOME - 1] = '\0';                           // Garantir que a terminação seja nula.

                    // Converter Male/Female => M/F.
                    limpar_string(s_genero);
                    char g = s_genero[0];                                                // Pega a primeira letra.
                    if ( g == 'M' || g == 'm' ) banco_dados[id].genero = 'M';            // Se a primeira letra for 'M' ou 'm', muda o genero para 'M'.
                    else if ( g == 'F' || g == 'f' ) banco_dados[id].genero = 'F';       // Se a primeira letra for 'F' ou 'f', muda o genero para 'F'.
                    else banco_dados[id].genero = '?';                                   // Gênero desconhecido.

                    // Extrair ano de nascimento do atleta.
                    if ( s_nascimento ) {
                        banco_dados[id].ano_nascimento = extrair_ano_string(s_nascimento);

                    } else {
                        banco_dados[id].ano_nascimento = 0;                              // Erro ao ler.
                    }

                    contador++;                                                          // Atualizar a quantidade de atletas lidos.
                }
            }

            // Liberar memória auxiliar das string.
            if ( s_id ) free( s_id );
            if ( s_nome ) free( s_nome );
            if ( s_genero ) free( s_genero );
            if ( s_nascimento ) free( s_nascimento );
        }
    
    fclose(f);                                                                           // Fechar o arquivo.
    return contador;                                                                     // Retornar o total de atletas lidos.
}


// Função para processar os resultados dos medalhistas:
int processar_resultados ( const char* caminho_arquivo, Atleta* banco_dados, Medalhista* lista_final ) {
    FILE* f = fopen(caminho_arquivo, "r");                                               // Abrir aquivo no modo leitura.
        if ( !f ) {
            printf( "Erro ao abrir o arquivo de resultados: %s\n", caminho_arquivo );
            return 0;
        }

        char linha[BUFFER_SIZE];
        int contador = 0;

        // Pular cabeçalho do arquivo.
        fgets(linha, BUFFER_SIZE, f); 
    
        // Loop de leitura.
        while ( fgets(linha, BUFFER_SIZE, f) ) {
            char* s_id = get_csv_field(linha, R_COL_ID);
            char* s_noc = get_csv_field(linha, R_COL_NOC);
            char* s_edicao = get_csv_field(linha, R_COL_EDICAO);
            char* s_medalha = get_csv_field(linha, R_COL_MEDALHA);
            char* s_esporte = get_csv_field(linha, R_COL_ESPORTE);

            // Filtrar se o atleta é medalhista:
            if ( s_medalha && strlen(s_medalha) > 0 && strcmp(s_medalha, "NA") != 0 ) {

                int id = atoi(s_id);

                // Verificar se o atleta existe no banco carregado.
                if ( banco_dados[id].id != 0 ) {
                    
                    int ano_jogo = extrair_ano_string(s_edicao);
                    int ano_nasc = banco_dados[id].ano_nascimento;

                    // Validar as datas.
                    if ( ano_nasc > 1800 && ano_jogo > 1890 ) {

                        // Copiar dados do Atleta (Memória).
                        strcpy(lista_final[contador].nome, banco_dados[id].nome);
                        lista_final[contador].genero = banco_dados[id].genero;

                        // Copiar dados do Resultado (CSV atual).
                        limpar_string(s_medalha);
                        strcpy(lista_final[contador].medalha, s_medalha);

                        // Copiar dados do Esporte.
                        limpar_string(s_esporte);
                        strncpy(lista_final[contador].modalidade, s_esporte, MAX_EVENTO - 1);
                        lista_final[contador].modalidade[MAX_EVENTO - 1] = '\0';

                        // Salvar o NOC do Medalhista.
                        if ( s_noc ) {
                            limpar_string(s_noc);
                            strncpy(lista_final[contador].noc, s_noc, MAX_NOC - 1);
                            lista_final[contador].noc[MAX_NOC - 1] = '\0';

                        } else {
                            strcpy(lista_final[contador].noc, "???");
                        }

                        lista_final[contador].ano_olimpiada = ano_jogo;

                        // Calcular idade do Atleta.
                        lista_final[contador].idade_no_evento = ano_jogo - ano_nasc;

                        contador++;
                    }
                }
            }

            // Liberar memória auxiliar das strings.
            if ( s_id ) free( s_id );
            if ( s_noc ) free( s_noc );
            if ( s_edicao ) free( s_edicao );
            if ( s_medalha ) free( s_medalha );
            if ( s_esporte ) free( s_esporte );
        }

    fclose(f);                                                                           // Fechar o arquivo.
    return contador;                                                                     // Retorna a quantidade de atletas medalhistas.
}