// test_conversor.cpp
// Suite de testes — 50+ casos cobrindo todas as conversoes
//
// Compile e execute com um unico comando:
//   g++ -o testes test_conversor.cpp conversor.cpp parser.cpp && ./testes
//
// (todos os arquivos devem estar no mesmo diretorio)

#include <iostream>
using namespace std;

#include "conversor.h"
#include "parser.h"

// ─────────────────────────────────────────────────────────────────────────────
// Infraestrutura de testes
// ─────────────────────────────────────────────────────────────────────────────
static int passed = 0;
static int failed = 0;
static int total  = 0;

void check(const string& descricao, const string& resultado, const string& esperado) {
    total++;
    if (resultado == esperado) {
        cout << "  [OK]   " << descricao << "\n";
        passed++;
    } else {
        cout << "  [FAIL] " << descricao
             << "\n         Esperado : \"" << esperado << "\""
             << "\n         Obtido   : \"" << resultado << "\"\n";
        failed++;
    }
}

void checkBool(const string& descricao, bool resultado, bool esperado) {
    total++;
    if (resultado == esperado) {
        cout << "  [OK]   " << descricao << "\n";
        passed++;
    } else {
        cout << "  [FAIL] " << descricao
             << "\n         Esperado : " << (esperado  ? "true" : "false")
             << "\n         Obtido   : " << (resultado ? "true" : "false") << "\n";
        failed++;
    }
}

