#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdbool.h>

#define SC 25
extern int NC;
extern int NA;
extern int NE;

#pragma region Structs
typedef struct
{
    int codCliente;
    char nome[SC];
    int nif;
    char pais[SC];
    bool estado;
} Clientes;

typedef struct {
    int codArtigo;
    char artigo[SC];
    double maoObra;
    double custosFixos;
    double margemLucro;
    int tamanhos[2];
    double defaultPreco;
    double incrementoPreco;
    bool estado;
    double tamanhoPreco[15][2];
} Artigos;

typedef struct {
    int codEncomenda;
    int codCliente;
    int codArtigo;
    int tamanho;
    int horaminuto;
    int dia;
    int mes;
    int ano;
    int quantidade;
    double total;
    bool estado;
} Encomendas;

typedef struct {
    int currentClient;
    char stringDesconto[7];
    double percentageDesconto;
} Runtime;

extern Clientes *cliente;
extern Artigos *artigo;
extern Encomendas *encomenda;
extern Runtime *runtime;

#pragma endregion

#pragma region Functions
/// Ficheiro main.c
int main();


/// Ficheiro util.c
int numberOfDigits(int num);

void clearScreen();
void pressEnter(bool alsoClearScreen);

int getDateInfo(int info);
bool checkString(const char* string);
bool checkNumber(double number);
bool checkCodPromo(char* codPromo);
bool availableForOperation(int op, int codArtigo, int parameter);
int getArtigoIndex(int codArtigo);

bool loggedClient();

void reallocStruct(int st);
void freeMemory(bool alsoStopTheProgram);



// Ficheiro queries.c
void listEncomendas(int queryType);
void statisticsEncomendas();
double getTotalEncomenda();

void listClientes(int queryType);
void listArtigos(int queryType);

char* getArtigoName(int codArtigo);
void printDataEncomenda(int enc);
void printDataArtigo(int art, bool query);
void printDataCliente(int cli);



// Ficheiro clientes.c
void addCliente();
bool getEstadoCliente(int codCliente);
char* getClientName(int codCliente);
void updateClient(int clienteToChange, int via);



// Ficheiro administrador.c
void updateArtigo();
void addArtigo();
void updateDesconto();



// Ficheiro data.c
void importData();
void exportData();
void importDefaultData();
void exportDefaultData();
void deleteData();
void exportEncomendas();


// Ficheiro shopping.c
void loginClient(), logoutClient();
void doShopping();
char *getEstadoEncomenda(int estado);
void cancelEncomenda();


// Ficheiro colors.c
void white(), red(), green(), blue(), yellow(), purple(), cyan(), bold(), boldRed();
#pragma endregion

#endif