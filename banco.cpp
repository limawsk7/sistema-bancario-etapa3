#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
using namespace std;

// dados das contas
const int MAX_CONTAS = 100; // limite maximo de contas
int numeroConta[MAX_CONTAS];
string nomeCliente[MAX_CONTAS];
string cpf[MAX_CONTAS];
int tipoConta[MAX_CONTAS];
double saldo[MAX_CONTAS];
bool contaAtiva[MAX_CONTAS];
int quantidadeContas = 0;

// historico de movimentacoes
const int MAX_MOVIMENTACOES = 500;
int contaMovimentacao[MAX_MOVIMENTACOES];
char tipoMovimentacao[MAX_MOVIMENTACOES];
double valorMovimentacao[MAX_MOVIMENTACOES];
int quantidadeMovimentacoes = 0;

// le um numero inteiro do teclado, devolve -1 se digitar algo invalido
int lerInt() {
    string texto;
    if (!getline(cin, texto)) {
        // acabou a entrada (ctrl+z / ctrl+d), sai pra nao ficar em loop
        exit(0);
    }
    try {
        size_t pos;
        int n = stoi(texto, &pos);
        if (texto.find_first_not_of(" \t", pos) != string::npos) {
            return -1;
        }
        return n;
    } catch (...) {
        return -1;
    }
}

// le um valor decimal do teclado (aceita virgula ou ponto), devolve -1 se for invalido
double lerDouble() {
    string texto;
    if (!getline(cin, texto)) {
        exit(0);
    }
    for (int i = 0; i < (int)texto.size(); i++) {
        if (texto[i] == ',') {
            texto[i] = '.';
        }
    }
    try {
        size_t pos;
        double d = stod(texto, &pos);
        if (texto.find_first_not_of(" \t", pos) != string::npos) {
            return -1;
        }
        return d;
    } catch (...) {
        return -1;
    }
}

// tira o \r do fim da linha (arquivos do windows) e o BOM do comeco
string limparLinha(string linha) {
    if (linha.size() >= 3 && (unsigned char)linha[0] == 0xEF &&
        (unsigned char)linha[1] == 0xBB && (unsigned char)linha[2] == 0xBF) {
        linha = linha.substr(3);
    }
    while (linha.size() > 0 && (linha[linha.size() - 1] == '\r' || linha[linha.size() - 1] == '\n')) {
        linha = linha.substr(0, linha.size() - 1);
    }
    return linha;
}

// le o arquivo contas.txt e joga nos arrays
void carregarContas() {
    ifstream arquivo("contas.txt");
    if (!arquivo.is_open()) {
        // primeira execucao, ainda nao tem arquivo
        return;
    }

    string linha;
    while (getline(arquivo, linha) && quantidadeContas < MAX_CONTAS) {
        linha = limparLinha(linha);
        if (linha == "") {
            continue;
        }

        // separa os campos pelo ponto e virgula
        stringstream ss(linha);
        string campos[6];
        int total = 0;
        string campo;
        while (total < 6 && getline(ss, campo, ';')) {
            campos[total] = campo;
            total++;
        }

        if (total != 6) {
            cout << "aviso: linha invalida em contas.txt ignorada: " << linha << endl;
            continue;
        }

        try {
            int numeroLido = stoi(campos[0]);
            int tipoLido = stoi(campos[3]);
            double saldoLido = stod(campos[4]);
            int ativaLida = stoi(campos[5]);

            numeroConta[quantidadeContas] = numeroLido;
            nomeCliente[quantidadeContas] = campos[1];
            cpf[quantidadeContas] = campos[2];
            tipoConta[quantidadeContas] = tipoLido;
            saldo[quantidadeContas] = saldoLido;
            contaAtiva[quantidadeContas] = (ativaLida == 1);
            quantidadeContas++;
        } catch (...) {
            cout << "aviso: linha invalida em contas.txt ignorada: " << linha << endl;
        }
    }

    arquivo.close();
}

