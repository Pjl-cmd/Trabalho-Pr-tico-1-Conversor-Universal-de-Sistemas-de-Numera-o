#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

#include "parser.h"
#include "conversor.h"
#include "formatador.h"

// =============================================================================
// F8 — Modo Batch
// =============================================================================
void modoBatch() {
    cout << "\n  Informe o nome do arquivo de entrada (ex: entrada.csv): ";
    string nomeEntrada;
    cin >> nomeEntrada;

    ifstream arqEntrada(nomeEntrada.c_str());
    if (!arqEntrada.is_open()) {
        cout << "  [ERRO] Nao foi possivel abrir: " << nomeEntrada << "\n";
        return;
    }

    ofstream arqSaida("saida.csv");
    arqSaida << "valor;base_origem;resultado;base_destino\n";

    string linha;
    int processados = 0;
    getline(arqEntrada, linha);
    while (getline(arqEntrada, linha)) {
        if (linha.empty()) continue;
        // Remove \r caso o arquivo venha do Windows (CRLF)
        if (!linha.empty() && linha.back() == '\r') linha.pop_back();
        if (linha.empty()) continue;
        string valor = "", baseOrigStr = "", baseDestStr = "";
        int campo = 0;
        for (int i = 0; i < (int)linha.size(); i++) {
            if (linha[i] == ';') { campo++; continue; }
            if (campo == 0) valor       += linha[i];
            if (campo == 1) baseOrigStr += linha[i];
            if (campo == 2) baseDestStr += linha[i];
        }
        if (valor.empty() || baseOrigStr.empty() || baseDestStr.empty()) continue;
        int bo = 0, bd = 0;
        for (int i = 0; i < (int)baseOrigStr.size(); i++) bo = bo*10+(baseOrigStr[i]-'0');
        for (int i = 0; i < (int)baseDestStr.size(); i++) bd = bd*10+(baseDestStr[i]-'0');
        if ((bo!=2&&bo!=8&&bo!=10&&bo!=16)||(bd!=2&&bd!=8&&bd!=10&&bd!=16)) {
            arqSaida << valor << ";" << bo << ";ERRO_BASE_INVALIDA;" << bd << "\n"; continue;
        }
        if (!validarEntrada(valor, bo)) {
            arqSaida << valor << ";" << bo << ";ERRO_VALOR_INVALIDO;" << bd << "\n"; continue;
        }
        string res = converterGeral(valor, bo, bd, false);
        arqSaida << valor << ";" << bo << ";" << res << ";" << bd << "\n";
        processados++;
    }
    arqEntrada.close();
    arqSaida.close();
    cout << "  Processados " << processados << " registros. Resultado em 'saida.csv'.\n";
}

// =============================================================================
// F9 — Quiz: geração aleatória, alternativas a/b/c/d/e, 5 níveis, 100 questões
// =============================================================================

// Retorna um número aleatório dentro da faixa do nível
//   Nivel 1: 1 – 15        (4 bits)
//   Nivel 2: 16 – 255      (5-8 bits)
//   Nivel 3: 256 – 4095    (9-12 bits)
//   Nivel 4: 4096 – 65535  (13-16 bits)
//   Nivel 5: 65536 – 1048575 (17-20 bits)
long long sortearNumero(int nivel) {
    long long minVal[] = {1,     16,   256,   4096,  65536};
    long long maxVal[] = {15,   255,  4095,  65535, 1048575};
    long long faixa = maxVal[nivel-1] - minVal[nivel-1] + 1;
    return minVal[nivel-1] + (long long)rand() % faixa;
}

// Gera uma alternativa errada diferente da correta e das já geradas
// Altera levemente o número decimal e reconverte
string gerarErrada(long long numDec, int baseDest, string* jaUsadas, int qtd) {
    int tentativas = 0;
    while (tentativas < 200) {
        tentativas++;
        // Delta proporcional ao número para evitar travamento em valores pequenos
        long long faixa = (numDec > 16) ? 8 : (numDec > 1 ? (long long)numDec - 1 : 1);
        long long delta = (long long)(rand() % faixa) + 1;
        if (rand() % 2 == 0 && numDec - delta > 0) delta = -delta;
        long long candidato = numDec + delta;
        if (candidato <= 0) continue;

        string opt = decimalParaBase(candidato, baseDest, false);

        // Verifica se já está na lista
        bool repetido = false;
        for (int i = 0; i < qtd; i++) {
            if (jaUsadas[i] == opt) { repetido = true; break; }
        }
        if (!repetido) return opt;
    }
    // Fallback: soma crescente
    long long fb = numDec + (long long)(qtd + 1) * 3;
    return decimalParaBase(fb, baseDest, false);
}

