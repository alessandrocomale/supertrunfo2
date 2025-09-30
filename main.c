
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char estado[50];
    char codigo[20];
    char nomeCidade[50];
    int populacao;
    float area;
    float pib;
    int pontosTuristicos;
    float densidadeDemografica;
    float pibPerCapita;
} Carta;

/*  */
void lerLinha(char *buf, int tamanho) {
    while (1) {
        if (fgets(buf, tamanho, stdin) == NULL) {
            buf[0] = '\0';
            return;
        }
        /* remove newline */
        size_t ln = strlen(buf);
        if (ln > 0 && buf[ln-1] == '\n') buf[ln-1] = '\0';
        /* se vazia, pede novamente */
        if (strlen(buf) == 0) {
            printf("Entrada vazia. Digite novamente: ");
            continue;
        }
        break;
    }
}

/*  */
void lerString(const char *prompt, char *dest, int tamanho) {
    printf("%s", prompt);
    lerLinha(dest, tamanho);
}

/* */
int lerInt(const char *prompt) {
    char buf[128];
    char *endptr;
    long val;
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
        /* */
        size_t ln = strlen(buf);
        if (ln > 0 && buf[ln-1] == '\n') buf[ln-1] = '\0';
        val = strtol(buf, &endptr, 10);
        if (endptr == buf || *endptr != '\0') {
            printf("Valor invalido. Digite um inteiro valido.\n");
            continue;
        }
        return (int) val;
    }
}

/* Lê float com validação */
float lerFloat(const char *prompt) {
    char buf[128];
    char *endptr;
    float val;
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 0.0f;
        size_t ln = strlen(buf);
        if (ln > 0 && buf[ln-1] == '\n') buf[ln-1] = '\0';
        val = strtof(buf, &endptr);
        if (endptr == buf || *endptr != '\0') {
            printf("Valor invalido. Digite um numero valido.\n");
            continue;
        }
        return val;
    }
}

void cadastrarCarta(Carta *c) {
    lerString("Digite o estado: ", c->estado, sizeof(c->estado));
    lerString("Digite o codigo da carta: ", c->codigo, sizeof(c->codigo));
    lerString("Digite o nome da cidade/pais: ", c->nomeCidade, sizeof(c->nomeCidade));
    c->populacao = lerInt("Digite a populacao (inteiro): ");
    c->area = lerFloat("Digite a area (km²): ");
    c->pib = lerFloat("Digite o PIB (em unidades, ex: 1234.56): ");
    c->pontosTuristicos = lerInt("Digite o numero de pontos turisticos (inteiro): ");

    /* protecoes contra divisao por zero */
    if (c->area == 0.0f) {
        c->densidadeDemografica = 0.0f;
    } else {
        c->densidadeDemografica = (float)c->populacao / c->area;
    }
    if (c->populacao == 0) {
        c->pibPerCapita = 0.0f;
    } else {
        c->pibPerCapita = c->pib / (float)c->populacao;
    }
}

/* */
const char* nomeAtributo(int a) {
    switch (a) {
        case 1: return "Populacao";
        case 2: return "Area";
        case 3: return "PIB";
        case 4: return "Pontos Turisticos";
        case 5: return "Densidade Demografica";
        case 6: return "PIB per Capita";
        default: return "Desconhecido";
    }
}

/* */
double obterValor(const Carta *c, int atributo) {
    switch (atributo) {
        case 1: return (double) c->populacao;
        case 2: return (double) c->area;
        case 3: return (double) c->pib;
        case 4: return (double) c->pontosTuristicos;
        case 5: return (double) c->densidadeDemografica;
        case 6: return (double) c->pibPerCapita;
        default: return 0.0;
    }
}

/*  */
int exibirMenu(int atributoJaEscolhido) {
    int opcao;
    while (1) {
        printf("\nEscolha um atributo para comparar:\n");
        if (atributoJaEscolhido != 1) printf("1 - Populacao\n");
        if (atributoJaEscolhido != 2) printf("2 - Area\n");
        if (atributoJaEscolhido != 3) printf("3 - PIB\n");
        if (atributoJaEscolhido != 4) printf("4 - Pontos Turisticos\n");
        if (atributoJaEscolhido != 5) printf("5 - Densidade Demografica\n");
        if (atributoJaEscolhido != 6) printf("6 - PIB per Capita\n");
        opcao = lerInt("Opcao: ");
        if (opcao < 1 || opcao > 6) {
            printf("Opcao invalida. Tente novamente.\n");
            continue;
        }
        if (opcao == atributoJaEscolhido) {
            printf("Voce ja escolheu esse atributo. Escolha outro.\n");
            continue;
        }
        return opcao;
    }
}


