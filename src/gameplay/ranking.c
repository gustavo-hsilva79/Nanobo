// Top 10 (#34, #77-#81): ranking em memoria e o arquivo NANOBO-RANKING 1;
// saiu de gameplay.c em 27/11 porque muda por outro motivo (persistencia)
#include "gameplay.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Top 10 (#77): cabecalho de versao, o recorde antigo e o nome padrao
#define RANKING_HEADER "NANOBO-RANKING 1"
#define LEGACY_HI_SCORE_FILE "nanobo_hiscore.txt"
#define RANKING_DEFAULT_NAME "Jogador"

// Top 10 em memoria e o caminho do arquivo: na pasta do jogo, relativo como
// os assets, por decisao do dono em 15/11 (#34, #77)
static RankingEntry ranking[RANKING_SIZE];
static int ranking_len = 0;
static char ranking_path[512] = "nanobo_ranking.txt";

// maior pontuacao do ranking, mostrada no menu (#34)
int hi_score()
{
    return ranking_len > 0 ? ranking[0].score : 0;
}

void ranking_set_path(const char* path)
{
    if (path != NULL && path[0] != '\0')
    {
        snprintf(ranking_path, sizeof(ranking_path), "%s", path);
    }
}

int ranking_count(void)
{
    return ranking_len;
}

const RankingEntry* ranking_entry(int position)
{
    if (position < 0 || position >= ranking_len)
    {
        return NULL;
    }

    return &ranking[position];
}

// copia o nome ate RANKING_NAME_CHARS caracteres UTF-8, sem caractere de
// controle nem espaco nas pontas; vazio vira o nome padrao (#81)
static void clean_name(const char* src, char* dst)
{
    size_t in = 0;
    size_t out = 0;
    int chars = 0;

    while (src != NULL && src[in] == ' ')
    {
        in++;
    }

    while (src != NULL && src[in] != '\0' && src[in] != '\n' && src[in] != '\r')
    {
        unsigned char c = (unsigned char)src[in];
        size_t len = 1;
        size_t k;

        // controle e byte solto de continuacao ficam de fora
        if (c < 32 || c == 127 || (c >= 0x80 && c < 0xC0))
        {
            in++;
            continue;
        }

        if (c >= 0xF0)
        {
            len = 4;
        }
        else if (c >= 0xE0)
        {
            len = 3;
        }
        else if (c >= 0xC0)
        {
            len = 2;
        }

        if (chars >= RANKING_NAME_CHARS || out + len >= (size_t)RANKING_NAME_BYTES)
        {
            break;
        }

        for (k = 0; k < len && src[in + k] != '\0'; k++)
        {
            dst[out + k] = src[in + k];
        }

        // caractere cortado no fim da linha: nao entra
        if (k < len)
        {
            break;
        }

        out = out + len;
        in = in + len;
        chars = chars + 1;
    }

    while (out > 0 && dst[out - 1] == ' ')
    {
        out--;
    }

    dst[out] = '\0';

    if (out == 0)
    {
        snprintf(dst, RANKING_NAME_BYTES, "%s", RANKING_DEFAULT_NAME);
    }
}

bool ranking_qualifies(int value)
{
    if (value < 0)
    {
        return false;
    }

    if (ranking_len < RANKING_SIZE)
    {
        return true;
    }

    // empatar com o ultimo nao basta: o registro mais antigo fica
    return value > ranking[RANKING_SIZE - 1].score;
}

int ranking_insert(int value, const char* name)
{
    int pos = 0;
    int i;

    if (!ranking_qualifies(value))
    {
        return -1;
    }

    // empate: o novo fica depois de todos os iguais (#34)
    while (pos < ranking_len && ranking[pos].score >= value)
    {
        pos++;
    }

    // desce quem esta abaixo; com o ranking cheio, o 11o cai fora
    i = ranking_len < RANKING_SIZE ? ranking_len : RANKING_SIZE - 1;

    for (; i > pos; i--)
    {
        ranking[i] = ranking[i - 1];
    }

    ranking[pos].score = value;
    clean_name(name, ranking[pos].name);

    if (ranking_len < RANKING_SIZE)
    {
        ranking_len = ranking_len + 1;
    }

    return pos;
}

