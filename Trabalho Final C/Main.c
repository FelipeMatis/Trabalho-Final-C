#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REGISTROS 1000

typedef struct {
    int ano;
    int mes;
    int dia;
    int temp_min, temp_max, temp_media;
    char precipitacao[50];
    int umidade;
    float velocidade_vento;
} Analise;

Analise registros[MAX_REGISTROS];
int total_registros = 0;

void salvarCadastroNoArquivo(const char *nomeArquivo, Analise A) {
    FILE *arquivo = fopen(nomeArquivo, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar o cadastro!\n");
        return;
    }
    fprintf(arquivo, "Cadastro - Data: %04d-%02d-%02d\nTemperatura Minima: %d\nTemperatura Maxima: %d\nTemperatura Media: %d\nPrecipitacao: %s\nUmidade do Ar: %d\nVelocidade do Vento: %.2f\n\n",
            A.ano, A.mes, A.dia, A.temp_min, A.temp_max, A.temp_media, A.precipitacao, A.umidade, A.velocidade_vento);
    fclose(arquivo);
}

void calcularMediaMensal(const char *nomeArquivo) {
    if (total_registros == 0) {
        printf("Nenhum dado cadastrado para calcular medias.\n");
        return;
    }

    int somaTemp[12] = {0}, contador[12] = {0};
    char relatorio[5000] = "Relatorio de Media Mensal de Temperaturas:\n";

    for (int i = 0; i < total_registros; i++) {
        int mes = registros[i].mes - 1; // Ajustar índice do mês (0-11)
        somaTemp[mes] += registros[i].temp_media;
        contador[mes]++;
    }

    for (int i = 0; i < 12; i++) {
        if (contador[i] > 0) {
            char linha[100];
            int media = somaTemp[i] / contador[i];
            sprintf(linha, "Mes %02d: Media de Temperatura: %d°C\n", i + 1, media);
            strcat(relatorio, linha);
        }
    }

    printf("%s", relatorio); // Exibir o relatório na tela

    FILE *arquivo = fopen(nomeArquivo, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar o relatorio!\n");
        return;
    }
    fprintf(arquivo, "%s\n", relatorio);
    fclose(arquivo);

    printf("Relatorio de media mensal salvo no arquivo '%s'.\n", nomeArquivo);
}

void identificarExtremos(const char *nomeArquivo) {
    if (total_registros == 0) {
        printf("Nenhum dado cadastrado para identificar extremos.\n");
        return;
    }

    int maiorTemp = -1000, menorTemp = 1000;
    char diaMaiorTemp[20], diaMenorTemp[20];
    char relatorio[5000] = "Relatorio de Dias com Extremos:\n";

    for (int i = 0; i < total_registros; i++) {
        if (registros[i].temp_max > maiorTemp) {
            maiorTemp = registros[i].temp_max;
            sprintf(diaMaiorTemp, "%04d-%02d-%02d", registros[i].ano, registros[i].mes, registros[i].dia);
        }
        if (registros[i].temp_min < menorTemp) {
            menorTemp = registros[i].temp_min;
            sprintf(diaMenorTemp, "%04d-%02d-%02d", registros[i].ano, registros[i].mes, registros[i].dia);
        }
    }

    sprintf(relatorio + strlen(relatorio),
            "Dia mais quente: %s com %d°C\nDia mais frio: %s com %d°C\n",
            diaMaiorTemp, maiorTemp, diaMenorTemp, menorTemp);

    for (int i = 0; i < total_registros; i++) {
        if (strcmp(registros[i].precipitacao, "Chuva Intensa") == 0) {
            char linha[100];
            sprintf(linha, "Chuva Intensa registrada em: %04d-%02d-%02d\n",
                    registros[i].ano, registros[i].mes, registros[i].dia);
            strcat(relatorio, linha);
        }
    }

    printf("%s", relatorio); // Exibir o relatório na tela

    FILE *arquivo = fopen(nomeArquivo, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar o relatorio!\n");
        return;
    }
    fprintf(arquivo, "%s\n", relatorio);
    fclose(arquivo);

    printf("Relatorio de extremos salvo no arquivo '%s'.\n", nomeArquivo);
}

