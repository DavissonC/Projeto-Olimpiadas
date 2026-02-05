#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <windows.h>

// Calcula os 10 atletas mais jovens com pior colocação de todos os jogos olímpicos, separados por sexo

// Dados de cada atleta (nome, país, gênero, pior posição)
typedef struct
{
    char name[256];    // Used Name
    char country[100]; // NOC
    char sex[12];      // Sex
    char born[50]; 
    char game[50];     // Games
    char sport[50];    // Discipline
    int id;            // athlete_id
    int position;      // Pos
    int age;
    int birth_year;
    int results;
} Athlete;

typedef struct {
    char name[100];
    int maxPos;
} DisciplineStats;

// Funções pré instanciadas
void ObterCampo(char *line, int column, char *destiny);
int CalculaPioresAtletasNovos(Athlete *mascAthletes, Athlete *femAthletes, int *biggestResults, int *preenchidos, Athlete nextAthlete);
int AlteraListaDosPiores(Athlete *athletesList, Athlete athlete, int *biggestResults, int *preenchidos);
int CalculaResultados(int age, int position, int maxPosInSport);
int ExtrairAno(char *str);

int main()
{
    // Tenta forçar o terminal a usar a configuração UTF-8
    if (!setlocale(LC_ALL, "en_US.UTF-8")) 
    {
        setlocale(LC_ALL, "pt_BR.UTF-8"); // Tenta português se falhar
    }

    // Configuração específica para o windows
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    int actualDisciplinesLimit = 300;
    DisciplineStats *sports = calloc(actualDisciplinesLimit, sizeof(DisciplineStats)); // Suporta até 300 esportes diferentes
    int sportCount = 0;

    // Listas que irão armazenar os dados dos 10 atletas mais novos e de pior colocação 
    Athlete mascAthletes[10];
    Athlete femAthletes[10];

    // Lista para armazenar todos os atletas
    int actualLimit = 150000;
    Athlete *allAthletes = calloc(actualLimit, sizeof(Athlete));

    // Maiores resultados armazenados nas listas
    int biggestResults[2] = {1000000, 1000000};

    // Quantidade de atletas já inseridos nas listas
    int atletasPreenchidos[2] = {0, 0};

    FILE *bios = fopen("bios.csv", "r");

    // Verifica se o arquivo foi aberto corretamente
    if (bios == NULL) 
    {
        printf("Erro ao abrir o ficheiro bios.csv!");
        return 1;
    }

    char buffer[1024];
    fgets(buffer, sizeof(buffer), bios);

    while (fgets(buffer, sizeof(buffer), bios))
    {
        char Sex[12], Name[100], ID[64], Born[256], Country[64];

        ObterCampo(buffer, 1, Sex);    // Sex
        ObterCampo(buffer, 3, Name);   // Used Name
        ObterCampo(buffer, 7, ID);     // athlete_id
        ObterCampo(buffer, 4, Born);   // Born
        ObterCampo(buffer, 6, Country);// NOC

        int id = atoi(ID);
        if (id > 0 && id <= actualLimit)
        {
            int anoNasc = ExtrairAno(Born);
            
            strcpy(allAthletes[id - 1].name, Name);
            strcpy(allAthletes[id - 1].sex, Sex);
            strcpy(allAthletes[id - 1].country, Country);
            
            allAthletes[id - 1].id = id;
            // Guardamos o ano de nascimento para uso posterior
            allAthletes[id - 1].birth_year = anoNasc; 
            allAthletes[id - 1].age = 200;      // Inicializa com idade alta
            allAthletes[id - 1].position = 0;    // Inicializa com posição 0
        }
    }   

    fclose(bios);

    // Agora, abrimos o arquivo de resultados para enfim, fazer o cálculo dos TOP 10
    FILE *results = fopen("results.csv", "r");

    if (results == NULL) 
    {
        printf("Erro ao abrir o ficheiro results.csv!");
        return 1;
    }

    // PASSO ZERO: Mapear a pior posição de cada esporte para normalização
    rewind(results); 
    fgets(buffer, sizeof(buffer), results); 

    while(fgets(buffer, sizeof(buffer), results)) 
    {
        char Sport[100], Pos[64];

        ObterCampo(buffer, 8, Sport); 
        ObterCampo(buffer, 3, Pos);

        if (Pos[0] == 'D' || Pos[0] == 'N' || Pos[0] == 'A') 
            continue; // Pula DNS, DNF, AC

        char *ptrPos = Pos;
        if (*ptrPos == '=') 
            ptrPos++;

        int posInt = atoi(ptrPos);

        if (posInt > 0) 
        {
            int found = -1;
            for (int i = 0; i < sportCount; i++) 
            {
                if (strcmp(sports[i].name, Sport) == 0) 
                {
                    if (posInt > sports[i].maxPos) 
                        sports[i].maxPos = posInt;
                    found = i;
                    break;
                }
            }
            if (found == -1 && sportCount < actualDisciplinesLimit) 
            {
                strcpy(sports[sportCount].name, Sport);
                sports[sportCount].maxPos = posInt;
                sportCount++;
            }
        }
    }

    // PASSO UM: Processar as idades e piores posições individuais
    rewind(results);
    fgets(buffer, sizeof(buffer), results);

    while(fgets(buffer, sizeof(buffer), results))
    {
        char Games[64], Pos[64], ID[64], Sport[64];

        ObterCampo(buffer, 0, Games); // Games (ex: 1934 Summer Olympics)
        ObterCampo(buffer, 3, Pos);   // Pos (ex: =17 ou 4)
        ObterCampo(buffer, 6, ID);    // athlete_id
        ObterCampo(buffer, 8, Sport); // Discipline

        int id = atoi(ID);
        if(id > 0 && id <= actualLimit)
        {
            int anoOlimpiada;
            sscanf(Games, "%d", &anoOlimpiada);
            
            char *ptrPos = Pos;
            if (*ptrPos == '=') ptrPos++;
            int posInt = atoi(ptrPos);

            int anoNasc = allAthletes[id - 1].birth_year; 

            if(anoNasc > 0 && posInt > 0)
            {
                int idadeNaEpoca = anoOlimpiada - anoNasc;
                
                // Se o atleta era mais jovem nesta olimpíada
                if(idadeNaEpoca < allAthletes[id - 1].age)
                {
                    allAthletes[id - 1].age = idadeNaEpoca;
                }
                
                // Mantém a maior posição numérica (pior colocação)
                if (posInt > allAthletes[id - 1].position)
                {
                    allAthletes[id - 1].position = posInt;
                    strcpy(allAthletes[id - 1].game, Games);
                    strcpy(allAthletes[id - 1].sport, Sport);
                }
            }
        }
    }

    // PASSO DOIS: Calcular scores e preencher os Rankings
    for(int i = 0; i < actualLimit; i++)
    {
        if(allAthletes[i].age < 200 && allAthletes[i].position > 0)
        {
            int mPos = 1;
            for(int j = 0; j < sportCount; j++) 
            {
                if(strcmp(sports[j].name, allAthletes[i].sport) == 0) 
                {
                    mPos = sports[j].maxPos;
                    break;
                }
            }
            // Passamos o mPos explicitamente para o cálculo
            allAthletes[i].results = CalculaResultados(allAthletes[i].age, allAthletes[i].position, mPos);
            
            CalculaPioresAtletasNovos(mascAthletes, femAthletes, biggestResults, atletasPreenchidos, allAthletes[i]);
        }
    }

    printf("\n%s\n", "==============================================================================================================================================================");
    printf("                                              TOP 10 ATLETAS MAIS NOVOS E PIORES COLOCADOS - MASCULINO                                              \n");
    printf("%s\n", "==============================================================================================================================================================");
    printf("%-4s | %-30s | %-5s | %-4s | %-25s | %-20s | %s\n", "Rank", "Atleta", "Idade", "Pos", "País", "Olimpíada", "Disciplina");
    printf("%s\n", "--------------------------------------------------------------------------------------------------------------------------------------------------------------");

    for(int i = 0; i < atletasPreenchidos[0]; i++) 
    {
        printf("%02d   | %-30.30s | %-5d | %-4d | %-25.25s | %-20.20s | %s\n", 
               i + 1, 
               mascAthletes[i].name, 
               mascAthletes[i].age, 
               mascAthletes[i].position, 
               mascAthletes[i].country, 
               mascAthletes[i].game, 
               mascAthletes[i].sport);
    }

    printf("\n%s\n", "==============================================================================================================================================================");
    printf("                                              TOP 10 ATLETAS MAIS NOVOS E PIORES COLOCADOS - FEMININO                                               \n");
    printf("%s\n", "==============================================================================================================================================================");
    printf("%-4s | %-30s | %-5s | %-4s | %-25s | %-20s | %s\n", "Rank", "Atleta", "Idade", "Pos", "País", "Olimpíada", "Disciplina");
    printf("%s\n", "--------------------------------------------------------------------------------------------------------------------------------------------------------------");

    for(int i = 0; i < atletasPreenchidos[1]; i++) 
    {
        printf("%02d   | %-30.30s | %-5d | %-4d | %-25.25s | %-20.20s | %s\n", 
               i + 1, 
               femAthletes[i].name, 
               femAthletes[i].age, 
               femAthletes[i].position, 
               femAthletes[i].country, 
               femAthletes[i].game, 
               femAthletes[i].sport);
    }
    printf("%s\n", "==============================================================================================================================================================");

    fclose(results);
    free(allAthletes);
    free(sports);
    return 0;
}

