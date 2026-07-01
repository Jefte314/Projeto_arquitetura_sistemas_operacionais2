#include <iostream>
#include <queue>
#include <vector>
using namespace std;

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

struct ComparaPrioridade { // Uma struct para comparar a prioridade do menor para o maior
    bool operator()(const Processo* a, const Processo* b) {
        // Quando duas prioridades são iguais
        // Vence quem chegou primeiro
        if(a->prioridade == b->prioridade) return a->tempo_chegada > b->tempo_chegada;
        return a->prioridade > b->prioridade;
    }
};

void EscalonamentoRoundRobin(int num_procs, int quantum, int time_troca) {
    queue<Processo*> fila_procs;
    int teste_vetor[] = {50, 15, 10, 100, 60}, tempo_total = 0, tempo_exec = 0, soma = 0, i;
    float media_retorno;

    cout << "=== Inicializando Processos ===\n";
    for (i = 0; i < num_procs; i++) {
        Processo *p = new Processo(i + 1, i, 0, teste_vetor[i]);
        fila_procs.push(p);
    }

    cout << "\n=== Iniciando Escalonamento Round Robin ===\n";
    while (!fila_procs.empty()) { // Executa enquanto a fila não estiver vazia
        // Pega-se o primeiro item e depois remove-o da fila
        Processo *proc_atual = fila_procs.front();
        fila_procs.pop();

        cout << "\n[Tempo: " << tempo_total << "] Executando Processo " << proc_atual->pid
                  << " (Restante: " << proc_atual->tempo_restante << ")\n";

        // Definição do tempo a ser executado
        if (proc_atual->tempo_restante > quantum) tempo_exec = quantum;
        else tempo_exec = proc_atual->tempo_restante;
        proc_atual->tempo_restante -= tempo_exec;
        tempo_total += tempo_exec;

        // A decisão do escalonador
        if (proc_atual->tempo_restante > 0) {
            cout << "-> Processo " << proc_atual->pid << " NÃO terminou. Voltando para o fim da fila.\n";
            tempo_total += time_troca;
            fila_procs.push(proc_atual);
        }
        else {
            cout << "-> Processo " << proc_atual->pid << " CONCLUÍDO.\n";
            proc_atual->tempo_retorno = tempo_total - proc_atual->tempo_chegada;
            soma += proc_atual->tempo_retorno;
            tempo_total += time_troca;
            delete proc_atual;
        }
    }

    // A média dos tempos de retorno dos procesos
    media_retorno = (float) soma / num_procs;
    cout << "\n=== Escalonamento concluído com sucesso em " << tempo_total << "ms e com a média do tempo de retorno " << media_retorno << "ms! ===\n";
}

void EscalonamentoPorPrioridade(int num_procs, int time_troca) {
    priority_queue<Processo*, vector<Processo*>, ComparaPrioridade> fila_procs;
    int teste_vetor_prioridade[] = {1, 0, 2, 0, 3}, teste_vetor_tempoCPU[] = {50, 15, 10, 100, 60}, tempo_total = 0, tempo_exec = 0, soma = 0, i;
    float media_retorno;

    cout << "=== Inicializando Processos ===\n";
    for (i = 0; i < num_procs; i++) {
        Processo *p = new Processo(i + 1, i, teste_vetor_prioridade[i], teste_vetor_tempoCPU[i]);
        fila_procs.push(p);
    }

    cout << "\n=== Iniciando Escalonamento Por Prioridades ===\n";
    while (!fila_procs.empty()) { // Executa enquanto a fila de prioridades não estiver vazia
        // Pega-se o primeiro item e depois remove-o da fila de prioridades
        Processo *proc_atual = fila_procs.top();
        fila_procs.pop();

        cout << "\n[Prioridade: " << proc_atual->prioridade << "] Executando Processo " << proc_atual->pid
                << " (Restante: " << proc_atual->tempo_restante << ")\n";

        // Alguma coisa
        tempo_exec = proc_atual->tempo_restante;
        proc_atual->tempo_restante -= tempo_exec;
        tempo_total += tempo_exec;

        // A decisão do escalonador
        cout << "-> Processo " << proc_atual->pid << " CONCLUÍDO.\n";
        proc_atual->tempo_retorno = tempo_total + time_troca - proc_atual->tempo_chegada;
        soma += proc_atual->tempo_retorno;
        tempo_total += time_troca;
        delete proc_atual;
    }

    // A média dos tempos de retorno dos procesos
    media_retorno = (float) soma / num_procs;
    cout << "\n=== Escalonamento concluído com sucesso em " << tempo_total << "ms e com a média do tempo de retorno " << media_retorno << "ms! ===\n";
}

int main() {
    // Teste primário da lógica dos algoritmos
    queue<Processo*> fila1;
    priority_queue<Processo*, vector<Processo*>, ComparaPrioridade> fila2;
    EscalonamentoRoundRobin(5, 20, 1);
    cout << "\n";
    EscalonamentoPorPrioridade(5, 1);
    return 0;
}
