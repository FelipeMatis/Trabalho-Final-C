#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REGISTROS 1000

typedef struct {
    int ano;
    int mes;
    int dia;
    int temp_min, temp_max, temp_media;
    float precipitacao;
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
    fprintf(arquivo, "Cadastro - Data: %04d-%02d-%02d\nTemperatura Minima: %d\nTemperatura Maxima: %d\nTemperatura Media: %d\nPrecipitacao: %.2f mm\nUmidade do Ar: %d\nVelocidade do Vento: %.2f km/h\n\n",
            A.ano, A.mes, A.dia, A.temp_min, A.temp_max, A.temp_media, A.precipitacao, A.umidade, A.velocidade_vento);
    fclose(arquivo);
}

void calcularMediaMensal(const char *nomeArquivo) {
    if (total_registros == 0) {
        printf("Nenhum dado cadastrado para calcular medias.\n");
        return;
    }

    int somaTemp[12] = {0}, contador[12] = {0};
    float somaPrecipitacao[12] = {0}; // Adicionado para calcular a média de precipitação
    char relatorio[5000] = "Relatorio de Media Mensal:\n";

    for (int i = 0; i < total_registros; i++) {
        int mes = registros[i].mes - 1;
        somaTemp[mes] += registros[i].temp_media;
        somaPrecipitacao[mes] += registros[i].precipitacao; // Soma precipitação
        contador[mes]++;
    }

    for (int i = 0; i < 12; i++) {
        if (contador[i] > 0) {
            char linha[150];
            int mediaTemp = somaTemp[i] / contador[i];
            float mediaPrecip = somaPrecipitacao[i] / contador[i]; // Média de precipitação
            sprintf(linha, "Mes %02d: Media de Temperatura: %d°C, Media de Precipitacao: %.2f mm\n", 
                    i + 1, mediaTemp, mediaPrecip);
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


void identificarExtremos(const char *nomeArquivo) {
    if (total_registros == 0) {
        printf("Nenhum dado cadastrado para identificar extremos.\n");
        return;
    }

    int maiorTemp = -1000, menorTemp = 1000;
    float maiorPrecipitacao = -1.0, menorPrecipitacao = 1000.0;
    char diaMaiorTemp[20], diaMenorTemp[20];
    char diaMaiorPrecipitacao[20], diaMenorPrecipitacao[20];
    char relatorio[5000] = "Relatorio de Dias com Extremos:\n";

    for (int i = 0; i < total_registros; i++) {
        // Identificando os extremos de temperatura
        if (registros[i].temp_max > maiorTemp) {
            maiorTemp = registros[i].temp_max;
            sprintf(diaMaiorTemp, "%04d-%02d-%02d", registros[i].ano, registros[i].mes, registros[i].dia);
        }
        if (registros[i].temp_min < menorTemp) {
            menorTemp = registros[i].temp_min;
            sprintf(diaMenorTemp, "%04d-%02d-%02d", registros[i].ano, registros[i].mes, registros[i].dia);
        }

        // Identificando os extremos de precipitação
        if (registros[i].precipitacao > maiorPrecipitacao) {
            maiorPrecipitacao = registros[i].precipitacao;
            sprintf(diaMaiorPrecipitacao, "%04d-%02d-%02d", registros[i].ano, registros[i].mes, registros[i].dia);
        }
        if (registros[i].precipitacao < menorPrecipitacao) {
            menorPrecipitacao = registros[i].precipitacao;
            sprintf(diaMenorPrecipitacao, "%04d-%02d-%02d", registros[i].ano, registros[i].mes, registros[i].dia);
        }
    }
// Adicionando os extremos ao relatório
    sprintf(relatorio + strlen(relatorio),
            "Dia mais quente: %s com %d°C\nDia mais frio: %s com %d°C\n",
            diaMaiorTemp, maiorTemp, diaMenorTemp, menorTemp);

    sprintf(relatorio + strlen(relatorio),
            "Dia com maior precipitação: %s com %.2f mm\nDia com menor precipitação: %s com %.2f mm\n",
            diaMaiorPrecipitacao, maiorPrecipitacao, diaMenorPrecipitacao, menorPrecipitacao);

    // Verificando dias com chuva intensa
    for (int i = 0; i < total_registros; i++) {
        if (registros[i].precipitacao > 50.0) {
            char linha[100];
            sprintf(linha, "Chuva Intensa registrada em: %04d-%02d-%02d\n",
                    registros[i].ano, registros[i].mes, registros[i].dia);
            strcat(relatorio, linha);
        }
    }

    printf("%s", relatorio);

    FILE *arquivo = fopen(nomeArquivo, "a"); // Modo append para não apagar dados
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar o relatorio!\n");
        return;
    }
    fprintf(arquivo, "%s\n", relatorio);
    fclose(arquivo);
}



void gerarRelatorioAnual(const char *nomeArquivo) {
    if (total_registros == 0) {
        printf("Nenhum dado cadastrado para gerar relatorio anual.\n");
        return;
    }

    int somaTemp[3000] = {0}, contador[3000] = {0}; 
    float somaPrecipitacao[3000] = {0}; // Para calcular a média de precipitação
    char relatorio[5000] = "Relatorio Anual de Temperaturas e Precipitacao:\n";

    for (int i = 0; i < total_registros; i++) {
        int ano = registros[i].ano;
        somaTemp[ano] += registros[i].temp_media;
        somaPrecipitacao[ano] += registros[i].precipitacao; // Soma precipitação do ano
        contador[ano]++;
    }

    int tempAnterior = -1;
    for (int i = 0; i < 3000; i++) {
        if (contador[i] > 0) {
            char linha[150];
            int mediaTemp = somaTemp[i] / contador[i];
            float mediaPrecip = somaPrecipitacao[i] / contador[i]; // Média de precipitação anual
            sprintf(linha, "Ano: %04d\nMedia de Temperatura: %d°C, Media de Precipitacao: %.2f mm\n", 
                    i, mediaTemp, mediaPrecip);
            strcat(relatorio, linha);

            if (tempAnterior != -1) {
                if (mediaTemp > tempAnterior) {
                    strcat(relatorio, "Tendencia: Aumento de temperatura.\n");
                } else if (mediaTemp < tempAnterior) {
                    strcat(relatorio, "Tendencia: Reducao de temperatura.\n");
                } else {
                    strcat(relatorio, "Tendencia: Temperatura estavel.\n");
                }
            }

            tempAnterior = mediaTemp;
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
    if ((A.dia < 1) || (A.dia > 31)) {
        printf("Dia invalido!\n");
        return;
    }

    printf("Informe a Temperatura Maxima: ");
    scanf("%d", &A.temp_max);
    printf("Informe a Temperatura Minima: ");
    scanf("%d", &A.temp_min);
    A.temp_media = (A.temp_min + A.temp_max) / 2;

    printf("Informe a Umidade do Ar (%%): ");
    scanf("%d", &A.umidade);

    printf("Digite a precipitacao em milimetros (mm): ");
    while (1) {
        if (scanf("%f", &A.precipitacao) == 1 && A.precipitacao >= 0) {
            break;
        } else {
            printf("Valor invalido! Digite um numero positivo para a precipitacao: ");
            while (getchar() != '\n');
        }
    }

    printf("Informe a Velocidade do Vento (km/h): ");
    scanf("%f", &A.velocidade_vento);

    registros[total_registros++] = A;
    salvarCadastroNoArquivo(nomeArquivo, A);
    printf("Dados cadastrados com sucesso!\n");
}

void detectarComportamentosAnormais(const char *nomeArquivo) {
    if (total_registros == 0) {
        printf("Nenhum dado cadastrado para identificar extremos.\n");
        return;
    }

    char relatorio[5000] = "Relatorio de Comportamentos Anormais:\n";
    
    int somaTempAno[3000] = {0}, contadorAno[3000] = {0};
    int anoMaisQuente = 0, anoMaisFrio = 0;
    float tempMediaMaisQuente = -1000.0, tempMediaMaisFria = 1000.0;

    for (int i = 0; i < total_registros; i++) {
        int ano = registros[i].ano;
        somaTempAno[ano] += registros[i].temp_media;
        contadorAno[ano]++;
    }

    for (int ano = 0; ano < 3000; ano++) {
        if (contadorAno[ano] > 0) {
            float mediaTempAno = (float)somaTempAno[ano] / contadorAno[ano];
            if (mediaTempAno > tempMediaMaisQuente) {
                tempMediaMaisQuente = mediaTempAno;
                anoMaisQuente = ano;
            }
            if (mediaTempAno < tempMediaMaisFria) {
                tempMediaMaisFria = mediaTempAno;
                anoMaisFrio = ano;
            }
        }
    }

    sprintf(relatorio + strlen(relatorio),
            "Ano mais quente: %d com média de %.2f°C\nAno mais frio: %d com média de %.2f°C\n",
            anoMaisQuente, tempMediaMaisQuente, anoMaisFrio, tempMediaMaisFria);

    for (int i = 0; i < total_registros; i++) {
        if (registros[i].precipitacao == 0.0) {
            char linha[100];
            sprintf(linha, "Periodo de seca registrado em: %04d-%02d-%02d\n",
                    registros[i].ano, registros[i].mes, registros[i].dia);
            strcat(relatorio, linha);
        }
        if (registros[i].precipitacao >= 50.0) {
            char linha[100];
            sprintf(linha, "Chuva Muito Forte registrada em: %04d-%02d-%02d com %.2f mm\n",
                    registros[i].ano, registros[i].mes, registros[i].dia, registros[i].precipitacao);
            strcat(relatorio, linha);
        }
        else if (registros[i].precipitacao >= 10.0) {
            char linha[100];
            sprintf(linha, "Chuva Forte registrada em: %04d-%02d-%02d com %.2f mm\n",
                    registros[i].ano, registros[i].mes, registros[i].dia, registros[i].precipitacao);
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
                printf("Novo ano: ");
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
                    (registros[encontrado].mes == 2 && registros[encontrado].dia > 28) ||
                    ((registros[encontrado].mes == 4 || registros[encontrado].mes == 6 || registros[encontrado].mes == 9 || registros[encontrado].mes == 11) && registros[encontrado].dia > 30) ||
                    (registros[encontrado].dia > 31)) {
                    printf("Dia invalido!\n");
                }
                break;
            case 4:
                printf("Nova temperatura maxima: ");
                scanf("%d", &registros[encontrado].temp_max);
                if (registros[encontrado].temp_max > 50) {
                    printf("Temperatura muito alta!\n");
                }
                break;
            case 5:
                printf("Nova temperatura minima: ");
                scanf("%d", &registros[encontrado].temp_min);
                if (registros[encontrado].temp_min < -14) {
                    printf("O Brasil nao chega a essa temperatura.\n");
                }
                break;
            case 6:
                printf("Nova umidade do ar: ");
                scanf("%d", &registros[encontrado].umidade);
                if (registros[encontrado].umidade > 100 || registros[encontrado].umidade < 0) {
                    printf("Erro! Umidade invalida.\n");
                }
                break;
            case 7:
                printf("Nova precipitacao (em mm): ");
                scanf("%f", &registros[encontrado].precipitacao);
                if (registros[encontrado].precipitacao < 0) {
                    printf("Erro! Precipitacao invalida.\n");
                }
                break;
            case 8:
                printf("Nova velocidade do vento (km/h): ");
                scanf("%f", &registros[encontrado].velocidade_vento);
                if (registros[encontrado].velocidade_vento > 408 || registros[encontrado].velocidade_vento < 0) {
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
        fprintf(arquivo, "Cadastro - Data: %04d-%02d-%02d\nTemperatura Minima: %d\nTemperatura Maxima: %d\nTemperatura Media: %d\nPrecipitacao: %.2f mm\nUmidade do Ar: %d\nVelocidade do Vento: %.2f km/h\n\n",
                registros[i].ano, registros[i].mes, registros[i].dia, registros[i].temp_min, registros[i].temp_max,
                registros[i].temp_media, registros[i].precipitacao, registros[i].umidade, registros[i].velocidade_vento);
    }
    fclose(arquivo);

    printf("Dados alterados com sucesso e atualizados no arquivo.\n");
}

int removerRegistroPorData(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para leitura!\n");
        return 0;
    }

    struct Registro {
        int ano, mes, dia;
        float tempMin, tempMax, tempMedia;
        char precipitacao[50];
        float umidade;
        float velVento;
    };

    struct Registro registros[1000];
    int numRegistros = 0;
    int anoParaRemover, mesParaRemover, diaParaRemover;

    printf("Informe a data do registro que deseja remover (YYYY MM DD): ");
    scanf("%d %d %d", &anoParaRemover, &mesParaRemover, &diaParaRemover);

    char buffer[256];
    int lendoRegistro = 0;
    while (fgets(buffer, sizeof(buffer), arquivo)) {
        if (strstr(buffer, "Cadastro - Data:") != NULL) {
            int ano, mes, dia;
            sscanf(buffer, "Cadastro - Data: %d-%d-%d", &ano, &mes, &dia);
            
            if (ano != anoParaRemover || mes != mesParaRemover || dia != diaParaRemover) {
                registros[numRegistros].ano = ano;
                registros[numRegistros].mes = mes;
                registros[numRegistros].dia = dia;
                lendoRegistro = 1;
            } else {
                lendoRegistro = 0;
                continue;
            }
        }
        
        if (lendoRegistro) {
            if (strstr(buffer, "Temperatura Minima:") != NULL)
                sscanf(buffer, "Temperatura Minima: %f", &registros[numRegistros].tempMin);
            else if (strstr(buffer, "Temperatura Maxima:") != NULL)
                sscanf(buffer, "Temperatura Maxima: %f", &registros[numRegistros].tempMax);
            else if (strstr(buffer, "Temperatura Media:") != NULL) {
                sscanf(buffer, "Temperatura Media: %f", &registros[numRegistros].tempMedia);
                numRegistros++;
            }
        }
    }
    fclose(arquivo);

    FILE *novoArquivo = fopen("temp.txt", "w");
    if (!novoArquivo) {
        printf("Erro ao criar arquivo temporário!\n");
        return 0;
    }

    for (int i = 0; i < numRegistros; i++) {
        fprintf(novoArquivo, "Cadastro - Data: %04d-%02d-%02d\n", 
                registros[i].ano, registros[i].mes, registros[i].dia);
        fprintf(novoArquivo, "Temperatura Minima: %.2f\n", registros[i].tempMin);
        fprintf(novoArquivo, "Temperatura Maxima: %.2f\n", registros[i].tempMax);
        fprintf(novoArquivo, "Temperatura Media: %.2f\n\n", registros[i].tempMedia);
    }
    
    fprintf(novoArquivo, "=== Relatórios ===\n\n");
    float mediaMensal[12] = {0};
    int contMensal[12] = {0};
    
    for (int i = 0; i < numRegistros; i++) {
        mediaMensal[registros[i].mes - 1] += registros[i].tempMedia;
        contMensal[registros[i].mes - 1]++;
    }
    for (int i = 0; i < 12; i++) {
        if (contMensal[i] > 0) {
            fprintf(novoArquivo, "Mes %d: Media de Temperatura: %.1f°C\n", 
                    i + 1, mediaMensal[i] / contMensal[i]);
        }
    }
    fprintf(novoArquivo, "\n");

    fclose(novoArquivo);
    remove(nomeArquivo);
    rename("temp.txt", nomeArquivo);

    printf("Registro de %04d-%02d-%02d removido e relatorios atualizados com sucesso!\n", 
           anoParaRemover, mesParaRemover, diaParaRemover);
    return 1;
}

int main() {
    int opcao;
    char nomeArquivo[] = "dados_climaticos.txt";

    // Ler os dados do arquivo antes de começar
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo != NULL) {
        while (fscanf(arquivo, "Cadastro - Data: %d-%d-%d\n", 
                      &registros[total_registros].ano, 
                      &registros[total_registros].mes, 
                      &registros[total_registros].dia) == 3) {

            fscanf(arquivo, "Temperatura Minima: %d\n", &registros[total_registros].temp_min);
            fscanf(arquivo, "Temperatura Maxima: %d\n", &registros[total_registros].temp_max);
            fscanf(arquivo, "Temperatura Media: %d\n", &registros[total_registros].temp_media);
            fscanf(arquivo, "Precipitacao: %f mm\n", &registros[total_registros].precipitacao);
            fscanf(arquivo, "Umidade do Ar: %d\n", &registros[total_registros].umidade);
            fscanf(arquivo, "Velocidade do Vento: %f km/h\n\n", &registros[total_registros].velocidade_vento);

            total_registros++;
            if (total_registros >= MAX_REGISTROS) {
                printf("Aviso: Limite de registros atingido!\n");
                break;
            }
        }
        fclose(arquivo);
    } else {
        printf("Nenhum arquivo encontrado. Começando com banco de dados vazio.\n");
    }

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
