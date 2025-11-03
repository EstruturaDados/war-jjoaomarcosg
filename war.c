#include <stdio.h>      // Para entrada e saída (printf, scanf, getchar, fgets)
#include <stdlib.h>     // Para alocação dinâmica (calloc, free) e aleatoriedade (rand, srand)
#include <string.h>     // Para manipulação de strings (strcpy, strcmp)
#include <time.h>       // Para inicializar a semente de aleatoriedade (srand(time(NULL)))
#include <locale.h>     // Para usar acentos em português (setlocale)

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings.
#define TOTAL_TERRITORIOS 5
#define TOTAL_MISSOES 2
#define MAX_NOME 30
#define MAX_COR 20

// --- Estrutura de Dados ---
// Define a estrutura para um território.
struct Territorio {
    char nome[MAX_NOME];
    char corExercito[MAX_COR];
    int tropas;
};

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa.

// Funções de setup e gerenciamento de memória:
struct Territorio* alocarMapa(int total);
void inicializarTerritorios(struct Territorio *mapa, int total);
void liberarMemoria(struct Territorio *mapa);

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const struct Territorio *mapa, int total);
void exibirMissao(int idMissao, const char *corJogador);
void limparBufferEntrada();
void pausarParaContinuar(); // Uma função extra útil

