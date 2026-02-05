// MOTOR GRÁFICO //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"
#include "raylib.h"


// ==== CONFIGURAÇÕES VISUAIS ===
#define LARGURA_TELA 1400
#define ALTURA_TELA 800
#define COR_FUNDO RAYWHITE
#define COR_BARRA_F MAROON
#define COR_BARRA_M DARKBLUE
#define COR_HOVER GOLD




// === FUNÇÕES AUXILIARES (PRIVADAS) ===

// Função para verificar se um nome já está na lista de ponteiros salvos (mesma lógica da função em 'leitura.c'):
int nome_ja_exibido_gui ( char** lista_nomes, int qtd_atual, char* nome_novo ) {
    for ( int i = 0 ; i < qtd_atual ; i++ ) {
        if ( strcmp(lista_nomes[i], nome_novo) == 0 ) return 1;                          // Comparar endereços de memória (ponteiros) ou strings.
    }

    return 0;
}


// Função para desenhar o gráfico.
void desenhar_grafico ( Medalhista* lista, int total, int top_n, char genero_alvo, int x_inicio, 
                       int y_inicio, int largura_disp, Color cor_base, char* titulo ) {

    DrawText(titulo, x_inicio, y_inicio - 40, 24, DARKGRAY);                             // Desenhar Título do gráfico.

    int contador = 0;
    int altura_barra = 30;
    int espaco = 15;
    int y_atual = y_inicio;

    // Alocar memória para lista de nomes vistos.
    char** nomes_exibidos = (char**) malloc(top_n * sizeof(char*));

    Vector2 mouse = GetMousePosition();                                                  // Vetor no R2 paa pegar a posição do mouse.

    // Caixa de Informações sobre o medalhista:
    int desenhar_tooltip = 0;
    Medalhista* atleta_foco = NULL;

    for ( int j = 0 ; j < total ; j++ ) {
        // Filtro de gênero e idade
        if ( lista[j].genero == genero_alvo && lista[j].idade_no_evento < 99 ) {

            if ( !nome_ja_exibido_gui(nomes_exibidos, contador, lista[j].nome) ) {

                // Calcular alturas das barras (H_max = 100 anos).
                float tamanho_barra = ( (float) lista[j].idade_no_evento / 100.0f ) * ( (float) largura_disp - 150 );

                Rectangle retangulo = { x_inicio + 150, y_atual, tamanho_barra, altura_barra };
             
                // Interatividade com o mouse.
                int mouse_em_cima = CheckCollisionPointRec(mouse, retangulo);
                Color cor_final = mouse_em_cima ? COR_HOVER : cor_base;

                if ( mouse_em_cima ) {
                    desenhar_tooltip = 1;                                                // desenhar_tooltip = True.
                    atleta_foco = &lista[j];
                }

                // --- DESENHO ---
                    // Desenhar Retângulo.
                    DrawRectangleRec(retangulo, cor_final);

                    // Desenhar Nome do(a) Medalhista. 
                    DrawText( TextFormat("%d. %s", contador + 1, lista[j].nome), x_inicio, y_atual + 5, 16, BLACK );

                    // Desenhar Idade do(a) Medalhista.
                    DrawText( TextFormat("%d anos", lista[j].idade_no_evento), retangulo.x + retangulo.width + 10, y_atual + 5, 16, DARKGRAY);
                //

                nomes_exibidos[contador] = lista[j].nome;
                y_atual += altura_barra + espaco;
                contador++;

                if ( contador >= top_n ) break;
            }
        }
    }

    free( nomes_exibidos );

    // Desenhar tooltip por cima dos gráficos e nomes.
        if ( desenhar_tooltip && atleta_foco != NULL ) {
            int tooltip_x = mouse.x + 15;
            int tooltip_y = mouse.y + 15;
            int tooltip_width = 400;                                                     // Largura do tooltip.
            int tooltip_heigth = 100;                                                    // Altura do tooltip.

            // Impedir que o tooltip saia da tela.
            if ( tooltip_x + tooltip_width > LARGURA_TELA ) tooltip_x = LARGURA_TELA - tooltip_width;
            if ( tooltip_y + tooltip_heigth > ALTURA_TELA ) tooltip_y = ALTURA_TELA - tooltip_heigth;

            // Desenhar fundo e borda.
            DrawRectangle( tooltip_x, tooltip_y, tooltip_width, tooltip_heigth, RAYWHITE );
            DrawRectangleLines( tooltip_x, tooltip_y, tooltip_width, tooltip_heigth, BLACK );
        
            // Exibir informações detalhadas dos atletas.
            DrawText( TextFormat("País: %s", atleta_foco->pais), tooltip_x + 10, tooltip_y + 10, 20, DARKBLUE);
            DrawText( TextFormat("Evento: %s", atleta_foco->modalidade), tooltip_x + 10, tooltip_y + 35, 16, BLACK);
            DrawText( TextFormat("Medalha: %s (%d)", atleta_foco->medalha, atleta_foco->ano_olimpiada), tooltip_x + 10, tooltip_y + 60, 16, DARKGRAY);
        }
    //
}




// === FUNÇÃO PRINCIPAL ===
void iniciar_gui_olimpiadas (Medalhista* lista, int total_medalhistas, int top_n ) {
    InitWindow( LARGURA_TELA, ALTURA_TELA, "Gráficos - Questão 13 (Versão Original)");
        SetTargetFPS(60);

        int margem_x = 50;
        int margem_y = 100;
        int largura_coluna = ( LARGURA_TELA - (margem_x * 3)) / 2;

        while ( !WindowShouldClose() ) {
            BeginDrawing();
                ClearBackground(COR_FUNDO);

                // Título principal:
                DrawText( "TOP 10 MEDALHISTAS MAIS VELHOS", LARGURA_TELA/2 - 200, 30, 30, BLACK );
                DrawText( "[Para mais detalhes, passe o mouse por cima do atleta]", LARGURA_TELA/2 - 220, 65, 16, GRAY );

                // Gráfico Feminio (Esquerda):
                desenhar_grafico(lista, total_medalhistas, top_n, 'F', margem_x, margem_y, largura_coluna, COR_BARRA_F, "FEMININO");

                // Gráfico Masculino (Direita):
                desenhar_grafico(lista, total_medalhistas, top_n, 'M', margem_x * 2 + largura_coluna, margem_y, largura_coluna, COR_BARRA_M, "MASCULINO" );
            EndDrawing();
        }
    CloseWindow();
}