#include "atleta.h"

void GerarArquivosGnuplotRankings(Athlete *athletesMasc, Athlete *athletesFem, int *filledAthletes) {
    // Ranking Masculino
    FILE *fMasc = fopen("ranking_masc.dat", "w");
    if (fMasc != NULL) {
        for (int i = 0; i < filledAthletes[0]; i++) {
            // O uso de %.30s garante que o nome não fique gigante no gráfico
            fprintf(fMasc, "%d \"%.30s\"\n", athletesMasc[i].results, athletesMasc[i].name);
        }
        fclose(fMasc);
    }

    // Ranking Feminino
    FILE *fFem = fopen("ranking_fem.dat", "w");
    if (fFem != NULL) {
        for (int i = 0; i < filledAthletes[1]; i++) {
            fprintf(fFem, "%d \"%.30s\"\n", athletesFem[i].results, athletesFem[i].name);
        }
        fclose(fFem);
    }
}

void GerarCodigoGnuplotRankings() {
    FILE *fCodigo = fopen("grafico_rankings.gp", "w");
    if (fCodigo == NULL) return;

    // Configuração de codificação para evitar símbolos estranhos
    fprintf(fCodigo, "set encoding utf8\n");
    fprintf(fCodigo, "set terminal qt size 1000,800\n");
    
    // Multiplot igual ao da Q9, mudando apenas o título
    fprintf(fCodigo, "set multiplot layout 2,1 title \"Rankings da Questao 19\" font \",16\"\n\n");

    // Estilo de histograma idêntico à Questão 9
    fprintf(fCodigo, "set grid y\n");
    fprintf(fCodigo, "set style data histograms\n");
    fprintf(fCodigo, "set style fill solid 0.8 border -1\n");
    
    // Rotação simples para leitura dos nomes
    fprintf(fCodigo, "set xtics rotate by -45\n");
    fprintf(fCodigo, "set ylabel \"Score\"\n\n");

    // Plotagem do bloco Masculino
    fprintf(fCodigo, "set title \"Top 10 Masculino\" font \",13\"\n");
    fprintf(fCodigo, "plot \"ranking_masc.dat\" using 1:xtic(2) lc rgb \"#4169E1\" title \"Score Atleta\"\n\n");

    // Plotagem do bloco Feminino
    fprintf(fCodigo, "set title \"Top 10 Feminino\" font \",13\"\n");
    fprintf(fCodigo, "set xlabel \"Nome do Atleta\"\n");
    fprintf(fCodigo, "plot \"ranking_fem.dat\" using 1:xtic(2) lc rgb \"#FF69B4\" title \"Score Atleta\"\n\n");

    fprintf(fCodigo, "unset multiplot\n");
    fclose(fCodigo);
}