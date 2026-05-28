#include "parser.h"
#include <iostream>
using namespace std;

// Verifica se um caractere é válido para a base informada
bool charValido(char c, int base) {
    if (base == 2)  return (c == '0' || c == '1');
    if (base == 8)  return (c >= '0' && c <= '7');
    if (base == 10) return (c >= '0' && c <= '9');
    if (base == 16) {
        return (c >= '0' && c <= '9') ||
               (c >= 'A' && c <= 'F') ||
               (c >= 'a' && c <= 'f');
    }
    return false;
}

// Converte char para dígito numérico 
int charParaDigito(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

// Valida uma string numérica para a base informada
// Retorna true se válida, false caso contrário
bool validarEntrada(const string& numero, int base) {
    if (numero.empty()) return false;

    int inicio = 0;
    // Permite sinal negativo
    if (numero[0] == '-') {
        if (numero.size() == 1) return false;
        inicio = 1;
    }

    bool temPonto  = false;
    bool temDigito = false;   // deve haver ao menos 1 dígito válido
    for (int i = inicio; i < (int)numero.size(); i++) {
        if (numero[i] == '.' || numero[i] == ',') {
            if (temPonto) return false; // dois separadores = inválido
            temPonto = true;
            continue;
        }
        if (!charValido(numero[i], base)) return false;
        temDigito = true;
    }
    return temDigito;   // rejeita ".", ",", "-.", "-," etc.
}

// Lê uma string do cin e valida para a base
string lerNumeroValido(int base) {
    string entrada;
    cin >> entrada;
    while (!validarEntrada(entrada, base)) {
        cout << "  [ERRO] Entrada invalida para base " << base
             << ". Tente novamente: ";
        cin >> entrada;
    }
    return entrada;
}

// Lê uma base válida (2, 8, 10 ou 16)
int lerBaseValida(const string& mensagem) {
    int base;
    cout << mensagem;
    cin >> base;
    while (base != 2 && base != 8 && base != 10 && base != 16) {
        cout << "  [ERRO] Base invalida. Use 2, 8, 10 ou 16. Digite novamente: ";
        cin >> base;
    }
    return base;
}
