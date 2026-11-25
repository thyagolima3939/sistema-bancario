#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

// Limites do sistema
const int MAX_CONTAS = 100;
const int MAX_MOVIMENTACOES = 500;

// Dados das contas
int numeroConta[MAX_CONTAS];
string nomeCliente[MAX_CONTAS];
string cpf[MAX_CONTAS];
int tipoConta[MAX_CONTAS];
double saldo[MAX_CONTAS];
bool contaAtiva[MAX_CONTAS];

int quantidadeContas = 0;

// Historico de movimentacoes
int contaMovimentacao[MAX_MOVIMENTACOES];
char tipoMovimentacao[MAX_MOVIMENTACOES];
double valorMovimentacao[MAX_MOVIMENTACOES];

int quantidadeMovimentacoes = 0;

// Declaracao das funcoes
void cadastrarConta();
void listarContas();
void consultarConta();
void depositar();
void sacar();
void transferir();
void registrarMovimentacao(int numero, char tipo, double valor);
void exibirExtrato();

void carregarContas();
void salvarContas();
void carregarMovimentacoes();
void salvarMovimentacoes();

int buscarConta(int numero);

// FUNCAO PRINCIPAL
int main() {

    int opcao;

    cout << fixed << setprecision(2);

    // Carregar dados dos arquivos ao iniciar
    carregarContas();
    carregarMovimentacoes();

    cout << "\nDados carregados com sucesso!\n";

    do {

        cout << "\n========== SISTEMA BANCARIO ==========\n";
        cout << "1 - Cadastrar conta\n";
        cout << "2 - Listar contas\n";
        cout << "3 - Consultar conta\n";
        cout << "4 - Realizar deposito\n";
        cout << "5 - Realizar saque\n";
        cout << "6 - Realizar transferencia\n";
        cout << "7 - Exibir extrato\n";
        cout << "0 - Sair\n";
        cout << "======================================\n";

        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {

            case 1:
                cadastrarConta();
                break;

            case 2:
                listarContas();
                break;

            case 3:
                consultarConta();
                break;

            case 4:
                depositar();
                break;

            case 5:
                sacar();
                break;

            case 6:
                transferir();
                break;

            case 7:
                exibirExtrato();
                break;

            case 0:
                cout << "\nEncerrando o sistema bancario...\n";
                break;

            default:
                cout << "\nOpcao invalida!\n";
        }

    } while (opcao != 0);

    // Salvar os dados antes de encerrar
    salvarContas();
    salvarMovimentacoes();

    cout << "\nDados salvos com sucesso!\n";

    return 0;
}

// FUNCAO PARA BUSCAR UMA CONTA
int buscarConta(int numero) {

    for (int i = 0; i < quantidadeContas; i++) {

        if (numeroConta[i] == numero) {
            return i;
        }
    }

    return -1;
}

// FUNCAO PARA CARREGAR AS CONTAS DO ARQUIVO
void carregarContas() {

    ifstream arquivo("contas.txt");

    if (!arquivo.is_open()) {
        cout << "\nArquivo contas.txt ainda nao existe.\n";
        return;
    }

    string linha;

    while (getline(arquivo, linha)) {

        if (quantidadeContas >= MAX_CONTAS) {
            cout << "\nLimite de contas atingido durante a leitura!\n";
            break;
        }

        string campoNumero, campoNome, campoCpf;
        string campoTipo, campoSaldo, campoSituacao;

        stringstream ss(linha);

        if (getline(ss, campoNumero, ';') &&
            getline(ss, campoNome, ';') &&
            getline(ss, campoCpf, ';') &&
            getline(ss, campoTipo, ';') &&
            getline(ss, campoSaldo, ';') &&
            getline(ss, campoSituacao)) {

            try {

                numeroConta[quantidadeContas] = stoi(campoNumero);
                nomeCliente[quantidadeContas] = campoNome;
                cpf[quantidadeContas] = campoCpf;
                tipoConta[quantidadeContas] = stoi(campoTipo);
                saldo[quantidadeContas] = stod(campoSaldo);
                contaAtiva[quantidadeContas] =
                    (stoi(campoSituacao) != 0);

                quantidadeContas++;

            } catch (...) {

                cout << "\nRegistro invalido encontrado em contas.txt.\n";
            }
        }
    }

    arquivo.close();

    cout << "\nContas carregadas: "
         << quantidadeContas << endl;
}

