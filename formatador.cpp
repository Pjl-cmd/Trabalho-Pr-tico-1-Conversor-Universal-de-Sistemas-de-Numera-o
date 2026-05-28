#include "formatador.h"
#include <iostream>
using namespace std;

void imprimirMenu() {
    cout << "\n";
    cout << "  ================================================\n";
    cout << "       CONVERSOR UNIVERSAL DE BASES NUMERICAS     \n";
    cout << "  ================================================\n";
    cout << "   GCC241 - Introducao a Computacao | UFLA 2026/1\n";
    cout << "  ================================================\n\n";
    cout << "  Opcoes:\n\n";
    cout << "   1. Decimal -> Binario / Octal / Hexadecimal\n";
    cout << "   2. Binario / Octal / Hex -> Decimal\n";
    cout << "   3. Binario <-> Octal  e  Binario <-> Hex\n";
    cout << "      (agrupamento direto de bits)\n";
    cout << "   4. Octal <-> Hexadecimal (via binario)\n";
    cout << "   5. Modo Passo a Passo (qualquer conversao)\n";
    cout << "   6. Modo Batch (ler entrada.csv, gerar saida.csv)\n";
    cout << "   7. Modo Quiz\n";
    cout << "   8. Calculadora de Maximos (dado k bits)\n";
    cout << "   9. Sair\n\n";
    cout << "  Escolha: ";
}

void imprimirNomeBases(int base) {
    if (base == 2)  cout << "Binario";
    else if (base == 8)  cout << "Octal";
    else if (base == 10) cout << "Decimal";
    else if (base == 16) cout << "Hexadecimal";
    else cout << "Base " << base;
}

void imprimirResultado(const string& entrada, int baseOrigem,
                       const string& resultado, int baseDestino) {
    cout << "\n  ------------------------------------------\n";
    cout << "  Entrada  : " << entrada << " (";
    imprimirNomeBases(baseOrigem);
    cout << ")\n";
    cout << "  Resultado: " << resultado << " (";
    imprimirNomeBases(baseDestino);
    cout << ")\n";
    cout << "  ------------------------------------------\n";
}

void imprimirSeparador() {
    cout << "\n  ==========================================\n\n";
}
