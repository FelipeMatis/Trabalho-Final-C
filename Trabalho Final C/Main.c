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
    FILE *arquivo = fopen(nomeArquivo, "a+");
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
        int mes = registros[i].mes - 1;
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

    printf("%s", relatorio);

    FILE *arquivo = fopen(nomeArquivo, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar o relatorio!\n");
        return;
    }
    fprintf(arquivo, "%s\n", relatorio);
    fclose(arquivo);

    printf("Relatorio de media mensal salvo no arquivo '%s'.\n", nomeArquivo);
}

void identificarExtremosRecursivo(int i, int *maiorTemp, int *menorTemp, char *diaMaiorTemp, char *diaMenorTemp) {
    if (i >= total_registros) {
        return;
    }

    if (registros[i].temp_max > *maiorTemp) {
        *maiorTemp = registros[i].temp_max;
        sprintf(diaMaiorTemp, "%04d-%02d-%02d", registros[i].ano, registros[i].mes, registros[i].dia);
    }
    if (registros[i].temp_min < *menorTemp) {
        *menorTemp = registros[i].temp_min;
        sprintf(diaMenorTemp, "%04d-%02d-%02d", registros[i].ano, registros[i].mes, registros[i].dia);
    }

    identificarExtremosRecursivo(i + 1, maiorTemp, menorTemp, diaMaiorTemp, diaMenorTemp);
}

