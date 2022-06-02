#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "definitions.h"

#pragma region GetArtigoIndex
/** @brief Função que permite obter o indíce de um artigo no array
 *  @param codArtigo - Código do artigo
 *  @return art - Indíce do artigo
 */
int getArtigoIndex(int codArtigo)
{
    for (int art = 0; art <= NA; art++)
    {
        if (artigo[art].codArtigo == codArtigo)
        {
            return art;
        }
    }
    return -1;
}
#pragma endregion

#pragma region getArtigoName
/** @brief Função que permite obter o nome do artigo
 *  @param idx - Indice no array de artigos
 *  @return artigo - O nome do artigo
 */
char* getArtigoName(int idx)
{         
    return artigo[idx].artigo;
}
#pragma endregion

#pragma region getTotalEncomenda
/**
 * @brief Função que permite obter o total (preço) de encomendas
 * @return int - total de encomendas
 */
double getTotalEncomenda()
{
    double total = 0;
    for (int enc = 0; enc <= NE+1; enc++)
    {
        if (encomenda[enc].codCliente == runtime->currentClient)
        {
            if (encomenda[enc].estado)
            {
                total += encomenda[enc].total;
            }
        }
    }
    return total;
}
#pragma endregion

#pragma region GetDateInfo
/** * @brief Obter certo parâmetro de data tal como hora, dia, mês, ano
 * @note A enumeração é usada para indicar o parâmetro a obter
 */
int getDateInfo(int info)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    switch (info)
    {
    enum InfoQuery {
        hourminute = 1,
        day = 2,
        month = 3,
        year = 4,
    };
        case hourminute:
            return tm.tm_hour * 100 + tm.tm_min;
        case day:
            return tm.tm_mday;
        case month:
            return tm.tm_mon + 1;
        case year:
            return tm.tm_year + 1900;
    }
    return -1;
}
#pragma endregion

#pragma region listArtigos
/** @brief Função que permite listar o(s) artigo(s)
 * @param queryType - opção de listagem
 * @note - a opção (10) é quando o cliente autenticado pretende listar as encomendas da sessão
 */
void listArtigos(int queryType)
{   
    // queryType 10&11 = listar todos os artigos disponiveis com valores REDUZIDOS
    // queryType 12 = listar todos os artigos disponiveis COM todos os valores
    // queryType 13 = listar um artigo especifico COM todos os valories

    enum queryType {
        reduzidaCompras = 10,
        reduzidaAdmin = 11,
        completa = 12,
        especifica = 13
    };

    if (NA == -1)
    {
        red(); printf("\n[!] Não existem artigos registados [!]"); white();
        pressEnter(true);
        return;
    }
    int count;
    cyan(); printf("\n------------------------ Artigos ------------------------\n"); white();
    switch (queryType)
    {
        case reduzidaCompras:
            for (int art = 0; art <= NA; art++)
            {
                if (artigo[art].estado)
                {
                    printf("\nArtigo: %s | Código: %d", artigo[art].artigo, artigo[art].codArtigo);
                    printf("\nPreço comum: %.2f€\n", artigo[art].defaultPreco);
                    printf("|_ Tamanhos disponiveis: %d - %d\n\n", artigo[art].tamanhos[0], artigo[art].tamanhos[1]);
                }
            }
            break;

        case reduzidaAdmin:
            for (int art = 0; art <= NA; art++)
            {
                if (artigo[art].estado)
                {
                    printDataArtigo(art, false);
                }
            }
            printf("\nTotal de Artigos - %d\n", NA + 1);
            
            if (queryType == reduzidaAdmin)
            {
                pressEnter(true);
            }
            break;

        case completa:
            for (int art = 0; art <= NA; art++)
            {
                printDataArtigo(art, true);
                
            }
            printf("\nTotal de Artigos - %d\n", NA + 1);
            pressEnter(true);
            break;
        
        case especifica:
            printf("\nPesquisar Artigo por (N)ome ou (C)ódigo? ");
            char internalQueryType[5];
            scanf(" %s", internalQueryType);
            if (strcmp(internalQueryType, "n") == 0 || strcmp(internalQueryType, "N") == 0)
            {
                printf("\nPesquisar por Nome: ");
                char nomePesquisa[SC];
                scanf(" %s", nomePesquisa);
                count = 0;
                for (int art = 0; art <= NA; art++)
                {
                    if (artigo[art].estado && (strcmp(artigo[art].artigo, nomePesquisa) == 0))
                    {
                        printDataArtigo(art, true);
                        count++;
                    }
                }
                if (count == 0)
                {
                    red(); printf("\n[!] Não existem artigos com esse nome [!]"); white();
                    pressEnter(true);
                    return;
                }
            }
            else if (strcmp(internalQueryType, "c") == 0 || strcmp(internalQueryType, "C") == 0)
            {
                printf("\nPesquisar por Código: ");
                int codigoPesquisa;
                scanf("%d", &codigoPesquisa);
                count = 0;
                for (int art = 0; art <= NA; art++)
                {
                    if (artigo[art].estado && (artigo[art].codArtigo == codigoPesquisa))
                    {
                        printDataArtigo(art, true);
                        count++;
                        break;
                    }
                }
                if (count == 0)
                {
                    red(); printf("\n[!] Não existem artigos com esse código [!]"); white();
                    pressEnter(true);
                    return;
                }
            } else {
                red(); printf("\n[!] Opção Inválida [!]"); white();
                pressEnter(true);
                return;
            }
        printf("\nTotal de Artigos - %d", NA + 1);
        pressEnter(true);
        break;
    }
}
#pragma endregion

