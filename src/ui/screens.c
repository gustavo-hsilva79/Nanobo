// telas fora da fase: menu, abertura, pausa, como jogar, resultados, ranking, vitoria e game over
#define _USE_MATH_DEFINES
#include "ui.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------------
// menu inicial
// ---------------------------------------------------------------
static void draw_ranking_panel(float x, float y, float w, int highlight, bool in_menu);


void draw_menu()
{
    char record[48];
    int count;

    const UiButton* buttons = screen_buttons(&count);

    // tempo real: o menu nao tem match_time
    double now = al_get_time();

    /* Fundo: vaso sanguineo com hemacias deslizando para a esquerda */
    if (menu_background)
    {
        ui_bitmap(menu_background, 0.0f, 0.0f, 0);
    }

    if (menu_cells)
    {
        float offset = (float)fmod(now * 16.0, (double)SCREEN_W);

        ui_bitmap(menu_cells, -offset, 0.0f, 0);
        ui_bitmap(menu_cells, (float)SCREEN_W - offset, 0.0f, 0);
    }

    /* Titulo */
    if (title_bitmap)
    {
        int tw = al_get_bitmap_width(title_bitmap);
        int th = al_get_bitmap_height(title_bitmap);

        al_draw_scaled_bitmap(
            title_bitmap,
            0.0f,
            0.0f,
            (float)tw,
            (float)th,
            floorf(
                ((float)SCREEN_W -
                    (float)(tw * TITLE_SCALE)) / 2.0f
            ),
            52.0f,
            (float)(tw * TITLE_SCALE),
            (float)(th * TITLE_SCALE),
            0
        );

        // Nanobo parado, flutuando a direita do titulo
        if (nanobo)
        {
            float title_right = floorf(((float)SCREEN_W + (float)(tw * TITLE_SCALE)) / 2.0f);
            int frame = (int)(now / 0.22) % 4;
            float float_y = 4.0f * (float)sin(2.0 * ALLEGRO_PI * now / 2.0);

            ui_bitmap_region(nanobo, (float)(frame * PLAYER_FRAME),
                (float)(PLAYER_FACING_DOWN * PLAYER_FRAME),
                (float)PLAYER_FRAME, (float)PLAYER_FRAME,
                title_right + 48.0f, 52.0f + float_y, 0);
        }
    }

    ui_text(UI_DIM,
        SCREEN_W / 2,
        104,
        ALLEGRO_ALIGN_CENTRE,
        "DEFENDA O ORGANISMO"
    );

    /* =========================================
       BOTAO E INSTRUCOES
       ========================================= */

    draw_buttons(buttons, count);

    draw_marked_text_aligned(SCREEN_W / 2, BUTTON_Y + 72, UI_DIM, ALLEGRO_ALIGN_CENTRE,
        "[setas] escolhem · [enter] confirma");

    // o melhor do Top 10, com o nome (#34)
    if (ranking_count() > 0)
    {
        snprintf(record, sizeof(record), "recorde %d · %s", hi_score(), ranking_entry(0)->name);
    }
    else
    {
        snprintf(record, sizeof(record), "nenhum recorde ainda");
    }
    ui_text(UI_DIM, SCREEN_W / 2, BUTTON_Y + 104, ALLEGRO_ALIGN_CENTRE, record);

    draw_marked_text_aligned(SCREEN_W - HUD_MARGIN, 672, UI_DIM, ALLEGRO_ALIGN_RIGHT,
        "[F11] tela cheia");

    // painel do Top 10 por cima do menu, aberto pelo RECORDES (#197)
    if (ranking_panel_open)
    {
        al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)SCREEN_H,
            UI_VEIL(0.85f));
        // painel de 184 px: termina em 244 e o rodape fica acima dos botoes (y 261)
        draw_ranking_panel(354.0f, 120.0f, 572.0f, -1, true);
        draw_marked_text_aligned(SCREEN_W / 2, 500, UI_DIM, ALLEGRO_ALIGN_CENTRE, "[enter] ou [esc] fecha");
    }
}

