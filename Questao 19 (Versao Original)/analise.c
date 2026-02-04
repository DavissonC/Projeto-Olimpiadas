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