void gerarRelatorioAnual(const char *nomeArquivo) {
    if (total_registros == 0) {
        printf("Nenhum dado cadastrado para gerar relatorio anual.\n");
        return;
    }

    int somaTemp[2025] = {0}, contador[2025] = {0};
    char relatorio[5000] = "Relatorio Anual de Temperaturas:\n";

    for (int i = 0; i < total_registros; i++) {
        int ano = registros[i].ano;
        somaTemp[ano] += registros[i].temp_media;
        contador[ano]++;
    }

    int tempAnterior = -1;
    for (int i = 0; i < 2025; i++) {
        if (contador[i] > 0) {
            char linha[100];
            int media = somaTemp[i] / contador[i];
            sprintf(linha, "Ano: %04d\nMedia de Temperatura: %dgraus\n", i, media);
            strcat(relatorio, linha);

            if (tempAnterior != -1) {
                if (media > tempAnterior) {
                    strcat(relatorio, "Tendencia: Aumento de temperatura em relacao ao ano anterior.\n");
                } else if (media < tempAnterior) {
                    strcat(relatorio, "Tendencia: Reducao de temperatura em relacao ao ano anterior.\n");
                } else {
                    strcat(relatorio, "Tendencia: Temperatura estavel em relacao ao ano anterior.\n");
                }
            }

            tempAnterior = media;
        }
    }

    printf("%s", relatorio); // Exibir o relatório na tela

    FILE *arquivo = fopen(nomeArquivo, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar o relatorio!\n");
        return;
    }
    fprintf(arquivo, "%s\n", relatorio);
    fclose(arquivo);

    printf("Relatorio anual salvo no arquivo '%s'.\n", nomeArquivo);
}

void cadastrarDados(const char *nomeArquivo) {
    if (total_registros >= MAX_REGISTROS) {
        printf("Limite de registros atingido!\n");
        return;
    }

    Analise A;
    int opcao2 = 0;

    printf("Informe o ano (YYYY): ");
    scanf("%d", &A.ano);
    if (A.ano > 2025 || A.ano < 1) {
        printf("Ano invalido!\n");
        return;
    }

    printf("Informe o mes (MM): ");
    scanf("%d", &A.mes);
    if (A.mes < 1 || A.mes > 12) {
        printf("Mes invalido!\n");
        return;
    }

    printf("Informe o dia (DD): ");
    scanf("%d", &A.dia);
    if ((A.dia < 1) || 
        (A.mes == 2 && A.dia > 28) ||
        ((A.mes == 4 || A.mes == 6 || A.mes == 9 || A.mes == 11) && A.dia > 30) ||
        (A.dia > 31)) {
        printf("Dia invalido!\n");
        return;
    }

    printf("Informe a Temperatura Maxima: ");
    scanf("%d", &A.temp_max);
    if(A.temp_max>50){
        printf("Temperatura muito alta!\n");
        return;
    }

    printf("Informe a Temperatura Minima: ");
    scanf("%d", &A.temp_min);
    if(A.temp_min < -14){
        printf("O Brasil nao chega a essa temperatura.\n");
        return;
    }

    A.temp_media = (A.temp_min + A.temp_max) / 2;

    printf("Informe a Umidade do Ar (%%): ");
    scanf("%d", &A.umidade);
    if(A.umidade>100 || A.umidade<0){
        printf("Erro! Umidade invalida.\n");
        return;
    }

    printf("Escolha o tipo de precipitacao:\n1- Tempo Normal\n2- Garoa\n3- Chuva\n4- Chuva Intensa\n5- Neve\n6- Granizo\n7- Geada\n");
    scanf("%d", &opcao2);

    const char *tipos_precipitacao[] = {
        "Tempo Normal", "Garoa", "Chuva", "Chuva Intensa", "Neve", "Granizo", "Geada"
    };
    if (opcao2 >= 1 && opcao2 <= 7) {
        strcpy(A.precipitacao, tipos_precipitacao[opcao2 - 1]);
    } else {
        strcpy(A.precipitacao, "Nao especificado");
    }

    printf("Informe a Velocidade do Vento (km/h): ");
    scanf("%f", &A.velocidade_vento);
    if(A.velocidade_vento > 408 || A.velocidade_vento < 0) {
        printf("Erro! Velocidade do vento invalida.\n");
        return;
    }

    registros[total_registros++] = A;
    salvarCadastroNoArquivo(nomeArquivo, A);
    printf("Dados cadastrados com sucesso!\n");
}

int main() {
    int opcao;
    char nomeArquivo[] = "dados_climaticos.txt";

    do {
        printf("\n--- Sistema de Analise de Dados Climaticos ---\n");
        printf("1. Cadastrar dados climaticos\n");
        printf("2. Gerar relatorio de media mensal\n");
        printf("3. Gerar relatorio de extremos\n");
        printf("4. Gerar relatorio anual\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarDados(nomeArquivo);
                break;
            case 2:
                calcularMediaMensal(nomeArquivo);
                break;
            case 3:
                identificarExtremos(nomeArquivo);
                break;
            case 4:
                gerarRelatorioAnual(nomeArquivo);
                break;
            case 5:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}
