#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "definitions.h"

#pragma region AddArtigo
/**
 * @brief Função que permite adicionar um artigo
 */
void addArtigo()
{
    reallocStruct(1);
    cyan(); printf("\n--------------- Novo Artigo ---------------\n"); white();
    do
    {
        printf("\nCódigo: ");
        scanf("%d", &artigo[NA].codArtigo);
    } while (!availableForOperation(4, artigo[NA].codArtigo, 0));

    do {
        printf("\nNome do Artigo: ");
        scanf("%s", artigo[NA].artigo);
    } while (!checkString(artigo[NA].artigo));

    do {
        printf("\nMão de Obra (em €): ");
        scanf(" %lf", &artigo[NA].maoObra);
    } while (!checkNumber(artigo[NA].maoObra));

    do {
        printf("\nCustos Fixos (em €): ");
        scanf(" %lf", &artigo[NA].custosFixos);
    } while (!checkNumber(artigo[NA].custosFixos));

    do {
        printf("\nMargem de Lucro (em €): ");
        scanf(" %lf", &artigo[NA].margemLucro);
    } while (!checkNumber(artigo[NA].margemLucro));

    do {
        printf("\nIncremento de preco dos tamanho (em €): ");
        scanf(" %lf", &artigo[NA].incrementoPreco);
    } while (!checkNumber(artigo[NA].incrementoPreco));
    yellow(); printf("\n[!] Caso pretenda alterar um preço especifico de um tamanho, \n" \
    "deve alterar o preço do incremento manualmente (Opção 51) [!]\n"); white();
    sleep(1);

    artigo[NA].defaultPreco = artigo[NA].maoObra + artigo[NA].custosFixos + artigo[NA].margemLucro;

    printf("\nPretende alterar os tamanhos disponiveis? (s/N)\n> ");
    char opcao;
    scanf(" %c", &opcao);
    if (opcao == 's' || opcao == 'S')
    {
        do {
            printf("\nTamanho Inicial: ");
            scanf(" %d", &artigo[NA].tamanhos[0]);
        } while (!checkNumber(artigo[NA].tamanhos[0]));

        do {
            printf("\nTamanho Final: ");
            scanf(" %d", &artigo[NA].tamanhos[1]);
            if (artigo[NA].tamanhos[1] < artigo[NA].tamanhos[0])
            {
                red(); printf("\n[!] Tamanho final inferior ao inicial! [!]\n"); white();
            }
        } while (!checkNumber(artigo[NA].tamanhos[1]) || artigo[NA].tamanhos[1] < artigo[NA].tamanhos[0]);
    }
    else
    {
        artigo[NA].tamanhos[0] = 34;
        artigo[NA].tamanhos[1] = 48;
    }

    int i = 0;
    for(int tam = artigo[NA].tamanhos[0]; tam <= artigo[NA].tamanhos[1]; tam++)
    {
        artigo[NA].tamanhoPreco[i][0] = artigo[NA].tamanhos[0] + i;
        artigo[NA].tamanhoPreco[i][1] = artigo[NA].defaultPreco;
        artigo[NA].defaultPreco += artigo[NA].incrementoPreco;
        i++;
    }

    artigo[NA].estado = true;

    green(); printf("\n[!] Artigo adicionado com sucesso [!]"); white();

    pressEnter(true);
}
#pragma endregion

#pragma region UpdateArtigo
/**
 * @brief Função de atualização de artigo e os seus valores
 * @note artigoToUpdate - Artigo a atualizar
 * @note opcao - Campo a alterar
 * @note novoXXX - Novo valor a ser atribuido
 */