// grava todas as contas no arquivo (sobrescreve o que tinha antes)
void salvarContas() {
    ofstream arquivo("contas.txt");
    if (!arquivo.is_open()) {
        cout << "erro ao abrir contas.txt para gravar." << endl;
        return;
    }

    arquivo << fixed << setprecision(2);
    for (int i = 0; i < quantidadeContas; i++) {
        arquivo << numeroConta[i] << ";"
                << nomeCliente[i] << ";"
                << cpf[i] << ";"
                << tipoConta[i] << ";"
                << saldo[i] << ";"
                << contaAtiva[i] << endl;
    }

    arquivo.close();
}

// le o arquivo movimentacoes.txt e joga nos arrays
void carregarMovimentacoes() {
    ifstream arquivo("movimentacoes.txt");
    if (!arquivo.is_open()) {
        return;
    }

    string linha;
    while (getline(arquivo, linha) && quantidadeMovimentacoes < MAX_MOVIMENTACOES) {
        linha = limparLinha(linha);
        if (linha == "") {
            continue;
        }

        stringstream ss(linha);
        string campos[3];
        int total = 0;
        string campo;
        while (total < 3 && getline(ss, campo, ';')) {
            campos[total] = campo;
            total++;
        }

        if (total != 3 || campos[1].size() != 1) {
            cout << "aviso: linha invalida em movimentacoes.txt ignorada: " << linha << endl;
            continue;
        }

        try {
            int contaLida = stoi(campos[0]);
            double valorLido = stod(campos[2]);

            contaMovimentacao[quantidadeMovimentacoes] = contaLida;
            tipoMovimentacao[quantidadeMovimentacoes] = campos[1][0];
            valorMovimentacao[quantidadeMovimentacoes] = valorLido;
            quantidadeMovimentacoes++;
        } catch (...) {
            cout << "aviso: linha invalida em movimentacoes.txt ignorada: " << linha << endl;
        }
    }

    arquivo.close();
}

// grava todo o historico no arquivo
void salvarMovimentacoes() {
    ofstream arquivo("movimentacoes.txt");
    if (!arquivo.is_open()) {
        cout << "erro ao abrir movimentacoes.txt para gravar." << endl;
        return;
    }

    arquivo << fixed << setprecision(2);
    for (int i = 0; i < quantidadeMovimentacoes; i++) {
        arquivo << contaMovimentacao[i] << ";"
                << tipoMovimentacao[i] << ";"
                << valorMovimentacao[i] << endl;
    }

    arquivo.close();
}