// Funções de lógica principal do jogo:
void faseDeAtaque(struct Territorio *mapa, int total, const char *corJogador);
void simularAtaque(struct Territorio *atacante, struct Territorio *defensor);
int sortearMissao(int totalMissoes);
int verificarVitoria(const struct Territorio *mapa, int total, int idMissao, const char *corJogador);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    setlocale(LC_ALL, "Portuguese"); // Permite usar acentos no console
    srand(time(NULL)); // Inicializa a semente para números aleatórios

    int escolha = 0; // Armazena a escolha do menu
    int venceu = 0;  // Flag para verificar se o jogador venceu
    const char *corJogador = "Azul"; // Define a cor do jogador

    // Aloca a memória para o mapa e verifica se a alocação foi bem-sucedida.
    struct Territorio *mapa = alocarMapa(TOTAL_TERRITORIOS);
    if (mapa == NULL) {
        printf("Erro fatal: Nao foi possivel alocar memoria para o mapa!\n");
        return 1; // Retorna 1 para indicar um erro
    }

    // Preenche os territórios e sorteia a missão.
    inicializarTerritorios(mapa, TOTAL_TERRITORIOS);
    int missaoID = sortearMissao(TOTAL_MISSOES);

    // 2. Laço Principal do Jogo (Game Loop):
    do {
        // Limpa a tela. Use "clear" se estiver no Linux/Mac
        // Se "cls" não funcionar, você pode comentar esta linha
        system("cls"); 
        
        exibirMapa(mapa, TOTAL_TERRITORIOS);
        exibirMissao(missaoID, corJogador);
        exibirMenuPrincipal();

        // Lê a escolha do jogador
        printf("\nSua escolha: ");
        scanf("%d", &escolha);
        limparBufferEntrada(); // ESSENCIAL para limpar o "Enter" do buffer

        switch (escolha) {
            case 1:
                faseDeAtaque(mapa, TOTAL_TERRITORIOS, corJogador);
                break;
            case 2:
                venceu = verificarVitoria(mapa, TOTAL_TERRITORIOS, missaoID, corJogador);
                if (venceu) {
                    printf("\n*** PARABENS! VOCE CUMPRIU SUA MISSAO E VENCEU O JOGO! ***\n");
                } else {
                    printf("\nVoce ainda nao cumpriu sua missao. Continue lutando!\n");
                }
                break;
            case 0:
                printf("\nSaindo do jogo... Ate a proxima!\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }

        if (escolha != 0) {
            pausarParaContinuar(); // Pausa a tela antes de limpar no próximo loop
        }

    } while (escolha != 0 && !venceu); // Continua se o jogador não saiu e não venceu

    // 3. Limpeza:
    // Libera a memória alocada para o mapa.
    liberarMemoria(mapa);

    return 0; // Retorna 0 para indicar sucesso
}


// --- Implementação das Funções de Setup, Memória e Utilidade ---

/*
 * alocarMapa(): Aloca dinamicamente a memória para o vetor de territórios.
 * Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
 */
struct Territorio* alocarMapa(int total) {
    // Usamos calloc pois ele, além de alocar, já inicializa a memória com zeros.
    struct Territorio *mapa;
    mapa = (struct Territorio*) calloc(total, sizeof(struct Territorio));
    return mapa; // Retorna o ponteiro (ou NULL se o calloc falhar)
}

/*
 * inicializarTerritorios(): Preenche os dados iniciais de cada território.
 * Esta função modifica o mapa diretamente através do ponteiro.
 */
void inicializarTerritorios(struct Territorio *mapa, int total) {
    // Dados iniciais do nosso mapa (Nível Mestre não pede para o usuário digitar)
    // strncpy é uma forma segura de copiar strings em C, evitando buffer overflow.
    
    // Território 0
    strncpy(mapa[0].nome, "Brasil", MAX_NOME);
    strncpy(mapa[0].corExercito, "Verde", MAX_COR);
    mapa[0].tropas = 8;

    // Território 1
    strncpy(mapa[1].nome, "Argentina", MAX_NOME);
    strncpy(mapa[1].corExercito, "Azul", MAX_COR);
    mapa[1].tropas = 5;

    // Território 2
    strncpy(mapa[2].nome, "Peru", MAX_NOME);
    strncpy(mapa[2].corExercito, "Verde", MAX_COR);
    mapa[2].tropas = 3;

    // Território 3
    strncpy(mapa[3].nome, "Siberia", MAX_NOME);
    strncpy(mapa[3].corExercito, "Vermelho", MAX_COR);
    mapa[3].tropas = 10;

    // Território 4
    strncpy(mapa[4].nome, "China", MAX_NOME);
    strncpy(mapa[4].corExercito, "Vermelho", MAX_COR);
    mapa[4].tropas = 8;
}

/*
 * liberarMemoria(): Libera a memória previamente alocada para o mapa.
 */
void liberarMemoria(struct Territorio *mapa) {
    printf("\nLiberando memoria do mapa...");
    free(mapa);
    mapa = NULL; // Boa prática: anular o ponteiro após liberar
    printf(" Memoria liberada.\n");
}

/*
 * limparBufferEntrada(): Função utilitária essencial para limpar
 * o buffer do teclado (stdin) após o uso do scanf.
 */
void limparBufferEntrada() {
    int c;
    // Lê e descarta todos os caracteres até encontrar uma nova linha ('\n')
    // ou o fim do arquivo (EOF).
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * pausarParaContinuar(): Função utilitária para pausar a tela.
 */
void pausarParaContinuar() {
    printf("\nPressione Enter para continuar...");
    // Limpa qualquer 'Enter' residual ANTES de esperar pelo novo Enter
    limparBufferEntrada(); 
    getchar(); // Espera o usuário pressionar Enter
}


// --- Implementação das Funções de Interface com o Usuário ---

/*
 * exibirMenuPrincipal(): Imprime na tela o menu de ações.
 */
void exibirMenuPrincipal() {
    printf("\n--- MENU DE ACOES ---\n");
    printf("1. Atacar\n");
    printf("2. Verificar Missao\n");
    printf("0. Sair do Jogo\n");
}

/*
 * exibirMapa(): Mostra o estado atual de todos os territórios.
 * Usa 'const' para garantir que a função apenas LEIA os dados.
 */
void exibirMapa(const struct Territorio *mapa, int total) {
    printf("--- MAPA DO MUNDO (Estado Atual) ---\n");
    printf("ID | Territorio      | Exercito   | Tropas\n");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < total; i++) {
        // Usamos %-15s para alinhar o texto à esquerda em 15 espaços
        printf("%-2d | %-15s | %-10s | %-6d\n", 
               i + 1, // ID amigável para o usuário (1-5)
               mapa[i].nome, 
               mapa[i].corExercito, 
               mapa[i].tropas);
    }
    printf("------------------------------------------------\n");
}

/*
 * exibirMissao(): Exibe a descrição da missão atual do jogador.
 */
void exibirMissao(int idMissao, const char *corJogador) {
    printf("\n--- SUA MISSAO SECRETA ---\n");
    printf("Voce eh o exercito: %s\n", corJogador);
    printf("Objetivo: ");
    
    switch (idMissao) {
        case 0:
            printf("Destruir o exercito Verde.\n");
            break;
        case 1:
            printf("Conquistar 3 territorios (possuir 3 ou mais).\n");
            break;
        default:
            printf("Missao desconhecida.\n");
            break;
    }
}


// --- Implementação das Funções de Lógica Principal do Jogo ---

/*
 * sortearMissao(): Sorteia e retorna um ID de missão aleatório.
 */
int sortearMissao(int totalMissoes) {
    // rand() % totalMissoes -> gera um número entre 0 e (totalMissoes - 1)
    // No nosso caso (TOTAL_MISSOES = 2), retorna 0 ou 1.
    return rand() % totalMissoes;
}

/*
 * faseDeAtaque(): Gerencia a interface para a ação de ataque,
 * solicitando ao jogador os territórios de origem e destino.
 */
void faseDeAtaque(struct Territorio *mapa, int total, const char *corJogador) {
    int idAtacante, idDefensor;
    
    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Escolha seu territorio de ataque (1-%d): ", total);
    scanf("%d", &idAtacante);
    limparBufferEntrada();
    
    printf("Escolha o territorio alvo (1-%d): ", total);
    scanf("%d", &idDefensor);
    limparBufferEntrada();
    
    // Converte o ID (1-5) para o índice do vetor (0-4)
    idAtacante--;
    idDefensor--;
    
    // --- Bloco de Validações ---
    // Verifica se os IDs são válidos
    if (idAtacante < 0 || idAtacante >= total || idDefensor < 0 || idDefensor >= total) {
        printf("\nERRO: IDs de territorio invalidos. Escolha entre 1 e %d.\n", total);
        return; // Sai da função de ataque
    }
    
    // Verifica se o jogador é dono do território de ataque
    // strcmp retorna 0 se as strings são IGUAIS.
    if (strcmp(mapa[idAtacante].corExercito, corJogador) != 0) {
        printf("\nERRO: Voce so pode atacar com seus proprios territorios (cor %s)!\n", corJogador);
        return;
    }
    
    // Verifica se o jogador não está atacando ele mesmo
    if (idAtacante == idDefensor || strcmp(mapa[idDefensor].corExercito, corJogador) == 0) {
        printf("\nERRO: Voce nao pode atacar um territorio que ja eh seu!\n");
        return;
    }
    
    // Verifica se o atacante tem tropas suficientes para atacar (precisa de > 1)
    if (mapa[idAtacante].tropas <= 1) {
        printf("\nERRO: Voce precisa de pelo menos 2 tropas para iniciar um ataque!\n");
        return;
    }
    
    // --- Fim das Validações ---
    
    printf("\nATAQUE IMINENTE: %s (%s) ataca %s (%s)!\n", 
           mapa[idAtacante].nome, mapa[idAtacante].corExercito, 
           mapa[idDefensor].nome, mapa[idDefensor].corExercito);

    // Passa os endereços das structs para a função de simulação
    simularAtaque(&mapa[idAtacante], &mapa[idDefensor]);
}

/*
 * simularAtaque(): Executa a lógica da batalha (rolagem de dados)
 * e atualiza o mapa (passado por referência).
 */
void simularAtaque(struct Territorio *atacante, struct Territorio *defensor) {
    // Lógica de dados simples: 1 dado para cada
    int dadoAtaque = (rand() % 6) + 1; // Número aleatório de 1 a 6
    int dadoDefesa = (rand() % 6) + 1; // Número aleatório de 1 a 6

    printf("... Rolando dados ...\n");
    printf("  > Ataque rolou: %d\n", dadoAtaque);
    printf("  > Defesa rolou: %d\n", dadoDefesa);

    // O Nível Aventureiro diz: "Empates favorecem o atacante"
    if (dadoAtaque >= dadoDefesa) {
        printf("\nRESULTADO: VITORIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        defensor->tropas--; // Modifica a tropa do defensor DIRETAMENTE via ponteiro

        // Verificar se o território foi conquistado
        if (defensor->tropas <= 0) {
            printf("!!! TERRITORIO CONQUISTADO !!!\n");
            printf("%s agora pertence ao exercito %s!\n", defensor->nome, atacante->corExercito);
            
            // 1. Mudar a cor do dono
            strncpy(defensor->corExercito, atacante->corExercito, MAX_COR);
            
            // 2. Mover tropas
            defensor->tropas = 1; // O território conquistado fica com 1 tropa
            atacante->tropas--;   // A tropa que se moveu do território atacante
        }
    } else {
        printf("\nRESULTADO: VITORIA DA DEFESA! O atacante perdeu 1 tropa.\n");
        atacante->tropas--; // Modifica a tropa do atacante DIRETAMENTE via ponteiro
    }
}

/*
 * verificarVitoria(): Verifica se o jogador cumpriu sua missão.
 * Usa 'const' pois apenas LÊ os dados.
 */
int verificarVitoria(const struct Territorio *mapa, int total, int idMissao, const char *corJogador) {
    int i;
    int territoriosDoJogador = 0;
    int tropasInimigasVerdes = 0; // Específico para a missão 0

    // Loop para coletar estatísticas do mapa
    for (i = 0; i < total; i++) {
        // 1. Contar quantos territórios o jogador possui
        if (strcmp(mapa[i].corExercito, corJogador) == 0) {
            territoriosDoJogador++;
        }
        
        // 2. Contar quantas tropas o exército "Verde" ainda possui
        if (strcmp(mapa[i].corExercito, "Verde") == 0) {
            // Garante que não estamos contando nossas próprias tropas
            if (strcmp(corJogador, "Verde") != 0) {
                tropasInimigasVerdes += mapa[i].tropas;
            }
        }
    }

    // Lógica de verificação da missão
    switch (idMissao) {
        case 0: // Missão: Destruir o exército Verde
            // Se o jogador FOR o Verde, ele não pode ter essa missão (ou não pode vencer)
            if (strcmp(corJogador, "Verde") == 0) {
                return 0; // Não pode vencer
            }
            // A condição de vitória é não haver mais tropas Verdes no mapa
            return (tropasInimigasVerdes == 0);

        case 1: // Missão: Conquistar 3 territórios
            // A condição de vitória é ter 3 ou mais territórios
            return (territoriosDoJogador >= 3);
            
        default:
            return 0; // Missão desconhecida
    }
}