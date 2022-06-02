#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "definitions.h"

#pragma region numberOfDigits
/** @brief Conta o número de digitos de um inteiro
 * @note Usado, por exemplo, para verificar o NIF do cliente
 * @return O número de digitos do inteiro para usar no respetivo "loop do while"
 */
int numberOfDigits(int num)
{
    int digits = 0;
    do
    {
        digits++;
        num /= 10;
    } while(num != 0);
    return digits;
}
#pragma endregion

#pragma region ClearScreen
/** * @brief Permite limpar o ecrã de forma "portátil" entre sistemas */
void clearScreen()
{
    printf("\033c");
}
#pragma endregion

#pragma region PressEnter
/** * @brief Permite dar a opção do tipico "Press Enter to continue" */
void pressEnter(bool alsoClearScreen)
{
    char stopper[5];
    printf("\n\nPressione [ENTER] para voltar...");
    scanf("%c", stopper);
    fgets(stopper,5,stdin);
    if (alsoClearScreen)
    {
        clearScreen();
    }
}
#pragma endregion

#pragma region CheckString
/** @brief Verifica se a string indicada é válida tendo em contas certas condições
 * @note A string é válida se: 
 * - não estiver vazia e conter pelo menos 3 caracteres
 * - não contém caracteres númericos
 * @return true se a string for válida, false caso contrário
 */
bool checkString(const char* string)
{
    if ( (string[0] >= 'A' && string[0] <= 'Z') && \
        (string[2] >= 'a' && string[2] <= 'z') ) 
    {
        return true;
    }
    else 
    {
        red(); printf("  \n[!] A palavra indicada devera seguir as seguintes regras [!]\n"
                      "- Comecar por uma letra maiúscula\n"
                      "- Ter pelo menos 3 caracteres\n"); white();
        return false;
    }
}
#pragma endregion

#pragma region CheckNumber
/** * @brief Controlar o inteiro indicado pelo utilizador
 * @return true se o inteiro for válido, false caso contrário
 */
bool checkNumber(double number)
{
    if (number <= 0)
    {
        red(); printf("  \n[!] O valor introduzido deverá ser maior que 0 [!]\n"); white();
        return false;
    }
    else
    {
        return true;
    }
}
#pragma endregion

#pragma region CheckCodigoPromocional
/** * @brief Verifica se o código promocional introduzido pelo utilizador é valido
 * @return true se o código for válido, false caso contrário
 */
bool checkCodPromo(char* codPromo)
{
    if (strcmp(codPromo, runtime->stringDesconto) == 0)
    {
        green(); printf("\n[!] Aplicado Desconto de %d%% [!]\n", \
        (int) (runtime->percentageDesconto * 100)); white(); 
        return true;
    }
    else
    {
        yellow(); printf("\n[!] Desconto Não Aplicado [!]\n"); white(); 
        return false;
    }
}
#pragma endregion

#pragma region availableForOperation
/** * @brief Verifica se o artigo OU parâmetro indicado está disponível ou é valido para X operação
 * @note As operações estão indicadas na enumeração
 * @return true se a operação/parâmetro é válido, false caso contrário
 * @note O valor do return é usado no respetivo "loop do while"
 */
bool availableForOperation(int op, int codArtigo, int parameter)
{
    enum Operation {
        delete = 1,
        update = 2,
        buy = 3,
        add = 4,
        checkSize = 5
    };

    switch (op)
    {
    case delete: // delete
        for (int e = 0; e <= NE; e++)
        {
            if (encomenda[e].codArtigo == codArtigo)
            {
                red(); printf("  \n[!] O Artigo NÃO pode ser removido [!]\n");
                printf("  [!] Existem encomendas com esse artigo [!]\n"); white();
                sleep(1);
                return false;
            }
        }
        return true;

    case update: // update
        for (int art = 0; art <= NA; art++)
        {
            if (artigo[art].codArtigo == codArtigo)
            {
                return true;
            }
        }
        red(); printf("\n[!] Artigo Não existe [!]\n"); white();
        return false;

    case buy: //comprar
        for (int art = 0; art < NA+1; art++)
        {
            if (artigo[art].codArtigo == codArtigo)
            {
                return true;
            }
        }
        if (!artigo[codArtigo].estado)
        {
            red(); printf("  \n[!] Artigo Não Disponivel [!]\n"); white();
            sleep(1);
            return false;
        }
        red(); printf("  \n[!] Artigo Não existe [!]\n"); white();
        sleep(1);
        return true;

    case add: // add
        if (!checkNumber((float) codArtigo)) {
            return false;
        }
        for (int art = 0; art < NA; art++)
        {
            if (artigo[art].codArtigo == codArtigo)
            {
                red(); printf("\n[!] Artigo já existe [!]\n"); white();
                sleep(1);
                return false;
            }
        }
        return true;

    case checkSize:
        codArtigo = getArtigoIndex(codArtigo);
        for (int tam = artigo[codArtigo].tamanhos[0]; tam <= artigo[codArtigo].tamanhos[1]; tam++)
        {
            if (tam == parameter)
            {
                return true;
            }
        }

        for (int tam = artigo[codArtigo].tamanhos[0]; tam <= artigo[codArtigo].tamanhos[1]; tam++)
        {
            if (tam == parameter)
            {
                return true;
            }
        }
        red(); printf("  \n[!] Tamanho não existe [!]\n"); white();
        sleep(1);

    default:
        return false;
    }
}
#pragma endregion

#pragma region loggedClient
/** * @brief Verifica se existe utilizador autenticado
 * @return true se existe utilizador autenticado, false caso contrário
 */
bool loggedClient()
{
    /// Não existe cliente logado
    if (runtime->currentClient != '\0')
    {
        return true;
    }
    return false;
}
#pragma endregion

#pragma region ReallocStruct
void reallocStruct(int st)
{
    switch (st)
    {
    case 1:
        artigo = realloc(artigo, sizeof(Artigos) * (++NA + 1));
        if (artigo == NULL)
        {
            red(); printf("  \n[!] Erro ao alocar memória [!]\n"); white();
            exit(1);
        }
        break;

    case 2:
        cliente = realloc(cliente, sizeof(Clientes) * (++NC + 1));
        if (cliente == NULL)
        {
            red(); printf("  \n[!] Erro ao alocar memória [!]\n"); white();
            exit(1);
        }
        break;

    case 3:
        encomenda = realloc(encomenda, sizeof(Encomendas) * (++NE + 1));
        if (encomenda == NULL)
        {
            red(); printf("  \n[!] Erro ao alocar memória [!]\n"); white();
            exit(1);
        }
        break;
    }
}
#pragma endregion

#pragma region FreeMemory
/** * @brief Liberta a memória antes de fechar o programa e/ou termina o programa 
 * @param alsoStopTheProgram - Opção de saída do programa
*/
void freeMemory(bool alsoStopTheProgram) {  

    free(artigo);
    free(cliente);
    free(encomenda);
    if (alsoStopTheProgram)
    {
        exit(0);
    }
}
#pragma endregion