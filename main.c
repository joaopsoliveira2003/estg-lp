/// @file main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @mainpage
 * Compilar e Executar o Programa:
 gcc .\main.c .\administrador.c .\shopping.c .\colors.c .\queries.c .\clientes.c .\util.c -o main && .\main
 */

#include "definitions.h"

int NC = -1;
int NA = -1;
int NE = -1;

Clientes *cliente;
Artigos *artigo;
Encomendas *encomenda;
Runtime *runtime;

/* =========================== MAIN =========================== */
/**
 * @brief Função Main do programa
 * @note tipoUtilizador - valores possiveis: 1 - Cliente, 2 - Administrador ou 0 - Sair
 * @note Esta é a primeira função a ser executada */
int main() 
{
    int op, opcao;

    // Ignorar [CTRL + C]
    // signal(SIGINT, SIG_IGN);

    // Alocação da memória para as estruturas
    cliente = malloc(sizeof(cliente) * (NC + 1));
    artigo = malloc(sizeof(artigo) * (NA + 1));
    encomenda = malloc(sizeof(encomenda) * (NE + 1));
    runtime = malloc(sizeof(runtime));

    // Permitir que os artigos "default" sejam utilizados
    importDefaultData();
    
    do {
        white();
        blue(); printf("\n////////"); white(); printf(" Menu "); blue(); printf("\\\\\\\\\\\\\\\\\n\n"); 
        green(); printf("  [1] Clientes e Vendas\n"); 
        red(); printf("  [2] Administração\n"); white();
        bold(); printf("\n  [0] Sair\n"); white();
        printf("\nTipo de Utilizador> ");
        scanf("%d", &op);
        switch (op) {
            case 0:
                blue(); printf("\nObrigado por comprar na nossa loja :)\n"); white();
                freeMemory(true);
                break;

            case 1:
                clearScreen();
                green(); printf("\n[!] Bem-vindo Cliente [!]\n"); white();
                do
                {
                    green(); printf("\n////////"); white(); printf(" Menu Cliente "); green(); printf("\\\\\\\\\\\\\\\\\n"); white();
                    printf("\n[1] Realizar Compras\n");
                    printf("[2] Registo Cliente\n");
                    printf("[3] Mostrar as minhas Encomendas\n");
                    printf("[4] Alterar Informação de Cliente\n");
                    green(); printf("[5] Login\n"); white();
                    yellow(); printf("[6] Logout\n"); white();
                    boldRed(); printf("[7] Cancelar Encomenda\n"); white();
                    bold(); printf("[0] Voltar\n"); white();
                    red(); printf("[9] SAIR\n"); white();

                    printf("\nOpção: ");
                    scanf("%d", &opcao);

                    switch (opcao)
                    {
                        case 0:
                            clearScreen();
                            break;
                        case 1:
                            doShopping();
                            break;
                        case 2:
                            addCliente();
                            break;
                        case 3:
                            listEncomendas(60);
                            break;
                        case 4:
                            updateClient(0, 1);
                            break;
                        case 5:
                            loginClient();
                            break;
                        case 6:
                            logoutClient();
                            break;
                        case 7:
                            cancelEncomenda();
                            break;

                        case 9:
                            blue(); printf("\nObrigado por comprar na nossa loja :)\n"); white();
                            freeMemory(true);

                        default:
                            red(); printf("\n[!] Opção Inválida [!]"); white();
                            pressEnter(true);
                            break;
                    }
                } while (opcao != 0);
                break;

            case 2:
                clearScreen();
                boldRed(); printf("\n[!] Bem-vindo Administrador [!]\n"); white();
                do
                {
                    red(); printf("\n////////"); white(); printf(" Menu Administrador "); red(); printf("\\\\\\\\\\\\\\\\\n\n"); white();
                    cyan(); printf("[[ Listagem de Artigos ]]\n"); white();
                    printf("  [11] Geral\n");
                    printf("  [12] Completa\n");
                    printf("  [13] Específica\n");
                    cyan(); printf("\n[[ Gestão de Artigos ]]\n"); white();
                    printf("  [21] Adicionar\n");
                    printf("  [22] Atualizar\n");
                    printf("\n[3] Gestão de Clientes\n");
                    printf("\n[4] Gestão de Desconto\n");
                    cyan(); printf("\n[[ Listagem de Clientes ]]\n"); white();
                    printf("  [51] Geral\n");
                    printf("  [52] Cliente Específico\n");
                    cyan(); printf("\n[[ Listagem de Encomendas ]]\n"); white();
                    printf("  [61] Todas as Encomendas\n");
                    printf("  [62] Encomendas de certo cliente\n");
                    printf("  [63] Encomendas por intervalo de tempo\n");
                    bold(); printf("  [64] Estatisticas\n"); white();
                    cyan(); printf("  [65] Exportar Encomendas\n"); white();
                    bold(); printf("\n\n[0] Voltar\n"); white();
                    green(); printf("\n[7] Importar Dados\n"); white();
                    yellow(); printf("\n[8] Exportar Dados\n"); white();
                    boldRed(); printf("\n[9] Eliminar Dados\n"); white();
                    red(); printf("\n[10] Sair\n"); white();

                    printf("\nOpcão: ");
                    scanf("%d", &opcao);

                    switch (opcao) {
                        case 11: case 12: case 13:
                            listArtigos(opcao);
                            break;
                        case 21:
                            addArtigo();
                            break;
                        case 22:
                            updateArtigo();
                            break;
                        case 3:
                            updateClient(0, 0);
                            break;
                        case 4:
                            updateDesconto();
                            break;
                        case 51: case 52:
                            listClientes(opcao);
                            break;
                        case 61: case 62: case 63:
                            listEncomendas(opcao);
                            break;
                        case 64:
                            statisticsEncomendas();
                            break;
                        case 65:
                            exportEncomendas();
                            break;

                        case 7:
                            printf("\nOs dados serão importados. Isto irá ");
                            red(); printf("apagar"); white(); printf(" todos os dados existentes não guardados!\n");
                            red(); printf("Tem a certeza? [s/N]: "); white();
                            char confirm[2];
                            scanf(" %c", confirm);

                            if (strcmp(confirm, "s") == 0 || strcmp(confirm, "S") == 0)
                            {
                                importData();
                            }
                            else
                            {
                                yellow(); printf("\n[!] Dados não importados (cancelado pelo utilizador) [!]"); white();
                                pressEnter(true);
                            }
                            break;

                        case 8:
                            exportData();
                            break;
                        case 9:
                            deleteData();
                            break;
                        case 0:
                            clearScreen();
                            break;

                        case 10:
                            blue(); printf("\nObrigado por comprar na nossa loja :)\n"); white();
                            freeMemory(true);
                            break;

                        default:
                            red(); printf("\n[!] Opção Inválida [!]"); white();
                            pressEnter(true);
                            break;
                    }
                } while (opcao != 0);
                break;
                
            default:
                red(); printf("\n[!] Opção Inválida [!]"); white();
                pressEnter(true);
                break;
        }
    } while (op != 0);
    return 0;
}