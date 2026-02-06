#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include "gui.h"
#include "raylib.h"


// ==== CORES ===
Color COR_TEXTO = { 50, 50, 60, 255 };                                                   // Preto.
Color COR_HOVER = { 255, 185, 0, 255 };                                                  // Dourado.
Color COR_BARRA_M = { 0, 80, 160, 255 };                                                 // Azul.
Color COR_BARRA_F = { 190, 40, 60, 255 };                                                // Vermelho.
Color COR_FUNDO = { 230, 230, 235, 255 };                                                // Off-White.




// === FUNÇÕES AUXILIARES ===

// Função para remover caracteres estranhos dos nomes e trocar por espaço:
void corrigir_nome_visual ( char* nome, char* buffer ) {
    strcpy( buffer, nome );
    for ( int i = 0 ; buffer[i] != '\0' ; i++ ) {
        // Se NÃO for letra, NÃO for número, NÃO for hífen e NÃO for espaço ==> Troca por espaço simples.
        if ( !isalnum((unsigned char)buffer[i]) && buffer[i] != '-' && buffer[i] != ' ' && buffer[i] != '\'' ) {
            buffer[i] = ' ';
        }
    }
}


// Função para verificar se um nome já está na lista de ponteiros salvos (mesma lógica da função em 'leitura.c'):
int nome_ja_exibido_gui ( char** lista_nomes, int qtd_atual, char* nome_novo ) {
    for ( int i = 0 ; i < qtd_atual ; i++ ) {
        if ( strcmp(lista_nomes[i], nome_novo) == 0 ) return 1;
    }

    return 0;
}


// Função para desenhar os gráficos:
void desenhar_grafico_responsivo( Medalhista* lista, int total, int top_n, char genero_alvo, 
                                 Rectangle area_desenho, Color cor_base, char* titulo, 
                                 Font fonte_titulos, Font fonte_dados, float scale ) {

    // Centralizar e Desenhar Título da Coluna.
    float tam_fonte_titulo = 28.0f * scale;                                              // Escala o tamanho de acordo com a janela.
    Vector2 dim_titulo = MeasureTextEx( fonte_titulos, titulo, tam_fonte_titulo, 1 );
    float x_titulo_centralizado = area_desenho.x + ( area_desenho.width - dim_titulo.x ) / 2;
    
    DrawTextEx( fonte_titulos, titulo, (Vector2){ x_titulo_centralizado, area_desenho.y - (45 * scale) }, tam_fonte_titulo, 1, COR_TEXTO );

    // Layout:
    float largura_nomes = area_desenho.width * 0.30f;
    float largura_barras = area_desenho.width * 0.60f;
    float altura_item = (area_desenho.height / top_n);
    float altura_barra = altura_item * 0.70f;
    float espaco = altura_item * 0.30f;

    float y_atual = area_desenho.y;
    int contador = 0;

    // Alocar memória para lista de nomes vistos.
    char** nomes_exibidos = (char**) malloc(top_n * sizeof(char*));

    Vector2 mouse = GetMousePosition();                                                  // Vetor no R2 paa pegar a posição do mouse.

    int desenhar_tooltip = 0;
    Medalhista* atleta_foco = NULL;

    for ( int j = 0 ; j < total ; j++ ) {
        // Filtro de gênero e idade.
        if ( lista[j].genero == genero_alvo && lista[j].idade_no_evento < 99 ) {
            
            if ( !nome_ja_exibido_gui( nomes_exibidos, contador, lista[j].nome ) ) {

                // Corrigir nomes dos Medalhistas.
                char nome_exibicao[200];
                corrigir_nome_visual( lista[j].nome, nome_exibicao );

                // === Desenhar Nomes (Utilizando a fonte 'Arial Rounded'):
                    float tamanho_fonte_nome = 22.0f * scale; 
                    Vector2 tamanho_texto = MeasureTextEx( fonte_dados, nome_exibicao, tamanho_fonte_nome, 1 );

                    // Reduz fonte se não couber na largura reservada
                    if ( tamanho_texto.x > largura_nomes - 5 ) {
                        tamanho_fonte_nome = tamanho_fonte_nome * 0.85f; 
                        tamanho_texto = MeasureTextEx( fonte_dados, nome_exibicao, tamanho_fonte_nome, 1 );
                    }

                    // Centralizar verticalmente
                    float y_texto = y_atual + (altura_barra / 2) - (tamanho_texto.y / 2);

                    DrawTextEx( fonte_dados, TextFormat( "%d. %s", contador + 1, nome_exibicao ), 
                            (Vector2){ area_desenho.x, y_texto }, tamanho_fonte_nome, 1, COR_TEXTO );
                //    


                // === Desenhar Barras:
                    float x_barra = area_desenho.x + largura_nomes;
                    float comprimento_barra = ( (float)lista[j].idade_no_evento / 100.0f ) * largura_barras;
                    Rectangle retangulo = { x_barra, y_atual, comprimento_barra, altura_barra };

                    int hover = CheckCollisionPointRec( mouse, retangulo );
                    DrawRectangleRec( retangulo, hover ? COR_HOVER : cor_base );
                //


                // === Desenhar Idades (Utilizando a fonte 'Arial Rounded'):
                    DrawTextEx( fonte_dados, TextFormat( "%d anos", lista[j].idade_no_evento ), 
                            (Vector2){ retangulo.x + retangulo.width + (10 * scale), y_texto }, 
                            20.0f * scale, 1, DARKGRAY );
                //           


                if ( hover ) {
                    desenhar_tooltip = 1;
                    atleta_foco = &lista[j];
                }

                nomes_exibidos[contador] = lista[j].nome;
                y_atual += altura_barra + espaco;
                contador++;

                if ( contador >= top_n ) break;
            }
        }
    }

    free( nomes_exibidos );


    // === Desenhar Tooltip:  
    if ( desenhar_tooltip && atleta_foco != NULL ) {
        float tooltip_w = 450 * scale; 
        float tooltip_h = 110 * scale;
        float tooltip_x = mouse.x + 15;
        float tooltip_y = mouse.y + 15;
        
        // Impedir que o tooltip saia da tela.
        if ( tooltip_x + tooltip_w > GetScreenWidth() ) tooltip_x = mouse.x - tooltip_w - 10;
        if ( tooltip_y + tooltip_h > GetScreenHeight() ) tooltip_y = mouse.y - tooltip_h - 10;

        // Desenhar fundo e borda.
        DrawRectangle(tooltip_x, tooltip_y, tooltip_w, tooltip_h, COR_FUNDO);            // Off-White.
        DrawRectangleLines(tooltip_x, tooltip_y, tooltip_w, tooltip_h, COR_TEXTO);       // Preto.
    
        // Exibir informações detalhadas dos atletas.
        DrawTextEx( fonte_dados, TextFormat( "País: %s", atleta_foco->pais ), (Vector2){ tooltip_x + 10, tooltip_y + 10 }, 22 * scale, 1, COR_BARRA_M );
        DrawTextEx( fonte_dados, TextFormat( "Evento: %s", atleta_foco->modalidade ), (Vector2){ tooltip_x + 10, tooltip_y + (40 * scale) }, 18 * scale, 1, COR_TEXTO );
        DrawTextEx( fonte_dados, TextFormat( "Medalha: %s (%d)", atleta_foco->medalha, atleta_foco->ano_olimpiada ), (Vector2){ tooltip_x + 10, tooltip_y + (70 * scale) }, 18 * scale, 1, BLACK );
    }
}




