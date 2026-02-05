/* Nesse arquivo, irei pré-instânciar a FUNÇÃO QUE DESENHARÁ OS GRÁFICOS.

   @author Mateus Aranha - (github.com/matt-aranha)
*/


#ifndef GUI_H
    #define GUI_H

    #include "dados.h"

    // Função que será chamada pelo main.c para abrir a janela gráfica.
    void iniciar_gui_olimpiadas ( Medalhista* lista, int total_medalhistas, int top_n );

#endif