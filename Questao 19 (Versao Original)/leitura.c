# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include "leitura.h"


// === CONFIGURAÇÃO DAS COLUNAS DOS .CSV ===

// Arquivo de ATLETAS ('bios.csv'):
// Col 1: Sex | Col 3: Name | Col 4: Born | Col 7: ID
# define A_COL_GENERO 1
# define A_COL_NOME 3
# define A_COL_NASCIMENTO 4
# define A_COL_ID 7

// Arquivo de RESULTADOS ('results.csv'):
// Col 0: Games | Col 4: Medal | Col 6: ID | Col 8 Discipline
# define R_COL_EDICAO 0
# define R_COL_MEDALHA 4
# define R_COL_ID 6
# define R_COL_ESPORTE 8

# define BUFFER_SIZE 4096           // Aumentado para garantir a leitura de linhas longas.




// === FUNÇÕES AUXILIARES (PRIVADAS)===

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
    char* tok;
    char* result = NULL;
    int i = 0;

    for ( tok = strtok(dup, ",;\n") ; tok ; tok = strtok(NULL, ",;\n") ) {
        if ( i == number ) {
            result = strdup(tok);
            break;
        }
        i++;
    }
    free(dup);
    return result;
}