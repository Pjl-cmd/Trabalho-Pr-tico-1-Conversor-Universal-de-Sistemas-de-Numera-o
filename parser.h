#ifndef PARSER_H
#define PARSER_H

#include <iostream>
using namespace std;

bool charValido(char c, int base);
int  charParaDigito(char c);
bool validarEntrada(const string& numero, int base);
string lerNumeroValido(int base);
int    lerBaseValida(const string& mensagem);

#endif
