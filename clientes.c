#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "definitions.h"

/* =========================== Operation Related Functions =========================== */

#pragma region addClient
/**
 * @brief Função que permite adicionar um cliente
 * @param Os parametros necessarios para a adição do cliente
 */
void addCliente()
{
    cyan(); printf("\n_______ Adicionar Cliente _______\n"); white();
    reallocStruct(2);

    do {
        printf("\nNome Cliente: ");
        scanf("%s", cliente[NC].nome);
    } while (!checkString(cliente[NC].nome));

    do
    {
        printf("\nNIF (deverá conter 9 números): ");
        scanf("%d", &cliente[NC].nif);
        
        if (numberOfDigits(cliente[NC].nif) != 9)
        {
            red(); printf("\n[!] NIF INVÁLIDO [!]\n");
            printf("[!] Este deverá conter 9 números [!]\n"); white();
        }
    } while (numberOfDigits(cliente[NC].nif) != 9);

    bool flag;
    do {
        flag = false;
        printf("\nCódigo Cliente (deverá conter até 4 números): ");
        scanf("%d", &cliente[NC].codCliente);
        if (numberOfDigits(cliente[NC].codCliente) != 4)
        {
            red(); printf("\n[!] Código Cliente INVÁLIDO [!]\n");
            printf("[!] Este deverá até 4 números [!]\n"); white();
            flag = true;
            continue;
        }
        for (int i = 0; i < NC; i++)
        {
            if (cliente[i].codCliente == cliente[NC].codCliente)
            {
                red(); printf("\n[!] Código Cliente JÁ EXISTE [!]\n");
                printf("[!] Este deverá ser único [!]\n"); white();
                flag = true;
                break;
            }
        }
    } while (flag);

    do
    {
        printf("\nPaís do cliente: ");
        scanf(" %24s", cliente[NC].pais);
    } while (!checkString(cliente[NC].pais));

    cliente[NC].estado = true;

    green(); printf("\n[!] Cliente %s criado com sucesso [!]\n", cliente[NC].nome); white();
    sleep(1); clearScreen();
}
#pragma endregion

#pragma region updateClient
/**
 * @brief Função que permite atualizar um cliente
 * @param clienteToChange - Cliente a ser atualizado
 * @param via - caso a função seja chamada via menu cliente,
 * apenas permitir a alteração do cliente autenticado (e perguntando o seu código)
 */