void secao(const string& titulo) {
    cout << "\n  ── " << titulo << " ──\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// main
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    cout << "\n  ============================================\n";
    cout << "   SUITE DE TESTES — GCC241 / UFLA 2026/1   \n";
    cout << "  ============================================\n";

    // =========================================================================
    // F1 — Decimal → outras bases
    // =========================================================================
    secao("F1: Decimal -> Binario");
    check("0  -> bin",          decimalParaBase(0,   2, false), "0");
    check("1  -> bin",          decimalParaBase(1,   2, false), "1");
    check("2  -> bin",          decimalParaBase(2,   2, false), "10");
    check("10 -> bin",          decimalParaBase(10,  2, false), "1010");
    check("255 -> bin",         decimalParaBase(255, 2, false), "11111111");
    check("256 -> bin",         decimalParaBase(256, 2, false), "100000000");
    check("1023 -> bin",        decimalParaBase(1023,2, false), "1111111111");

    secao("F1: Decimal -> Octal");
    check("0   -> oct",         decimalParaBase(0,   8, false), "0");
    check("8   -> oct",         decimalParaBase(8,   8, false), "10");
    check("64  -> oct",         decimalParaBase(64,  8, false), "100");
    check("255 -> oct",         decimalParaBase(255, 8, false), "377");
    check("512 -> oct",         decimalParaBase(512, 8, false), "1000");

    secao("F1: Decimal -> Hexadecimal");
    check("0   -> hex",         decimalParaBase(0,   16, false), "0");
    check("10  -> hex",         decimalParaBase(10,  16, false), "A");
    check("15  -> hex",         decimalParaBase(15,  16, false), "F");
    check("16  -> hex",         decimalParaBase(16,  16, false), "10");
    check("255 -> hex",         decimalParaBase(255, 16, false), "FF");
    check("256 -> hex",         decimalParaBase(256, 16, false), "100");
    check("4095 -> hex",        decimalParaBase(4095,16, false), "FFF");

    // =========================================================================
    // F2 — Outras bases → decimal
    // =========================================================================
    secao("F2: Binario -> Decimal");
    check("0 (bin) -> dec",          baseParaDecimal("0",          2, false), "0");
    check("1 (bin) -> dec",          baseParaDecimal("1",          2, false), "1");
    check("1010 (bin) -> dec",       baseParaDecimal("1010",       2, false), "10");
    check("11111111 (bin) -> dec",   baseParaDecimal("11111111",   2, false), "255");
    check("100000000 (bin) -> dec",  baseParaDecimal("100000000",  2, false), "256");
    check("1111111111 (bin) -> dec", baseParaDecimal("1111111111", 2, false), "1023");

    secao("F2: Octal -> Decimal");
    check("10 (oct) -> dec",         baseParaDecimal("10",  8, false), "8");
    check("100 (oct) -> dec",        baseParaDecimal("100", 8, false), "64");
    check("377 (oct) -> dec",        baseParaDecimal("377", 8, false), "255");
    check("1000 (oct) -> dec",       baseParaDecimal("1000",8, false), "512");

    secao("F2: Hexadecimal -> Decimal");
    check("A (hex) -> dec",          baseParaDecimal("A",   16, false), "10");
    check("F (hex) -> dec",          baseParaDecimal("F",   16, false), "15");
    check("10 (hex) -> dec",         baseParaDecimal("10",  16, false), "16");
    check("FF (hex) -> dec",         baseParaDecimal("FF",  16, false), "255");
    check("1A (hex) -> dec",         baseParaDecimal("1A",  16, false), "26");
    check("FFF (hex) -> dec",        baseParaDecimal("FFF", 16, false), "4095");

    // =========================================================================
    // F3 — Binário ↔ Octal (agrupamento de 3 bits)
    // =========================================================================
    secao("F3: Binario -> Octal");
    check("111 (bin) -> oct",        binarioParaOctal("111",      false), "7");
    check("1010 (bin) -> oct",       binarioParaOctal("1010",     false), "12");
    check("11111111 (bin) -> oct",   binarioParaOctal("11111111", false), "377");
    check("1000000000 (bin) -> oct", binarioParaOctal("1000000000",false),"1000");

    secao("F3: Octal -> Binario");
    check("7 (oct) -> bin",          octalParaBinario("7",    false), "111");
    check("10 (oct) -> bin",         octalParaBinario("10",   false), "1000");
    check("377 (oct) -> bin",        octalParaBinario("377",  false), "11111111");
    check("1000 (oct) -> bin",       octalParaBinario("1000", false), "1000000000");

    secao("F3: Binario -> Hexadecimal");
    check("1010 (bin) -> hex",       binarioParaHex("1010",     false), "A");
    check("1111 (bin) -> hex",       binarioParaHex("1111",     false), "F");
    check("11111111 (bin) -> hex",   binarioParaHex("11111111", false), "FF");
    check("100000000 (bin) -> hex",  binarioParaHex("100000000",false), "100");

    secao("F3: Hexadecimal -> Binario");
    check("A (hex) -> bin",          hexParaBinario("A",   false), "1010");
    check("F (hex) -> bin",          hexParaBinario("F",   false), "1111");
    check("FF (hex) -> bin",         hexParaBinario("FF",  false), "11111111");
    check("1F (hex) -> bin",         hexParaBinario("1F",  false), "11111");
    check("100 (hex) -> bin",        hexParaBinario("100", false), "100000000");

    // =========================================================================
    // F4 — Octal ↔ Hexadecimal via binário
    // =========================================================================
    secao("F4: Octal -> Hexadecimal");
    check("10 (oct) -> hex",         octalParaHex("10",  false), "8");
    check("377 (oct) -> hex",        octalParaHex("377", false), "FF");
    check("1000 (oct) -> hex",       octalParaHex("1000",false), "200");

    secao("F4: Hexadecimal -> Octal");
    check("8 (hex) -> oct",          hexParaOctal("8",   false), "10");
    check("FF (hex) -> oct",         hexParaOctal("FF",  false), "377");
    check("100 (hex) -> oct",        hexParaOctal("100", false), "400");

    // =========================================================================
    // F5 — Validação de entradas (parser)
    // =========================================================================
    secao("F5: Validacao de entradas");
    checkBool("'1010' valido  base 2",   validarEntrada("1010",  2),  true);
    checkBool("'1012' invalido base 2",  validarEntrada("1012",  2),  false);
    checkBool("'0' valido base 2",       validarEntrada("0",     2),  true);
    checkBool("'377' valido  base 8",    validarEntrada("377",   8),  true);
    checkBool("'389' invalido base 8",   validarEntrada("389",   8),  false);
    checkBool("'255' valido  base 10",   validarEntrada("255",  10),  true);
    checkBool("'10.5' valido base 10",   validarEntrada("10.5", 10),  true);
    checkBool("'10.5.1' inv  base 10",   validarEntrada("10.5.1",10), false);
    checkBool("'' invalido",             validarEntrada("",     10),  false);
    checkBool("'FF' valido  base 16",    validarEntrada("FF",   16),  true);
    checkBool("'GG' invalido base 16",   validarEntrada("GG",   16),  false);
    checkBool("'1a' valido  base 16",    validarEntrada("1a",   16),  true);

    // =========================================================================
    // F6 — Fracionários (parte inteira + parte fracionária separadas)
    // =========================================================================
    secao("F6: Fracionarios (10.625 dec -> bin)");
    {
        long long pi = stringParaLong("10", 10);
        double    pf = fracaoParaDouble("625", 10);
        check("parte inteira  10 -> 1010", decimalParaBase(pi, 2, false), "1010");
        check("parte frac 0.625 -> 101",   fracaoDecimalParaBase(pf, 2, false), "101");
    }

    secao("F6: Fracionarios (0.5 dec -> bin/oct/hex)");
    {
        double meia = fracaoParaDouble("5", 10);
        check("0.5 dec -> .1   bin", fracaoDecimalParaBase(meia, 2,  false), "1");
        check("0.5 dec -> .4   oct", fracaoDecimalParaBase(meia, 8,  false), "4");
        check("0.5 dec -> .8   hex", fracaoDecimalParaBase(meia, 16, false), "8");
    }

    // =========================================================================
    // F10 — Calculadora de máximos (verifica valores gerados)
    // =========================================================================
    secao("F10: Maximos por numero de bits");
    // 4 bits -> 15
    check("15 -> bin  (4 bits)", decimalParaBase(15, 2,  false), "1111");
    check("15 -> oct  (4 bits)", decimalParaBase(15, 8,  false), "17");
    check("15 -> hex  (4 bits)", decimalParaBase(15, 16, false), "F");
    // 8 bits -> 255
    check("255 -> bin (8 bits)", decimalParaBase(255, 2,  false), "11111111");
    check("255 -> oct (8 bits)", decimalParaBase(255, 8,  false), "377");
    check("255 -> hex (8 bits)", decimalParaBase(255, 16, false), "FF");
    // 16 bits -> 65535
    check("65535 -> hex (16 bits)", decimalParaBase(65535, 16, false), "FFFF");
    check("65535 -> oct (16 bits)", decimalParaBase(65535, 8,  false), "177777");

    // =========================================================================
    // Resultado final
    // =========================================================================
    cout << "\n  ============================================\n";
    cout << "  Total  : " << total  << "\n";
    cout << "  OK     : " << passed << "\n";
    cout << "  FAIL   : " << failed << "\n";
    if (failed == 0)
        cout << "  Status : TUDO OK \n";
    else
        cout << "  Status : " << failed << " FALHA(S) — veja acima\n";
    cout << "  ============================================\n\n";

    return failed > 0 ? 1 : 0;
}