int CalculaResultados(int age, int position, int maxPosInSport) 
{
    if (maxPosInSport <= 0) maxPosInSport = 1; 
    double meio = maxPosInSport / 2.0;
    double distRelativa = (position - meio) / meio;
    double impPos = distRelativa * fabs(distRelativa);
    // Fórmula final equilibrada para idade, posição e tamanho do esporte
    double score = (age * 100.0) - (impPos * 500.0) - (log10(maxPosInSport) * 150.0);
    return (int)score;
}

int CalculaPioresAtletasNovos(Athlete *mascAthletes, Athlete *femAthletes, int *biggestResults, int *preenchidos, Athlete nextAthlete) 
{
    // Divisão por sexo dos atletas
    if(strcmp(nextAthlete.sex, "Male") == 0) 
    {
        if(nextAthlete.results < biggestResults[0]) 
            AlteraListaDosPiores(mascAthletes, nextAthlete, &biggestResults[0], &preenchidos[0]);
    } 
    else 
    {
        if(nextAthlete.results < biggestResults[1]) 
            AlteraListaDosPiores(femAthletes, nextAthlete, &biggestResults[1], &preenchidos[1]);
    }
    return 0;
}

int AlteraListaDosPiores(Athlete *list, Athlete athlete, int *biggestResults, int *preenchidos) 
{
    // Se todos os espaços ainda não tiverem sido preenchidos, preenche-os
    if (*preenchidos < 10) 
    {
        list[*preenchidos] = athlete;
        (*preenchidos)++;
    } 
    // Senão, substitui o de maior pontuação (último da lista)
    else 
    {
        list[9] = athlete;
    }

    // Reorganiza a lista para que esteja em ordem crescente de pontuação (Bubble Sort)
    for (int i = 0; i < *preenchidos - 1; i++) 
    {
        for (int j = 0; j < *preenchidos - i - 1; j++) 
        {
            if (list[j].results > list[j + 1].results) 
            {
                Athlete temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
    if (*preenchidos == 10) *biggestResults = list[9].results;
    return 0;
}

int ExtrairAno(char *string) 
{
    int ano = 0;
    for (int i = 0; string[i] != '\0'; i++) 
    {
        if (isdigit(string[i]) && isdigit(string[i+1]) && isdigit(string[i+2]) && isdigit(string[i+3])) 
        {
            sscanf(&string[i], "%4d", &ano);
            return ano;
        }
    }
    return 0;
}

void ObterCampo(char *line, int column, char *destiny) 
{
    int actualColumn = 0, i = 0, j = 0, entreAspas = 0;
    while (line[i] != '\0' && actualColumn <= column) 
    {
        if (line[i] == '\"') 
        { 
            entreAspas = !entreAspas; 
        }
        else if (line[i] == ',' && !entreAspas) 
        {
            if (actualColumn == column) break;
            actualColumn++; j = 0;
        } 
        else if (actualColumn == column) 
        {
            if (line[i] != '\n' && line[i] != '\r') destiny[j++] = line[i];
        }
        i++;
    }
    destiny[j] = '\0';
}