void compararCartas(const Carta *c1, const Carta *c2, int a1, int a2) {
    double v1a1 = obterValor(c1, a1);
    double v2a1 = obterValor(c2, a1);
    double v1a2 = obterValor(c1, a2);
    double v2a2 = obterValor(c2, a2);

    printf("\n--- Comparacao entre %s e %s ---\n", c1->nomeCidade, c2->nomeCidade);

    
    printf("\nAtributo 1: %s\n", nomeAtributo(a1));
    if (a1 == 1 || a1 == 4) { /* inteiros (populacao/pontos) */
        printf("%s: %d | %s: %d\n", c1->nomeCidade, c1->populacao, c2->nomeCidade, c2->populacao);
    } else {
        printf("%s: %.6g | %s: %.6g\n", c1->nomeCidade, v1a1, c2->nomeCidade, v2a1);
    }

   
    int vencedorA1 = 0; /* 0 empate, 1 carta1, 2 carta2 */
    if (a1 == 5) { /* densidade: menor vence */
        if (v1a1 < v2a1) vencedorA1 = 1;
        else {
            if (v1a1 > v2a1) vencedorA1 = 2;
            else vencedorA1 = 0;
        }
    } else {
        if (v1a1 > v2a1) vencedorA1 = 1;
        else {
            if (v1a1 < v2a1) vencedorA1 = 2;
            else vencedorA1 = 0;
        }
    }

    if (vencedorA1 == 1) printf("-> Vencedor do atributo 1: %s\n", c1->nomeCidade);
    else if (vencedorA1 == 2) printf("-> Vencedor do atributo 1: %s\n", c2->nomeCidade);
    else printf("-> Atributo 1: Empate!\n");

    /*  */
    printf("\nAtributo 2: %s\n", nomeAtributo(a2));
    if (a2 == 1 || a2 == 4) {
        printf("%s: %d | %s: %d\n", c1->nomeCidade, c1->populacao, c2->nomeCidade, c2->populacao);
    } else {
        printf("%s: %.6g | %s: %.6g\n", c1->nomeCidade, v1a2, c2->nomeCidade, v2a2);
    }

    
    int vencedorA2 = 0;
    if (a2 == 5) {
        if (v1a2 < v2a2) vencedorA2 = 1;
        else {
            if (v1a2 > v2a2) vencedorA2 = 2;
            else vencedorA2 = 0;
        }
    } else {
        if (v1a2 > v2a2) vencedorA2 = 1;
        else {
            if (v1a2 < v2a2) vencedorA2 = 2;
            else vencedorA2 = 0;
        }
    }

    if (vencedorA2 == 1) printf("-> Vencedor do atributo 2: %s\n", c1->nomeCidade);
    else if (vencedorA2 == 2) printf("-> Vencedor do atributo 2: %s\n", c2->nomeCidade);
    else printf("-> Atributo 2: Empate!\n");

    /* Para soma: inverter sinal dos valores de densidade para que menor densidade "valha mais" */
    double s1a1 = v1a1, s2a1 = v2a1, s1a2 = v1a2, s2a2 = v2a2;
    if (a1 == 5) { s1a1 = -s1a1; s2a1 = -s2a1; }
    if (a2 == 5) { s1a2 = -s1a2; s2a2 = -s2a2; }

    double soma1 = s1a1 + s1a2;
    double soma2 = s2a1 + s2a2;

    printf("\nSoma dos atributos (após ajustar densidade):\n");
    printf("%s: %.6g\n", c1->nomeCidade, soma1);
    printf("%s: %.6g\n", c2->nomeCidade, soma2);

    if (soma1 > soma2) printf("\nResultado final: %s venceu!\n", c1->nomeCidade);
    else if (soma2 > soma1) printf("\nResultado final: %s venceu!\n", c2->nomeCidade);
    else printf("\nResultado final: Empate!\n");
}

int main() {
    Carta carta1, carta2;

    printf("=== Cadastro da Carta 1 ===\n");
    cadastrarCarta(&carta1);

    printf("\n=== Cadastro da Carta 2 ===\n");
    cadastrarCarta(&carta2);

    printf("\nCartas cadastradas com sucesso!\n");

    int atributo1 = exibirMenu(0);
    int atributo2 = exibirMenu(atributo1);

    compararCartas(&carta1, &carta2, atributo1, atributo2);

    return 0;
}
