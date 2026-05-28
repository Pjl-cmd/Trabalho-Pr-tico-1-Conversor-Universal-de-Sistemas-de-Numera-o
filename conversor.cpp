#include "conversor.h"
#include "parser.h"
#include <iostream>
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// Utilitários internos
// ─────────────────────────────────────────────────────────────────────────────

// Converte dígito numérico (0-15) para caractere
char digitoParaChar(int d) {
    if (d < 10) return '0' + d;
    return 'A' + (d - 10);
}

// Inverte uma string in-place
string inverter(string s) {
    int ini = 0, fim = (int)s.size() - 1;
    while (ini < fim) {
        char tmp = s[ini]; s[ini] = s[fim]; s[fim] = tmp;
        ini++; fim--;
    }
    return s;
}

// Converte string numérica de qualquer base para long long (parte inteira)
// Suporta sinal negativo opcional no primeiro caractere
long long stringParaLong(const string& num, int base) {
    long long resultado = 0;
    int inicio = 0;
    bool negativo = false;
    if (!num.empty() && num[0] == '-') {
        negativo = true;
        inicio = 1;
    }
    for (int i = inicio; i < (int)num.size(); i++) {
        resultado = resultado * base + charParaDigito(num[i]);
    }
    return negativo ? -resultado : resultado;
}

// Converte string fracionária (após o ponto) de qualquer base para double
double fracaoParaDouble(const string& frac, int base) {
    double resultado = 0.0;
    double peso = 1.0 / base;
    for (int i = 0; i < (int)frac.size(); i++) {
        resultado += charParaDigito(frac[i]) * peso;
        peso /= base;
    }
    return resultado;
}

// Separa parte inteira e fracionária de uma string numérica
void separarPartes(const string& numero, string& inteira, string& fracionaria) {
    inteira = "";
    fracionaria = "";
    bool achouPonto = false;
    for (int i = 0; i < (int)numero.size(); i++) {
        if (numero[i] == '.' || numero[i] == ',') {
            achouPonto = true;
            continue;
        }
        if (!achouPonto) inteira += numero[i];
        else             fracionaria += numero[i];
    }
    if (inteira.empty()) inteira = "0";
}

// ─────────────────────────────────────────────────────────────────────────────
// F1 — Decimal (inteiro) → outra base (divisões sucessivas)
// ─────────────────────────────────────────────────────────────────────────────
string decimalParaBase(long long n, int base, bool passoAPasso) {
    if (n == 0) return "0";

    bool negativo = (n < 0);
    long long tmp = negativo ? -n : n;
    string resultado = "";

    if (passoAPasso) {
        cout << "\n  === PASSO A PASSO (divisoes sucessivas) ===\n";
        if (negativo) cout << "  [Numero negativo — operando com valor absoluto]\n";
        cout << "  Dividendo   | Quociente | Resto\n";
        cout << "  ------------+-----------+------\n";
    }

    while (tmp != 0) {
        int   resto    = (int)(tmp % base);
        long long quoc = tmp / base;
        if (passoAPasso) {
            cout << "  " << tmp << "          | " << quoc
                 << "         | " << digitoParaChar(resto) << "\n";
        }
        resultado += digitoParaChar(resto);
        tmp = quoc;
    }

    if (passoAPasso) {
        cout << "  Lendo os restos de baixo para cima: ";
        string r = inverter(resultado);
        for (int i = 0; i < (int)r.size(); i++) cout << r[i] << " ";
        cout << "\n";
    }

    string res = inverter(resultado);
    return negativo ? "-" + res : res;
}

