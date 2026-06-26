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