// o recorde antigo (um inteiro em nanobo_hiscore.txt) vira o primeiro
// registro, para a troca nao apagar o que ja foi conquistado (#78)
static void load_legacy_record(void)
{
    FILE* file = NULL;
    int value = 0;

    if (fopen_s(&file, LEGACY_HI_SCORE_FILE, "r") != 0 || file == NULL)
    {
        return;
    }

    if (fscanf_s(file, "%d", &value) == 1 && value > 0)
    {
        ranking_insert(value, "Recorde");
    }

    fclose(file);
}

// abre o ranking; sem o arquivo, tenta a copia .bak que a gravacao
// interrompida entre as duas trocas de nome deixa para tras
static FILE* open_ranking_file(void)
{
    FILE* file = NULL;
    char backup[sizeof(ranking_path) + 8];

    if (fopen_s(&file, ranking_path, "r") == 0 && file != NULL)
    {
        return file;
    }

    snprintf(backup, sizeof(backup), "%s.bak", ranking_path);

    if (fopen_s(&file, backup, "r") == 0 && file != NULL)
    {
        return file;
    }

    return NULL;
}

void load_hi_score()
{
    FILE* file = NULL;
    char line[128];

    ranking_len = 0;
    file = open_ranking_file();

    if (file == NULL)
    {
        load_legacy_record();
        return;
    }

    // sem o cabecalho o arquivo e de outro formato: vale ranking vazio
    if (fgets(line, sizeof(line), file) == NULL
        || strncmp(line, RANKING_HEADER, strlen(RANKING_HEADER)) != 0)
    {
        fclose(file);
        return;
    }

    while (ranking_len < RANKING_SIZE && fgets(line, sizeof(line), file) != NULL)
    {
        char* rest = NULL;
        long value = strtol(line, &rest, 10);

        // sem numero, numero fora da faixa ou sem o espaco antes do nome: ignorada
        if (rest == line || value < 0 || value > 9999999 || *rest != ' ')
        {
            continue;
        }

        // a insercao ordena e, no empate, mantem a ordem do arquivo
        ranking_insert((int)value, rest + 1);
    }

    fclose(file);
}

bool ranking_save(void)
{
    FILE* file = NULL;
    char temp[sizeof(ranking_path) + 8];
    char backup[sizeof(ranking_path) + 8];
    int i;
    bool ok;
    bool had_old;

    snprintf(temp, sizeof(temp), "%s.tmp", ranking_path);
    snprintf(backup, sizeof(backup), "%s.bak", ranking_path);

    if (fopen_s(&file, temp, "w") != 0 || file == NULL)
    {
        return false;
    }

    ok = fprintf(file, "%s\n", RANKING_HEADER) > 0;

    for (i = 0; i < ranking_len && ok; i++)
    {
        ok = fprintf(file, "%d %s\n", ranking[i].score, ranking[i].name) > 0;
    }

    ok = fclose(file) == 0 && ok;

    // o arquivo antigo so sai depois que o novo ficou inteiro
    if (!ok)
    {
        remove(temp);
        return false;
    }

    // o antigo vira .bak antes da troca: nenhum passo deixa o ranking sem copia;
    // sem o arquivo principal o .bak pode ser a unica copia e so sai no fim
    had_old = false;

    if (fopen_s(&file, ranking_path, "r") == 0 && file != NULL)
    {
        fclose(file);
        remove(backup);
        had_old = rename(ranking_path, backup) == 0;
    }

    if (rename(temp, ranking_path) != 0)
    {
        // a troca falhou: o antigo volta para o lugar e o novo e descartado
        if (had_old)
        {
            rename(backup, ranking_path);
        }

        remove(temp);
        return false;
    }

    remove(backup);
    return true;
}