// Parte fracionária: decimal → outra base (multiplicações sucessivas)
string fracaoDecimalParaBase(double frac, int base, bool passoAPasso) {
    string resultado = "";
    int casas = 0;
    bool truncado = false;

    if (passoAPasso) {
        cout << "\n  === PARTE FRACIONARIA (multiplicacoes sucessivas) ===\n";
        cout << "  Valor      | Parte inteira\n";
        cout << "  -----------+--------------\n";
    }

    // Tolerância para evitar erro de ponto flutuante (ex: 0.625 em bin)
    while (frac > 1e-10 && casas < 16) {
        frac *= base;
        int parte = (int)(frac + 1e-12);
        frac -= parte;
        resultado += digitoParaChar(parte);
        if (passoAPasso) {
            cout << "  " << frac + parte << "   | " << digitoParaChar(parte) << "\n";
        }
        casas++;
    }
    if (frac > 1e-10) truncado = true;

    if (truncado) {
        cout << "  [AVISO] Resultado fracionario truncado em " << casas << " casas.\n";
    }
    return resultado;
}

// ─────────────────────────────────────────────────────────────────────────────
// F2 — Outra base → decimal (somatório posicional)
// ─────────────────────────────────────────────────────────────────────────────
string baseParaDecimal(const string& numero, int base, bool passoAPasso) {
    string intStr, fracStr;
    separarPartes(numero, intStr, fracStr);

    if (passoAPasso) {
        cout << "\n  === PASSO A PASSO (somatorio posicional) ===\n";
        cout << "  Digito | Posicao | Valor\n";
        cout << "  -------+---------+------\n";
    }

    long long parteInt = 0;
    for (int i = 0; i < (int)intStr.size(); i++) {
        int d = charParaDigito(intStr[i]);
        int pos = (int)intStr.size() - 1 - i;
        // calcula base^pos manualmente
        long long pot = 1;
        for (int p = 0; p < pos; p++) pot *= base;
        long long contrib = (long long)d * pot;
        if (passoAPasso) {
            cout << "  " << intStr[i] << "      | " << pos
                 << "       | " << d << " x " << base << "^" << pos
                 << " = " << contrib << "\n";
        }
        parteInt += contrib;
    }

    double parteFrac = 0.0;
    if (!fracStr.empty()) {
        if (passoAPasso) cout << "  -- parte fracionaria --\n";
        double peso = 1.0 / base;
        for (int i = 0; i < (int)fracStr.size(); i++) {
            int d = charParaDigito(fracStr[i]);
            double contrib = d * peso;
            if (passoAPasso) {
                cout << "  " << fracStr[i] << "      | -" << (i+1)
                     << "      | " << d << " x " << base << "^-" << (i+1)
                     << " = " << contrib << "\n";
            }
            parteFrac += contrib;
            peso /= base;
        }
    }

    if (passoAPasso) {
        cout << "  => Soma parte inteira: " << parteInt << "\n";
        if (!fracStr.empty()) cout << "  => Soma parte fracionaria: " << parteFrac << "\n";
    }

    // Monta string de resultado
    // Parte inteira como string
    string resInt = decimalParaBase(parteInt, 10, false);
    if (fracStr.empty()) return resInt;

    // Parte fracionária como string
    string resFrac = "";
    double tmpFrac = parteFrac;
    int casas = 0;
    bool truncado = false;
    while (tmpFrac > 1e-10 && casas < 16) {
        tmpFrac *= 10;
        int d = (int)(tmpFrac + 1e-12);
        tmpFrac -= d;
        resFrac += digitoParaChar(d);
        casas++;
    }
    if (tmpFrac > 1e-10) truncado = true;
    if (truncado) cout << "  [AVISO] Resultado fracionario truncado em " << casas << " casas.\n";

    return resInt + "." + resFrac;
}

// ─────────────────────────────────────────────────────────────────────────────
// F3 — Binário ↔ Octal (agrupamento de 3 bits) e Binário ↔ Hex (4 bits)
// ─────────────────────────────────────────────────────────────────────────────

// Garante que a string binária tenha comprimento múltiplo de n, preenchendo à esquerda
string padEsquerda(string s, int multiplo) {
    while ((int)s.size() % multiplo != 0) s = "0" + s;
    return s;
}

