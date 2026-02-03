// Calcula os 10 atletas mais jovens com pior colocação de todos os jogos olímpicos

// Dados de cada atleta (nome, país, gênero, pior posição)
typedef struct
{
    char name[100];
    char country[40];
    char sex[12];
    int worstPosition;
    int age;
    int results;
} Athlete;

// Funções pré instanciadas
int CalculaPioresAtletasNovos(Athlete *mascAthletes, Athlete *femAthletes, int *biggestResults, int *preenchidos, Athlete nextAthlete);
int AlteraListaDosPiores(Athlete *athletesList, Athlete athlete, int *biggestResults, int *preenchidos);
int CalculaResultados(int age, int position);

int main()
{
    // Listas que irão armazenar os dados dos 10 atletas mais novos e de pior colocação 
    Athlete mascAthletes[10];
    Athlete femAthletes[10];

    // Maiores resultados armazenados nas listas
    int biggestResults[2] = {1000000, 1000000};

    // Quantidade de atletas já inseridos nas listas
    int preenchidos[2] = {0, 0};

    // Verifica se ainda há algum atleta no banco de dados ou se já podemos parar o laço
    int hasMoreAthletes = 1;

    while(hasMoreAthletes)
    {

    }

    return 0;
}

int CalculaPioresAtletasNovos(Athlete *mascAthletes, Athlete *femAthletes, int *biggestResults, int *preenchidos, Athlete nextAthlete)
{
     // Pontuação ponderada baseada na idade e na posição do atleta 
    int athleteResults = CalculaResultados(nextAthlete.age, nextAthlete.worstPosition);

    // Divisão por sexo dos atletas
    if(strcmp(nextAthlete.sex, "Male") == 0)
    {
        // Esse atleta tem pontuação menor que o melhor da lista dos piores?
        if(athleteResults < *biggestResults)
        {
            AlteraListaDosPiores(mascAthletes, nextAthlete, biggestResults, preenchidos);
        }
    }
    else
    {
        // Esse atleta tem pontuação menor que o melhor da lista dos piores?
        if(athleteResults < *(biggestResults + 1))
        {
            AlteraListaDosPiores(femAthletes, nextAthlete, biggestResults + 1, preenchidos + 1);
        }
    }
}

int AlteraListaDosPiores(Athlete *athletesList, Athlete athlete, int *biggestResults, int *preenchidos)
{
    // Se todas os espaços ainda não tiverem sido preenchidos, preenche-os
    if (*preenchidos < 10)
    {
        *(athletesList + *preenchidos) = athlete;
        *preenchidos += 1;
        (athletesList + *preenchidos) -> results = CalculaResultados(athlete.age, athlete.worstPosition);
    }
    // Senão, substitui o de maior pontuação (último da lista)
    else
    {
        *(athletesList + 9) = athlete;
        (athletesList + 9) -> results = CalculaResultados(athlete.age, athlete.worstPosition);
    }

    // Reorganiza a lista para que esteja em ordem crescente de pontuação
    for (int i = 0; i < *preenchidos - 1; i++) {
        for (int j = 0; j < *preenchidos - i - 1; j++) {

            // Se o resultado do próximo for menor, ele é "pior" e deve subir (ordenar crescente)
            if (athletesList[j].results > athletesList[j + 1].results) {
                Athlete temp = athletesList[j];
                athletesList[j] = athletesList[j + 1];
                athletesList[j + 1] = temp;
            }
        }
    }

    if (*preenchidos == 10) {
        *biggestResults = athletesList[9].results;
    }
}

int CalculaResultados(int age, int position)
{
    return (age * 1000) - position;
}