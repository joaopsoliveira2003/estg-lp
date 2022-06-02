#include <stdio.h>

#include "definitions.h"

#pragma region Colors
/**
 * @brief Permite o uso de "output" colorido
 */
void white()
{
    printf("\033[0m");
}

void red()
{
    printf("\033[0;31m");
}

void green()
{
    printf("\033[0;32m");
}

void blue()
{
    printf("\033[1;34m");
}

void yellow()
{
    printf("\033[0;33m");
}

void purple()
{
    printf("\033[0;35m");
}

void bold()
{
    printf("\033[1m");
}

void boldRed()
{
    printf("\033[1;31m");
}

void cyan()
{
    printf("\033[0;36m");
}
#pragma endregion