// Embaralha um vetor de strings (Fisher-Yates)
void embaralharStrings(string* v, int n) {
    for (int i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        string tmp = v[i]; v[i] = v[j]; v[j] = tmp;
    }
}

void modoQuiz() {
    // ── Escolha do nível ──────────────────────────────────────────────────────
    cout << "\n  ============================================\n";
    cout << "   QUIZ DE CONVERSAO DE BASES — 100 QUESTOES \n";
    cout << "  ============================================\n";
    cout << "  Niveis de dificuldade:\n";
    cout << "    1 — Iniciante    (valores de  1 a 15)\n";
    cout << "    2 — Basico       (valores de 16 a 255)\n";
    cout << "    3 — Intermediario(valores de 256 a 4095)\n";
    cout << "    4 — Avancado     (valores de 4096 a 65535)\n";
    cout << "    5 — Expert       (valores de 65536 a 1048575)\n";
    cout << "  --------------------------------------------\n";
    cout << "  Escolha o nivel (1-5): ";

    int nivel;
    cin >> nivel;
    if (nivel < 1 || nivel > 5) nivel = 1;

    cout << "  --------------------------------------------\n";
    cout << "  Digite a letra da alternativa (a-e)\n";
    cout << "  ou 0 para encerrar o quiz.\n";
    cout << "  ============================================\n\n";

    // ── Tipos de conversão disponíveis ────────────────────────────────────────
    // Cada par: {baseOrigem, baseDestino}
    int pares[10][2] = {
        {10, 2},  {10, 8},  {10,16},
        { 2,10},  { 8,10},  {16,10},
        { 2, 8},  { 8, 2},
        { 2,16},  {16, 2}
    };
    string nomeBase[17];
    nomeBase[2]  = "Binario";
    nomeBase[8]  = "Octal";
    nomeBase[10] = "Decimal";
    nomeBase[16] = "Hexadecimal";

    char letras[] = {'a','b','c','d','e'};
    int acertos = 0;
    const int TOTAL = 100;

    // ── Loop das 100 questões ─────────────────────────────────────────────────
    for (int q = 1; q <= TOTAL; q++) {

        // Sorteia tipo de conversão
        int tipo = rand() % 10;
        int baseOrig = pares[tipo][0];
        int baseDest = pares[tipo][1];

        // Sorteia número no nível escolhido (em decimal)
        long long numDec = sortearNumero(nivel);

        // Converte para a base de origem (enunciado) e base destino (gabarito)
        string numOrig   = decimalParaBase(numDec, baseOrig, false);
        string gabCorreto = decimalParaBase(numDec, baseDest, false);

        // Gera 4 alternativas erradas únicas
        string opts[5];
        opts[0] = gabCorreto;
        for (int i = 1; i < 5; i++)
            opts[i] = gerarErrada(numDec, baseDest, opts, i);

        // Guarda índice do gabarito antes de embaralhar
        // (opts[0] é a correta antes do embaralhamento)
        embaralharStrings(opts, 5);

        // Descobre onde a correta foi parar
        int idxCorreto = 0;
        for (int i = 0; i < 5; i++) {
            if (opts[i] == gabCorreto) { idxCorreto = i; break; }
        }

        // ── Exibe a questão ──────────────────────────────────────────────────
        cout << "  [" << q << "/100]  "
             << nomeBase[baseOrig] << " -> " << nomeBase[baseDest] << "\n";
        cout << "  Converta " << numOrig
             << " (" << nomeBase[baseOrig] << ") para "
             << nomeBase[baseDest] << "\n\n";

        for (int i = 0; i < 5; i++)
            cout << "    " << letras[i] << ") " << opts[i] << "\n";

        cout << "\n  Resposta: ";
        char resp;
        cin >> resp;

        // ── Encerramento antecipado ──────────────────────────────────────────
        if (resp == '0') {
            int respondidas = q - 1;
            cout << "\n  Quiz encerrado na questao " << q << ".\n";
            if (respondidas > 0) {
                cout << "  Acertos: " << acertos << " de " << respondidas
                     << " (" << (acertos * 100 / respondidas) << "%)\n";
            }
            return;
        }

        int idx = (int)(resp - 'a');
        if (idx < 0 || idx > 4) {
            cout << "  [AVISO] Letra invalida, contando como errada.\n";
            idx = -1;
        }

        // ── Feedback ─────────────────────────────────────────────────────────
        if (idx == idxCorreto) {
            acertos++;
            cout << "  CORRETO!";
        } else {
            cout << "  ERRADO. ";
        }
        cout << "  |  " << acertos << "/" << q
             << "  (" << (acertos * 100 / q) << "%)\n\n";
    }

    // ── Resultado final ───────────────────────────────────────────────────────
    cout << "  ============================================\n";
    cout << "  RESULTADO FINAL  —  Nivel " << nivel << "\n";
    cout << "  Acertos: " << acertos << " de 100 (" << acertos << "%)\n";
    if      (acertos >= 90) cout << "  Conceito: EXCELENTE\n";
    else if (acertos >= 70) cout << "  Conceito: BOM\n";
    else if (acertos >= 50) cout << "  Conceito: REGULAR\n";
    else                    cout << "  Conceito: INSUFICIENTE — revise a Aula 02!\n";
    cout << "  ============================================\n";
}