void updateClient(int clienteToChange, int via)
{
    if (NC == -1)
    {
        red(); printf("\n[!] Não existem clientes [!]"); white();
        pressEnter(true);
        return;
    }

    cyan(); printf("\n_______ Atualizar Cliente _______\n"); white();

    /** @note Via Menu Cliente, onde se realiza verificação de segurança */
    if (runtime->currentClient != 0 && via == 1)
    {
        yellow(); printf("\nO cliente %s está autenticado\n", getClientName(runtime->currentClient));
        yellow(); printf("\nPor questões de segurança prove que é o utilizador correto introduzindo o seu código: ");  white();
        int antibruteforce = 0;
        scanf("%d", &clienteToChange);
        while (clienteToChange != runtime->currentClient && antibruteforce < 2)
        {
            red(); printf("\n[!] Código incorreto [!]\n"); white();
            yellow(); printf("\nPor questões de segurança prove que é o utilizador correto introduzindo o seu código: "); white();
            scanf("%d", &clienteToChange);
            antibruteforce++;
        }
        if (antibruteforce == 2)
        {
            red(); printf("\n[!] Número de tentativas excedidas [!]\n"); white();
            pressEnter(true);
            return;
        }
    }
    else if (runtime->currentClient == 0 && via == 1)
    {
        red(); printf("\n[!] Não existe cliente autenticado [!]\n"); white();
        yellow(); printf("\nPor questões de segurança, o cliente a ser alterado deverá ser autenticado\n"); white();
        pressEnter(true);
        return;
    }

    /** @note Via Menu Administrador */
    if (via == 0)
    {    
        do
        {
            printf("\nCliente (Código): ");
            scanf("%d", &clienteToChange);
            if (!getEstadoCliente(clienteToChange))
            {
                red(); printf("\n[!] Cliente não existe, indique um cliente válido [!]\n"); white();
            }
        } while (!getEstadoCliente(clienteToChange));
    }


    printf("\nInformação a alterar para o cliente %s:\n", getClientName(clienteToChange));
    printf("  [1] Nome\n");
    printf("  [2] NIF\n");
    printf("  [3] Código Cliente\n");
    printf("  [4] País\n> ");
    int opcao;
    scanf("%d", &opcao);
    
    for (int clients = 0; clients <= NC; clients++)
    {
        if (cliente[clients].codCliente == clienteToChange)
        {
            clienteToChange = clients;
        }
    }

    switch (opcao)
    {
        char newString[SC]; int newValue;
        case 1:
            do
            {
                printf("\nNovo Nome: ");
                scanf("%24s", newString);
            } while (!checkString(newString));
            strcpy(cliente[clienteToChange].nome, newString);
            green(); printf("\n[!] Nome alterado com sucesso [!]\n"); white();
            break;

        case 2:
            do
            {
                printf("\nNovo NIF: ");
                scanf("%d", &newValue);
                if (numberOfDigits(newValue) != 9)
                {
                    red(); printf("\n[!] NIF INVÁLIDO [!]\n");
                    printf("[!] Este deverá conter 9 números [!]\n"); white();
                }
            } while (numberOfDigits(newValue) != 9);
            cliente[clienteToChange].nif = newValue;
            green(); printf("\n[!] NIF alterado com sucesso [!]\n"); white();
            break;

        case 3:
            do
            {
                printf("\nNovo Código Cliente: ");
                scanf(" %d", &newValue);
            } while (numberOfDigits(newValue) != 4);
            cliente[clienteToChange].codCliente = newValue;
            green(); printf("\n[!] Código Cliente alterado com sucesso [!]\n"); white();
            break;

        case 4:
            do
            {
                printf("\nNovo País: ");
                scanf(" %24s", newString);
            } while (!checkString(newString));
            strcpy(cliente[clienteToChange].pais, newString);
            green(); printf("\n[!] País alterado com sucesso [!]\n"); white();
            break;

        default:
            red(); printf("\n[!] Opção Inválida [!]\n"); white();
            sleep(1); clearScreen();
            updateClient(clienteToChange, via);
    }

    printf("\nPretende alterar outra informação? (s/N)\n> ");
    char opcao3;
    scanf(" %c", &opcao3);
    if (opcao3 == 's' || opcao3 == 'S')
    {
        updateClient(clienteToChange, via);
    }
    clearScreen();
}
#pragma endregion

/* =========================== Cliente Specific Queries =========================== */

#pragma region getEstadoCliente
/**
 * @brief Função que verifica o estado do cliente indicado
 * @param ID do cliente
 * @return Retorna True - caso o cliente exista ou esteja disponivel
 * False - caso o cliente não exista ou esteja indisponivel
 */
bool getEstadoCliente(int codCliente)
{
    for (int user = 0; user <= NC; user++)
    {
        if (cliente[user].codCliente == codCliente)
        {
            if (cliente[user].estado)
            {
                return true;
            }
            else
            {
                red(); printf("\n[!] Cliente não está disponível [!]\n"); white();
                return false;
            }
        }
    }
    return 0;
}
#pragma endregion

#pragma region getClienteName
/**
 * @brief Função que obtêm o nome do cliente
 * @param ID do cliente
 * @return Retorna o nome do cliente (string)
 */
char* getClientName(int codCliente)
{
    for (int user = 0; user <= NC; user++)
    {
        if (cliente[user].codCliente == codCliente)
        {
            return cliente[user].nome;
        }
    }
    return NULL;
}
#pragma endregion