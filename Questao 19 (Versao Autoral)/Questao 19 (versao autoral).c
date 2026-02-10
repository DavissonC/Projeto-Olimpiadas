#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <windows.h>

// Dados de cada atleta (nome, país, gênero, pior posição)
typedef struct
{
    char name[256];    // Used Name
    char country[100]; // NOC
    char sex[12];      // Sex
    char born[50];     // Born
    char game[50];     // Games
    char sport[50];    // Discipline
    int id;            // athlete_id
    int position;      // Pos
    int age;           // Game year - Birth year
    int birth_year;
    int results;       // Pontuação final
} Athlete;

typedef struct
{
    char name[100];    // Discipline
    int maxPos;        // Pior posição (de maior índice)
} DisciplineStats;

// Funções de Processamento Modularizadas
void ConfigurarTerminal();
void CarregarBiografias(Athlete *allAthletes, int athletesLimit);
int MapearPiorPosicaoEsportes(DisciplineStats *disciplines, int disciplinesLimit);
void ProcessarResultadosIndividuais(Athlete *allAthletes, int athletesLimit);
void CalcularRankingsFinais(Athlete *allAthletes, int athletesLimit, DisciplineStats *disciplines, int disciplinesQuantity, Athlete *athletesMasc, Athlete *athletesFem, int *cuttingResults, int *filledAthletes);
void ExibirResultados(Athlete *athletesMasc, Athlete *athletesFem, int *filledAthletes);

// Funções Utilitárias e de Lógica
void ObterCampo(char *line, int column, char *destiny);
int CalculaPioresAtletasNovos(Athlete *athletesMasc, Athlete *athletesFem, int *cuttingResults, int *filledAthletes, Athlete nextAthlete);
int AlteraListaDosPiores(Athlete *athletesList, Athlete athlete, int *cuttingResults, int *filledAthletes);
int CalculaResultados(int age, int position, int maxPosInSport);
int ExtrairAno(char *str);
void NormalizarString(char *nome);
void LimparNome(char *nome);
void RemoverAcentos(char *nome);

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

    free(allAthletes);
    free(disciplines);
    return 0;
}

void ConfigurarTerminal()
{
    // Tenta forçar o terminal a usar a configuração UTF-8
    if (!setlocale(LC_ALL, "en_US.UTF-8")) 
    {
        setlocale(LC_ALL, "pt_BR.UTF-8"); // Tenta português se falhar
    }

    // Configuração específica para o windows
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}