static const DialogEntry opening_entries[OPENING_ENTRIES] =
{
    {
        "Hospital · ficha do paciente", 3,
        {
            {
                "Paciente: {Léo Martins Ferreira}, 16 anos.",
                "Chegou ao hospital com febre alta.",
                NULL
            },
            {
                "Exames: uma {infecção} que se espalha pelo sangue.",
                NULL,
                NULL
            },
            {
                "Conduta: testar um {nanorrobô} médico, ainda protótipo,",
                "dentro do corpo de Léo.",
                NULL
            }
        },
        false
    },
    {
        "Nanobô · Anotação 0", 3,
        {
            {
                "{Sistema ligado}! Minha memória está vazia.",
                "Minha missão: aprender a reconhecer as ameaças e eliminá-las.",
                NULL
            },
            {
                "Meu paciente é o {Léo}, de 16 anos, com febre alta.",
                "Estou dentro do sangue dele.",
                NULL
            },
            {
                "Vou descobrir o que está {atacando} o Léo.",
                "Anoto aqui tudo o que aprender, até os meus erros.",
                NULL
            }
        },
        true
    }
};

DialogState opening_dialog;
int opening_entry = 0;

// entrar em BRIEFING (menu ou Game Over) recomeca a abertura da ficha
void start_opening(void)
{
    opening_entry = 0;
    dialog_open_entry(&opening_dialog, &opening_entries[0]);
}

// enter, espaco ou clique: avanca; o fim da ultima entrada entra na Fase 1
void press_opening()
{
    dialog_press(&opening_dialog);

    if (opening_dialog.open)
    {
        return;
    }

    if (opening_entry + 1 < OPENING_ENTRIES)
    {
        opening_entry = opening_entry + 1;
        dialog_open_entry(&opening_dialog, &opening_entries[opening_entry]);
    }
    else
    {
        game_set_scene(GAME_SCENE_GAME);
    }
}

// fundo escuro, a caixa da entrada atual e o rodape (BR-04)
void draw_briefing()
{
    // o quarto do Léo atrás da ficha; o véu de 60% vem do draw_dialog
    if (hospital_opening)
    {
        ui_bitmap(hospital_opening, 0.0f, 0.0f, 0);
    }
    draw_dialog(&opening_entries[opening_entry], &opening_dialog);

    draw_marked_text_aligned(SCREEN_W / 2, 48, UI_DIM, ALLEGRO_ALIGN_CENTRE,
        "[enter] ou [clique] avança · [esc] pula");
}

// pausa: o jogo fica visivel e congelado atras do painel
void draw_pause()
{
    static const char* phase_names[] = {
        "Fase 1 · bactérias no sangue",
        "Fase 2 · vírus nas vias aéreas",
        "Fase 3 · fungos no pulmão"
    };
    // objetivo completo da fase atual: tres linhas por fase (PA-03)
    static const char* goals[][3] = {
        {
            "Elimine as bactérias do sangue do Léo e vá ao destino.",
            "<p:Gram+>: <wR:Ruptura>. <r:Gram->: <wD:Dissolução>. Resistente: <wS:Reserva>.",
            "Leia antes de disparar. Ler todos vale <g:+15 pontos>."
        },
        {
            "Elimine os vírus nas vias aéreas do Léo e vá ao destino.",
            "Vírus livre: <wN:Neutralização>. Célula infectada: <wT:Ataque T>.",
            "Leia a célula antes: o <wT:Ataque T> custa vitais do Léo."
        },
        {
            "Elimine os fungos no pulmão do Léo e vá ao destino.",
            "Antibiótico não tem alvo no fungo: use a <wF:Perfuração>.",
            "No bolor, mire na base da colônia, não na hifa."
        }
    };
    int count;
    const UiButton* buttons = screen_buttons(&count);
    int phase = current_phase() - 1;
    int i;

    if (phase < 0 || phase > 2)
    {
        phase = 0;
    }

    // veu mais escuro que o antigo: o painel maior precisa de contraste
    al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)SCREEN_H,
        UI_VEIL(210.0f / 255.0f));
    draw_panel(160.0f, 140.0f, 960.0f, 432.0f, UI_INK, UI_FRAME);
    ui_text(UI_TEXT, SCREEN_W / 2, 164, ALLEGRO_ALIGN_CENTRE, "PAUSA");
    draw_rule((float)SCREEN_W / 2, 192.0f, 400.0f, UI_FRAME);
    ui_text(UI_AMBER, SCREEN_W / 2, 208, ALLEGRO_ALIGN_CENTRE, phase_names[phase]);

    // texto de x 96 a 544: a maior linha tem 56 caracteres (448 px)
    for (i = 0; i < 3; i++)
    {
        draw_marked_text(192.0f, 248.0f + 32.0f * (float)i, UI_TEXT, goals[phase][i]);
    }

    draw_marked_text(192.0f, 360.0f, UI_DIM, "[WASD] mover · [clique] dispara · [botão direito] lê");
    draw_marked_text(192.0f, 392.0f, UI_DIM, "[X] defesa · [1] [2] [3] ou [roda]: arma");

    draw_buttons(buttons, count);
    draw_marked_text_aligned(SCREEN_W / 2, 528, UI_DIM, ALLEGRO_ALIGN_CENTRE, "[esc] continua");
}

