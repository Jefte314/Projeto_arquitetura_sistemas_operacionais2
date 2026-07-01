#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
using namespace std;

struct Config_geral { // Guarda as informações em um arquivo
    int num_procs;
    int quantum;
    int tempo_troca;
};

class Processo {
    public:
        int pid, tempo_chegada, prioridade, tempo_cpu, tempo_restante, tempo_retorno;

        // Construtor da classe processo
        Processo(int pid, int tempo_chegada, int prioridade, int tempo_cpu) {
            this->pid = pid;
            this->tempo_chegada = tempo_chegada;
            this->prioridade = prioridade;
            this->tempo_cpu = tempo_cpu;
            this->tempo_restante = tempo_cpu; // Tempo restante de CPU
            this->tempo_retorno = 0; // Famoso turnaround
        }
};

// Lê o arquivo de entrada e retorna um vetor de ponteiros para os processos
vector<Processo*> LerArquivo(string nomeArquivo, Config_geral &config){
    ifstream arquivo(nomeArquivo);
    vector<Processo*> processos;
    char lixo;
    int i;

    // Verifica se o arquivo foi aberto corretamente
    if (!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo!\n";
        exit(1);
    }

    arquivo >> config.num_procs >> lixo >> config.quantum >> lixo >> config.tempo_troca;
    for (int i=0;i<config.num_procs;i++) {
        int id, chegada, prioridade, cpu;
        if (!(arquivo >> id >> lixo >> chegada >> lixo >> prioridade >> lixo >> cpu)) {
            cout << "Erro no formato do arquivo!\n";
            exit(1);
        }
        processos.push_back(new Processo(id, chegada, prioridade, cpu));
    }

    arquivo.close();
    return processos;
}

// Escreve as informações do processo no arquivo de saída
void EscreverArquivo(ofstream &saida, Processo *p){
    saida << "Processo " << p->pid << " | Chegada: " << p->tempo_chegada << " | CPU: " << p->tempo_cpu << " | Retorno: " << p->tempo_retorno << " ms\n";
}

struct ComparaPrioridade { // Uma struct para comparar a prioridade do menor para o maior
    bool operator()(const Processo* a, const Processo* b) {
        // Quando duas prioridades são iguais
        // Vence quem chegou primeiro
        if(a->prioridade == b->prioridade) return a->tempo_chegada > b->tempo_chegada;
        return a->prioridade > b->prioridade;
    }
};

void EscalonamentoRoundRobin(vector<Processo*> &processos, int quantum, int time_troca, ofstream &saida) {
    // Verifica se o arquivo foi aberto corretamente
    if(!saida.is_open()){
        cout << "Erro ao abrir o arquivo!\n";
        exit(1);
    }

    queue<Processo*> fila_procs;
    int tempo_total = 0, tempo_exec = 0, num_chav = 0, soma = 0;
    float media_retorno, overhead;

    saida << "=== Inicializando Processos ===\n";
    for(Processo *p : processos) fila_procs.push(p);

    saida << "=============================\n";
    saida << "ESCALONAMENTO ROUND ROBIN\n";
    saida << "=============================\n\n";
    saida << "Quantidade de processos: " << processos.size() << '\n';
    saida << "Quantum: " << quantum << '\n';
    saida << "Tempo de troca de contexto: " << time_troca << " ms\n\n";

    while (!fila_procs.empty()) { // Executa enquanto a fila não estiver vazia
        // Pega-se o primeiro item e depois remove-o da fila
        Processo *proc_atual = fila_procs.front();
        fila_procs.pop();
        saida << "[Tempo " << tempo_total << "] -> Processo " << proc_atual->pid << " executando...\n";

        // Definição do tempo a ser executado
        if (proc_atual->tempo_restante > quantum) tempo_exec = quantum;
        else tempo_exec = proc_atual->tempo_restante;
        proc_atual->tempo_restante -= tempo_exec;
        tempo_total += tempo_exec;

        // A decisão do escalonador
        if (proc_atual->tempo_restante > 0) {
            saida << "-> Processo " << proc_atual->pid << " NÃO terminou. Voltando para o fim da fila.\n";
            tempo_total += time_troca;
            num_chav++;
            fila_procs.push(proc_atual);
        }
        else {
            saida << "-> Processo " << proc_atual->pid << " CONCLUÍDO.\n";
            proc_atual->tempo_retorno = tempo_total - proc_atual->tempo_chegada;
            soma += proc_atual->tempo_retorno;
            if (!fila_procs.empty()) {
                tempo_total += time_troca; // Adiciona tempo da troca de contexto
                num_chav++;
            }
            EscreverArquivo(saida, proc_atual);
            delete proc_atual;
        }
    }

    // A média dos tempos de retorno dos procesos
    media_retorno = (float) soma / processos.size();
    processos.clear(); // Limpa o vetor de processos
    overhead = (float) (num_chav * time_troca) / tempo_total;
    saida << "\nTempo total: " << tempo_total << " ms\n";
    saida << "Tempo medio de retorno: " << media_retorno << " ms\n";
    saida << "Número de trocas de contexto: " << num_chav << "\n";
    saida << "Overhead de chaveamento: " << overhead * 100 << "%\n";
    saida << "\n=== Escalonamento concluído com sucesso em " << tempo_total << "ms e com a média do tempo de retorno " << media_retorno << "ms! ===\n";
}

