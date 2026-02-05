/*
 * ==========================================================================================================
 * @brief OLYMPIC DATA ANALYSIS / ANÁLISE DE DADOS OLÍMPICOS
 * ==========================================================================================================
 * 
 * @section [PT] DESCRIÇÃO DA QUESTÃO 19 (VERSÃO ORIGINAL):
 * Este programa processa bases de dados históricos das Olímpiadas (1896-2022) para responder
 * a seguinte pergunta não-trivial — proposta pelo Prof. Dr. Kalil Bispo: "Faça o ranking dos
 * 10 atletas medalhistas mais velhos de todos os tempos que participaram das olimpíadas, por
 * gênero."
 * 
 * ARQUIVOS DE ENTRADA:
 * - "bios.csv" (Dados Demográficos dos Atletas)
 * - "results.csv" (Resultados e Medalhas dos Atletas)
 * Ambos arquivos foram retirados do seguinte repositório: https://github.com/KeithGalli/Olympics-Dataset
 * 
 * ----------------------------------------------------------------------------------------------------------
 * 
 * @section [EN] DESCRIPTION OF QUESTION 19 (ORIGINAL VERSION):
 * This program processes historical Olympic datasets (1896-2022) to answer the following non-trivial
 * question, proposed by Professor Kalil Bispo, Ph.D.: "Rank the 10 oldest Olympic medal-winning athletes
 * of all time, by gender."
 * 
 * INPUT FILES:
 * - "bios.csv" (Demographic data of the Athletes)
 * - "results.csv" (Results and medals of the Athletes)
 * Both files were taking of the following repository: https://github.com/KeithGalli/Olympics-Dataset
 * 
 * ==========================================================================================================
 * @author Mateus Aranha (github.com/matt-aranha)
 * @version 1.0
 * ==========================================================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include "dados.h"
#include "leitura.h"
#include "analise.h"

#define MAX_ATLETAS 200000                                                               // No arquivo 'bios.csv', o ID vai até 149.814
#define MAX_MEDALHISTAS 50000




int main () {

    printf( "--- SISTEMA DE ANALISE OLIMPICA ---\n" );


    // === ALOCAR MEMÓRIA DINÂMICAMENTE (HEAP):
        printf( "Iniciando Alocacao de Memoria..." );
        
        Atleta* bd_atletas = (Atleta *) calloc(MAX_ATLETAS, sizeof(Atleta));
        Medalhista* lista_final = (Medalhista *) calloc(MAX_MEDALHISTAS, sizeof(Medalhista));
        
        // Verificação de segurança caso falte memória RAM.
        if ( bd_atletas == NULL || lista_final == NULL ) {
            printf( "\nERRO: Falha na alocacao de memoria. Tente Reduzir os valores MAX\n");
            return 1;
        }
    //


    // === LEITURA DOS ATLETAS:
        printf( "\nLendo arquivo de atletas..." );
        int qtd_atletas = carregar_atletas("../Banco de Dados/bios.csv", bd_atletas);

        if ( qtd_atletas <= 0 ) {
            printf( "\nERRO: Nenhum atleta lido. Verifique o nome do arquivo ou formato.\n" );
            free(bd_atletas);
            free(lista_final);
            return 1;
        }
        printf( "\nExito! (%d registros carregados)\n", qtd_atletas );
    //


    // === PROCESSAMENTO E CRUZAMENTO DE DADOS (JOIN):
        printf( "\nLendo arquivo de resultados e cruzando os dados..." );
        int qtd_medalhistas = processar_resultados("../Banco de Dados/results.csv", bd_atletas, lista_final);

        if ( qtd_medalhistas == 0 ) {
            printf( "\nAVISO: Nenhum medalhista encontrado. Verifique os filtros de leitura.\n");

        } else {
            printf( "\nExito! (%d medalhas processadas)\n", qtd_medalhistas );

            printf( "\nOrdenando ranking por idade... " );
            ordenar_medalhistas(lista_final, qtd_medalhistas);
            printf( "Concluido:\n" );

            exibir_ranking_idade(lista_final, qtd_medalhistas, 10);
        }
    //


    // === LIBERAR MEMÓRIA ALOCADA:
    free( bd_atletas );
    free( lista_final );

    printf( "\nPrograma finalizado com sucesso.\n");
    
    return 0;
}