void CarregarBiografias(Athlete *allAthletes, int athletesLimit)
{
    FILE *bios = fopen("bios.csv", "r");

    // Verifica se o arquivo foi aberto corretamente
    if (bios == NULL) 
    {
        printf("Erro ao abrir o ficheiro bios.csv!");
        exit(1);
    }

    char buffer[1024];
    fgets(buffer, sizeof(buffer), bios);

    while (fgets(buffer, sizeof(buffer), bios))
    {
        char Sex[12], Name[100], ID[64], Born[256], Country[64];

        ObterCampo(buffer, 1, Sex);    // Sex
        ObterCampo(buffer, 3, Name);   // Used Name

        NormalizarString(Name);

        ObterCampo(buffer, 7, ID);     // athlete_id
        ObterCampo(buffer, 4, Born);   // Born
        ObterCampo(buffer, 6, Country);// NOC

        NormalizarString(Country);

        int id = atoi(ID);
        if (id > 0 && id <= athletesLimit)
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
}

int MapearPiorPosicaoEsportes(DisciplineStats *disciplines, int disciplinesLimit)
{
    // Mapear a pior posição de cada esporte para normalização
    FILE *results = fopen("results.csv", "r");
    if (results == NULL) return 0;

    char buffer[1024];
    int disciplinesQuantity = 0;
    fgets(buffer, sizeof(buffer), results); 

    while(fgets(buffer, sizeof(buffer), results)) 
    {
        char Discipline[100], Pos[64];

        ObterCampo(buffer, 8, Discipline); 
        ObterCampo(buffer, 3, Pos);

        if (Pos[0] == 'D' || Pos[0] == 'N' || Pos[0] == 'A') 
            continue; // Pula DNS, DNF, AC

        char *ptrPos = (Pos[0] == '=') ? Pos + 1 : Pos;
        int posInt = atoi(ptrPos);

        if (posInt > 0) 
        {
            int found = -1;
            for (int i = 0; i < disciplinesQuantity; i++) 
            {
                if (strcmp(disciplines[i].name, Discipline) == 0) 
                {
                    if (posInt > disciplines[i].maxPos) 
                        disciplines[i].maxPos = posInt;
                    found = i;
                    break;
                }
            }

            if (found == -1 && disciplinesQuantity < disciplinesLimit) 
            {
                strcpy(disciplines[disciplinesQuantity].name, Discipline);
                disciplines[disciplinesQuantity].maxPos = posInt;
                disciplinesQuantity++;
            }
        }
    }
    fclose(results);
    return disciplinesQuantity;
}

void ProcessarResultadosIndividuais(Athlete *allAthletes, int athletesLimit)
{
    // Processar as idades e piores posições individuais
    FILE *results = fopen("results.csv", "r");
    if (results == NULL) 
    {
        printf("Erro ao abrir o ficheiro results.csv!");
        return;
    }

    char buffer[1024];
    fgets(buffer, sizeof(buffer), results);

    while(fgets(buffer, sizeof(buffer), results))
    {
        char Games[64], Pos[64], ID[64], Sport[64];

        ObterCampo(buffer, 0, Games); // Games (ex: 1934 Summer Olympics)
        ObterCampo(buffer, 3, Pos);   // Pos 
        ObterCampo(buffer, 6, ID);    // athlete_id
        ObterCampo(buffer, 8, Sport); // Discipline

        int id = atoi(ID);
        if(id > 0 && id <= athletesLimit)
        {
            int anoOlimpiada;
            sscanf(Games, "%d", &anoOlimpiada);
            
            char *ptrPos = (Pos[0] == '=') ? Pos + 1 : Pos;
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
    fclose(results);
}

void CalcularRankingsFinais(Athlete *allAthletes, int athletesLimit, DisciplineStats *disciplines, int disciplinesQuantity, Athlete *athletesMasc, Athlete *athletesFem, int *cuttingResults, int *filledAthletes)
{
    // Calcular scores e preencher os Rankings
    for(int i = 0; i < athletesLimit; i++)
    {
        if(allAthletes[i].age < 200 && allAthletes[i].position > 0)
        {
            int mPos = 1;
            for(int j = 0; j < disciplinesQuantity; j++) 
            {
                if(strcmp(disciplines[j].name, allAthletes[i].sport) == 0) 
                {
                    mPos = disciplines[j].maxPos;
                    break;
                }
            }
            // Passamos o mPos explicitamente para o cálculo
            allAthletes[i].results = CalculaResultados(allAthletes[i].age, allAthletes[i].position, mPos);
            
            CalculaPioresAtletasNovos(athletesMasc, athletesFem, cuttingResults, filledAthletes, allAthletes[i]);
        }
    }
}

void ExibirResultados(Athlete *athletesMasc, Athlete *athletesFem, int *filledAthletes)
{
    char tempPos[12];
    char *secao[] = {"MASCULINO", "FEMININO"};
    Athlete *listas[] = {athletesMasc, athletesFem};

    for (int k = 0; k < 2; k++)
    {
        printf("\n==============================================================================================================================================================\n");
        printf("                                               TOP 10 ATLETAS MAIS NOVOS E PIORES COLOCADOS - %s                                                              \n", secao[k]);
        printf("==============================================================================================================================================================\n");
        printf("%-4s | %-28s | %-5s | %-4s | %-26s | %-21s | %s\n", "Rank", "Atleta", "Idade", "Pos", "País", "Olimpíada", "Disciplina");
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

        for(int i = 0; i < filledAthletes[k]; i++) 
        {
            sprintf(tempPos, "%dº", listas[k][i].position);
            printf("%02d   | %-30.30s | %-5d | %-5s | %-25.25s | %-20.20s | %s\n", i + 1, listas[k][i].name, listas[k][i].age, tempPos, listas[k][i].country, listas[k][i].game, listas[k][i].sport);
        }
        printf("==============================================================================================================================================================\n");
    }
}

void NormalizarString(char *nome)
{
    LimparNome(nome); 
    RemoverAcentos(nome);
    char *p = nome;
    while(isspace((unsigned char)*p)) p++;
    if (p != nome) memmove(nome, p, strlen(p) + 1);
    int len = strlen(nome);
    while(len > 0 && isspace((unsigned char)nome[len-1])) len--;
    nome[len] = '\0';
}

void LimparNome(char *nome) 
{
    int i, j = 0;
    char temp[256];
    for (i = 0; nome[i] != '\0'; i++) 
    {
        unsigned char c = (unsigned char)nome[i];
        if (c == 0xC2 && (unsigned char)nome[i+1] == 0xB7) { temp[j++] = ' '; i++; }
        else if (c == 183 || c == 149 || c == 0xB7) { temp[j++] = ' '; }
        else if (c >= 32) { temp[j++] = nome[i]; }
    }
    temp[j] = '\0';
    strcpy(nome, temp);
}

void RemoverAcentos(char *nome) 
{
    struct { char *com; char sem; } de_para[] = {
        {"á", 'a'}, {"à", 'a'}, {"â", 'a'}, {"ã", 'a'}, {"ä", 'a'}, {"é", 'e'}, {"è", 'e'}, {"ê", 'e'}, {"ë", 'e'},
        {"í", 'i'}, {"ì", 'i'}, {"î", 'i'}, {"ï", 'i'}, {"ó", 'o'}, {"ò", 'o'}, {"ô", 'o'}, {"õ", 'o'}, {"ö", 'o'},
        {"ú", 'u'}, {"ù", 'u'}, {"û", 'u'}, {"ü", 'u'}, {"ç", 'c'}, {"ș", 's'}, {"ț", 't'}, {"ñ", 'n'},
        {"Á", 'A'}, {"É", 'E'}, {"Í", 'I'}, {"Ó", 'O'}, {"Ú", 'U'}, {"Ç", 'C'}
    };
    char temp[256] = "";
    int i, k, found;
    for (i = 0; nome[i] != '\0'; ) 
    {
        found = 0;
        for (k = 0; k < sizeof(de_para) / sizeof(de_para[0]); k++) 
        {
            int len = strlen(de_para[k].com);
            if (strncmp(&nome[i], de_para[k].com, len) == 0) 
            {
                strncat(temp, &de_para[k].sem, 1);
                i += len; found = 1; break;
            }
        }
        if (!found) { strncat(temp, &nome[i], 1); i++; }
    }
    strcpy(nome, temp);
}

void ObterCampo(char *line, int column, char *destiny) 
{
    int actualColumn = 0, i = 0, j = 0, entreAspas = 0;
    while (line[i] != '\0' && actualColumn <= column) 
    {
        if (line[i] == '\"') entreAspas = !entreAspas;
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

int CalculaPioresAtletasNovos(Athlete *athletesMasc, Athlete *athletesFem, int *cuttingResults, int *filledAthletes, Athlete nextAthlete) 
{
    if(strcmp(nextAthlete.sex, "Male") == 0) 
    {
        if(nextAthlete.results < cuttingResults[0]) 
            AlteraListaDosPiores(athletesMasc, nextAthlete, &cuttingResults[0], &filledAthletes[0]);
    } 
    else 
    {
        if(nextAthlete.results < cuttingResults[1]) 
            AlteraListaDosPiores(athletesFem, nextAthlete, &cuttingResults[1], &filledAthletes[1]);
    }
    return 0;
}

int AlteraListaDosPiores(Athlete *list, Athlete athlete, int *cuttingResults, int *filledAthletes) 
{
    if (*filledAthletes < 10) 
    { 
        list[*filledAthletes] = athlete; 
        (*filledAthletes)++; 
    } 
    else 
    { 
        list[9] = athlete; 
    }
    for (int i = 0; i < *filledAthletes - 1; i++) 
    {
        for (int j = 0; j < *filledAthletes - i - 1; j++) 
        {
            if (list[j].results > list[j + 1].results) 
            {
                Athlete temp = list[j]; 
                list[j] = list[j + 1]; 
                list[j + 1] = temp;
            }
        }
    }
    if (*filledAthletes == 10) *cuttingResults = list[9].results;
    return 0;
}

int CalculaResultados(int age, int position, int maxPosInSport) 
{
    if (maxPosInSport <= 0) maxPosInSport = 1; 
    double meio = maxPosInSport / 2.0;
    double distRelativa = (position - meio) / meio;
    double score = (age * 100.0) - (distRelativa * fabs(distRelativa) * 500.0) - (log10(maxPosInSport) * 150.0);
    return (int)score;
}

int ExtrairAno(char *string) 
{
    int ano = 0;
    for (int i = 0; string[i] != '\0'; i++) 
    {
        if (isdigit(string[i]) && isdigit(string[i+1]) && isdigit(string[i+2]) && isdigit(string[i+3])) 
        {
            sscanf(&string[i], "%4d", &ano); return ano;
        }
    }
    return 0;
}