#pragma region listClientes
/** @brief Função que permite listar o(s) cliente(s)
 * @param queryType - opção de listagem
 */
void listClientes(int queryType)
{
    if (NC == -1)
    {
        red(); printf("\n[!] Não existem clientes registados [!]"); white();
        pressEnter(true);
        return;
    }
    enum queryType {
        todos = 51,
        especifico = 52,
        autoQuery = 53
    };

    cyan(); printf("\n------------------------ Clientes ------------------------\n"); white();
    switch (queryType)
    {
        case todos:
            for (int user = 0; user <= NC; user++)
            {
                if (cliente[user].estado)
                {
                    printDataCliente(user);                   
                }
            }
            break;

        case especifico:
            printf("Pesquisar Cliente por (N)ome ou (C)ódigo? ");
            char internalQueryType[SC];
            scanf(" %s", internalQueryType);
            if (strcmp(internalQueryType, "N") == 0 || strcmp(internalQueryType, "n") == 0)
            {
                printf("\nNome: ");
                char nome[50];
                scanf(" %50s", nome);
                for (int user = 0; user <= NC; user++)
                {
                    if (cliente[user].estado)
                    {
                        if (strcmp(cliente[user].nome, nome) == 0)
                        {
                            printDataCliente(user);
                        }
                    }
                }
            }
            else if (strcmp(internalQueryType, "C") == 0 || strcmp(internalQueryType, "c") == 0)
            {
                printf("\nCódigo Cliente: ");
                int codCliente;
                scanf("%d", &codCliente);
                for (int user = 0; user <= NC; user++)
                {
                    if (cliente[user].estado && cliente[user].codCliente == codCliente)
                    {
                        printDataCliente(user);
                    }
                }
            break;
        }
    }
    pressEnter(true);
}
#pragma endregion

#pragma region EstatisticasEncomendas
/**
 * @brief Estatisticas das encomendas
 * @note Média do Preço e Quantidade
 * @note Melhor e Pior produto (em termos de vendas)
 */
