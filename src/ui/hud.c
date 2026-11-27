// HUD do rodape e ficha do scanner que flutua na faixa do mundo
#define _USE_MATH_DEFINES
#include "ui.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// y de cada linha do HUD, contando de cima para baixo dentro do painel
static int hud_y(int line)
{
    return SCAN_PANEL_Y + HUD_MARGIN + line * HUD_LINE_H;
}

// barra da recarga do ataque: segmento cheio quando o disparo esta liberado
static void draw_cooldown_bar(float x, float y, float fraction)
{
    int i;
    int checks = (int)floorf((1.0f - fraction) * (float)UI_BAR_SEGMENTS + 0.5f);

    for (i = 0; i < UI_BAR_SEGMENTS; i++)
    {
        float sx = x + (float)i * (float)(UI_SEG_W + UI_SEG_GAP);
        ALLEGRO_COLOR color = i < checks ? UI_AMBER : UI_SLOT;

        al_draw_filled_rectangle(sx, y, sx + (float)UI_SEG_W, y + (float)UI_SEG_H, color);
    }
}

// linha de rotulo a esquerda e valor a direita
void draw_stat(float x, float y, float right, const char* label, const char* value,
    ALLEGRO_COLOR color)
{
    al_draw_text(font, UI_DIM, x, y, 0, label);
    al_draw_text(font, color, right, y, ALLEGRO_ALIGN_RIGHT, value);
}

// ---------------------------------------------------------------
// painel do scanner: um instrumento, nao uma linha de log
// ---------------------------------------------------------------

static void draw_scan_swatch(Species species, float x, float y)
{
    // célula e vírus não têm coloração de Gram: sem amostra (#M4)
    if (species == SPECIES_CELL || species == SPECIES_VIRUS
        || species_is_fungus(species))
    {
        return;
    }

    al_draw_filled_rectangle(x, y + 1.0f, x + 8.0f, y + 9.0f,
        species == SPECIES_GRAM_POSITIVE ? UI_PURPLE : UI_PINK);
}

// alvo da ficha do scanner: o agente sob o cursor ou a ultima leitura, enquanto
// ela segue ativa e ao alcance do scanner; sem alvo, a ficha some (#243)
int scan_card_target(bool* from_cursor)
{
    float lx = to_logical_x(mouse_x);
    float ly = to_logical_y(mouse_y);
    float dx;
    float dy;
    int target;

    *from_cursor = false;

    // cursor sobre o painel do HUD nao tem alvo: o clique ali nao escaneia
    if (lx >= 0.0f && lx < (float)SCREEN_W && ly >= 0.0f && ly < (float)PLAY_H)
    {
        target = pick_target_at(lx + camera.x, ly + camera.y);

        if (target != -1)
        {
            *from_cursor = true;
            return target;
        }
    }

    target = last_scan_target();

    if (target == -1 || !agents[target].active)
    {
        return -1;
    }

    dx = agents[target].x - player.x;
    dy = agents[target].y - player.y;

    if (dx * dx + dy * dy > (float)(SCAN_RANGE * SCAN_RANGE))
    {
        return -1;
    }

    return target;
}

// a ficha fica embaixo da faixa do mundo e sobe quando o alvo esta na metade
// de baixo, para nunca cobrir o agente que ela descreve (#243)
float scan_card_y(int target)
{
    if (view_y(agents[target].y) > (float)PLAY_H / 2.0f)
    {
        return (float)CARD_MARGIN;
    }

    return (float)(PLAY_H - CARD_MARGIN - CARD_H);
}

// retrato 24x24 da ficha: silhueta antes da 1a leitura, como no mundo; a
// bacteria lida usa o sprite cinza da forma, tingido pelo Gram na 2a (SC-01)
static void draw_scan_portrait(int target, float x, float y)
{
    ALLEGRO_BITMAP* bmp;
    ALLEGRO_COLOR tint = al_map_rgb_f(1.0f, 1.0f, 1.0f);

    if (agents[target].layers == 0)
    {
        bmp = silhouette_sprite;
    }
    else if (species_is_bacterium(agents[target].species))
    {
        bmp = bacteria_sprite(agents[target].shape, agents[target].variant);

        if (agents[target].layers >= MAX_LAYERS)
        {
            tint = agents[target].species == SPECIES_GRAM_POSITIVE ? GRAM_POS_TINT : GRAM_NEG_TINT;
        }
    }
    else
    {
        bmp = species_sprite(agents[target].species, agents[target].variant);
    }

    al_draw_tinted_scaled_bitmap(bmp, tint, 0.0f, 0.0f,
        (float)al_get_bitmap_width(bmp), (float)al_get_bitmap_height(bmp),
        x, y, (float)CARD_PORTRAIT, (float)CARD_PORTRAIT, 0);
}