// FUNCAO PARA SALVAR AS CONTAS NO ARQUIVO
void salvarContas() {

    ofstream arquivo("contas.txt");

    if (!arquivo.is_open()) {
        cout << "\nErro ao abrir contas.txt para gravacao!\n";
        return;
    }

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

// FUNCAO PARA CARREGAR AS MOVIMENTACOES
void carregarMovimentacoes() {

    ifstream arquivo("movimentacoes.txt");

    if (!arquivo.is_open()) {
        cout << "\nArquivo movimentacoes.txt ainda nao existe.\n";
        return;
    }

    string linha;

    while (getline(arquivo, linha)) {

        if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) {
            cout << "\nLimite de movimentacoes atingido durante a leitura!\n";
            break;
        }

        string campoNumero, campoTipo, campoValor;

        stringstream ss(linha);

        if (getline(ss, campoNumero, ';') &&
            getline(ss, campoTipo, ';') &&
            getline(ss, campoValor)) {

            try {

                if (campoTipo.empty()) {
                    continue;
                }

                contaMovimentacao[quantidadeMovimentacoes] =
                    stoi(campoNumero);

                tipoMovimentacao[quantidadeMovimentacoes] =
                    campoTipo[0];

                valorMovimentacao[quantidadeMovimentacoes] =
                    stod(campoValor);

                quantidadeMovimentacoes++;

            } catch (...) {

                cout << "\nRegistro invalido encontrado em movimentacoes.txt.\n";
            }
        }
    }

    arquivo.close();

    cout << "Movimentacoes carregadas: "
         << quantidadeMovimentacoes << endl;
}

// FUNCAO PARA SALVAR AS MOVIMENTACOES
void salvarMovimentacoes() {

    ofstream arquivo("movimentacoes.txt");

    if (!arquivo.is_open()) {
        cout << "\nErro ao abrir movimentacoes.txt para gravacao!\n";
        return;
    }

    for (int i = 0; i < quantidadeMovimentacoes; i++) {

        arquivo << contaMovimentacao[i] << ";"
                << tipoMovimentacao[i] << ";"
                << valorMovimentacao[i] << endl;
    }

    arquivo.close();
}

