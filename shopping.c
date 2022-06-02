#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "definitions.h"
#define IVA 1.23

#pragma region GetEstadoEncomenda
/** 
 * @brief Retornar o estado de certa encomenda
 * @param estado - O estado de do artigo em int (ex: artigo[1].estado)
 * @return Disponível ou Indisponível (string)
 */
char *getEstadoEncomenda(int estado)
{
    if (estado == 1)
    {
        return "Disponível\n";
    }
    else
    {
        return "Indisponível\n";
    }
}
#pragma endregion

#pragma region LoginClient
/** 
 * @brief Função que autentica certo utilizador
 * @note Verifica se já existe um utilizador autenticado, 
 * se sim dá a opção de sair ou de autenticar outro utilizador
 */
void loginClient()
{
    if (NC == -1)
    {
        red(); printf("\n[!] Não existem clientes registados [!]\n"); white();
        addCliente();
    }

    if (runtime->currentClient != 0)
    {
        yellow(); printf("\n[!] Já existe um cliente logado [!]\n"); white();
        printf("\n[1] Pretende voltar ao menu com o cliente %s\n"
        "[2] Ou iniciar sessão com outro utilizador?\n> ", getClientName(runtime->currentClient));
        
        int opcao;
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            return;
        
        case 2:
            logoutClient();
            loginClient();
            break;

        default:
            red(); printf("[!] Opção inválida [!]"); white();
            pressEnter(true);
            loginClient();
            break;
        }
    }

    if (runtime->currentClient == 0)
    {
        cyan(); printf("\n_______ Login _______\n"); white();
        printf("\nCódigo de Cliente: ");
        scanf(" %d", &runtime->currentClient);
    }

    if (getEstadoCliente(runtime->currentClient))
    {
        green(); printf("\n*** Bem Vindo, %s ***\n", getClientName(runtime->currentClient)); white();
        sleep(1);
        clearScreen();
    }
    else
    {
        red(); printf("\n[!] Cliente não existe, indique um cliente válido [!]\n"); white();
        runtime->currentClient = 0;
        loginClient();
    }
}
#pragma endregion

#pragma region Logout
/** 
 * @brief Função que desautentica certo utilizador
 * @note Verifica se já existe um utilizador autenticado
 */
void logoutClient()
{
    if (runtime->currentClient == 0)
    {
        yellow(); printf("\n[!] Não existe cliente logado [!]\n"); white();
        sleep(2);
        clearScreen();
    }
    else
    {
        yellow(); printf("\n[!] Logout Cliente %s [!]\n", getClientName(runtime->currentClient)); white();
        runtime->currentClient = 0;
    }
}
#pragma endregion

#pragma region Shopping
/** 
 * @brief Função que adiciona uma encomenda, ou seja uma compra
 * @note Verifica se já existe um utilizador autenticado, e se existem artigos disponíveis
 */