void EscalonamentoPorPrioridade(vector<Processo*> &processos, int time_troca, ofstream &saida) {
    // Verifica se o arquivo foi aberto corretamente
    if(!saida.is_open()){
        cout << "Erro ao abrir o arquivo!\n";
        exit(1);
    }

    priority_queue<Processo*, vector<Processo*>, ComparaPrioridade> fila_procs;
    int tempo_total = 0, tempo_exec = 0, num_chav = 0, soma = 0;
    float media_retorno, overhead;

    saida << "=== Inicializando Processos ===\n";
    for(Processo *p : processos) fila_procs.push(p);

    saida << "=============================\n";
    saida << "ESCALONAMENTO POR PRIORIDADE\n";
    saida << "=============================\n\n";
    saida << "Quantidade de processos: " << processos.size() << '\n';
    saida << "Tempo de troca de contexto: " << time_troca << " ms\n\n";

    while (!fila_procs.empty()) { // Executa enquanto a fila de prioridades não estiver vazia
        // Pega-se o primeiro item e depois remove-o da fila de prioridades
        Processo *proc_atual = fila_procs.top();
        fila_procs.pop();
        saida << "[Prioridade " << proc_atual->prioridade << "] -> Processo " << proc_atual->pid << " executando...\n";

        // Definição do tempo a ser executado
        if (proc_atual->tempo_restante > 0) {
            tempo_exec = proc_atual->tempo_restante;
            proc_atual->tempo_restante = 0;
            tempo_total += tempo_exec;
        }

        // A decisão do escalonador
        saida << "-> Processo " << proc_atual->pid << " CONCLUÍDO.\n";
        proc_atual->tempo_retorno = tempo_total - proc_atual->tempo_chegada;
        soma += proc_atual->tempo_retorno;
        if (!fila_procs.empty()) {
            tempo_total += time_troca; // Adiciona tempo da troca de contexto
            num_chav++;
        }

        EscreverArquivo(saida, proc_atual);
        delete proc_atual;
    }

    // A média dos tempos de retorno dos procesos
    media_retorno = (float) soma / processos.size();
    processos.clear(); // Limpa o vetor de processos
    overhead = (float) (num_chav * time_troca) / tempo_total;
    saida << "\nTempo total: " << tempo_total << " ms\n";
    saida << "Tempo medio de retorno: " << media_retorno << " ms\n";
    saida << "Número de trocas de contexto: " << num_chav << "\n";
    saida << "Overhead de chaveamento: " << overhead * 100 << "%\n";
    saida << "\n=== Escalonamento concluído com sucesso em " << tempo_total << "ms e com a média do tempo de retorno " << media_retorno << "ms! ===\n";
}

int main(void) {
    Config_geral config;
    ofstream saida("saida.txt");

    // Verifica se o arquivo de saída foi aberto corretamente
    if (!saida.is_open()) {
        cout << "Erro ao criar arquivo de saída!\n";
        return 1;
    }

    vector<Processo*> processosRR = LerArquivo("entrada.txt", config);
    EscalonamentoRoundRobin(processosRR, config.quantum, config.tempo_troca, saida);
    saida << "\n\n=========================================\n\n";

    vector<Processo*> processosPrioridade = LerArquivo("entrada.txt", config);
    EscalonamentoPorPrioridade(processosPrioridade, config.tempo_troca, saida);
    saida.close();
    return 0;
}