// Binário → Octal por agrupamento de 3 bits
string binarioParaOctal(const string& bin, bool passoAPasso) {
    string intStr, fracStr;
    separarPartes(bin, intStr, fracStr);

    string intPad = padEsquerda(intStr, 3);
    string resultado = "";

    if (passoAPasso) {
        cout << "\n  === AGRUPAMENTO DE 3 BITS (bin -> oct) ===\n";
        cout << "  Grupo | Octal\n";
        cout << "  ------+------\n";
    }

    for (int i = 0; i < (int)intPad.size(); i += 3) {
        int val = (charParaDigito(intPad[i])   * 4) +
                  (charParaDigito(intPad[i+1]) * 2) +
                   charParaDigito(intPad[i+2]);
        if (passoAPasso)
            cout << "  " << intPad[i] << intPad[i+1] << intPad[i+2]
                 << "   | " << digitoParaChar(val) << "\n";
        resultado += digitoParaChar(val);
    }

    if (fracStr.empty()) return resultado;

    // Parte fracionária: pad à direita
    string fracPad = fracStr;
    while ((int)fracPad.size() % 3 != 0) fracPad += "0";
    string resFrac = "";
    for (int i = 0; i < (int)fracPad.size(); i += 3) {
        int val = (charParaDigito(fracPad[i])   * 4) +
                  (charParaDigito(fracPad[i+1]) * 2) +
                   charParaDigito(fracPad[i+2]);
        resFrac += digitoParaChar(val);
    }
    // Remove zeros à direita da parte fracionária
    while (!resFrac.empty() && resFrac.back() == '0') resFrac.pop_back();
    if (resFrac.empty()) return resultado;
    return resultado + "." + resFrac;
}

// Octal → Binário: cada dígito vira 3 bits
string octalParaBinario(const string& oct, bool passoAPasso) {
    string intStr, fracStr;
    separarPartes(oct, intStr, fracStr);

    if (passoAPasso) {
        cout << "\n  === EXPANSAO DE 3 BITS POR DIGITO (oct -> bin) ===\n";
        cout << "  Octal | Binario\n";
        cout << "  ------+-------\n";
    }

    string resultado = "";
    for (int i = 0; i < (int)intStr.size(); i++) {
        int d = charParaDigito(intStr[i]);
        string bits = "";
        bits += digitoParaChar((d >> 2) & 1);
        bits += digitoParaChar((d >> 1) & 1);
        bits += digitoParaChar( d       & 1);
        if (passoAPasso)
            cout << "  " << intStr[i] << "     | " << bits << "\n";
        resultado += bits;
    }

    // Remove zeros à esquerda (mantém ao menos 1 dígito)
    int inicio = 0;
    while (inicio < (int)resultado.size() - 1 && resultado[inicio] == '0') inicio++;
    resultado = resultado.substr(inicio);

    if (fracStr.empty()) return resultado;

    string resFrac = "";
    for (int i = 0; i < (int)fracStr.size(); i++) {
        int d = charParaDigito(fracStr[i]);
        resFrac += digitoParaChar((d >> 2) & 1);
        resFrac += digitoParaChar((d >> 1) & 1);
        resFrac += digitoParaChar( d       & 1);
    }
    // Remove zeros à direita da parte fracionária
    while (!resFrac.empty() && resFrac.back() == '0') resFrac.pop_back();
    if (resFrac.empty()) return resultado;
    return resultado + "." + resFrac;
}

