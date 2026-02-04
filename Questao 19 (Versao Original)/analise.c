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
    printf( " RELATÓRIO: 10 MEDALHISTAS MAIS VELHOS DA HISTÓRIA POR GÊNERO\n" );
    printf( "===================================================================\n" );


    // LOOP 1 - FEMININO:
    printf( "\n--- FEMININO ---\n" );
    int contador = 0;

    for ( int i = 0 ; i < total_medalhistas ; i++ ) {
        // Filtrar por gênero 'F'.
        if ( lista[i].genero == 'F' ) {

            if ( lista[i].idade_no_evento > 0 && lista[i].idade_no_evento < 98 ) {

                printf( "%d. %s (%s)\n", contador + 1, lista[i].nome, lista[i].noc );
                printf( "   Idade: %d anos | Medalha: %s\n", lista[i].idade_no_evento, lista[i].medalha );
                printf( "   Evento: %s (%d)\n", lista[i].modalidade, lista[i].ano_olimpiada );
                printf( "-------------------------------------------------------\n" );

                contador++;
                if ( contador >= top_n ) break;                                             // Para assim que completar o Top N.
            }
        }
    }
    if ( contador == 0 ) printf( "Nenhum registro encontrado.\n" );


    // LOOP 2 - MASCULINO:
    printf( "\n--- MASCULINO ---\n" );
    int contador = 0;

    for ( int i = 0 ; i < total_medalhistas ; i++ ) {
        // Filtrar por gênero 'F'.
        if ( lista[i].genero == 'M' ) {

            if ( lista[i].idade_no_evento > 0 && lista[i].idade_no_evento < 98 ) {

                printf( "%d. %s (%s)\n", contador + 1, lista[i].nome, lista[i].noc );
                printf( "   Idade: %d anos | Medalha: %s\n", lista[i].idade_no_evento, lista[i].medalha );
                printf( "   Evento: %s (%d)\n", lista[i].modalidade, lista[i].ano_olimpiada );
                printf( "-------------------------------------------------------\n" );

                contador++;
                if ( contador >= top_n ) break;                                             // Para assim que completar o Top 10).
            }
        }
    }
    if ( contador == 0 ) printf( "Nenhum registro encontrado.\n" );

    printf( "=======================================================\n" );
}