// ---------------------------------------------------------------
// como jogar: 4 paginas, abre do menu e da pausa
// ---------------------------------------------------------------

#define HOWTO_PANEL_Y 208.0f
#define HOWTO_ICON_X  80.0f   // coluna de icone 96x96 entre x 48 e 208
#define HOWTO_TEXT_X  240.0f
#define HOWTO_LINES   6

// volta para a cena de onde o como jogar abriu (menu ou pausa)
void howto_return()
{
    game_set_scene(game_howto_origin());
}

// enter, espaco ou clique: avanca; na ultima pagina, volta
void howto_advance()
{
    if (howto_page >= HOWTO_PAGES - 1)
    {
        howto_return();
        return;
    }

    howto_page++;
}

// coluna de icone da pagina, em 96x96
static void draw_howto_icon(float x, float y)
{
    float row_y;
    int frame;

    if (howto_page == 0)
    {
        // o Nanobo de frente, quadro parado 0
        ui_bitmap_region(nanobo, 0.0f, (float)(PLAYER_FACING_DOWN * PLAYER_FRAME),
            (float)PLAYER_FRAME, (float)PLAYER_FRAME, x, y, 0);
    }
    else if (howto_page == 1)
    {
        // scanner x2 e os dois marcadores da ficha: camada 1 lida, camada 2 vazia
        al_draw_scaled_bitmap(scanner_icon, 0.0f, 0.0f, 16.0f, 16.0f, x + 16.0f, y, 64.0f, 64.0f, 0);
        al_draw_filled_rectangle(x + 30.0f, y + 76.0f, x + 30.0f + (float)CARD_MARK, y + 76.0f + (float)CARD_MARK, UI_AMBER);
        al_draw_filled_rectangle(x + 54.0f, y + 76.0f, x + 54.0f + (float)CARD_MARK, y + 76.0f + (float)CARD_MARK, UI_SLOT);
    }
    else if (howto_page == 2)
    {
        // as 6 armas: fase 1 em cima, fases 2 e 3 embaixo
        row_y = y + 8.0f;
        ui_bitmap(rupture_icon, x, row_y, 0);
        ui_bitmap(dissolution_icon, x + 32.0f, row_y, 0);
        ui_bitmap(reserve_icon, x + 64.0f, row_y, 0);
        row_y = y + 56.0f;
        ui_bitmap(antibody_icon, x, row_y, 0);
        ui_bitmap(lyse_icon, x + 32.0f, row_y, 0);
        ui_bitmap(perforate_icon, x + 64.0f, row_y, 0);
    }
    else
    {
        // cabeca do robo ao lado das linhas da integridade, coracao ao lado dos vitais
        frame = (int)fmod(al_get_time() / 0.25, 3.0);
        al_draw_scaled_bitmap(nanobo_icon, 0.0f, 0.0f, 16.0f, 16.0f, x + 16.0f, y - 48.0f, 64.0f, 64.0f, 0);
        al_draw_scaled_bitmap(heart_pulse, (float)(frame * 16), 0.0f, 16.0f, 16.0f,
            x + 16.0f, y + 16.0f, 64.0f, 64.0f, 0);
    }
}

