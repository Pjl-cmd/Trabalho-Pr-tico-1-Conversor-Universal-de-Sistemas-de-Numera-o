#ifndef CONVERSOR_H
#define CONVERSOR_H

#include <iostream>
using namespace std;

// Utilitários
char   digitoParaChar(int d);
string inverter(string s);
long long stringParaLong(const string& num, int base);
double fracaoParaDouble(const string& frac, int base);
void   separarPartes(const string& numero, string& inteira, string& fracionaria);
string padEsquerda(string s, int multiplo);

// F1 — Decimal → base (divisões sucessivas)
string decimalParaBase(long long n, int base, bool passoAPasso);
string fracaoDecimalParaBase(double frac, int base, bool passoAPasso);

// F2 — Base → decimal (somatório posicional)
string baseParaDecimal(const string& numero, int base, bool passoAPasso);

// F3 — Binário ↔ Octal / Hex (agrupamento de bits)
string binarioParaOctal(const string& bin, bool passoAPasso);
string octalParaBinario(const string& oct, bool passoAPasso);
string binarioParaHex  (const string& bin, bool passoAPasso);
string hexParaBinario  (const string& hex, bool passoAPasso);

// F4 — Octal ↔ Hex via binário
string octalParaHex(const string& oct, bool passoAPasso);
string hexParaOctal(const string& hex, bool passoAPasso);

// F6 — Conversão geral (inclui fracionários)
string converterGeral(const string& numero, int baseOrigem, int baseDestino, bool passoAPasso);

// F10 — Calculadora de máximos
void calcularMaximo(int k);

#endif