int main() {
    int opcao;
    int numero, indice, indiceOrigem, indiceDestino;
    double valor;

    // recupera o que foi salvo na ultima vez
    carregarContas();
    carregarMovimentacoes();

    do {
        cout << "\n********************************" << endl;
        cout << "**        BANCO INF101        **" << endl;
        cout << "********************************" << endl;
        cout << "1 - Cadastrar conta" << endl;
        cout << "2 - Listar contas" << endl;
        cout << "3 - Consultar conta" << endl;
        cout << "4 - Depositar" << endl;
        cout << "5 - Sacar" << endl;
        cout << "6 - Transferir" << endl;
        cout << "7 - Extrato" << endl;
        cout << "8 - Sair" << endl;
        cout << "Opcao: ";
        opcao = lerInt();

        switch (opcao) {

            case 1: // cadastrar conta
                if (quantidadeContas == MAX_CONTAS) {
                    cout << "limite de contas atingido." << endl;
                    break;
                }

                cout << "Numero da conta: ";
                numero = lerInt();
                while (numero <= 0) {
                    cout << "numero invalido, digite maior que zero: ";
                    numero = lerInt();
                }

                // ve se ja existe uma conta com esse numero
                indice = -1;
                for (int i = 0; i < quantidadeContas; i++) {
                    if (numeroConta[i] == numero) {
                        indice = i;
                    }
                }
                if (indice != -1) {
                    cout << "ja existe uma conta com esse numero." << endl;
                    break;
                }

                numeroConta[quantidadeContas] = numero;

                cout << "nome do titular: ";
                getline(cin, nomeCliente[quantidadeContas]);

                cout << "CPF: ";
                getline(cin, cpf[quantidadeContas]);

                // ponto e virgula e o separador do arquivo, entao tiro dos textos
                for (int i = 0; i < (int)nomeCliente[quantidadeContas].size(); i++) {
                    if (nomeCliente[quantidadeContas][i] == ';') nomeCliente[quantidadeContas][i] = ' ';
                }
                for (int i = 0; i < (int)cpf[quantidadeContas].size(); i++) {
                    if (cpf[quantidadeContas][i] == ';') cpf[quantidadeContas][i] = ' ';
                }

                cout << "tipo da conta (1 = Corrente, 2 = Poupanca): ";
                tipoConta[quantidadeContas] = lerInt();
                while (tipoConta[quantidadeContas] != 1 && tipoConta[quantidadeContas] != 2) {
                    cout << "tipo invalido, digite 1 ou 2: ";
                    tipoConta[quantidadeContas] = lerInt();
                }

                cout << "saldo inicial: ";
                saldo[quantidadeContas] = lerDouble();
                while (saldo[quantidadeContas] < 0) {
                    cout << "saldo nao pode ser negativo: ";
                    saldo[quantidadeContas] = lerDouble();
                }

                contaAtiva[quantidadeContas] = true;
                quantidadeContas++;
                salvarContas();
                cout << "conta cadastrada!" << endl;
                break;

            case 2: // listar contas
                if (quantidadeContas == 0) {
                    cout << "nenhuma conta cadastrada." << endl;
                } else {
                    for (int i = 0; i < quantidadeContas; i++) {
                        cout << "\nconta: " << numeroConta[i] << endl;
                        cout << "titular: " << nomeCliente[i] << endl;
                        cout << "saldo: R$ " << saldo[i] << endl;
                    }
                }
                break;

            case 3: // consultar conta pelo numero
                cout << "numero da conta: ";
                numero = lerInt();

                indice = -1;
                for (int i = 0; i < quantidadeContas; i++) {
                    if (numeroConta[i] == numero) {
                        indice = i;
                    }
                }

                if (indice == -1) {
                    cout << "conta nao encontrada." << endl;
                } else {
                    cout << "numero: " << numeroConta[indice] << endl;
                    cout << "titular: " << nomeCliente[indice] << endl;
                    cout << "CPF: " << cpf[indice] << endl;
                    if (tipoConta[indice] == 1) {
                        cout << "tipo: Corrente" << endl;
                    } else {
                        cout << "tipo: Poupanca" << endl;
                    }
                    cout << "saldo: R$ " << saldo[indice] << endl;
                    if (contaAtiva[indice]) {
                        cout << "situacao: Ativa" << endl;
                    } else {
                        cout << "situacao: Inativa" << endl;
                    }
                }
                break;

            case 4: // deposito
                cout << "numero da conta: ";
                numero = lerInt();

                indice = -1;
                for (int i = 0; i < quantidadeContas; i++) {
                    if (numeroConta[i] == numero) {
                        indice = i;
                    }
                }

                if (indice == -1) {
                    cout << "conta nao encontrada." << endl;
                } else if (!contaAtiva[indice]) {
                    cout << "conta inativa." << endl;
                } else if (quantidadeMovimentacoes == MAX_MOVIMENTACOES) {
                    cout << "historico cheio." << endl;
                } else {
                    cout << "valor do deposito: ";
                    valor = lerDouble();
                    if (valor <= 0) {
                        cout << "valor invalido." << endl;
                    } else {
                        saldo[indice] = saldo[indice] + valor;
                        contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indice];
                        tipoMovimentacao[quantidadeMovimentacoes] = 'D';
                        valorMovimentacao[quantidadeMovimentacoes] = valor;
                        quantidadeMovimentacoes++;
                        salvarContas();
                        salvarMovimentacoes();
                        cout << "deposito realizado!" << endl;
                    }
                }
                break;

            case 5: // saque
                cout << "numero da conta: ";
                numero = lerInt();

                indice = -1;
                for (int i = 0; i < quantidadeContas; i++) {
                    if (numeroConta[i] == numero) {
                        indice = i;
                    }
                }

                if (indice == -1) {
                    cout << "conta nao encontrada." << endl;
                } else if (!contaAtiva[indice]) {
                    cout << "conta inativa." << endl;
                } else if (quantidadeMovimentacoes == MAX_MOVIMENTACOES) {
                    cout << "historico cheio." << endl;
                } else {
                    cout << "valor do saque: ";
                    valor = lerDouble();
                    if (valor <= 0) {
                        cout << "valor invalido." << endl;
                    } else if (valor > saldo[indice]) {
                        cout << "saldo insuficiente." << endl;
                    } else {
                        saldo[indice] = saldo[indice] - valor;
                        contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indice];
                        tipoMovimentacao[quantidadeMovimentacoes] = 'S';
                        valorMovimentacao[quantidadeMovimentacoes] = valor;
                        quantidadeMovimentacoes++;
                        salvarContas();
                        salvarMovimentacoes();
                        cout << "saque realizado!" << endl;
                    }
                }
                break;

            case 6: // transferencia
                cout << "numero da conta de origem: ";
                numero = lerInt();
                indiceOrigem = -1;
                for (int i = 0; i < quantidadeContas; i++) {
                    if (numeroConta[i] == numero) {
                        indiceOrigem = i;
                    }
                }

                cout << "numero da conta de destino: ";
                numero = lerInt();
                indiceDestino = -1;
                for (int i = 0; i < quantidadeContas; i++) {
                    if (numeroConta[i] == numero) {
                        indiceDestino = i;
                    }
                }

                if (indiceOrigem == -1 || indiceDestino == -1) {
                    cout << "conta nao encontrada." << endl;
                } else if (indiceOrigem == indiceDestino) {
                    cout << "as contas precisam ser diferentes." << endl;
                } else if (!contaAtiva[indiceOrigem] || !contaAtiva[indiceDestino]) {
                    cout << "uma das contas esta inativa." << endl;
                } else if (quantidadeMovimentacoes + 2 > MAX_MOVIMENTACOES) {
                    cout << "historico sem espaco." << endl;
                } else {
                    cout << "valor da transferencia: ";
                    valor = lerDouble();
                    if (valor <= 0) {
                        cout << "valor invalido." << endl;
                    } else if (valor > saldo[indiceOrigem]) {
                        cout << "saldo insuficiente." << endl;
                    } else {
                        saldo[indiceOrigem] = saldo[indiceOrigem] - valor;
                        saldo[indiceDestino] = saldo[indiceDestino] + valor;

                        // saida na origem
                        contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indiceOrigem];
                        tipoMovimentacao[quantidadeMovimentacoes] = 'S';
                        valorMovimentacao[quantidadeMovimentacoes] = valor;
                        quantidadeMovimentacoes++;

                        // entrada no destino
                        contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indiceDestino];
                        tipoMovimentacao[quantidadeMovimentacoes] = 'D';
                        valorMovimentacao[quantidadeMovimentacoes] = valor;
                        quantidadeMovimentacoes++;

                        salvarContas();
                        salvarMovimentacoes();
                        cout << "transferencia realizada!" << endl;
                    }
                }
                break;

            case 7: // extrato
                cout << "numero da conta: ";
                numero = lerInt();

                indice = -1;
                for (int i = 0; i < quantidadeContas; i++) {
                    if (numeroConta[i] == numero) {
                        indice = i;
                    }
                }

                if (indice == -1) {
                    cout << "conta nao encontrada." << endl;
                } else {
                    cout << "\n--- extrato da conta " << numero << " ---" << endl;
                    for (int i = 0; i < quantidadeMovimentacoes; i++) {
                        if (contaMovimentacao[i] == numero) {
                            if (tipoMovimentacao[i] == 'D') {
                                cout << "deposito: R$ " << valorMovimentacao[i] << endl;
                            } else if (tipoMovimentacao[i] == 'S') {
                                cout << "saque: R$ " << valorMovimentacao[i] << endl;
                            }
                        }
                    }
                    cout << "saldo atual: R$ " << saldo[indice] << endl;
                }
                break;

            case 8:
                cout << "saindo..." << endl;
                break;

            default:
                cout << "opcao invalida." << endl;
        }

    } while (opcao != 8);

    return 0;
}