void identificarExtremos(const char *nomeArquivo) {
    if (total_registros == 0) {
        printf("Nenhum dado cadastrado para identificar extremos.\n");
        return;
    }

    int maiorTemp = -1000, menorTemp = 1000;
    char diaMaiorTemp[20], diaMenorTemp[20];
    char relatorio[5000] = "Relatorio de Dias com Extremos:\n";

    identificarExtremosRecursivo(0, &maiorTemp, &menorTemp, diaMaiorTemp, diaMenorTemp);

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

    printf("%s", relatorio);

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

    printf("%s", relatorio);

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
void detectarComportamentosAnormais(const char *nomeArquivo) {
    if (total_registros == 0) {
        printf("Nenhum dado cadastrado para detectar comportamentos anormais.\n");
        return;
    }

    int somaTemp[12] = {0}, somaUmidade[12] = {0}, contador[12] = {0};
    int extremosChuva[12] = {0};
    char relatorio[5000] = "Relatorio de Comportamentos Anormais:\n";

    for (int i = 0; i < total_registros; i++) {
        int mes = registros[i].mes - 1;
        somaTemp[mes] += registros[i].temp_media;
        somaUmidade[mes] += registros[i].umidade;
        contador[mes]++;

        if (strcmp(registros[i].precipitacao, "Chuva Intensa") == 0 ||
            strcmp(registros[i].precipitacao, "Granizo") == 0) {
            extremosChuva[mes]++;
        }
    }


    for (int i = 0; i < total_registros; i++) {
        int mes = registros[i].mes - 1;

        if (contador[mes] > 0) {
            int mediaTemp = somaTemp[mes] / contador[mes];
            int mediaUmidade = somaUmidade[mes] / contador[mes];


            if (abs(registros[i].temp_media - mediaTemp) > 5) {
                char linha[200];
                sprintf(linha, "Temperatura anormal em %04d-%02d: Media %d, registrada %d\n",
                        registros[i].ano, registros[i].mes, mediaTemp, registros[i].temp_media);
                strcat(relatorio, linha);
            }


            if (abs(registros[i].umidade - mediaUmidade) > 10) {
                char linha[200];
                sprintf(linha, "Umidade anormal em %04d-%02d: Media %d%%, registrada %d%%\n",
                        registros[i].ano, registros[i].mes, mediaUmidade, registros[i].umidade);
                strcat(relatorio, linha);
            }
        }

        if (extremosChuva[mes] > (contador[mes] / 2)) {
            char linha[200];
            sprintf(linha, "Chuva excessiva detectada no mes %02d do ano %04d\n", registros[i].mes, registros[i].ano);
            strcat(relatorio, linha);
        }
    }


    FILE *arquivo = fopen(nomeArquivo, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar o relatorio!\n");
        return;
    }
    fprintf(arquivo, "%s\n", relatorio);
    fclose(arquivo);

    printf("Relatorio de comportamentos anormais salvo no arquivo '%s'.\n", nomeArquivo);
}

void ArrumarErro(const char *nomeArquivo) {
    if (total_registros == 0) {
        printf("Nenhum dado cadastrado para alterar.\n");
        return;
    }

    int ano, mes, dia;
    printf("Informe o ano (YYYY): ");
    scanf("%d", &ano);
    printf("Informe o mes (MM): ");
    scanf("%d", &mes);
    printf("Informe o dia (DD): ");
    scanf("%d", &dia);

    // Encontrar o registro correspondente
    int encontrado = -1;
    for (int i = 0; i < total_registros; i++) {
        if (registros[i].ano == ano && registros[i].mes == mes && registros[i].dia == dia) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Nenhum registro encontrado para essa data.\n");
        return;
    }

    int opcao;
    do {
        printf("\nQual informacao deseja alterar?\n");
        printf("1- Ano\n2- Mes\n3- Dia\n4- Temperatura Maxima\n5- Temperatura Minima\n");
        printf("6- Umidade do Ar\n7- Precipitacao\n8- Velocidade do Vento\n9- Sair\n");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Novo ano:");
                scanf("%d", &registros[encontrado].ano);
                if (registros[encontrado].ano > 2025 || registros[encontrado].ano < 1) {
                printf("Ano invalido!\n");
                }
                break;
            case 2:
                printf("Novo mes: ");
                scanf("%d", &registros[encontrado].mes);
                if (registros[encontrado].mes < 1 || registros[encontrado].mes > 12) {
                printf("Mes invalido!\n");
                }


                break;
            case 3:
                printf("Novo dia: ");
                scanf("%d", &registros[encontrado].dia);
                if ((registros[encontrado].dia < 1) ||
            (registros[encontrado].dia == 2 && registros[encontrado].dia > 28) ||
            ((registros[encontrado].mes == 4 || registros[encontrado].mes == 6 || registros[encontrado].mes == 9 || registros[encontrado].mes == 11) && registros[encontrado].dia > 30) ||
            (registros[encontrado].dia > 31)) {
            printf("Dia invalido!\n");
            }
                break;
            case 4:
                printf("Nova temperatura maxima: ");
                scanf("%d", &registros[encontrado].temp_max);
                if(registros[encontrado].temp_max>50){
                printf("Temperatura muito alta!\n");
                }
                break;
            case 5:
                printf("Nova temperatura minima: ");
                scanf("%d", &registros[encontrado].temp_min);
                if(registros[encontrado].temp_min < -14){
            printf("O Brasil nao chega a essa temperatura.\n");
            }
                break;
            case 6:
                printf("Nova umidade do ar: ");
                scanf("%d", &registros[encontrado].umidade);
                if(registros[encontrado].umidade>100 || registros[encontrado].umidade<0){
                printf("Erro! Umidade invalida.\n");
                }
                break;
            case 7:
                printf("Nova precipitacao: ");
                scanf(" %49[^\n]", registros[encontrado].precipitacao);
                break;
            case 8:
                printf("Nova velocidade do vento: ");
                scanf("%f", &registros[encontrado].velocidade_vento);
                if(registros[encontrado].velocidade_vento > 408 || registros[encontrado].velocidade_vento < 0) {
                printf("Erro! Velocidade do vento invalida.\n");
                }
                break;
            case 9:
                printf("Saindo da edicao.\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 9);

    FILE *arquivo = fopen(nomeArquivo, "w"); // Atualiza os dados no arquivo
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para atualizar os dados.\n");
        return;
    }
    for (int i = 0; i < total_registros; i++) {
        fprintf(arquivo, "Cadastro - Data: %04d-%02d-%02d\nTemperatura Minima: %d\nTemperatura Maxima: %d\nTemperatura Media: %d\nPrecipitacao: %s\nUmidade do Ar: %d\nVelocidade do Vento: %.2f\n\n",
                registros[i].ano, registros[i].mes, registros[i].dia, registros[i].temp_min, registros[i].temp_max,
                registros[i].temp_media, registros[i].precipitacao, registros[i].umidade, registros[i].velocidade_vento);
    }
    fclose(arquivo);

    printf("Dados alterados com sucesso e atualizados no arquivo.\n");
}

void removerRegistroPorData(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para leitura!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Erro ao criar arquivo temporário!\n");
        fclose(arquivo);
        return;
    }

    int ano, mes, dia;
    printf("Informe a data do registro que deseja remover (YYYY MM DD): ");
    scanf("%d %d %d", &ano, &mes, &dia);

    char buffer[256];
    int apagar = 0; // Flag para identificar o bloco a ser apagado

    while (fgets(buffer, sizeof(buffer), arquivo)) {
        // Se encontrar um novo registro, verifica se deve apagar ou não
        if (strstr(buffer, "Cadastro - Data:") != NULL) {
            int a, m, d;
            sscanf(buffer, "Cadastro - Data: %d-%d-%d", &a, &m, &d);

            if (a == ano && m == mes && d == dia) {
                apagar = 1; // Encontrou o registro, ativa a flag para removê-lo
                continue;    // Pula a escrita desse cabeçalho no arquivo temporário
            } else {
                apagar = 0;  // Encontrou um novo registro diferente, então para de apagar
            }
        }

        // Somente escreve no novo arquivo se a flag `apagar` não estiver ativada
        if (!apagar) {
            fputs(buffer, temp);
        }
    }

    fclose(arquivo);
    fclose(temp);

    remove(nomeArquivo);
    rename("temp.txt", nomeArquivo);

    printf("Registro de %04d-%02d-%02d removido com sucesso!\n", ano, mes, dia);
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
        printf("5. Gerar relatorio de comportamentos fora do padrao\n");
        printf("6. Arrumar informacoes\n");
        printf("7. Apagar Registro\n");
        printf("8. Sair\n");
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
                 detectarComportamentosAnormais(nomeArquivo);
                 break;
            case 6:
                ArrumarErro(nomeArquivo);
                break;
            case 7:
               removerRegistroPorData(nomeArquivo);
                break;
            case 8:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 8);

    return 0;
}