void draw_howto()
{
    static const char* titles[HOWTO_PAGES] = {
        "Mover e mirar",
        "Ler com o scanner",
        "Armas",
        "As duas barras"
    };
    static const char* lines[HOWTO_PAGES][HOWTO_LINES] = {
        {
            "[Setas] ou [WASD]: mover o Nanobô.",
            "[Mouse]: mirar. [Botão esquerdo]: disparar a arma.",
            "[Botão direito]: escanear o alvo sob o cursor.",
            "[X]: defesa por 2 s. Contato não fere; toxinas, vírus e",
            "esporos em voo se desfazem. A hifa passa. Recarga: 3 s.",
            "[Esc]: pausa. [F11]: tela cheia."
        },
        {
            "[Botão direito] num agente: eu leio uma camada por vez.",
            "Camada 1: a {forma}. Coco, bacilo, levedura, bolor...",
            "Camada 2: a {coloração de Gram} e o nome da espécie.",
            "A forma não basta: existe coco <p:Gram+> e coco <r:Gram->.",
            "Com as duas camadas lidas, o agente está identificado.",
            "Identificar todos os agentes da fase vale <g:+15 pontos>."
        },
        {
            "[1], [2], [3] ou [roda do mouse]: trocar de arma.",
            "Fase 1: [1] <wR:Ruptura> · [2] <wD:Dissolução> · [3] <wS:Reserva> (2 cargas)",
            "Cada bactéria pede 2 doses, a segunda em até 6 segundos.",
            "Fase 2: [1] <wN:Neutralização> (vírus livre) · [2] <wT:Ataque T>",
            "Fase 3: [1] <wF:Perfuração> · [2] <wR:Ruptura> (não age em fungo)",
            "Regra do jogo: as armas se inspiram em remédios reais."
        },
        {
            "Cabeça de robô: a minha {Integridade}. Ela cai quando",
            "um invasor, uma toxina, a hifa ou a nuvem me acertam.",
            "Coração: os {Vitais} do Léo. Caem quando uma célula dele",
            "é destruída ou atingida por engano, e no <wT:Ataque T>.",
            "Se uma das barras zera, a equipe me retira do corpo.",
            "Nova fase: Integridade cheia e Vitais de no mínimo 60."
        }
    };
    char counter[16];
    float line_y = HOWTO_PANEL_Y + 72.0f;
    int page = howto_page;
    int i;

    if (page < 0 || page >= HOWTO_PAGES)
    {
        page = 0;
        howto_page = 0;
    }

    if (howto_background)
    {
        ui_bitmap(howto_background, 0.0f, 0.0f, 0);
    }
    ui_text(UI_TEXT, SCREEN_W / 2, 32, ALLEGRO_ALIGN_CENTRE, "Como jogar");
    draw_rule((float)SCREEN_W / 2, 60.0f, 400.0f, UI_FRAME);

    // painel central quase opaco (cor pre-multiplicada): o radar do fundo aparece de leve
    draw_panel(32.0f, HOWTO_PANEL_Y, (float)SCREEN_W - 64.0f, 288.0f,
        UI_VEIL(230.0f / 255.0f), UI_FRAME);
    ui_text(UI_AMBER, HOWTO_TEXT_X, HOWTO_PANEL_Y + 24.0f, 0, titles[page]);
    snprintf(counter, sizeof(counter), "%d/%d", page + 1, HOWTO_PAGES);
    ui_text(UI_DIM, (float)SCREEN_W - 48.0f, HOWTO_PANEL_Y + 24.0f, ALLEGRO_ALIGN_RIGHT, counter);

    draw_howto_icon(HOWTO_ICON_X, line_y + 40.0f);

    for (i = 0; i < HOWTO_LINES; i++)
    {
        draw_marked_text(HOWTO_TEXT_X, line_y + 32.0f * (float)i, UI_TEXT, lines[page][i]);
    }

    draw_marked_text_aligned(SCREEN_W / 2, 672, UI_DIM, ALLEGRO_ALIGN_CENTRE,
        page == HOWTO_PAGES - 1
            ? "[enter], [espaço] ou [clique] volta · [setas] trocam · [esc] volta"
            : "[enter], [espaço] ou [clique] avança · [setas] trocam · [esc] volta");
}

// ---------------------------------------------------------------
// fim de fase: resultados, vitoria e game over
// ---------------------------------------------------------------