// =============================================================================
// Main
// =============================================================================
int main() {
    srand((unsigned int)time(0));
    int opcao = 0;

    while (opcao != 9) {
        imprimirMenu();
        cin >> opcao;

        if (opcao == 1) {
            cout << "\n  Digite o numero decimal: ";
            string num = lerNumeroValido(10);
            cout << "  Converter para qual base? (2, 8 ou 16): ";
            int baseDest; cin >> baseDest;
            while (baseDest != 2 && baseDest != 8 && baseDest != 16) {
                cout << "  [ERRO] Escolha 2, 8 ou 16: ";
                cin >> baseDest;
            }
            string resultado = converterGeral(num, 10, baseDest, false);
            imprimirResultado(num, 10, resultado, baseDest);

        } else if (opcao == 2) {
            int baseOrig = lerBaseValida("\n  Base de origem (2, 8 ou 16): ");
            while (baseOrig == 10) {
                cout << "  [AVISO] Ja esta em decimal! Use outra base: ";
                cin >> baseOrig;
            }
            cout << "  Numero em base " << baseOrig << ": ";
            string num = lerNumeroValido(baseOrig);
            imprimirResultado(num, baseOrig, baseParaDecimal(num, baseOrig, false), 10);

        } else if (opcao == 3) {
            cout << "\n  1.Bin->Oct  2.Oct->Bin  3.Bin->Hex  4.Hex->Bin\n  Escolha: ";
            int sub; cin >> sub;
            string num, res;
            if      (sub==1) { cout<<"  Binario: "; num=lerNumeroValido(2);  res=binarioParaOctal(num,false); imprimirResultado(num,2,res,8); }
            else if (sub==2) { cout<<"  Octal: ";   num=lerNumeroValido(8);  res=octalParaBinario(num,false); imprimirResultado(num,8,res,2); }
            else if (sub==3) { cout<<"  Binario: "; num=lerNumeroValido(2);  res=binarioParaHex(num,false);   imprimirResultado(num,2,res,16); }
            else if (sub==4) { cout<<"  Hex: ";     num=lerNumeroValido(16); res=hexParaBinario(num,false);   imprimirResultado(num,16,res,2); }
            else cout << "  [ERRO] Sub-opcao invalida.\n";

        } else if (opcao == 4) {
            cout << "\n  1.Oct->Hex  2.Hex->Oct\n  Escolha: ";
            int sub; cin >> sub;
            string num, res;
            if      (sub==1) { cout<<"  Octal: "; num=lerNumeroValido(8);  res=octalParaHex(num,false); imprimirResultado(num,8,res,16); }
            else if (sub==2) { cout<<"  Hex: ";   num=lerNumeroValido(16); res=hexParaOctal(num,false); imprimirResultado(num,16,res,8); }
            else cout << "  [ERRO] Sub-opcao invalida.\n";

        } else if (opcao == 5) {
            int bo = lerBaseValida("\n  Base de origem (2,8,10,16): ");
            cout << "  Numero: "; string num = lerNumeroValido(bo);
            int bd = lerBaseValida("  Base de destino (2,8,10,16): ");
            imprimirResultado(num, bo, converterGeral(num, bo, bd, true), bd);

        } else if (opcao == 6) {
            modoBatch();

        } else if (opcao == 7) {
            modoQuiz();

        } else if (opcao == 8) {
            cout << "\n  Numero de bits (k): ";
            int k; cin >> k;
            if (k <= 0 || k > 62) cout << "  [ERRO] Use 1 a 62.\n";
            else calcularMaximo(k);

        } else if (opcao == 9) {
            cout << "\n  Saindo... Ate logo!\n\n";

        } else {
            cout << "\n  [ERRO] Opcao invalida.\n";
        }

        if (opcao != 9) imprimirSeparador();
    }
    return 0;
}