// === FUNÇÃO PRINCIPAL ===
void iniciar_gui_olimpiadas ( Medalhista* lista, int total_medalhistas, int top_n ) {
   
    SetConfigFlags( FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT );
    

    // Inicia em 720p base
    InitWindow(1280, 720, "Gráficos - Questão 13 (Versão Final)");
        SetTargetFPS(60);


        // Carregar Fontes.
        Font fonte_titulos = LoadFontEx( "../Fontes/consola.ttf", 80, 0, 250 ); 
        SetTextureFilter( fonte_titulos.texture, TEXTURE_FILTER_BILINEAR );

        Font fonte_dados = LoadFontEx( "../Fontes/arial_rounded.ttf", 64, 0, 250 );
        SetTextureFilter( fonte_dados.texture, TEXTURE_FILTER_BILINEAR );


            while ( !WindowShouldClose() ) {
                int screen_width = GetScreenWidth();
                int screen_height = GetScreenHeight();

                // Calcular tamanho da janela (Base 720p).
                float scale = (float)screen_height / 720.0f;
                if ( scale < 0.6f ) scale = 0.6f;                                        // Limitar o scale mínimo para não ficar ilegível em janelas muito pequenas

                float margem_x = screen_width * 0.05f;
                float margem_y = screen_height * 0.15f;
                float largura_util = screen_width - (margem_x * 2);
                float altura_util = screen_height - margem_y - (50 * scale);

                float largura_grafico = (largura_util * 0.47f);
                float x_coluna_masculina = margem_x + largura_grafico + (largura_util * 0.06f);

                BeginDrawing();
                    ClearBackground( COR_FUNDO );

                    // Título Principal Centralizado:
                    const char* texto_titulo = "TOP 10 MEDALHISTAS MAIS VELHOS DA HISTÓRIA";
                    float tam_fonte_titulo_principal = 40.0f * scale;                    // Responsivo
                    Vector2 tamanho_titulo = MeasureTextEx( fonte_titulos, texto_titulo, tam_fonte_titulo_principal, 2 );
                    DrawTextEx( fonte_titulos, texto_titulo, (Vector2){ (screen_width - tamanho_titulo.x)/2, 30 * scale }, tam_fonte_titulo_principal, 2, COR_TEXTO );

                    Rectangle area_feminina = { margem_x, margem_y, largura_grafico, altura_util };
                    Rectangle area_masculina = { x_coluna_masculina, margem_y, largura_grafico, altura_util };

                    desenhar_grafico_responsivo( lista, total_medalhistas, top_n, 'F', area_feminina, COR_BARRA_F, "FEMININO", fonte_titulos, fonte_dados, scale );
                    desenhar_grafico_responsivo( lista, total_medalhistas, top_n, 'M', area_masculina, COR_BARRA_M, "MASCULINO", fonte_titulos, fonte_dados, scale );

                EndDrawing();
            }

        // Descarregar Fontes.    
        UnloadFont(fonte_titulos);
        UnloadFont(fonte_dados);
        
    CloseWindow();
}