// relatorio da fase em linguagem de jogador: cada linha retoma um conteudo
// com os numeros da partida, sem os codigos internos dos objetivos (#223)
static void results_report_line(int row, char* line, size_t size)
{
    int neutralized = threats_total() - threats_left();
    int reserve_spent = progress.reserve_on_resistant + progress.reserve_on_common;

    line[0] = '\0';

    // fase de fungos: o que foi eliminado, o antibiotico que nao age e a carga de esporos (#232)
    if (phase_has_fungi())
    {
        if (row == 0)
        {
            snprintf(line, size, "eliminei %d leveduras", progress.yeasts_eliminated);
        }
        else if (row == 1)
        {
            snprintf(line, size, "eliminei %d colônias (com as germinadas)", progress.molds_eliminated);
        }
        else if (row == 2)
        {
            snprintf(line, size, "gastei %d tiros de antibiótico em fungo", progress.antibiotic_on_fungus);
        }
        else if (row == 3)
        {
            snprintf(line, size, "esporos pela defesa fraca do Léo: %d", spore_load_current());
        }
        else
        {
            snprintf(line, size, "anotei: fungo tem quitina e ergosterol");
        }

        return;
    }

    // fase de virus: anticorpos, Lise e ciclo litico (#88)
    if (phase_has_viruses())
    {
        if (row == 0)
        {
            snprintf(line, size, "prendi %d vírus livres com anticorpos", progress.viruses_neutralized);
        }
        else if (row == 1)
        {
            snprintf(line, size, "<wT:Ataque T>: contive %d células a tempo", progress.cells_lysed);
        }
        else if (row == 2)
        {
            snprintf(line, size, "%d células romperam no ciclo lítico", progress.cells_burst);
        }
        else if (row == 3)
        {
            snprintf(line, size, "eliminei %d de %d ameaças", neutralized, threats_total());
        }
        else
        {
            snprintf(line, size, "anotei: vírus sem parede nem ribossomos");
        }

        return;
    }

    if (row == 0)
    {
        snprintf(line, size, "li %d <p:Gram+> e %d <r:Gram-> até a coloração",
            progress.scans_gram_positive, progress.scans_gram_negative);
    }
    else if (row == 1)
    {
        // nenhuma resistencia nao e perda: o zero fica sem o coral
        snprintf(line, size, progress.resistances > 0
                ? "tratei %d até o fim; <x:%d resistiram>"
                : "tratei %d até o fim; %d resistiram",
            progress.treatments_completed, progress.resistances);
    }
    else if (row == 2)
    {
        if (progress.wrong_decisions == 0)
        {
            snprintf(line, size, "não errei nenhum antibiótico");
        }
        else
        {
            snprintf(line, size, "<x:errei %d antibióticos> (%d pela forma)",
                progress.wrong_decisions, progress.shape_guesses);
        }
    }
    else if (row == 3)
    {
        if (reserve_spent == 0)
        {
            snprintf(line, size, "guardei a <wS:Reserva>: nenhuma carga gasta");
        }
        else
        {
            snprintf(line, size, "<wS:Reserva>: %d na resistente, %d na comum",
                progress.reserve_on_resistant, progress.reserve_on_common);
        }
    }
    else
    {
        snprintf(line, size, "acertei %d células do Léo por engano", progress.cell_attacks);
    }
}