void doShopping()
{
    /// Caso cliente não esteja autenticado, e pretenda fazer compras
    if (!loggedClient())
    {
        yellow(); printf("\n[!] Por favor inicie sessão [!]\n"); white();
        loginClient();
    }

    if (NA == -1) {
        red(); printf("\n[!] Não existem artigos disponíveis [!]"); white();
        pressEnter(true);
        return;
    }

    int artigoComprar, quantidadeComprar, tamanhoComprar;
    char codPromo[8];
    reallocStruct(3);
    encomenda[NE].codCliente = runtime->currentClient;

    do
    {
        listArtigos(10);
        printf("\nSelecionar o artigo a comprar (Código): ");
        scanf("%d", &artigoComprar);
    } while (!availableForOperation(3, artigoComprar, 0));
    encomenda[NE].codArtigo = artigoComprar;

    do
    {
        printf("\nQuantidade de pares de %s pretendidos: ", getArtigoName(getArtigoIndex(artigoComprar)));
        scanf("%d", &quantidadeComprar);
    } while (!checkNumber(quantidadeComprar));
    encomenda[NE].quantidade = quantidadeComprar;
    
    do
    {
        printf("\nTamanho pretendido: ");
        scanf("%d", &tamanhoComprar);
    } while (!availableForOperation(5, artigoComprar, tamanhoComprar));
    encomenda[NE].tamanho = tamanhoComprar;

    printf("\n  Desconto Promocional\n[Press ENTER caso não tenha]: ");
    scanf("%c", &codPromo);
    fgets(codPromo,5,stdin);

    /// Cálculo do preço
    /// defaultPreco = (maoObra + custosFixos) * margemLucro
    artigoComprar = getArtigoIndex(artigoComprar);

    double subtotal = artigo[artigoComprar].defaultPreco * (double) (quantidadeComprar);
    if (checkCodPromo(codPromo))
    {
        subtotal -= runtime->percentageDesconto;
    }
    double totalIva = (double) (subtotal * IVA);

    printf("\n Subtotal: %.2lf€", subtotal); bold();
    printf("\n Total a pagar (c/IVA): %.2lf€\n", totalIva);  white();
    encomenda[NE].total = totalIva;

    encomenda[NE].codEncomenda = NE + 1;

    encomenda[NE].horaminuto = getDateInfo(1);
    encomenda[NE].dia = getDateInfo(2);
    encomenda[NE].mes = getDateInfo(3);
    encomenda[NE].ano = getDateInfo(4);

    encomenda[NE].estado = true;

    green(); printf("\n[!] A encomenda foi registada com sucesso [!]\n"); white();

    printf("\nDeseja adicionar mais algum artigo? (s/N): ");
    char opcao;
    scanf(" %c", &opcao);
    if (opcao == 's' || opcao == 'S')
    {
        doShopping();
    }
    clearScreen();
}
#pragma endregion

#pragma region ListEncomendas
/**
 * @brief Função que permite listar as encomendas
 * @param queryType - Tipo de "query" a ser executada
 * @note De acordo com o tipo de "query" definido na enumeração, a função lista as encomendas
 * @note Por exemplo, a "query" (61) lista todas as encomendas de todos os clientes
 */