void statisticsEncomendas()
{
    if (NE == -1) {
        red(); printf("\n[!] Não existem encomendas! [!]"); white();
        pressEnter(true);
        return;
    } else if (NE == 0) {
        red(); printf("\n[!] Não existem encomendas sufientes para gerar estatisticas! [!]"); white();
        pressEnter(true);
        return;
    }
    /** @note Encontrar as médias (preço e quantidade) das encomendas */
    float averagePrice = 0, averageQuantity = 0;
    for (int enc = 0; enc <= NE; enc++)
    {
        if (encomenda[enc].estado)
        {
            averagePrice += encomenda[enc].total;
            averageQuantity += (float) encomenda[enc].quantidade;
        }
    }
    averagePrice = averagePrice / (float) (NE + 1);
    averageQuantity = averageQuantity / (float) (NE + 1);

    cyan(); printf("\n  [ Média Encomendas ]"); white();
    printf("\n  | Preço: %0.2f€", averagePrice);
    printf("\n  | Quantidade: %d", (int) averageQuantity);
    printf("\n  | Total Encomendas: %d\n", NE + 1);

    // Best and Worst Seller
    int codArtigoHQ, higherQuantity, codArtigoLQ, lowerQuantity, codBestBuyer, bestBuyerQuantity = 0;
    
    /** @note Encontrar o produto mais vendido */
    for (int enc = 0; enc <= NE; enc++)
    {
        if (encomenda[enc].estado)
        {
            if (encomenda[enc].quantidade > higherQuantity)
            {
                higherQuantity = encomenda[enc].quantidade;
                codArtigoHQ = encomenda[enc].codArtigo;
            }
        }
    }

    /** @note Encontrar o produto menos vendido */
    lowerQuantity = higherQuantity;
    for (int enc = 0; enc <= NE; enc++)
    {
        if (encomenda[enc].estado)
        {
            if (encomenda[enc].quantidade < lowerQuantity)
            {
                lowerQuantity = encomenda[enc].quantidade;
                codArtigoLQ = encomenda[enc].codArtigo;
            }
        }
    }
    
    codArtigoHQ = getArtigoIndex(codArtigoHQ); codArtigoLQ = getArtigoIndex(codArtigoLQ);
    
    cyan(); printf("\n  [ Produto Mais & Menos Vendido ]"); white();
    printf("\n  | Artigo:"); green(); printf(" %s com %d unidades vendidas", \
    getArtigoName(codArtigoHQ), higherQuantity); white();
    printf("\n  | Artigo:"); red(); printf(" %s com %d unidades vendidas\n", \
    getArtigoName(codArtigoLQ), lowerQuantity); white();


    /** @note Encontrar o cliente com mais unidades compradas */
    for (int enc = 0; enc <= NE; enc++)
    {
        if (encomenda[enc].estado)
        {
            if (encomenda[enc].quantidade > bestBuyerQuantity)
            {
                bestBuyerQuantity = encomenda[enc].quantidade;
                codBestBuyer = encomenda[enc].codCliente;
            }
        }
    }

    cyan(); printf("\n  [ Cliente que mais comprou ]"); white();
    printf("\n  | Cliente:"); green(); printf(" %s com %d unidades compradas\n", \
    getClientName(codBestBuyer), bestBuyerQuantity); white();
    

    /** @note Encontrar o artigo com maior margem de lucro */
    int codArtigoMLH, codArtigoMLL;
    float higherMargemLucro;
    for (int art = 0; art <= NA; art++)
    {
        if (artigo[art].estado)
        {
            if (artigo[art].margemLucro > higherMargemLucro)
            {
                higherMargemLucro = artigo[art].margemLucro;
                codArtigoMLH = art;
            }
        }
    }

    /** @note Encontrar o artigo com menor margem de lucro */
    float lowerMargemLucro = higherMargemLucro;
    for (int art = 0; art <= NA; art++)
    {
        if (artigo[art].estado)
        {
            if (artigo[art].margemLucro < lowerMargemLucro)
            {
                lowerMargemLucro = artigo[art].margemLucro;
                codArtigoMLL = art;
            }
        }
    }

    cyan(); printf("\n  [ Melhor & Pior Margem de Lucro ]"); white();
    printf("\n  | Artigo:"); green(); printf(" %s com %0.2f€ de margem de lucro", \
    getArtigoName(codArtigoMLH), higherMargemLucro); white();
    printf("\n  | Artigo:"); red(); printf(" %s com %0.2f€ de margem de lucro\n", \
    getArtigoName(codArtigoMLL), lowerMargemLucro); white();

    
    /** @note Encontrar a quantidade de encomendas ativas/canceladas */
    int stateEncomendas[2] = {0, 0};
    for (int enc = 0; enc <= NE; enc++)
    {
        if (encomenda[enc].estado)
        {
            stateEncomendas[0] += 1;
        }
        else
        {
            stateEncomendas[1] += 1;
        }
    }

    double percentagemEncomendas[2] = {0, 0};
    percentagemEncomendas[0] = (double) stateEncomendas[0] / (double) (NE + 1);
    percentagemEncomendas[1] = (double) stateEncomendas[1] / (double) (NE + 1);

    cyan(); printf("\n  [ Encomendas Ativas & Canceladas ]"); white();
    printf("\n  | Ativas: %d -", stateEncomendas[0]); green();
    printf(" %0.2f%% das encomendas", percentagemEncomendas[0] * 100); white();
    printf("\n  | Canceladas: %d -", stateEncomendas[1]); red();
    printf(" %0.2f%% das encomendas", percentagemEncomendas[1] * 100); white();

    pressEnter(true);
}
#pragma endregion