void draw_results()
{
    char line[128];
    char value[32];
    int count;
    const UiButton* buttons = screen_buttons(&count);
    int neutralized = threats_total() - threats_left();
    float left = 304.0f;
    float right = (float)SCREEN_W - 304.0f;
    float row;
    int report_row;
    ALLEGRO_COLOR bonus_color;
    float map_y = camera.y;

    // fundo: o mapa da fase concluida onde a camera parou, na tela inteira
    // (sem passar da borda de baixo do PNG) e com um veu escuro por cima
    if (world_map && map_y > (float)(al_get_bitmap_height(world_map) - SCREEN_H / WORLD_SCALE))
    {
        map_y = fmaxf(0.0f, (float)(al_get_bitmap_height(world_map) - SCREEN_H / WORLD_SCALE));
    }

    draw_phase_map(-floorf(camera.x), -floorf(map_y), (float)SCREEN_H);
    al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)SCREEN_H,
        UI_VEIL(160.0f / 255.0f));

    snprintf(line, sizeof(line), "Fase %d concluída", current_phase());
    ui_text(UI_TEXT, SCREEN_W / 2, 48, ALLEGRO_ALIGN_CENTRE, line);
    draw_rule((float)SCREEN_W / 2, 76.0f, 400.0f, UI_FRAME);

    // interludio (#244): entre as fases, o estado do paciente e o proximo
    // invasor; os vitais ja com o piso de 60 que a passagem aplica (#218)
    if (has_next_phase())
    {
        int vitals = (int)fmaxf(patient_vitals, VITALS_MIN_BETWEEN);
        const char* state = vitals >= 70 ? "estável" : (vitals >= 40 ? "em observação" : "grave");

        snprintf(line, sizeof(line), "vitais do Léo: %s (%d) · a seguir: %s",
            state, vitals, current_phase() == 1 ? "vírus nas vias aéreas" : "fungos no pulmão");
        ui_text(UI_DIM, SCREEN_W / 2, 92, ALLEGRO_ALIGN_CENTRE, line);
    }
    else
    {
        // ultima fase: so o estado e os vitais reais, sem o piso; a alta vem na vitoria
        int vitals = (int)fmaxf(patient_vitals, 0.0f);
        const char* state = vitals >= 70 ? "estável" : (vitals >= 40 ? "em observação" : "grave");

        snprintf(line, sizeof(line), "vitais do Léo: %s (%d)", state, vitals);
        ui_text(UI_DIM, SCREEN_W / 2, 92, ALLEGRO_ALIGN_CENTRE, line);
    }

    // resultado principal: pontos e ameacas, o que fecha a fase
    draw_panel(280.0f, 116.0f, 720.0f, 88.0f, UI_INK, UI_FRAME);

    snprintf(value, sizeof(value), "%d", score);
    draw_stat(left, 132.0f, right, "pontos", value, UI_AMBER);

    snprintf(value, sizeof(value), "%d/%d", neutralized, threats_total());
    draw_stat(left, 164.0f, right, "ameaças eliminadas", value, UI_TEXT);

    // objetivos secundarios: identificacao e bonus
    draw_panel(280.0f, 212.0f, 720.0f, 104.0f, UI_INK, UI_FRAME);

    snprintf(value, sizeof(value), "%d/%d", identified_count, identification_goal());
    draw_stat(left, 228.0f, right, "agentes identificados", value, UI_TEXT);

    if (identification_bonus_awarded())
    {
        bonus_color = UI_GREEN;
        snprintf(value, sizeof(value), "+%d", IDENTIFICATION_BONUS);
    }
    else
    {
        bonus_color = UI_DIM;
        snprintf(value, sizeof(value), "não obtido");
    }

    draw_stat(left, 260.0f, right, "bônus de identificação", value, bonus_color);

    // o bonus dos vitais fecha o painel dos objetivos secundarios (#218)
    snprintf(value, sizeof(value), "+%d", vitals_bonus_awarded());
    draw_stat(left, 292.0f, right, "bônus pelos vitais do Léo", value,
        vitals_bonus_awarded() > 0 ? UI_GREEN : UI_DIM);

    // resumo educativo: so contadores da partida atual (#174, #202)
    draw_panel(280.0f, 324.0f, 720.0f, 184.0f, UI_INK, UI_FRAME);

    ui_text(UI_AMBER, left, 340.0f, 0, "O que anotei nesta fase");


    // relatorio da fase: cinco linhas com os numeros da partida (#223)
    row = 368.0f;

    for (report_row = 0; report_row < 5; report_row++)
    {
        results_report_line(report_row, line, sizeof(line));
        draw_marked_text(left, row, UI_TEXT, line);
        row = row + 28.0f;
    }

    draw_buttons(buttons, count);

    draw_marked_text_aligned(SCREEN_W / 2, 592, UI_DIM, ALLEGRO_ALIGN_CENTRE,
        "[enter] também continua");

#ifdef _DEBUG
    // contadores da #200 fora da interface normal: F1 liga e desliga (#202)
    if (show_debug)
    {
        snprintf(line, sizeof(line), "QA scans %d/%d decisoes %d/%d celulas %d",
            progress.scans_gram_positive, progress.scans_gram_negative,
            progress.correct_decisions, progress.wrong_decisions, progress.cell_attacks);
        ui_text(UI_DIM, SCREEN_W / 2, 680, ALLEGRO_ALIGN_CENTRE, line);
    }
#endif
}