void updateArtigo()
{
    if (NA == -1)
    {
        red(); printf("\n[!] Não existem artigos para atualizar! [!]"); white();
        pressEnter(true);
        return;
    }
    cyan(); printf("\n------------------------ Artigos ------------------------\n"); white();
    for (int art = 0; art <= NA; art++)
    {
        printf("Artigo: %s\n\t| Código: %d\n\n", artigo[art].artigo, artigo[art].codArtigo);
    }

    int artigoToUpdate;
    do
    {
        printf("\nArtigo a atualizar (Código): ");
        scanf("%d", &artigoToUpdate);
    } while (!availableForOperation(2, artigoToUpdate, 0));
    
    artigoToUpdate = getArtigoIndex(artigoToUpdate);

    int opcao;
    do
    {
        yellow(); printf("\n-------------------- Alterar %s", getArtigoName(artigoToUpdate)); printf(" --------------------\n"); white();
        printf("  [1] - Alterar Nome\n");
        printf("  [2] - Alterar Mão de Obra\n");
        printf("  [3] - Alterar Margem de Lucro\n");
        printf("  [4] - Alterar Preço por Defeito\n");
        printf("  [5] - Alterar Preço por Tamanho\n");
        printf("  [6] - Alterar Estado\n");
        printf("  [7] - Alterar Código\n");
        bold(); printf("\n  [0] - Voltar ao Menu\n"); white();

        printf("\nOpcão: ");
        scanf("%d", &opcao);

        int tamanhoAlterar, codAntigo;
        float novoPreco;
        int novoCodigo;
        bool flag;
        char novoNome[SC];

        enum menuUpdate {
            updateNome = 1,
            updateMaoObra = 2,
            updateMargemLucro = 3,
            updatePrecoDefault = 4,
            updatePrecoTamanho = 5,
            updateEstado = 6,
            updateCodigo = 7,
            updateIncremento = 8,
            voltar = 0
        };

        switch (opcao)
        {
            case updateNome:

                do {
                    printf("Novo Nome: ");
                    scanf("%s", novoNome);
                } while (!checkString(novoNome));
                strcpy(artigo[artigoToUpdate].artigo, novoNome);

                green(); printf("\n[!] Nome do Artigo [%s] alterado com sucesso [!]\n", \
                artigo[artigoToUpdate].artigo); white();
                break;

            case updateMaoObra:
                printf("\nPreco atual da mão de obra [%0.2f€]: ", artigo[artigoToUpdate].maoObra);

                do {
                    printf("\nNova Mão de Obra: ");
                    scanf(" %f", &novoPreco);
                } while (!checkNumber(novoPreco));

                yellow(); printf("\nNovo preço da mão de obra [%0.2f€]: ", novoPreco); white();
                green(); printf("\n[!] Preço da mão de obra [Artigo %s] alterado com sucesso [!]\n", \
                artigo[artigoToUpdate].artigo); white();

                artigo[artigoToUpdate].maoObra = novoPreco;
                break;

            case updateMargemLucro:
                printf("\nPreço atual da margem de lucro [%0.2f€]: ", artigo[artigoToUpdate].margemLucro);

                do {
                    printf("\nNova Margem de Lucro: ");
                    scanf(" %f", &novoPreco);
                } while (!checkNumber(novoPreco));

                yellow(); printf("\nNovo preço da margem de lucro [%0.2f€]: ", novoPreco); white();
                green(); printf("\n[!] Preço da margem de lucro [Artigo %s] alterado com sucesso [!]\n", \
                artigo[artigoToUpdate].artigo); white();

                artigo[artigoToUpdate].margemLucro = novoPreco;
                break;

            case updatePrecoDefault:
                printf("\nPreço atual por defeito [%0.2f€]: ", artigo[artigoToUpdate].defaultPreco);

                do {
                    printf("\nNovo Preço por Defeito: ");
                    scanf(" %f", &novoPreco);
                } while (!checkNumber(novoPreco));

                yellow(); printf("\nNovo preço por defeito [%0.2f€]: ", novoPreco); white();
                green(); printf("\n[!] Preco por defeito [Artigo %s] alterado com sucesso [!]\n", \
                artigo[artigoToUpdate].artigo); white();

                artigo[artigoToUpdate].defaultPreco = novoPreco;
                break;

            case updatePrecoTamanho:
                do {
                    printf("\nTamanho a alterar: ");
                    scanf("%d", &tamanhoAlterar);
                } while (!availableForOperation(5, artigoToUpdate, tamanhoAlterar));

                for (int tam = artigo[artigoToUpdate].tamanhos[0]; tam <= artigo[artigoToUpdate].tamanhos[1]; tam++)
                {
                    if (tam == tamanhoAlterar)
                    {
                        printf("\nPreço atual do tamanho [%d] [%0.2f€]\n", tam, artigo[artigoToUpdate].tamanhoPreco[tam - artigo[artigoToUpdate].tamanhos[0]][1]);

                        do {
                            printf("\nNovo Preço: ");
                            scanf(" %f", &novoPreco);
                        } while (checkNumber(novoPreco) == false);

                        yellow(); printf("\nNovo preço [%0.2f€]: ", novoPreco); white();
                        green(); printf("\n[!] Preço [Artigo %s] alterado com sucesso [!]\n", \
                        artigo[artigoToUpdate].artigo); white();

                        artigo[artigoToUpdate].tamanhoPreco[tam - artigo[artigoToUpdate].tamanhos[0]][1] = novoPreco;
                        break;
                    }
                }
                break;

            case updateEstado:
                printf("\nEstado atual [%s]: ", artigo[artigoToUpdate].estado ? "Disponivel" : "Indisponivel");
                printf("\nNovo Estado\n  [1] Disponivel\n  [2] Indisponivel\n> ");
                int novoEstado;
                scanf("%d", &novoEstado);
                if (novoEstado == 1)
                {
                    artigo[artigoToUpdate].estado = true;
                }
                else
                {
                    artigo[artigoToUpdate].estado = false;
                }
                green(); printf("\n[!] Estado [Artigo %s] alterado com sucesso [!]\n", \
                getArtigoName(artigoToUpdate)); white();
                break;

            case updateCodigo:
                codAntigo = artigo[artigoToUpdate].codArtigo;
                do {
                    flag = false;
                    do {
                        printf("\nNovo Código: ");
                        scanf("%d", &novoCodigo);
                    } while (!checkNumber(novoCodigo));
                    for (int i = 0; i <= NA; i++) {
                        if (artigo[i].codArtigo == novoCodigo) {
                            red(); printf("\nCódigo já existente!\n"); white();
                            flag = true;
                            break;
                        }
                    }
                } while (flag);
                artigo[artigoToUpdate].codArtigo = novoCodigo;
                green(); printf("\n[!] Codigo [Artigo %s] alterado com sucesso [!]\n", \
                artigo[artigoToUpdate].artigo); white();
                for (int i = 0; i <= NE; i++) {
                    if (encomenda[i].codArtigo == codAntigo) {
                        encomenda[i].codArtigo = novoCodigo;
                    }
                }
                break;

            case updateIncremento:
                do {
                    printf("\nIncremento atual [%0.2f€]: ", artigo[artigoToUpdate].incrementoPreco);
                    printf("\nNovo Incremento: ");
                    scanf(" %lf", &artigo[artigoToUpdate].incrementoPreco);
                } while (!checkNumber(artigo[artigoToUpdate].incrementoPreco));

            case voltar:
                return;

            default:
                red(); printf("\n[!] Opcão Inválida [!]\n"); white();
                break;
        }

        /** @note -- Recalcula o preço de todos os artigos de acordo com os novos valores */
        yellow(); printf("\n[!] Recalculando preços [!]\n"); white();
        artigo[artigoToUpdate].defaultPreco = artigo[artigoToUpdate].maoObra + artigo[artigoToUpdate].custosFixos + artigo[artigoToUpdate].margemLucro;
        int count = 0;
        for (int tam = artigo[artigoToUpdate].tamanhos[0]; tam <= artigo[artigoToUpdate].tamanhos[1]; tam++)
        {
            artigo[artigoToUpdate].tamanhoPreco[count][1] = artigo[artigoToUpdate].defaultPreco + (artigo[artigoToUpdate].incrementoPreco * (count + 1));
            count++;
        }
        printf("\nPretende realizar outra operacão? (s/N): ");
        char resposta[2];
        scanf(" %c", &resposta);
        if (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0)
        {
            updateArtigo();
        }
        else
        {
            return;
        }
    } while (opcao != 0);
}
#pragma endregion