#pragma region printDataEncomenda
/**
 * @brief Imprime os dados de uma encomenda
 * @param enc - Encomenda a imprimir
 */
void printDataEncomenda(int enc)
{
    printf("\n[Código Encomenda]: %d ", encomenda[enc].codEncomenda);
    printf("\n  | Código Cliente: %d (%s)", encomenda[enc].codCliente, getClientName(encomenda[enc].codCliente));
    printf("\n  | %d pare(s) de %s", encomenda[enc].quantidade, getArtigoName(getArtigoIndex(encomenda[enc].codArtigo)));
    printf("\n  | Preço Encomenda: %0.2f€", encomenda[enc].total);
    printf("\n  | Data: %d %d/%d/%d", encomenda[enc].horaminuto, encomenda[enc].dia, encomenda[enc].mes, encomenda[enc].ano);
    printf("\n  | [Estado]: %s\n", getEstadoEncomenda(encomenda[enc].estado));
}
#pragma endregion

#pragma region printDataArtigo
/**
 * @brief Imprime os dados de um artigo
 * @param art - Artigo a imprimir
 * @param printTamanhas - Define se os tamanhos e os respetivos preços devem ser imprimidos
 */
void printDataArtigo(int art, bool printTamanhos)
{
    int looperTamanhos = 0;

    printf("\n"
    "Artigo: %s (Código: %d)\n"
    "  | Mão Obra: %0.2f€\n"
    "  | Custos Fixos: %0.2f€\n"
    "  | Margem Lucro: %0.2f€\n"
    "  | Preco por defeito: %0.2f€\n"
    "  | Incremento (por tamanho): +%0.2f€\n"
    "  [Estado] %s", artigo[art].artigo, artigo[art].codArtigo, artigo[art].maoObra,  \
    artigo[art].custosFixos, artigo[art].margemLucro, artigo[art].defaultPreco, artigo[art].incrementoPreco, getEstadoEncomenda(artigo[art].estado));

    if (printTamanhos) {
        printf("\t| Tamanho(s) e Preço(s):\n");
        for (int j = artigo[art].tamanhos[0]; j <= artigo[art].tamanhos[1]; j++)
        {
            printf("\t|_ Tamanho: %d | Preço: %0.2f€\n", \
            (int) artigo[art].tamanhoPreco[looperTamanhos][0], artigo[art].tamanhoPreco[looperTamanhos][1]);
            looperTamanhos++;
        }
    }
}
#pragma endregion

#pragma region printDataCliente
/**
 * @brief Imprime os dados de um cliente
 * @param cli - Cliente a imprimir
 */
void printDataCliente(int cli)
{
    printf("\n[Código Cliente]: %d", cliente[cli].codCliente);
    printf("\n  | Nome: %s", cliente[cli].nome);
    printf("\n  | NIF: %d", cliente[cli].nif);
    printf("\n  | País: %s\n", cliente[cli].pais);
}
#pragma endregion