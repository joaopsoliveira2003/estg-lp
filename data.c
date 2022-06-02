#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "definitions.h"

#pragma region ImportData
/**
 * @brief Importa os dados dos ficheiros binários
 * @note Caso nao exista algum ficheiro ou caso surja erro na leitura, o programa termina
 */
void importData()
{
    FILE *fcliente = fopen("clientes.bin", "rb");
    FILE *fartigo = fopen("artigos.bin", "rb");
    FILE *fencomenda = fopen("encomendas.bin", "rb");

    if (fcliente == NULL) {
        printf("Erro ao abrir o ficheiro clientes.bin\n");
        freeMemory(false);
        exit(1);
    }
    if (fartigo == NULL) {
        printf("Erro ao abrir o ficheiro artigos.bin\n");
        freeMemory(false);
        exit(1);
    }
    if (fencomenda == NULL) {
        printf("Erro ao abrir o ficheiro encomendas.bin\n");
        freeMemory(false);
        exit(1);
    }

    freeMemory(false);

    fread(&NC, sizeof(int), 1, fcliente);
    fread(&NA, sizeof(int), 1, fartigo);
    fread(&NE, sizeof(int), 1, fencomenda);

    cliente = malloc(sizeof(Clientes) * (NC + 1));
    artigo = malloc(sizeof(Artigos) * (NA + 1));
    encomenda = malloc(sizeof(Encomendas) * (NE + 1));

    for (int i = 0; i <= NC; i++) {
        fread(&cliente[i], sizeof(Clientes), 1, fcliente);
    }
    for (int i = 0; i <= NA; i++) {
        fread(&artigo[i], sizeof(Artigos), 1, fartigo);
    }
    for (int i = 0; i <= NE; i++) {
        fread(&encomenda[i], sizeof(Encomendas), 1, fencomenda);
    }

    fclose(fcliente);
    fclose(fartigo);
    fclose(fencomenda);

    green(); printf("\nDados importados com sucesso!\n"); white();
    sleep(1);
}
#pragma endregion

#pragma region ExportData
/**
 * @brief Exporta os dados dos ficheiros binários
 * @note Caso surja algum erro na escrita, o programa termina
 */
void exportData()
{
    FILE *fcliente = fopen("clientes.bin", "wb");
    FILE *fartigo = fopen("artigos.bin", "wb");
    FILE *fencomenda = fopen("encomendas.bin", "wb");

    if (fcliente == NULL) {
        printf("Erro ao escreve o ficheiro clientes.bin\n");
        freeMemory(false);
        exit(1);
    }
    if (fartigo == NULL) {
        printf("Erro ao escrever no ficheiro artigos.bin\n");
        freeMemory(false);
        exit(1);
    }
    if (fencomenda == NULL) {
        printf("Erro ao escrever no ficheiro encomendas.bin\n");
        freeMemory(false);
        exit(1);
    }

    fwrite(&NC, sizeof(int), 1, fcliente);
    fwrite(&NA, sizeof(int), 1, fartigo);
    fwrite(&NE, sizeof(int), 1, fencomenda);

    fwrite(cliente, sizeof(Clientes) * (NC + 1), NC + 1, fcliente);
    fwrite(artigo, sizeof(Artigos) * (NA + 1), NA + 1, fartigo);
    fwrite(encomenda, sizeof(Encomendas) * (NE + 1), NE + 1, fencomenda);

    fclose(fcliente);
    fclose(fartigo);
    fclose(fencomenda);

    green(); printf("\n[!] Dados exportados com sucesso! [!]\n"); white();
    sleep(1);
}
#pragma endregion

#pragma region importDefaultData
/**
 * @brief Importa os dados padrão (default.bin)
 * @note Caso o ficheiro não exista a função exportDefaultData() é chamada para criar o ficheiro com os dados padrão
 */
void importDefaultData() {
    FILE *fdefault = fopen("default.bin", "rb");
    
    if (fdefault == NULL) {
        red(); printf("\n[!] O Ficheiro default.bin não existe! A criar... [!]\n"); white();
        exportDefaultData();
        return;
    }

    fread(&NA, sizeof(int), 1, fdefault);

    artigo = malloc(sizeof(Artigos) * (NA + 1));
    if (artigo == NULL) {
        red(); printf("[!] Erro ao alocar memória [!]\n"); white();
        freeMemory(false);
        exit(1);
    }

    for (int i = 0; i <= NA; i++) {
        fread(&artigo[i], sizeof(Artigos), 1, fdefault);
    }

    strcpy(runtime->stringDesconto, "5off");
    runtime->percentageDesconto = 0.05;

    fclose(fdefault);
}
#pragma endregion

#pragma region exportDefaultData
/**
 * @brief Exporta os dados padrão do programa
 * @note Coloca os dados padrão em memória e em seguida exporta para o ficheiro default.bin
 */