#pragma region UpdateDesconto
/**
 * @brief Função de atualização do valor e código de desconto
 * @note runtime->percentageDesconto - Percentagem de desconto
 * @note runtime->stringDesconto - Código de desconto
 */
void updateDesconto()
{
    cyan(); printf("\n--------------- Alterar Desconto ---------------\n"); white();
    yellow(); printf("\nValor de desconto atual: %.f%%", runtime->percentageDesconto * 100); white();
    yellow(); printf("\nCódigo de desconto atual: %s\n", runtime->stringDesconto); white();

    printf("\nInformação a alterar\n  [1] Valor\n  [2] Código\n> ");
    int opcao;
    scanf("%d", &opcao);

    switch (opcao)
    {
        case 1:
            do {
                printf("\nNovo Valor de Desconto (em %%): ");
                scanf("%lf", &runtime->percentageDesconto);
            } while (!checkNumber(runtime->percentageDesconto));
            yellow(); printf("[!] Novo Valor de Desconto: %.f%% [!]\n", runtime->percentageDesconto); white();
            runtime->percentageDesconto /= 100;
            break;
        
        case 2:
            printf("\nNovo Código de Desconto: ");
            scanf("%7c", runtime->stringDesconto);
            yellow(); printf("\n[!] Novo Código de Desconto: %s [!]\n", runtime->stringDesconto); white();
            break;
        
        default:
            red(); printf("\n[!] Opção inválida [!]\n"); white();
            sleep(1);
            clearScreen();
            updateDesconto();
            break;
    }

    printf("\nPretende alterar outra informação? (s/N): ");
    char opcao2;
    scanf(" %c", &opcao2);
    if (opcao2 == 's' || opcao2 == 'S')
    {
        clearScreen();
        updateDesconto();
    }
}
#pragma endregion
