#include "atleta.h"

int main()
{
    ConfigurarTerminal();

    int disciplinesLimit = 300;
    DisciplineStats *disciplines = calloc(disciplinesLimit, sizeof(DisciplineStats)); 
    int disciplinesQuantity = 0;

    // Listas que irão armazenar os dados dos 10 atletas mais novos e de pior colocação 
    Athlete athletesMasc[10], athletesFem[10];

    // Lista para armazenar todos os atletas
    int athletesLimit = 150000;
    Athlete *allAthletes = calloc(athletesLimit, sizeof(Athlete));

    // Maiores resultados armazenados nas listas
    int cuttingResults[2] = {1000000, 1000000};

    // Quantidade de atletas já inseridos nas listas
    int filledAthletes[2] = {0, 0};

    CarregarBiografias(allAthletes, athletesLimit);

    disciplinesQuantity = MapearPiorPosicaoEsportes(disciplines, disciplinesLimit);

    ProcessarResultadosIndividuais(allAthletes, athletesLimit);

    CalcularRankingsFinais(allAthletes, athletesLimit, disciplines, disciplinesQuantity, athletesMasc, athletesFem, cuttingResults, filledAthletes);

    ExibirResultados(athletesMasc, athletesFem, filledAthletes);

    GerarArquivosGnuplotRankings(athletesMasc, athletesFem, filledAthletes);

    GerarCodigoGnuplotRankings();

    system("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -p grafico_rankings.gp");

    free(allAthletes);
    free(disciplines);
    return 0;
}