void exportDefaultData()
{
    /* --- Sapatos --- */
    reallocStruct(1);

    strcpy(artigo[NA].artigo, "Sapato");
    artigo[NA].codArtigo = 1;
    artigo[NA].maoObra = 5;
    artigo[NA].custosFixos = 3;
    artigo[NA].margemLucro = 0.3;   
    artigo[NA].tamanhos[0] = 34;
    artigo[NA].tamanhos[1] = 48;
    artigo[NA].defaultPreco = artigo[NA].maoObra + artigo[NA].custosFixos + artigo[NA].margemLucro;
    artigo[NA].incrementoPreco = 0.05;
    int i = 0;
    for(int tam = artigo[NA].tamanhos[0]; tam <= artigo[NA].tamanhos[1]; tam++)
    {
        artigo[NA].tamanhoPreco[i][0] = artigo[NA].tamanhos[0] + i;
        artigo[NA].tamanhoPreco[i][1] = artigo[NA].defaultPreco + (artigo[NA].incrementoPreco * (i + 1));
        i++;
    }
    artigo[NA].estado = true;

    /* --- Sandálias --- */
    reallocStruct(1);

    strcpy(artigo[NA].artigo, "Sandalias");
    artigo[NA].codArtigo = 3;
    artigo[NA].maoObra = 5;
    artigo[NA].margemLucro = 0.2;
    artigo[NA].custosFixos = 3;
    artigo[NA].tamanhos[0] = 34;
    artigo[NA].tamanhos[1] = 48;
    artigo[NA].defaultPreco = artigo[NA].maoObra + artigo[NA].custosFixos + artigo[NA].margemLucro;
    artigo[NA].incrementoPreco = 0.25;
    i = 0;
    for(int tam = artigo[NA].tamanhos[0]; tam <= artigo[NA].tamanhos[1]; tam++)
    {
        artigo[NA].tamanhoPreco[i][0] = artigo[NA].tamanhos[0] + i;
        artigo[NA].tamanhoPreco[i][1] = artigo[NA].defaultPreco + (artigo[NA].incrementoPreco * (i + 1));
        i++;
    }
    artigo[NA].estado = true;

    /* --- Botas --- */
    reallocStruct(1);

    strcpy(artigo[NA].artigo, "Botas");
    artigo[NA].codArtigo = 5;
    artigo[NA].maoObra = 5;
    artigo[NA].custosFixos = 3;  
    artigo[NA].margemLucro = 0.4; 
    artigo[NA].tamanhos[0] = 34;
    artigo[NA].tamanhos[1] = 48;
    artigo[NA].defaultPreco = artigo[NA].maoObra + artigo[NA].custosFixos + artigo[NA].margemLucro;
    artigo[NA].incrementoPreco = 0.75;
    i = 0;
    for(int tam = artigo[NA].tamanhos[0]; tam <= artigo[NA].tamanhos[1]; tam++)
    {
        artigo[NA].tamanhoPreco[i][0] = artigo[NA].tamanhos[0] + i;
        artigo[NA].tamanhoPreco[i][1] = artigo[NA].defaultPreco + (artigo[NA].incrementoPreco * (i + 1));
        i++;
    }
    artigo[NA].estado = true;

    strcpy(runtime->stringDesconto, "5off");
    runtime->percentageDesconto = 0.05;

    /* --- Exportação dos dados padrão --- */
    FILE *fdefault = fopen("default.bin", "wb");

    if (fdefault == NULL) {
        boldRed(); printf("[!] Erro ao escrever no ficheiro default.bin [!]\n"); white();
        freeMemory(false);
        exit(1);
    }

    fwrite(&NA, sizeof(int), 1, fdefault);
    fwrite(artigo, sizeof(Artigos) * (NA + 1), NA + 1, fdefault);

    fclose(fdefault);

    green(); printf("\n[!] Dados padrão exportados com sucesso! [!]\n"); white();
}
#pragma endregion

#pragma region ExportEncomendas
/**
 * @brief Exporta as encomendas para um ficheiro CSV 
 */
void exportEncomendas()
{
    if (NE == -1) {
        red(); printf("\n[!] Não existem encomendas para exportar [!]"); white();
        pressEnter(true);
        return;
    }

    FILE *f = fopen("encomendas.csv", "w");

    if (f == NULL) {
        printf("Erro ao abrir o ficheiro encomendas.csv\n");
        exit(1);
    }

    fprintf(f, "CodEncomenda,CodArtigo,CodCliente,Artigo,NomeCliente,Tamanho,Quantidade,Preco,Data,Estado\n");

    for (int i = 0; i <= NE; i++) {
        fprintf(f, "%d,%d,%d,%s,%s,%d,%d,%0.2f,%d/%d/%d,%s\n", \
        encomenda[i].codEncomenda, encomenda[i].codArtigo, encomenda[i].codCliente, \
        getArtigoName(getArtigoIndex(encomenda[i].codArtigo)), getClientName(encomenda[i].codCliente), \
        encomenda[i].tamanho, encomenda[i].quantidade, encomenda[i].total, \
        encomenda[i].dia, encomenda[i].mes, encomenda[i].ano, encomenda[i].estado ? "Ativa" : "Cancelada");
    }

    fclose(f);
    green(); printf("\nFicheiro encomendas.csv exportado com sucesso!"); white();
    pressEnter(true);
}
#pragma endregion

#pragma region DeleteData
/**
 * @brief Apaga todos os dados em memória do programa
 */
void deleteData() {
    red(); printf("\n[!] Tem a certeza que pretende ");
    boldRed(); printf("ELIMINAR"); red(); printf(" todos os dados? [!]\n");
    white(); printf("[!] Esta acção não pode ser revertida! (s/N): ");
    char choice;
    scanf(" %c", &choice);
    if (choice == 's' || choice == 'S') {
        NA = -1;
        NC = -1;
        NE = -1;
        artigo = realloc(artigo, sizeof(Artigos) * (NA + 1));
        cliente = realloc(cliente, sizeof(Clientes) * (NC + 1));
        encomenda = realloc(encomenda, sizeof(Encomendas) * (NE + 1));
        green(); printf("\n[!] Dados eliminados com sucesso! [!]"); white();
    } else {
        bold(); printf("\n[!] Ação cancelada!"); white();
    }
    pressEnter(true);
}
#pragma endregion