// painel do Top 10 (#197): posicao, nome e pontos; o registro novo em ambar
static void draw_ranking_panel(float x, float y, float w, int highlight, bool in_menu)
{
    char line[32];
    int i;

    draw_panel(x, y, w, 368.0f, UI_INK, UI_FRAME);
    ui_text(UI_AMBER, x + w / 2.0f, y + 16.0f, ALLEGRO_ALIGN_CENTRE, "Top 10");

    if (ranking_count() == 0)
    {
        ui_text(UI_DIM, x + w / 2.0f, y + 172.0f, ALLEGRO_ALIGN_CENTRE,
            "nenhum recorde ainda");
        // a dica so no menu: na vitoria o Leo ja deu alta
        if (in_menu)
        {
            ui_text(UI_DIM, x + w / 2.0f, y + 204.0f, ALLEGRO_ALIGN_CENTRE,
                "dê alta ao Léo para entrar aqui");
        }
        return;
    }

    for (i = 0; i < ranking_count(); i++)
    {
        const RankingEntry* e = ranking_entry(i);
        float row = y + 56.0f + (float)i * 30.0f;
        ALLEGRO_COLOR color = i == highlight ? UI_AMBER : UI_TEXT;

        if (i == highlight)
        {
            al_draw_filled_rectangle(x + 8.0f, row - 6.0f, x + w - 8.0f, row + 22.0f, UI_HOVER);
        }

        snprintf(line, sizeof(line), "%2d", i + 1);
        ui_text(UI_DIM, x + 24.0f, row, 0, line);
        ui_text(color, x + 80.0f, row, 0, e->name);
        snprintf(line, sizeof(line), "%d", e->score);
        ui_text(color, x + w - 24.0f, row, ALLEGRO_ALIGN_RIGHT, line);
    }
}

void draw_victory()
{
    char value[48];
    char timer_text[16];
    int total_seconds = (int)run_time;
    int count;
    const UiButton* buttons = screen_buttons(&count);
    float left = 72.0f;
    float right = 604.0f;

    // o quarto de dia, leito arrumado: o Leo foi para casa; o veu segura o texto
    if (hospital_victory)
    {
        ui_bitmap(hospital_victory, 0.0f, 0.0f, 0);
    }
    al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)SCREEN_H, UI_VEIL(150.0f / 255.0f));

    ui_text(UI_TEXT, SCREEN_W / 2, 36, ALLEGRO_ALIGN_CENTRE, "Vitória");
    draw_rule((float)SCREEN_W / 2, 64.0f, 400.0f, UI_FRAME);
    // final da narrativa (#244): a alta do Leo e a linha do Nanobo
    ui_text(UI_TEXT, SCREEN_W / 2, 76, ALLEGRO_ALIGN_CENTRE,
        "O Léo recebeu alta e vai para casa. Missão cumprida!");
    ui_text(UI_CYAN, SCREEN_W / 2, 108, ALLEGRO_ALIGN_CENTRE,
        "Anotei tudo o que aprendi. Obrigado por me guiar!");

    // o resultado da run, a esquerda
    draw_panel(48.0f, 148.0f, 580.0f, 184.0f, UI_INK, UI_FRAME);

    snprintf(value, sizeof(value), "%d", score);
    draw_stat(left, 168.0f, right, "pontuação final", value, UI_AMBER);

    snprintf(value, sizeof(value), "%d/%d", run_progress.neutralized, run_progress.threats);
    draw_stat(left, 204.0f, right, "ameaças eliminadas", value, UI_TEXT);

    snprintf(value, sizeof(value), "%d/%d", run_progress.identified, run_progress.agents);
    draw_stat(left, 240.0f, right, "agentes identificados", value, UI_TEXT);

    snprintf(timer_text, sizeof(timer_text), "%02d:%02d", total_seconds / 60,
        total_seconds % 60);
    draw_stat(left, 276.0f, right, "tempo total", timer_text, UI_TEXT);

    // o Top 10, a direita, com o registro novo em destaque (#197)
    draw_ranking_panel(660.0f, 148.0f, 572.0f, ranking_highlight, false);

    // entrada do nome quando a pontuacao entra no Top 10 (#81)
    draw_panel(48.0f, 348.0f, 580.0f, 168.0f, UI_INK, name_entry_active ? UI_AMBER : UI_FRAME);

    if (name_entry_active)
    {
        float tx = 80.0f;
        bool blink = (int)(al_get_time() * 2.0) % 2 == 0;

        ui_text(UI_AMBER, 338.0f, 364.0f, ALLEGRO_ALIGN_CENTRE, "novo recorde no Top 10!");
        draw_marked_text_aligned(338.0f, 396.0f, UI_DIM, ALLEGRO_ALIGN_CENTRE, "digite seu nome e tecle [enter]");
        al_draw_filled_rectangle(tx, 428.0f, 596.0f, 464.0f, UI_PANEL);
        al_draw_rectangle(tx + 1.0f, 429.0f, 595.0f, 463.0f, UI_FRAME, 2.0f);
        ui_text(UI_TEXT, tx + 12.0f, 438.0f, 0, name_entry);

        if (blink && name_entry_chars < RANKING_NAME_CHARS)
        {
            ui_text(UI_CYAN, tx + 12.0f + (float)ui_text_width(name_entry), 438.0f, 0, "_");
        }

        // o limite de 10 caracteres dito na tela
        draw_marked_text_aligned(338.0f, 480.0f, UI_DIM, ALLEGRO_ALIGN_CENTRE, "até 10 letras · [backspace] apaga");
    }
    else if (ranking_highlight >= 0)
    {
        snprintf(value, sizeof(value), "posição %d no Top 10", ranking_highlight + 1);
        ui_text(UI_GREEN, 338.0f, 404.0f, ALLEGRO_ALIGN_CENTRE, value);

        if (ranking_save_failed)
        {
            ui_text(UI_RED, 338.0f, 444.0f, ALLEGRO_ALIGN_CENTRE,
                "não foi possível gravar o recorde");
        }
    }
    else
    {
        ui_text(UI_DIM, 338.0f, 420.0f, ALLEGRO_ALIGN_CENTRE,
            "a pontuação não entrou no Top 10");
    }

    // com o nome sendo digitado, o botao espera a confirmacao
    if (!name_entry_active)
    {
        draw_buttons(buttons, count);
        draw_marked_text_aligned(SCREEN_W / 2, 624, UI_DIM, ALLEGRO_ALIGN_CENTRE,
            "[enter] também começa de novo");
    }
}

