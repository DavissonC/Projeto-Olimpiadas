#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analise.h"




// === FUNÇÕES AUXILIARES (PRIVADAS) ===

// Função para comparar quem vem antes de quem (Importante para o qsort):
    // Retorna:
    //          > 0 se A deve vir DEPOIS de B
    //          < 0 se A deve vir ANTES de B
    //          0 se A e B forem iguais
int comparar_idade_decrescente ( const void* a, const void* b ) {
    Medalhista* m1 = (Medalhista *)a;
    Medalhista* m2 = (Medalhista *)b;

    // Critério 1: Idade (Maior p/ Menor).
    if ( m2->idade_no_evento != m1->idade_no_evento ) {
        return m2->idade_no_evento - m1->idade_no_evento;
    }

    // Critério 2: Nome (Ordem Alfabética).
    return strcmp(m1->nome, m2->nome);
}


// Função para verificar se um nome já está na lista de ponteiros salvos:
int nome_ja_exibido (char** lista_nomes, int qtd_atual, char* nome_novo ) {
    for ( int k = 0 ; k < qtd_atual ; k++ ) {
        if ( strcmp(lista_nomes[k], nome_novo) == 0 ) {
            return 1;                                                                    // Verdadeiro => Nome já impresso.
        }
    }
    return 0;                                                                            // Falso => Nome novo.
}




// === FUNÇÕES PRINCIPAIS (DO HEADER) ===

// Função para ordenar o vetor de medalhistas do mais velho para o mais novo:
void ordenar_medalhistas (Medalhista* lista, int total_medalhistas ) {

    // qsort(array, tamanho, tamanho_elemento, funcao_comparadora)
    qsort( lista, total_medalhistas, sizeof(Medalhista), comparar_idade_decrescente );
}


// Função para exibir o relatório final (Top N por gênero).
void exibir_ranking_idade (Medalhista* lista, int total_medalhistas, int top_n ) {

    // TÍTULO GERAL:
    printf( "\n===================================================================\n" );
    printf( " RELATÓRIO: TOP %d MEDALHISTAS MAIS VELHOS DA HISTÓRIA POR GÊNERO\n", top_n );
    printf( "===================================================================\n" );


    // Alocar dinâmicamente a memória de um array de ponteiros para strings (char*).
    char** nomes_impressos = (char**) malloc(top_n * sizeof(char*));


    // LOOP 1 - FEMININO:
    printf( "\n--- FEMININO ---\n" );
    int contador = 0;

    for ( int i = 0 ; i < total_medalhistas ; i++ ) {
        // Filtrar por gênero 'F'.
        if ( lista[i].genero == 'F' ) {

            if ( lista[i].idade_no_evento > 0 && lista[i].idade_no_evento < 98 ) {

                // Verificar se a atleta já apareceu no ranking.
                if ( !nome_ja_exibido(nomes_impressos, contador, lista[i].nome) ) {

                    printf( "%d. %s (%s)\n", contador + 1, lista[i].nome, lista[i].pais );
                    printf( "   Idade: %d anos | Medalha: %s\n", lista[i].idade_no_evento, lista[i].medalha );
                    printf( "   Evento: %s (%d)\n", lista[i].modalidade, lista[i].ano_olimpiada );
                    printf( "-------------------------------------------------------\n" );

                    nomes_impressos[contador] = lista[i].nome;
                    contador++;
                }

                if ( contador >= top_n ) break;                                          // Para assim que completar o Top N.
            }
        }
    }

    if ( contador == 0 ) printf( "Nenhum registro encontrado.\n" );


    // LOOP 2 - MASCULINO:
    printf( "\n--- MASCULINO ---\n" );
    contador = 0;                                                                        // Reiniciar o contador.

    for ( int i = 0 ; i < total_medalhistas ; i++ ) {
        // Filtrar por gênero 'F'.
        if ( lista[i].genero == 'M' ) {

            if ( lista[i].idade_no_evento > 0 && lista[i].idade_no_evento < 98 ) {

                if ( !nome_ja_exibido(nomes_impressos, contador, lista[i].nome) ) {

                    printf( "%d. %s (%s)\n", contador + 1, lista[i].nome, lista[i].pais );
                    printf( "   Idade: %d anos | Medalha: %s\n", lista[i].idade_no_evento, lista[i].medalha );
                    printf( "   Evento: %s (%d)\n", lista[i].modalidade, lista[i].ano_olimpiada );
                    printf( "-------------------------------------------------------\n" );

                    nomes_impressos[contador] = lista[i].nome;
                    contador++;
                }

                if ( contador >= top_n ) break;                                          // Para assim que completar o Top 10).
            }
        }
    }
    
    if ( contador == 0 ) printf( "Nenhum registro encontrado.\n" );

    printf( "=======================================================\n" );

    free( nomes_impressos );                                                             // Limpar memória auxiliar.
}