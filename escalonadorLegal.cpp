/*
Criação

-> Código em C com as especificações do projeto
-> Arquivo de entrada em formato txt
-> Arquivo de saída esperado em txt
-> Arquivo de saída gerado por código

Ferramentas

-> Bibliotecas sobre processos
-> Estruturas de dados
	-> Filas circulares
	-> Filas com prioridade
	-> Listas encadeadas
	-> Vetores ou até structs
-> Criação de arquivos
-> Algoritmos de escalonamento de processos

Algoritmos

-> Escalonamento circular (Round Robin) (Quantum fixo)
-> Escalonamento por prioridade

Processo

Desenvolver a simulação de um sistema com escalonador preemptivo de processos e analisar diferentes algoritmos de escalonamento quanto ao tempo médio de retorno dos processos, número de chaveamento de contexto e o
overhead causado pelo chaveamento de contexto em relação ao tempo total de
simulação

Descrição

-> Ler um arquivo de entrada contendo as informações dos processos e
parâmetros do sistema
-> Executar os algoritmos de escalonamento, simulando a ocupação de
CPU, troca de contexto e registrando os eventos.
-> Gerar um arquivo de saída com os resultados utilizando cada um dos
algoritmos de escalonamento
	-> Tempo médio de retorno
	-> Linha do tempo de ocupação da CPU, indicando o processo que
está ocupando a CPU em cada momento
	-> Número de chaveamento de processos
	-> Overhead de chaveamento de processos (fração entre o tempo
total gasto com os chaveamentos e o tempo total da simulação)
	-> Tempo total para executar todos os processos

Observações

-> No algoritmo baseado em prioridade, quando menor o valor
numérico da prioridade, maior é a prioridade do processo
-> Caso dois ou mais processos com mesma prioridade sejam criados ao mesmo
tempo, a ordem de escalonamento deve seguir o ID do processo, e o processo
com menor ID é escalonado primeiro que o de maior ID
-> De mesmo modo, no Round Robin caso dois ou mais processos sejam criados no mesmo instante, a ordem de escalonamento deve ser a do ID do processo
-> Processo com menor ID é escalonado primeiro que o de maior ID
-> No Round Robin, caso um processo finalize antes de expirar a sua fatia de
tempo (quantum), a troca de contexto deve acontecer logo
-> Na criação da linha do tempo de ocupação da CPU, indicar “Escalonador”
quando se estiver realizando a troca de contexto
-> deve-se: 1) elaborar dois arquivos de entrada e dois
arquivos de saída correspondentes que possam ser usados para teste da
implementação. Tais arquivos devem ser produzidos com dados obtidos na
execução manual dos algoritmos

Arquivo de entrada

-> Na primeira linha, os valores dos parâmetros nProc, quantum e tTroca,
separados por vírgula, onde
	-> (nProc) é o número de processos a escalonar
	-> (quantum) representa o valor da fatia de tempo a ser adotada no algoritmo de escalonamento Round Robin
	-> (tTroca) denota o tempo de chaveamento de contexto, isto é, o tempo necessário para a troca de processos na CPU
-> Em cada linha, a partir da segunda linha até a linha número nProc+1, têm-se, separados por vírgula e nesta ordem
	-> Identificador do processo (ID)
	-> Tempo de chegada/criação do processo (Tch)
	-> Prioridade do processo (Prio)
	-> Tempo total de CPU (Tcpu) que o processo necessita
-> As linhas subsequentes do arquivo podem apresentar apenas as legendas dos itens descritos anteriormente
-> Abaixo, segue um exemplo de arquivo de entrada (EntradaProcessos.txt), com 5 processos, quantum de 20ms, tempo de chaveamento de 1ms e na primeira
linha tem-se o processo com ID=1, que foi criado no instante 0, tem prioridade igual a 1 e demanda 50ms de tempo de CPU pra finalizar seu trabalho
{
5,20,1
1,0,1,50
2,1,0,15
3,3,2,10
4,5,0,100
5,6,3,60
-----x-----x-----x-----x-----x-----x-----
nProc,quantum,tTroca
ID,Tch,Prio,Tcpu
nProc: número de processos
quantum: valor da fatia de tempo do Round
Robin
tTroca: Tempo de chaveamento de contexto
ID:identificador do processo
Tch: Tempo de chegada/criação do processo
Prio: Prioridade do processo
Tcpu: tempo total de CPU que o processo
necessita
-----x-----x-----x-----x-----x-----x-----
}

Arquivo de saída

-> Para cada algoritmo de escalonamento, têm-se
	-> Tempo de retorno
	-> Número de chaveamento de processos
	-> Overhead de chaveamento de processos (fração entre o tempo total gasto com os chaveamentos e o tempo total da simulação)
	-> Tempo total para executar todos os processos
	-> Linha do tempo de ocupação da CPU, indicando o processo que está
ocupando a CPU em cada momento, do inicio ao fim da simulação
{
Escalonamento Round Robin
Tempo médio de retorno: x ms
Tempo total de execução: x ms
Overhead de chaveamento: x,yz ms
Número de chaveamento: x
Ocupação do CPU: xyz ...

Escalonamento por prioridades
Tempo médio de retorno: x ms
Tempo total de execução: x ms
Overhead de chaveamento: x ms
Número de chaveamento: x ms
Ocupação de CPU: xyz ...
}
*/

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
        return a->prioridade > b->prioridade;
    }
};