// Binário → Hexadecimal por agrupamento de 4 bits
string binarioParaHex(const string& bin, bool passoAPasso) {
    string intStr, fracStr;
    separarPartes(bin, intStr, fracStr);

    string intPad = padEsquerda(intStr, 4);
    string resultado = "";

    if (passoAPasso) {
        cout << "\n  === AGRUPAMENTO DE 4 BITS (bin -> hex) ===\n";
        cout << "  Grupo | Hex\n";
        cout << "  ------+----\n";
    }

    for (int i = 0; i < (int)intPad.size(); i += 4) {
        int val = (charParaDigito(intPad[i])   * 8) +
                  (charParaDigito(intPad[i+1]) * 4) +
                  (charParaDigito(intPad[i+2]) * 2) +
                   charParaDigito(intPad[i+3]);
        if (passoAPasso)
            cout << "  " << intPad[i] << intPad[i+1] << intPad[i+2] << intPad[i+3]
                 << " | " << digitoParaChar(val) << "\n";
        resultado += digitoParaChar(val);
    }

    if (fracStr.empty()) return resultado;

    string fracPad = fracStr;
    while ((int)fracPad.size() % 4 != 0) fracPad += "0";
    string resFrac = "";
    for (int i = 0; i < (int)fracPad.size(); i += 4) {
        int val = (charParaDigito(fracPad[i])   * 8) +
                  (charParaDigito(fracPad[i+1]) * 4) +
                  (charParaDigito(fracPad[i+2]) * 2) +
                   charParaDigito(fracPad[i+3]);
        resFrac += digitoParaChar(val);
    }
    // Remove zeros à direita da parte fracionária
    while (!resFrac.empty() && resFrac.back() == '0') resFrac.pop_back();
    if (resFrac.empty()) return resultado;
    return resultado + "." + resFrac;
}

// Hexadecimal → Binário: cada dígito vira 4 bits
string hexParaBinario(const string& hex, bool passoAPasso) {
    string intStr, fracStr;
    separarPartes(hex, intStr, fracStr);

    if (passoAPasso) {
        cout << "\n  === EXPANSAO DE 4 BITS POR DIGITO (hex -> bin) ===\n";
        cout << "  Hex | Binario\n";
        cout << "  ----+--------\n";
    }

    string resultado = "";
    for (int i = 0; i < (int)intStr.size(); i++) {
        int d = charParaDigito(intStr[i]);
        string bits = "";
        bits += digitoParaChar((d >> 3) & 1);
        bits += digitoParaChar((d >> 2) & 1);
        bits += digitoParaChar((d >> 1) & 1);
        bits += digitoParaChar( d       & 1);
        if (passoAPasso)
            cout << "  " << intStr[i] << "   | " << bits << "\n";
        resultado += bits;
    }

    // Remove zeros à esquerda
    int inicio = 0;
    while (inicio < (int)resultado.size() - 1 && resultado[inicio] == '0') inicio++;
    resultado = resultado.substr(inicio);

    if (fracStr.empty()) return resultado;

    string resFrac = "";
    for (int i = 0; i < (int)fracStr.size(); i++) {
        int d = charParaDigito(fracStr[i]);
        resFrac += digitoParaChar((d >> 3) & 1);
        resFrac += digitoParaChar((d >> 2) & 1);
        resFrac += digitoParaChar((d >> 1) & 1);
        resFrac += digitoParaChar( d       & 1);
    }
    // Remove zeros à direita da parte fracionária
    while (!resFrac.empty() && resFrac.back() == '0') resFrac.pop_back();
    if (resFrac.empty()) return resultado;
    return resultado + "." + resFrac;
}

// ─────────────────────────────────────────────────────────────────────────────
// F4 — Octal ↔ Hexadecimal via binário como intermediário
// ─────────────────────────────────────────────────────────────────────────────
string octalParaHex(const string& oct, bool passoAPasso) {
    if (passoAPasso) cout << "\n  [Passo 1] Octal -> Binario:\n";
    string bin = octalParaBinario(oct, passoAPasso);
    if (passoAPasso) cout << "  Binario intermediario: " << bin << "\n";
    if (passoAPasso) cout << "\n  [Passo 2] Binario -> Hexadecimal:\n";
    return binarioParaHex(bin, passoAPasso);
}