void draw_game_over()
{
    char value[32];
    char timer_text[16];
    int total_seconds = (int)run_time;
    int count;
    const UiButton* buttons = screen_buttons(&count);
    float left = 304.0f;
    float right = (float)SCREEN_W - 304.0f;

    // o quarto em luz baixa, monitor estavel: o Leo segue internado
    if (hospital_game_over)
    {
        ui_bitmap(hospital_game_over, 0.0f, 0.0f, 0);
    }
    al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)SCREEN_H, UI_VEIL(100.0f / 255.0f));

    ui_text(UI_RED, SCREEN_W / 2, 60, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
    draw_rule((float)SCREEN_W / 2, 88.0f, 400.0f, UI_RED);
    // game over da narrativa (#244): a equipe retira o Nanobô e diz por que;
    // o Leo nao morre, segue no tratamento convencional
    ui_text(UI_TEXT, SCREEN_W / 2, 104, ALLEGRO_ALIGN_CENTRE,
        game_over_patient ? "Os vitais do Léo caíram demais: a equipe retirou o Nanobô."
                          : "O Nanobô foi danificado: a equipe o retirou do corpo.");
    ui_text(UI_TEXT, SCREEN_W / 2, 136, ALLEGRO_ALIGN_CENTRE,
        "O Léo segue no tratamento convencional do hospital.");
    ui_text(UI_CYAN, SCREEN_W / 2, 168, ALLEGRO_ALIGN_CENTRE,
        "Anotei os meus erros. Vamos tentar de novo?");

    draw_panel(280.0f, 208.0f, 720.0f, 192.0f, UI_INK, UI_FRAME);

    snprintf(value, sizeof(value), "%d", score);
    draw_stat(left, 228.0f, right, "pontos", value, UI_TEXT);

    snprintf(value, sizeof(value), "%d/%d", run_progress.neutralized, run_progress.threats);
    draw_stat(left, 268.0f, right, "ameaças eliminadas", value, UI_TEXT);

    snprintf(value, sizeof(value), "%d/%d", run_progress.identified, run_progress.agents);
    draw_stat(left, 308.0f, right, "agentes identificados", value, UI_TEXT);

    snprintf(timer_text, sizeof(timer_text), "%02d:%02d", total_seconds / 60,
        total_seconds % 60);
    draw_stat(left, 348.0f, right, "tempo total", timer_text, UI_TEXT);

    draw_buttons(buttons, count);

    draw_marked_text_aligned(SCREEN_W / 2, 520, UI_DIM, ALLEGRO_ALIGN_CENTRE,
        "[enter] também reinicia");
}
