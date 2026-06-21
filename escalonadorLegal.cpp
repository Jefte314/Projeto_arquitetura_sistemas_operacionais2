#include <iostream>
#include <queue>
#include <vector>
#define QUANTUM 60 // Tempo de fatia de CPU para o Round Robin em milissegundos
using namespace std;

// Estrutura limpa, sem macros de ponteiros internos
typedef struct {
    int id;
    int tempo_chegada;
    int prioridade;
    int tempo_cpu;
} Processo;

int main() {
    // Criação da fila utilizando a STL do C++
    queue<Processo*> pronto_queue;

    // Criação e inserção de processos na fila para o teste
    int tempos_iniciais[] = {40, 20, 80};
    int num_processos = sizeof(tempos_iniciais) / sizeof(tempos_iniciais[0]);

    cout << "=== Inicializando Processos ===\n";
    for (int i = 0; i < num_processos; i++) {
        // Alocação dinâmica padrão C++
        Processo *p = new Processo();
        p->id = i + 1;
        p->tempo_chegada = tempos_iniciais[i];
        p->prioridade = 0; // Inicializando campos extras
        p->tempo_cpu = 0;

        // Inserção no fim da fila
        pronto_queue.push(p);
        cout << "Processo " << p->id << " criado (Tempo total necessário: " << p->tempo_chegada << ")\n";
    }

    cout << "\n=== Iniciando Escalonamento Round Robin (Quantum = " << QUANTUM << ") ===\n";
    int tempo_total = 0;

    // Loop executa enquanto a fila não estiver vazia
    while (!pronto_queue.empty()) {
        // Pega-se o primeiro item e depois remove-o da fila
        Processo *p_atual = pronto_queue.front();
        pronto_queue.pop();

        cout << "\n[Tempo: " << tempo_total << "] Executando Processo " << p_atual->id
                  << " (Restante: " << p_atual->tempo_chegada << ")\n";

        // Definição de quanto tempo o processo vai rodar nesta rodada
        int tempo_execucao = (p_atual->tempo_chegada > QUANTUM) ? QUANTUM : p_atual->tempo_chegada;

        // Uma simulação do processamento
        p_atual->tempo_chegada -= tempo_execucao;
        tempo_total += tempo_execucao;
        cout << "-> Processo " << p_atual->id << " rodou por " << tempo_execucao << " unidades de tempo.\n";

        // Decisão do Escalonador
        if (p_atual->tempo_chegada > 0) {
            // Esse sofreu preempção: volta para o fim da fila de prontos
            cout << "-> Processo " << p_atual->id << " NÃO terminou. Voltando para o fim da fila.\n";
            pronto_queue.push(p_atual);
        } else {
            // O processo terminou sua execução
            cout << "-> Processo " << p_atual->id << " CONCLUÍDO.\n";
            delete p_atual; // Liberação de memória padrão C++
        }
    }

    cout << "\n=== Escalonamento concluído com sucesso em " << tempo_total << " unidades de tempo! ===\n";
    return 0;
}
