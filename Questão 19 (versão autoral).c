#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Calcula os 10 atletas mais jovens com pior colocação de todos os jogos olímpicos

// Dados de cada atleta (nome, país, gênero, pior posição)
typedef struct
{
    char name[100];
    char country[40];
    char sex[12];
    char born[50]; 
    int id;
    int Position;
    int age;
    int results;
} Athlete;

// Funções pré instanciadas
int CalculaPioresAtletasNovos(Athlete *mascAthletes, Athlete *femAthletes, int *biggestResults, int *preenchidos, Athlete nextAthlete);
int AlteraListaDosPiores(Athlete *athletesList, Athlete athlete, int *biggestResults, int *preenchidos);
int CalculaResultados(int age, int position);
int CompararAtletasPorID(const void *a, const void *b);

int main()
{
    // Listas que irão armazenar os dados dos 10 atletas mais novos e de pior colocação 
    Athlete mascAthletes[10];
    Athlete femAthletes[10];

    // Lista para armazenar todos os atletas
    int actualLimit = 150000;
    Athlete *allAthletes = malloc(sizeof(Athlete) * actualLimit);
    int totalAcessedAthletes = 0;

    // Maiores resultados armazenados nas listas
    int biggestResults[2] = {1000000, 1000000};

    // Quantidade de atletas já inseridos nas listas
    int preenchidos[2] = {0, 0};

    FILE *bios = fopen("bios.csv", "r");

    // Verifica se o arquivo foi aberto corretamente
    if (bios == NULL) 
    {
        printf("Erro ao abrir o ficheiro bios.csv!");
        return 1;
    }

    // Escaneia cada atleta e guarda os dados necessários para a resolução
    while(fscanf(bios, " %*[^,],%[^,],%*[^,],%[^,],%[^,],%*[^,],%[^,],%d%*[^ \n]"
        ,allAthletes[totalAcessedAthletes].sex, allAthletes[totalAcessedAthletes].name, 
        allAthletes[totalAcessedAthletes].born, allAthletes[totalAcessedAthletes].country, 
        &allAthletes[totalAcessedAthletes].id) == 5)
    {
        totalAcessedAthletes++;
        if (totalAcessedAthletes >= actualLimit)
        {
            actualLimit *= 2;
            Athlete *temp = realloc(allAthletes, sizeof(Athlete) * actualLimit);
            if (temp != NULL) 
            {
                allAthletes = temp;
            } 
            else 
            {
                free(allAthletes);
                return 1;
            }
        }
    }

    fclose(bios);

    // Ordenamos a lista com base nos IDs, para garantir que depois, possamos usar a busca binária corretamente
    qsort(allAthletes, totalAcessedAthletes, sizeof(Athlete), CompararAtletasPorID);

    // Agora, abrimos o arquivo de resultados para enfim, fazer o cálculo dos TOP 10
    FILE *results = fopen("results.csv", "r");

    if (results == NULL) 
    {
        printf("Erro ao abrir o ficheiro results.csv!");
        return 1;
    }

    // searchKey é uma struct vazia para guardar o ultimo ID lido em resultados
    // found é um ponteiro que deve apontar para a localização exata do atleta na lista de atletas
    Athlete searchKey, *found;
    
    char Game[50];
    int yearOfGame;
    
    while(fscanf(results, " %[^,],%*[^,],%*[^,],%d,%*[^,],%*[^,],%d%*[^ \n]", Game, &searchKey.Position, &searchKey.id) == 3)
    {
        // Executamos uma busca binária pelo atleta atual
        found = bsearch(&searchKey, allAthletes, totalAcessedAthletes, sizeof(Athlete), CompararAtletasPorID);

        if (found != NULL)
        {
            // Obtemos a idade do atleta
            if (sscanf(Game, "%d", &yearOfGame) == 1) 
            {
                // Calcula a idade baseada no ano da Olimpíada
                int birthYear;
                sscanf(found->born, "%d", &birthYear); 
                found->age = yearOfGame - birthYear;
                found->Position = searchKey.Position;
            }
            // Tenta inserir nas listas Top 10
            CalculaPioresAtletasNovos(mascAthletes, femAthletes, biggestResults, preenchidos, *found);
        }
    }

    printf("\nTOP 10 PIORES E MAIS JOVENS - MASCULINO\n");
    for(int i=0; i<preenchidos[0]; i++) 
        printf("%d. %-20s | Idade: %d | Pos: %d | Score: %d\n", i+1, mascAthletes[i].name, mascAthletes[i].age, mascAthletes[i].Position, mascAthletes[i].results);

    printf("\nTOP 10 PIORES E MAIS JOVENS - FEMININO\n");
    for(int i=0; i<preenchidos[1]; i++) 
        printf("%d. %-20s | Idade: %d | Pos: %d | Score: %d\n", i+1, femAthletes[i].name, femAthletes[i].age, femAthletes[i].Position, femAthletes[i].results);

    fclose(results);

    free(allAthletes);
    return 0;
}

int CalculaPioresAtletasNovos(Athlete *mascAthletes, Athlete *femAthletes, int *biggestResults, int *preenchidos, Athlete nextAthlete)
{
     // Pontuação ponderada baseada na idade e na posição do atleta 
    nextAthlete.results = CalculaResultados(nextAthlete.age, nextAthlete.Position);

    // Divisão por sexo dos atletas
    if(strcmp(nextAthlete.sex, "Male") == 0)
    {
        // Esse atleta tem pontuação menor que o melhor da lista dos piores?
        if(nextAthlete.results < *biggestResults)
        {
            AlteraListaDosPiores(mascAthletes, nextAthlete, biggestResults, preenchidos);
        }
    }
    else
    {
        // Esse atleta tem pontuação menor que o melhor da lista dos piores?
        if(nextAthlete.results < *(biggestResults + 1))
        {
            AlteraListaDosPiores(femAthletes, nextAthlete, biggestResults + 1, preenchidos + 1);
        }
    }

    return 0;
}

int AlteraListaDosPiores(Athlete *athletesList, Athlete athlete, int *biggestResults, int *preenchidos)
{
    // Se todas os espaços ainda não tiverem sido preenchidos, preenche-os
    if (*preenchidos < 10)
    {
        *(athletesList + *preenchidos) = athlete;
        *preenchidos += 1;
    }
    // Senão, substitui o de maior pontuação (último da lista)
    else
    {
        *(athletesList + 9) = athlete;
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

    if (*preenchidos == 10) 
    {
        *biggestResults = athletesList[9].results;
    }
    else  
    {
        *biggestResults = 1000000; // Garante entrada até encher o Top 10
    }

    return 0;
}

int CalculaResultados(int age, int position)
{
    return (age * 1000) - position;
}

int CompararAtletasPorID(const void *a, const void *b) 
{
    return ((Athlete *)a)->id - ((Athlete *)b)->id;
}