// um marcador de camada: quadrado 6x6 e o rotulo; devolve o x depois dele
static float draw_scan_marker(float x, float y, ALLEGRO_COLOR color, bool lit, const char* label)
{
    al_draw_filled_rectangle(x, y + 1.0f, x + (float)CARD_MARK, y + 1.0f + (float)CARD_MARK, color);
    al_draw_text(font, lit ? UI_TEXT : UI_DIM, x + (float)CARD_MARK + 4.0f, y, 0, label);

    return x + (float)CARD_MARK + 4.0f + (float)al_get_text_width(font, label);
}

void draw_scan_card(int target, bool from_cursor, float card_y)
{
    int text_x = CARD_TEXT_X;
    int row_state = (int)card_y + 6;
    int row_info = row_state + CARD_ROW_H;
    int row_action = row_info + CARD_ROW_H;
    const char* state;
    const char* category;
    const char* source;
    const char* second_label;
    ALLEGRO_COLOR source_color;
    ALLEGRO_COLOR second_color;
    float marks_x;
    bool is_bacteria = species_is_bacterium(agents[target].species);

    draw_panel(8.0f, card_y, (float)SCREEN_W - 16.0f, (float)CARD_H, UI_PANEL, UI_FRAME);
    draw_scan_portrait(target, 14.0f, card_y + 5.0f);

    // de onde vem a leitura: o cursor manda, a ultima leitura e o retorno (#199)
    if (from_cursor)
    {
        source = " · alvo sob o cursor";
        source_color = UI_CYAN;
    }
    else
    {
        source = " · última leitura";
        source_color = UI_DIM;
    }

    if (agents[target].layers == 0)
    {
        state = "Alvo detectado";
    }
    else if (agents[target].layers < MAX_LAYERS)
    {
        state = "Leitura incompleta";
    }
    else
    {
        state = "Identificação concluída";
    }

    // a categoria so e desenhada sem leitura, entao a celula e sempre "celula"
    if (agents[target].species == SPECIES_CELL)
    {
        category = "célula";
    }
    else if (agents[target].species == SPECIES_VIRUS)
    {
        category = "vírus";
    }
    else if (species_is_fungus(agents[target].species))
    {
        category = "fungo";
    }
    else
    {
        category = "bactéria";
    }

    al_draw_text(font, UI_AMBER, (float)text_x, (float)row_state, 0, state);
    al_draw_text(font, source_color, (float)(text_x + al_get_text_width(font, state)),
        (float)row_state, 0, source);
    // a categoria so aparece sem leitura: depois, as linhas 1 e 2 ja dizem o
    // tipo e a linha de estado precisa do espaco para os marcadores
    marks_x = (float)(SCREEN_W - 16);

    if (agents[target].layers == 0)
    {
        al_draw_text(font, UI_TEXT, (float)(SCREEN_W - 16), (float)row_state,
            ALLEGRO_ALIGN_RIGHT, category);
        marks_x = marks_x - (float)(al_get_text_width(font, category) + 8);
    }

    // marcadores das 2 camadas, encostados a direita: forma acende na 1a;
    // o 2o acende na 2a e fica verde com a identificacao (SC-11 a SC-13)
    second_label = is_bacteria ? "Gram e espécie" : "estrutura";
    marks_x = marks_x
        - (float)(2 * (CARD_MARK + 4) + 8)
        - (float)al_get_text_width(font, "forma") - (float)al_get_text_width(font, second_label);
    second_color = UI_SLOT;

    if (agents[target].layers >= MAX_LAYERS)
    {
        second_color = agents[target].identified ? UI_GREEN : UI_AMBER;
    }

    marks_x = draw_scan_marker(marks_x, (float)row_state,
        agents[target].layers >= 1 ? UI_AMBER : UI_SLOT, agents[target].layers >= 1, "forma");
    draw_scan_marker(marks_x + 8.0f, (float)row_state, second_color,
        agents[target].layers >= MAX_LAYERS, second_label);

    if (agents[target].layers == 0)
    {
        draw_marked_text((float)text_x, (float)row_info, UI_TEXT,
            "[botão direito]: escanear");
        al_draw_text(font, UI_DIM, (float)text_x, (float)row_action, 0,
            "cada clique revela uma camada");
        return;
    }

    // camada 1: so a forma, sem amostra de cor que entregue o grupo (#214)
    if (agents[target].layers < MAX_LAYERS)
    {
        al_draw_text(font, UI_TEXT, (float)(text_x + 12), (float)row_info, 0,
            layer_one_text(target));
        draw_marked_text((float)text_x, (float)row_action, UI_DIM,
            is_bacteria
                ? "[botão direito] de novo: revela a coloração de Gram"
                : "[botão direito] de novo: conclui a leitura");
        return;
    }

    // camada 2: coloracao, estrutura e o que a celula e, sem indicar a arma (#214)
    draw_scan_swatch(agents[target].species, (float)text_x, (float)row_info);
    draw_marked_text((float)(text_x + 12), (float)row_info, UI_WARM,
        layer_two_text(target));
    al_draw_text(font, UI_WARM, (float)text_x, (float)row_action, 0,
        layer_two_reason(target));
}