void listEncomendas(int queryType)
{
    if (NE == -1) {
        red(); printf("\n[!] Não existem encomendas! [!]"); white();
        pressEnter(true);
        return;
    }

    enum queryType {
        currentClient = 60,
        todas = 61,
        especifica = 62,
        dataEspecifica = 63,
    };

    cyan(); printf("\n------------------------ Encomenda(s) ------------------------\n"); white();

    int count = 0, semanaExportar, loopDias, looper = 1, mes;
    switch (queryType)
    {
    case currentClient:
        if (runtime->currentClient == 0)
        {
            red(); printf("\n[!] O utilizador em questão não se encontra com sessão iniciada! [!]\n"); white();
            loginClient();
        }
        for (int enc = 0; enc <= NE; enc++)
        {
            if (encomenda[enc].estado && encomenda[enc].codCliente == runtime->currentClient)
            {
                printDataEncomenda(enc);
                count++;
            }
        }
        if (count == 0)
        {
            red(); printf("\n[!] Não existem encomendas para o cliente atual [!]\n"); white();
            return;
        }
        bold(); printf("\n  [Total]: %0.2f€\n", getTotalEncomenda()); white();
        count = 0;
        break;

    case todas:
        for (int enc = 0; enc <= NE; enc++)
        {
            printDataEncomenda(enc);
        }
        if (NE == -1)
        {
            red(); printf("\n[!] Não existem encomendas registadas [!]\n"); white();
            pressEnter(true);
        }
        break;
    
    case especifica:
        /** @note Permite listar as encomendas de certo cliente,
         * este que é indicado via Nome ou pelo seu Código */
        printf("\nPesquisar Cliente por (N)ome ou (C)ódigo? ");
        char internalQueryType[10];
        scanf(" %s", internalQueryType);
        if (strcmp(internalQueryType, "N") == 0 || strcmp(internalQueryType, "n") == 0)
        {
            printf("\nNome do Cliente: ");
            char nomeCliente[SC];
            scanf(" %24s", nomeCliente);
            for (int enc = 0; enc <= NE; enc++)
            {
                if (encomenda[enc].estado && strcmp(cliente[encomenda[enc].codCliente].nome, nomeCliente) == 0)
                {
                    printDataEncomenda(enc);
                    count++;
                }
            }
        }
        else if (strcmp(internalQueryType, "C") == 0 || strcmp(internalQueryType, "c") == 0)
        {
            printf("\nCódigo do Cliente: ");
            int codCliente;
            scanf("%d", &codCliente);
            for (int enc = 0; enc <= NE; enc++)
            {
                if (encomenda[enc].estado && encomenda[enc].codCliente == codCliente)
                {
                    printDataEncomenda(enc);
                    count++;
                }
            }
        }
        if (count == 0)
        {
            red(); printf("\n[!] Não existem encomendas para o cliente indicado [!]"); white();
        }
        break;
    
    case dataEspecifica:
        /** @note Permite listar as encomendas de acordo com certa data */

        printf("Indique a semana a exportar:"
        "\n\n[1] Semana Atual"
        "\n[2] Semana Anterior\n> ");
        scanf("%d", &semanaExportar);

        switch (semanaExportar)
        {
        case 1:
            loopDias = getDateInfo(2) + 6;
            mes = getDateInfo(3);
            cyan(); printf("\n[Encomendas da Semana]\n"); white();
            while (looper <= loopDias)
            {
                for (int enc = 0; enc <= NE; enc++)
                {
                    if (encomenda[enc].estado && encomenda[enc].dia == looper && encomenda[enc].mes == mes)
                    {
                        printDataEncomenda(enc);
                        count++;
                    }
                }
                looper++;
            }
            break;

        case 2:
            loopDias = getDateInfo(2) - 6;
            cyan(); printf("\n[Encomendas da Semana Anterior]\n"); white();
            while (looper <= loopDias)
            {
                for (int enc = 0; enc <= NE; enc++)
                {
                    if (encomenda[enc].estado && encomenda[enc].dia == looper)
                    {
                        printDataEncomenda(enc);
                        count++;
                    }
                }
                looper++;
            }
            break;
        }
        
        if (count == 0)
        {
            red(); printf("\n[!] Não existem encomendas para a semana indicada [!]"); white();
        }
    }
    pressEnter(true);
}
#pragma endregion

#pragma region CancelEncomenda
/**
 * @brief Função que permite cancelar uma encomenda
 */
void cancelEncomenda()
{
    if (NE == -1) {
        red(); printf("\n[!] Não existem encomendas! [!]"); white();
        pressEnter(true);
        return;
    }
    else if (!loggedClient())
    {
        yellow(); printf("\n[!] Por favor inicie sessão [!]\n"); white();
        loginClient();
    }
    
    for (int enc = 0; enc <= NE; enc++)
    {
        if (encomenda[enc].estado && encomenda[enc].codCliente == runtime->currentClient)
        {
            printDataEncomenda(enc);
        }
    }

    int encomendaToCancel;
    printf("\nIndique o número da encomenda a cancelar: ");
    scanf("%d", &encomendaToCancel);
    encomendaToCancel--;

    if (encomenda[encomendaToCancel].estado && encomenda[encomendaToCancel].codCliente == runtime->currentClient)
    {
        encomenda[encomendaToCancel].estado = false;
        red(); printf("\n[!] Encomenda cancelada com sucesso! [!]"); white();
    }
    else
    {
        red(); printf("\n[!] Encomenda não encontrada! [!]"); white();
    }
    pressEnter(true);
}
#pragma endregion