void EscalonamentoRoundRobin(int num_procs, int quantum, int time_troca) {
    queue<Processo*> fila_procs;
    num_procs = 5, quantum = 20, time_troca = 1;
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
        if (proc_atual->tempo_cpu > quantum) tempo_exec = quantum;
        else tempo_exec = proc_atual->tempo_cpu;
        proc_atual->tempo_restante -= tempo_exec;
        tempo_total += tempo_exec;

        // A decisão do escalonador
        if (proc_atual->tempo_restante > 0) {
            cout << "-> Processo " << proc_atual->pid << " NÃO terminou. Voltando para o fim da fila.\n";
            fila_procs.push(proc_atual);
        }
        else {
            cout << "-> Processo " << proc_atual->pid << " CONCLUÍDO.\n";
            proc_atual->tempo_retorno = tempo_total + time_troca;
            delete proc_atual;
        }
    }

    // A média dos tempos de retorno dos procesos
    while (!fila_procs.empty()) {
        Processo *proc_atual = fila_procs.front();
        fila_procs.pop();
        soma += proc_atual->tempo_retorno;
        delete proc_atual;
    }
    media_retorno = soma / num_procs;
    cout << "\n=== Escalonamento concluído com sucesso em " << tempo_total << "ms e com a média do tempo de execução " << media_retorno << "ms! ===\n";
}

void EscalonamentoPorPrioridade(int num_procs, int time_troca) {
    priority_queue<Processo*, vector<Processo*>, ComparaPrioridade> fila_procs;
    num_procs = 5, time_troca = 1;
    int teste_vetor[] = {1, 0, 2, 0, 3}, tempo_total = 0, tempo_exec = 0, soma = 0, i;
    float media_retorno;

    cout << "=== Inicializando Processos ===\n";
    for (i = 0; i < num_procs; i++) {
        Processo *p = new Processo(i + 1, i, teste_vetor[i], 0);
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
        tempo_exec = proc_atual->tempo_cpu;
        proc_atual->tempo_restante -= tempo_exec;
        tempo_total += tempo_exec;

        // A decisão do escalonador
        proc_atual->tempo_retorno = tempo_total + time_troca;
        delete proc_atual;
    }

    // A média dos tempos de retorno dos procesos
    while (!fila_procs.empty()) {
        Processo *proc_atual = fila_procs.top();
        fila_procs.pop();
        soma += proc_atual->tempo_retorno;
        delete proc_atual;
    }
    media_retorno = soma / num_procs;
    cout << "\n=== Escalonamento concluído com sucesso em " << tempo_total << "ms e com a média do tempo de execução " << media_retorno << "ms! ===\n";
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