// FUNCAO PARA CADASTRAR CONTA
void cadastrarConta() {

    if (quantidadeContas >= MAX_CONTAS) {
        cout << "\nLimite de contas atingido!\n";
        return;
    }

    int numero;

    cout << "\nNumero da conta: ";
    cin >> numero;

    if (buscarConta(numero) != -1) {
        cout << "\nConta ja cadastrada!\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    numeroConta[quantidadeContas] = numero;

    cout << "Nome do cliente: ";
    getline(cin, nomeCliente[quantidadeContas]);

    cout << "CPF: ";
    getline(cin, cpf[quantidadeContas]);

    cout << "Tipo de conta (1 - Corrente / 2 - Poupanca): ";
    cin >> tipoConta[quantidadeContas];

    while (tipoConta[quantidadeContas] != 1 &&
           tipoConta[quantidadeContas] != 2) {

        cout << "Tipo invalido! Digite 1 ou 2: ";
        cin >> tipoConta[quantidadeContas];
    }

    cout << "Saldo inicial: R$ ";
    cin >> saldo[quantidadeContas];

    while (saldo[quantidadeContas] < 0) {

        cout << "Saldo invalido. Digite novamente: ";
        cin >> saldo[quantidadeContas];
    }

    contaAtiva[quantidadeContas] = true;

    quantidadeContas++;

    // Salvar as contas atualizadas
    salvarContas();

    cout << "\nConta cadastrada com sucesso!\n";
}

// FUNCAO PARA LISTAR CONTAS
void listarContas() {

    if (quantidadeContas == 0) {
        cout << "\nNenhuma conta cadastrada!\n";
        return;
    }

    cout << "\n========== CONTAS CADASTRADAS ==========\n";

    for (int i = 0; i < quantidadeContas; i++) {

        cout << "\nNumero: " << numeroConta[i];
        cout << "\nCliente: " << nomeCliente[i];
        cout << "\nCPF: " << cpf[i];

        if (tipoConta[i] == 1) {
            cout << "\nTipo: Conta Corrente";
        } else {
            cout << "\nTipo: Conta Poupanca";
        }

        cout << "\nSaldo: R$ " << saldo[i];

        if (contaAtiva[i]) {
            cout << "\nSituacao: Ativa\n";
        } else {
            cout << "\nSituacao: Inativa\n";
        }

        cout << "----------------------------------------\n";
    }
}

// FUNCAO PARA CONSULTAR CONTA
void consultarConta() {

    int numero;

    cout << "\nDigite o numero da conta: ";
    cin >> numero;

    int indice = buscarConta(numero);

    if (indice == -1) {
        cout << "\nConta nao encontrada!\n";
        return;
    }

    cout << "\n========== DADOS DA CONTA ==========\n";
    cout << "Numero: " << numeroConta[indice] << endl;
    cout << "Cliente: " << nomeCliente[indice] << endl;
    cout << "CPF: " << cpf[indice] << endl;

    if (tipoConta[indice] == 1) {
        cout << "Tipo: Conta Corrente\n";
    } else {
        cout << "Tipo: Conta Poupanca\n";
    }

    cout << "Saldo: R$ " << saldo[indice] << endl;

    if (contaAtiva[indice]) {
        cout << "Situacao: Ativa\n";
    } else {
        cout << "Situacao: Inativa\n";
    }
}

// FUNCAO PARA REGISTRAR MOVIMENTACAO
void registrarMovimentacao(int numero, char tipo, double valor) {

    if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) {
        cout << "\nLimite de movimentacoes atingido!\n";
        return;
    }

    contaMovimentacao[quantidadeMovimentacoes] = numero;
    tipoMovimentacao[quantidadeMovimentacoes] = tipo;
    valorMovimentacao[quantidadeMovimentacoes] = valor;

    quantidadeMovimentacoes++;

    // Salvar o historico atualizado
    salvarMovimentacoes();
}

// FUNCAO PARA DEPOSITAR
void depositar() {

    int numero;
    double valor;

    cout << "\nNumero da conta: ";
    cin >> numero;

    int indice = buscarConta(numero);

    if (indice == -1) {
        cout << "\nConta nao encontrada!\n";
        return;
    }

    if (!contaAtiva[indice]) {
        cout << "\nConta inativa!\n";
        return;
    }

    if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) {
        cout << "\nLimite de movimentacoes atingido!\n";
        return;
    }

    cout << "Valor do deposito: R$ ";
    cin >> valor;

    if (valor <= 0) {
        cout << "\nValor invalido!\n";
        return;
    }

    saldo[indice] += valor;

    registrarMovimentacao(numeroConta[indice], 'D', valor);

    // Salvar o saldo atualizado
    salvarContas();

    cout << "\nDeposito realizado com sucesso!\n";
    cout << "Novo saldo: R$ " << saldo[indice] << endl;
}

