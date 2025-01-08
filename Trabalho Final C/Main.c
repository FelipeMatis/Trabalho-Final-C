#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_REGISTROS 1000 // Limite máximo de registros que o programa pode armazenar

typedef struct {
    char data[11];
    int temp_min, temp_max, temp_media;
    float precipitacao_acumulada;
    int umidade;
    float velocidade_vento;
} Analise;

Analise registros[MAX_REGISTROS]; // Array para armazenar os dados
int total_registros = 0;          // Contador de registros cadastrados

void salvarDadosNoArquivo(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    for (int i = 0; i < total_registros; i++) {
        fprintf(arquivo, "%s,%d,%d,%d,%.2f,%d,%.2f\n",
                registros[i].data,
                registros[i].temp_min,
                registros[i].temp_max,
                registros[i].temp_media,
                registros[i].precipitacao_acumulada,
                registros[i].umidade,
                registros[i].velocidade_vento);
    }

    fclose(arquivo);
    printf("Dados salvos no arquivo '%s' com sucesso!\n", nomeArquivo);
}

void cadastrarDados() {
    if (total_registros >= MAX_REGISTROS) {
        printf("Limite de registros atingido!\n");
        return;
    }

    Analise A;
    printf("Informe a data (YYYY-MM-DD): ");
    scanf("%s", A.data);

    printf("Informe a Temperatura Maxima: ");
    scanf("%d", &A.temp_max);

    printf("Informe a Temperatura Minima: ");
    scanf("%d", &A.temp_min);

    A.temp_media = (A.temp_min + A.temp_max) / 2;

    printf("Informe a Umidade do Ar: ");
    scanf("%d", &A.umidade);

    printf("Informe a Precipitacao Acumulada: ");
    scanf("%f", &A.precipitacao_acumulada);

    printf("Informe a Velocidade do Vento: ");
    scanf("%f", &A.velocidade_vento);

    registros[total_registros++] = A;
    printf("Dados cadastrados com sucesso!\n");
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    int opcao;
    char nomeArquivo[] = "dados_climaticos.csv";

    do {
        printf("\n--- Sistema de Analise de Dados Climaticos ---\n");
        printf("1. Cadastrar dados climaticos\n");
        printf("2. Salvar dados no arquivo\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarDados();
                break;
            case 2:
                salvarDadosNoArquivo(nomeArquivo);
                break;
            case 3:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 3);

    return 0;
}