string hexParaOctal(const string& hex, bool passoAPasso) {
    if (passoAPasso) cout << "\n  [Passo 1] Hexadecimal -> Binario:\n";
    string bin = hexParaBinario(hex, passoAPasso);
    if (passoAPasso) cout << "  Binario intermediario: " << bin << "\n";
    if (passoAPasso) cout << "\n  [Passo 2] Binario -> Octal:\n";
    return binarioParaOctal(bin, passoAPasso);
}

// ─────────────────────────────────────────────────────────────────────────────
// F6 — Conversão geral entre quaisquer bases suportadas (com fracionários)
// ─────────────────────────────────────────────────────────────────────────────
string converterGeral(const string& numero, int baseOrigem, int baseDestino, bool passoAPasso) {
    // Caso especial: mesma base
    if (baseOrigem == baseDestino) return numero;

    // Caso especial: bin <-> oct e bin <-> hex direto (F3)
    if (baseOrigem == 2  && baseDestino == 8)  return binarioParaOctal(numero, passoAPasso);
    if (baseOrigem == 8  && baseDestino == 2)  return octalParaBinario(numero, passoAPasso);
    if (baseOrigem == 2  && baseDestino == 16) return binarioParaHex(numero, passoAPasso);
    if (baseOrigem == 16 && baseDestino == 2)  return hexParaBinario(numero, passoAPasso);

    // Caso especial: oct <-> hex via binário (F4)
    if (baseOrigem == 8  && baseDestino == 16) return octalParaHex(numero, passoAPasso);
    if (baseOrigem == 16 && baseDestino == 8)  return hexParaOctal(numero, passoAPasso);

    // Caso geral: passa pelo decimal
    string intStr, fracStr;
    separarPartes(numero, intStr, fracStr);

    // Origem → decimal
    long long parteInt;
    double    parteFrac = 0.0;

    if (baseOrigem == 10) {
        parteInt = stringParaLong(intStr, 10);
        if (!fracStr.empty()) parteFrac = fracaoParaDouble(fracStr, 10);
    } else {
        // baseOrigem != 10: converte para decimal primeiro
        string decStr = baseParaDecimal(numero, baseOrigem, passoAPasso);
        string dInt, dFrac;
        separarPartes(decStr, dInt, dFrac);
        parteInt = stringParaLong(dInt, 10);
        if (!dFrac.empty()) parteFrac = fracaoParaDouble(dFrac, 10);
    }

    // Decimal → baseDestino
    string resInt = decimalParaBase(parteInt, baseDestino, passoAPasso);
    // Não há parte fracionária: retorna apenas a parte inteira
    if (fracStr.empty()) return resInt;
    if (parteFrac < 1e-10) return resInt;

    string resFrac = fracaoDecimalParaBase(parteFrac, baseDestino, passoAPasso);
    return resInt + "." + resFrac;
}

// ─────────────────────────────────────────────────────────────────────────────
// F10 — Calculadora de máximos: dado k bits, mostra 2^k - 1 em todas as bases
// ─────────────────────────────────────────────────────────────────────────────
void calcularMaximo(int k) {
    // Calcula 2^k - 1 manualmente
    long long maximo = 1;
    for (int i = 0; i < k; i++) maximo *= 2;
    maximo -= 1;

    string binStr  = decimalParaBase(maximo, 2,  false);
    string octStr  = decimalParaBase(maximo, 8,  false);
    string hexStr  = decimalParaBase(maximo, 16, false);

    cout << "\n  Para " << k << " bits, o maior valor representavel (2^" << k << " - 1 = " << maximo << ") eh:\n";
    cout << "  Decimal:     " << maximo  << "\n";
    cout << "  Binario:     " << binStr  << "\n";
    cout << "  Octal:       " << octStr  << "\n";
    cout << "  Hexadecimal: " << hexStr  << "\n";
}