// FUNCAO PARA SACAR
void sacar() {

    int numero;
    double valor;

    cout << "\nNumero da conta: ";
    cin >> numero;

    int indice = buscarConta(numero);

    if (indice == -1) {
        cout << "\nConta nao encontrada!\n";
        return;
    }

    if (!contaAtiva[indice]) {
        cout << "\nConta inativa!\n";
        return;
    }

    if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) {
        cout << "\nLimite de movimentacoes atingido!\n";
        return;
    }

    cout << "Valor do saque: R$ ";
    cin >> valor;

    if (valor <= 0) {
        cout << "\nValor invalido!\n";
        return;
    }

    if (valor > saldo[indice]) {
        cout << "\nSaldo insuficiente!\n";
        return;
    }

    saldo[indice] -= valor;

    registrarMovimentacao(numeroConta[indice], 'S', valor);

    // Salvar o saldo atualizado
    salvarContas();

    cout << "\nSaque realizado com sucesso!\n";
    cout << "Novo saldo: R$ " << saldo[indice] << endl;
}

// FUNCAO PARA TRANSFERIR
void transferir() {

    int origem, destino;
    double valor;

    cout << "\nNumero da conta de origem: ";
    cin >> origem;

    int indiceOrigem = buscarConta(origem);

    if (indiceOrigem == -1) {
        cout << "\nConta de origem nao encontrada!\n";
        return;
    }

    if (!contaAtiva[indiceOrigem]) {
        cout << "\nConta de origem inativa!\n";
        return;
    }

    cout << "Numero da conta de destino: ";
    cin >> destino;

    int indiceDestino = buscarConta(destino);

    if (indiceDestino == -1) {
        cout << "\nConta de destino nao encontrada!\n";
        return;
    }

    if (!contaAtiva[indiceDestino]) {
        cout << "\nConta de destino inativa!\n";
        return;
    }

    if (indiceOrigem == indiceDestino) {
        cout << "\nNao e possivel transferir para a mesma conta!\n";
        return;
    }

    if (quantidadeMovimentacoes + 2 > MAX_MOVIMENTACOES) {
        cout << "\nEspaco insuficiente para registrar a transferencia!\n";
        return;
    }

    cout << "Valor da transferencia: R$ ";
    cin >> valor;

    if (valor <= 0) {
        cout << "\nValor invalido!\n";
        return;
    }

    if (valor > saldo[indiceOrigem]) {
        cout << "\nSaldo insuficiente!\n";
        return;
    }

    // Atualizar os saldos
    saldo[indiceOrigem] -= valor;
    saldo[indiceDestino] += valor;

    // Registrar a saida da conta de origem
    registrarMovimentacao(origem, 'S', valor);

    // Registrar a entrada na conta de destino
    registrarMovimentacao(destino, 'D', valor);

    // Salvar as contas atualizadas
    salvarContas();

    cout << "\nTransferencia realizada com sucesso!\n";

    cout << "\nNovo saldo da conta de origem: R$ "
         << saldo[indiceOrigem] << endl;

    cout << "Novo saldo da conta de destino: R$ "
         << saldo[indiceDestino] << endl;
}

// FUNCAO PARA EXIBIR EXTRATO
void exibirExtrato() {

    int numero;
    bool encontrou = false;

    cout << "\nNumero da conta: ";
    cin >> numero;

    int indice = buscarConta(numero);

    if (indice == -1) {
        cout << "\nConta nao encontrada!\n";
        return;
    }

    cout << "\n========== EXTRATO BANCARIO ==========\n";
    cout << "Conta: " << numeroConta[indice] << endl;
    cout << "Cliente: " << nomeCliente[indice] << endl;

    cout << "\nTipo          Valor\n";
    cout << "------------------------------\n";

    for (int i = 0; i < quantidadeMovimentacoes; i++) {

        if (contaMovimentacao[i] == numero) {

            encontrou = true;

            if (tipoMovimentacao[i] == 'D') {

                cout << "Deposito/Transferencia recebida: R$ "
                     << valorMovimentacao[i] << endl;
            }
            else if (tipoMovimentacao[i] == 'S') {

                cout << "Saque/Transferencia enviada: R$ "
                     << valorMovimentacao[i] << endl;
            }
        }
    }

    if (!encontrou) {
        cout << "\nNenhuma movimentacao encontrada!\n";
    }

    cout << "------------------------------\n";
    cout << "Saldo atual: R$ " << saldo[indice] << endl;
    cout << "======================================\n";
}