// moldura da barra de vida e os segmentos cheios tingidos (10 de 5x4)
static void draw_life_bar(ALLEGRO_BITMAP* frame, float x, float y, int segments, ALLEGRO_COLOR color)
{
    int i;

    al_draw_bitmap(frame, x, y, 0);

    for (i = 0; i < segments && i < LIFE_SEGMENTS; i++)
    {
        al_draw_tinted_bitmap(bar_segment, color, x + 2.0f + 6.0f * (float)i, y + 2.0f, 0);
    }
}
// icone de cada arma no HUD, ao lado do nome
static ALLEGRO_BITMAP* weapon_icon(Laser laser)
{
    switch (laser)
    {
    case LASER_GRAM_POSITIVE:
        return rupture_icon;
    case LASER_RESERVE:
        return reserve_icon;
    case LASER_NEUTRALIZE:
        return antibody_icon;
    case LASER_LYSE:
        return lyse_icon;
    case LASER_PERFORATE:
        return perforate_icon;
    case LASER_GRAM_NEGATIVE:
    default:
        return dissolution_icon;
    }
}

// nome da arma e o que ela atinge, na cor dela (<wX:...>); a Reserva monta o
// proprio texto com as cargas no draw_hud
static const char* weapon_label(Laser laser)
{
    switch (laser)
    {
    case LASER_GRAM_POSITIVE:
        return "<wR:Ruptura> · parede";
    case LASER_NEUTRALIZE:
        return "<wN:Neutralização> · vírus livre";
    case LASER_LYSE:
        return "<wT:Ataque T> · célula infectada";
    case LASER_PERFORATE:
        return "<wF:Perfuração> · antifúngico";
    case LASER_GRAM_NEGATIVE:
    case LASER_RESERVE:
    default:
        return "<wD:Dissolução> · membrana";
    }
}

void draw_hud()
{
    int x;
    ALLEGRO_COLOR vitals_color;
    float period;
    double beat;
    int heart_frame;
    int total_seconds;
    int x_weapon;
    int x_icon;
    int x_bar;
    int x_word;
    char line[96];
    char score_text[32];
    char timer_text[16];
    const char* state_text;
    const char* module_name;
    char reserve_text[40];
    ALLEGRO_BITMAP* laser_icon;

    al_draw_filled_rectangle(0.0f, (float)SCAN_PANEL_Y, (float)SCREEN_W, (float)SCREEN_H, UI_INK);
    al_draw_line(0.0f, (float)SCAN_PANEL_Y - 0.5f, (float)SCREEN_W,
        (float)SCAN_PANEL_Y - 0.5f, UI_LINE, 1.0f);

    // nada do HUD pode vazar para a faixa do mundo; o clip vale so no painel
    al_set_clipping_rectangle(0, SCAN_PANEL_Y, SCREEN_W, PANEL_H);

    // coluna de vida: icone 16x16 centrado na linha + barra 64x8 em x 28
    // linha 0: integridade do nanobo, bateria em ciano
    al_draw_bitmap(nanobo_icon, (float)HUD_MARGIN, (float)(hud_y(0) - 4), 0);
    draw_life_bar(battery_bar, 28.0f, (float)hud_y(0),
        life_segments(player.health, PLAYER_MAX_HEALTH), UI_CYAN);

    // linha 1: vitais do paciente no ECG, na cor da faixa
    if (patient_vitals >= VITALS_MIN_BETWEEN)
    {
        vitals_color = UI_GREEN;
    }
    else if (patient_vitals >= VITALS_LOW)
    {
        vitals_color = UI_AMBER;
    }
    else
    {
        vitals_color = UI_RED;
    }

    // coracao: batida (1), contracao (2) e repouso (0); bate mais rapido com vitais baixos
    period = heart_period(patient_vitals);
    beat = fmod(match_time, (double)period);
    heart_frame = 0;
    if (beat < 0.15 * period)
    {
        heart_frame = 1;
    }
    else if (beat < 0.30 * period)
    {
        heart_frame = 2;
    }
    al_draw_bitmap_region(heart_pulse, (float)(heart_frame * 16), 0.0f, 16.0f, 16.0f,
        (float)HUD_MARGIN, (float)(hud_y(1) - 4), 0);
    draw_life_bar(ecg_bar, 28.0f, (float)hud_y(1),
        life_segments(patient_vitals, PATIENT_VITALS_MAX), vitals_color);

    // linha 1, depois da coluna de vida: pontos e tempo a partir de x 100
    total_seconds = (int)match_time;
    snprintf(timer_text, sizeof(timer_text), "%02d:%02d", total_seconds / 60, total_seconds % 60);
    snprintf(score_text, sizeof(score_text), "pontos %d", score);
    x = 100;
    al_draw_text(font, UI_TEXT, (float)x, (float)hud_y(1), 0, score_text);
    x += al_get_text_width(font, score_text) + 4;
    al_draw_text(font, UI_DIM, (float)x, (float)hud_y(1), 0, "·");
    x += al_get_text_width(font, "·") + 4;
    al_draw_text(font, UI_DIM, (float)x, (float)hud_y(1), 0, timer_text);

    // linha 0, a direita: a ferramenta com o alvo dela, o icone e a recarga,
    // tudo medido da borda
    if (defense_state() == 1)
    {
        state_text = "defesa";
    }
    else if (fire_locked())
    {
        state_text = "travado";
    }
    else if (attack_ready())
    {
        state_text = "pronto";
    }
    else
    {
        state_text = "recarga";
    }

    // o HUD diz a arma pelo nome e o que ela atinge (#199, #243)
    laser_icon = weapon_icon(player.laser);
    module_name = weapon_label(player.laser);

    // Reserva: nome com as cargas e a palavra de estado quando acabam (#216)
    if (player.laser == LASER_RESERVE)
    {
        snprintf(reserve_text, sizeof(reserve_text), "<wS:Reserva> %d/%d · resistente",
            reserve_charges_left(), RESERVE_CHARGES);
        module_name = reserve_text;

        if (reserve_charges_left() <= 0 && defense_state() != 1)
        {
            state_text = "esgotada";
        }
    }

    x_word = SCREEN_W - HUD_MARGIN - al_get_text_width(font, state_text);
    x_bar = x_word - 6 - UI_BAR_W;
    x_icon = x_bar - 4 - al_get_bitmap_width(laser_icon);
    x_weapon = x_icon - 4 - marked_text_width(module_name);

    // o nome da arma sai na cor dela (<wX:...>)
    draw_marked_text((float)x_weapon, (float)hud_y(0), UI_TEXT, module_name);

    al_draw_bitmap(laser_icon, (float)x_icon, (float)(hud_y(0) - 4), 0);
    draw_cooldown_bar((float)x_bar, (float)(hud_y(0) + 1),
        fire_locked() ? fire_lock_left() : attack_cooldown_left());
    al_draw_text(font, UI_DIM, (float)x_word, (float)hud_y(0), 0, state_text);

    // linha 1, a direita: o objetivo saiu do HUD (vira o aviso de objetivo)
    // ameacas com contador e identificados, que e o progresso do bonus
    snprintf(line, sizeof(line), "ameaças %d/%d · identificados %d/%d",
        threats_total() - threats_left(), threats_total(),
        identified_count, identification_goal());
    al_draw_text(font, UI_DIM, (float)(SCREEN_W - HUD_MARGIN), (float)hud_y(1),
        ALLEGRO_ALIGN_RIGHT, line);

    // o quadro seguinte desenha o mundo no mesmo canvas: sem isso ele sai cortado
    al_reset_clipping_rectangle();
}
