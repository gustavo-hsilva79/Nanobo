// desenho da fase: mapa, correntes, perigos, agentes, projeteis, destino e mira
#define _USE_MATH_DEFINES
#include "../ui/ui.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// sprite de cada especie e variante, alinhado com as constantes do entities.h
// variante 1 usa o segundo desenho da especie; qualquer outra, o primeiro (#211)
ALLEGRO_BITMAP* species_sprite(Species species, int variant)
{
    // bacteria: o sprite cinza da forma; quem desenha usa o bacteria_sprite (#214)
    if (species_is_bacterium(species))
    {
        return coccus_grey[variant == 1 ? 1 : 0];
    }

    if (species == SPECIES_VIRUS)
    {
        return variant == 1 ? virus_sprite_b : virus_sprite;
    }

    // fungos da Fase 3: a levedura tem as duas variantes; colonia e esporo, uma
    if (species == SPECIES_YEAST)
    {
        return yeast_sprite[variant == 1 ? 1 : 0];
    }

    if (species == SPECIES_MOLD)
    {
        return mold_sprite;
    }

    if (species == SPECIES_SPORE)
    {
        return spore_sprite;
    }

    return variant == 1 ? cell_sprite_b : cell_sprite;
}

// sprite cinza da bacteria pela forma e variante; o grupo vem do tingimento (#214)
ALLEGRO_BITMAP* bacteria_sprite(Shape shape, int variant)
{
    int v = variant == 1 ? 1 : 0;

    if (shape == SHAPE_BACILLUS)
    {
        return bacillus_grey[v];
    }

    return coccus_grey[v];
}

// coordenada do mundo para o pixel do canvas, arredondada para nao tremer
float view_x(float x)
{
    return floorf(x - camera.x);
}

float view_y(float y)
{
    return floorf(y - camera.y);
}

// quando o destino esta fora da faixa visivel, uma marca na borda mostra o caminho (#19);
// top e bottom limitam a marca a parte livre da faixa, fora da ficha do scanner (#243)
static void draw_destination_hint(float top, float bottom)
{
    float sx = view_x(DESTINATION_X) * WORLD_SCALE;
    float sy = view_y(DESTINATION_Y) * WORLD_SCALE;
    int label_w = ui_text_width("destino");
    int label_h = ui_line_height();
    float margin_x = (float)label_w / 2.0f + 4.0f;
    float margin_y = 20.0f + (float)label_h + 12.0f;
    float cx;
    float cy;

    if (sx >= 0.0f && sx <= (float)SCREEN_W && sy >= 0.0f && sy <= (float)PLAY_H)
    {
        return;
    }

    cx = sx;
    cy = sy;

    if (cx < margin_x)
    {
        cx = margin_x;
    }

    if (cx > (float)SCREEN_W - margin_x)
    {
        cx = (float)SCREEN_W - margin_x;
    }

    if (cy < top + margin_y)
    {
        cy = top + margin_y;
    }

    if (cy > bottom - margin_y)
    {
        cy = bottom - margin_y;
    }

    al_draw_filled_circle(cx, cy, 12.0f, UI_AMBER);

    // na metade de baixo o rotulo sobe, para nao encostar na linha do painel
    if (cy > (top + bottom) / 2.0f)
    {
        ui_text(UI_AMBER, cx, cy - 20.0f - (float)label_h,
            ALLEGRO_ALIGN_CENTRE, "destino");
    }
    else
    {
        ui_text(UI_AMBER, cx, cy + 20.0f, ALLEGRO_ALIGN_CENTRE, "destino");
    }
}

// quadro do spritesheet: 1-4 parado (220 ms), 5-8 andando (110 ms) (#30)
static int player_frame()
{
    int base;
    double frame_time;
    bool moving = player.vx != 0.0f || player.vy != 0.0f;

    if (moving)
    {
        base = 4;
        frame_time = 0.110;
    }
    else
    {
        base = 0;
        frame_time = 0.220;
    }

    return base + (int)(match_time / frame_time) % 4;
}

// marcas do tratamento sobre a bacteria: o arco ambar esvazia ate a 2a dose
// e a resistente ganha anel vermelho e um R (#215)
static void draw_treatment_marks(int index)
{
    float x = view_x(agents[index].x);
    float y = view_y(agents[index].y);

    if (agents[index].dose_timer > 0.0f)
    {
        float left = agents[index].dose_timer / (float)INCOMPLETE_TIME;

        al_draw_arc(x, y, agent_radius(index) + 7.0f, -ALLEGRO_PI / 2.0f,
            left * 2.0f * ALLEGRO_PI, UI_AMBER, 2.0f);
    }

    if (agents[index].resistant)
    {
        al_draw_circle(x, y, agent_radius(index) + 7.0f, UI_RED, 2.0f);
        al_draw_text(font, UI_RED, x, y - agent_radius(index) - 18.0f,
            ALLEGRO_ALIGN_CENTRE, "R");
    }

    // aviso do ataque: anel claro que pisca enquanto o invasor se prepara (#235)
    if (agents[index].windup > 0.0f && (int)(agents[index].windup * 20.0f) % 2 == 0)
    {
        al_draw_circle(x, y, agent_radius(index) + 4.0f, UI_TEXT, 2.0f);
    }
}

// fungo sem leitura: a mesma silhueta da bacteria, na escala do raio dele (#232, D7)
static void draw_fungus_silhouette(int index)
{
    float size = 2.0f * agent_radius(index) + 2.0f;

    al_draw_scaled_bitmap(silhouette_sprite, 0.0f, 0.0f,
        (float)al_get_bitmap_width(silhouette_sprite), (float)al_get_bitmap_height(silhouette_sprite),
        view_x(agents[index].x) - size / 2.0f, view_y(agents[index].y) - size / 2.0f,
        size, size, 0);
}

// marcas do fungo: as do tratamento e o anel de identificado na 2a camada (#232)
static void draw_fungus_marks(int index)
{
    if (agents[index].layers >= MAX_LAYERS)
    {
        al_draw_circle(view_x(agents[index].x), view_y(agents[index].y),
            agent_radius(index) + 2.0f, UI_GREEN, 1.0f);
    }

    draw_treatment_marks(index);
}

// cor de cada arma na mira, na ordem do Laser (#246)
ALLEGRO_COLOR laser_color(Laser laser)
{
    switch (laser)
    {
    case LASER_GRAM_POSITIVE:
        return UI_PURPLE;
    case LASER_GRAM_NEGATIVE:
        return UI_PINK;
    case LASER_RESERVE:
        return UI_AMBER;
    case LASER_NEUTRALIZE:
        return UI_CYAN;
    case LASER_LYSE:
        return UI_GREEN;
    case LASER_PERFORATE:
    default:
        return UI_WARM;
    }
}

// aviso de objetivo (OB-01..OB-04): placa no topo do mundo, esmaece no ultimo
// 0,5 s; com a ficha do scanner em cima, desce para logo abaixo dela
static void draw_objective_notice(int card_target, float card_y)
{
    static const char* phase_goals[] = {
        "Objetivo: elimine as bactérias e chegue ao {destino}",
        "Objetivo: elimine os vírus e chegue ao {destino}",
        "Objetivo: elimine os fungos e chegue ao {destino}"
    };
    float left = objective_notice_left();
    float alpha = 1.0f;
    int phase = current_phase() - 1;
    const char* text;
    int text_w;
    float box_x;
    float box_y = 16.0f;

    // com a caixa do diario aberta o aviso espera ela fechar
    if (left <= 0.0f || pending_card() != CARD_NONE)
    {
        return;
    }

    if (objective_notice_goal() == GOAL_DESTINATION)
    {
        text = "Ameaças eliminadas! Agora vá ao {destino}";
    }
    else if (phase >= 0 && phase < (int)(sizeof(phase_goals) / sizeof(phase_goals[0])))
    {
        text = phase_goals[phase];
    }
    else
    {
        return;
    }

    if (left < NOTICE_FADE_TIME)
    {
        alpha = left / NOTICE_FADE_TIME;
    }

    if (card_target != -1 && card_y < (float)PLAY_H / 2.0f)
    {
        box_y = card_y + (float)CARD_H + 16.0f;
    }

    text_w = marked_text_width(text);
    box_x = floorf(((float)SCREEN_W - (float)text_w) / 2.0f);

    draw_panel(box_x - UI_PAD, box_y, (float)text_w + 2.0f * UI_PAD, (float)NOTICE_H,
        faded_color(UI_INK, alpha), faded_color(UI_AMBER, alpha));
    draw_marked_text(box_x, box_y + (float)(NOTICE_H - HUD_FONT_H) / 2.0f,
        faded_color(UI_WARM, alpha), text);
}

// mapa da fase atual com o tinte da fase e o veu azulado ate a altura h;
// o jogo e os resultados desenham o mesmo fundo; x e y em pixels do mundo
void draw_phase_map(float x, float y, float h)
{
    load_phase_map(current_phase());

    if (world_map)
    {
        ALLEGRO_TRANSFORM saved;
        ALLEGRO_TRANSFORM scaled;

        al_copy_transform(&saved, al_get_current_transform());
        al_identity_transform(&scaled);
        al_scale_transform(&scaled, (float)WORLD_SCALE, (float)WORLD_SCALE);
        al_compose_transform(&scaled, &saved);
        al_use_transform(&scaled);
        al_draw_tinted_bitmap(world_map, phase_has_viruses() ? MAP_TINT_AIRWAY : MAP_TINT,
            x, y, 0);
        al_use_transform(&saved);
    }
    al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, h, MAP_VEIL);
}

void draw_game()
{
    int i;
    bool is_bacteria;
    ALLEGRO_TRANSFORM shake;
    ALLEGRO_TRANSFORM world;
    ALLEGRO_COLOR nanobo_tint = al_map_rgb_f(1.0f, 1.0f, 1.0f);
    const char* message;
    float aim_x;
    float aim_y;
    float px;
    float py;
    int card_target;
    bool card_from_cursor;
    float card_y;

    // o mundo para na faixa jogavel: sprite nenhum entra no painel
    al_set_clipping_rectangle(0, 0, SCREEN_W, PLAY_H);

    // tremor so no desenho do mundo: a camera e a simulacao nao mudam (#221);
    // o tremor e medido em pixels do mundo
    al_identity_transform(&shake);
    al_translate_transform(&shake, feel_offset_x() * WORLD_SCALE, feel_offset_y() * WORLD_SCALE);
    al_use_transform(&shake);

    // mapa do mundo deslocado pela camera, atras de destino, agentes e nanobo
    // recuado: escurecido e com um veu azulado, sem mudar o PNG (#220); a fase
    // nova troca o mapa no primeiro quadro dela (#89)
    draw_phase_map(-floorf(camera.x), -floorf(camera.y), (float)PLAY_H);

    // daqui ate o fim do mundo tudo e desenhado em pixels do mundo, ampliado
    al_identity_transform(&world);
    al_scale_transform(&world, (float)WORLD_SCALE, (float)WORLD_SCALE);
    al_compose_transform(&world, &shake);
    al_use_transform(&world);

    // destino da fase: anel e rotulo visiveis desde o inicio (#26)
    al_draw_filled_circle(view_x(DESTINATION_X), view_y(DESTINATION_Y), DESTINATION_RADIUS,
        al_map_rgb(40, 34, 18));
    al_draw_circle(view_x(DESTINATION_X), view_y(DESTINATION_Y), DESTINATION_RADIUS,
        UI_AMBER, 3);
    al_draw_text(font, UI_AMBER, view_x(DESTINATION_X),
        view_y(DESTINATION_Y) - 40, ALLEGRO_ALIGN_CENTRE, "destino");

    // correnteza: tracos claros que andam na direcao do fluxo (#237)
    {
        const Phase* ph = &phases[current_phase() - 1];
        int c;
        int k;

        for (c = 0; c < ph->current_count; c++)
        {
            const Current* cur = &ph->currents[c];
            float w = cur->x1 - cur->x0;
            float h = cur->y1 - cur->y0;

            for (k = 0; k < 40; k++)
            {
                float sx = cur->x0 + fmodf((float)(k * 97) + (float)match_time * cur->vx, w);
                float sy = cur->y0 + fmodf((float)(k * 37), h);

                al_draw_line(view_x(sx), view_y(sy), view_x(sx) + 6.0f, view_y(sy),
                    al_map_rgba_f(0.18f, 0.18f, 0.2f, 0.18f), 1.0f);
            }
        }
    }

    // perigos: biofilme amarelado e nuvem de endotoxina arroxeada, por baixo dos agentes (#237)
    for (i = 0; i < MAX_HAZARDS; i++)
    {
        float a;

        if (!hazards[i].active)
        {
            continue;
        }

        if (hazards[i].kind == HAZARD_BIOFILM)
        {
            a = 0.30f;
            al_draw_filled_circle(view_x(hazards[i].x), view_y(hazards[i].y), hazards[i].radius,
                al_map_rgba_f(0.62f * a, 0.66f * a, 0.30f * a, a));
            al_draw_circle(view_x(hazards[i].x), view_y(hazards[i].y), hazards[i].radius,
                al_map_rgba_f(0.8f * 0.6f, 0.85f * 0.6f, 0.45f * 0.6f, 0.6f), 1.0f);
        }
        else
        {
            a = 0.30f + 0.10f * sinf((float)match_time * 9.0f);
            al_draw_filled_circle(view_x(hazards[i].x), view_y(hazards[i].y),
                hazards[i].radius * (0.9f + 0.1f * (hazards[i].life / ENDOTOXIN_TIME)),
                al_map_rgba_f(0.70f * a, 0.22f * a, 0.70f * a, a));
        }
    }

    for (i = 0; i < agents_total(); i++)
    {
        // agente neutralizado sai do desenho; o virus de dentro da celula e
        // desenhado junto com ela (#85)
        if (!agents[i].active || virus_inside(i))
        {
            continue;
        }

        is_bacteria = species_is_bacterium(agents[i].species);

        // sombra de contato: o agente deixa de parecer colado no fundo (#220);
        // no tamanho de cada especie, do virus de 16 px a celula de 64 (#235)
        al_draw_filled_ellipse(view_x(agents[i].x), view_y(agents[i].y) + 0.85f * agent_radius(i),
            0.85f * agent_radius(i), 0.3f * agent_radius(i), CONTACT_SHADOW);

        // bacteria sem leitura: silhueta azulada, sem forma nem cor que entregue o grupo (#220)
        // o virus fica de fora: o estado dele e lido no mundo, sem scanner (#84)
        if (is_bacteria && agents[i].layers == 0)
        {
            // disco de holograma com "?": ainda nao se sabe o que e (#241)
            al_draw_bitmap(silhouette_sprite, view_x(agents[i].x) - (float)AGENT_RADIUS,
                view_y(agents[i].y) - (float)AGENT_RADIUS, 0);
            draw_treatment_marks(i);
            continue;
        }

        // bacteria lida: sprite cinza da forma; na camada 2, tingido pelo Gram e com anel (#214)
        if (is_bacteria)
        {
            ALLEGRO_COLOR tint = al_map_rgb_f(1.0f, 1.0f, 1.0f);
            bool stained = agents[i].layers >= MAX_LAYERS;

            if (stained)
            {
                tint = agents[i].species == SPECIES_GRAM_POSITIVE ? GRAM_POS_TINT : GRAM_NEG_TINT;
            }

            al_draw_tinted_bitmap(bacteria_sprite(agents[i].shape, agents[i].variant), tint,
                view_x(agents[i].x) - AGENT_RADIUS,
                view_y(agents[i].y) - AGENT_RADIUS, 0);

            if (stained)
            {
                al_draw_circle(view_x(agents[i].x), view_y(agents[i].y),
                    (float)AGENT_RADIUS + 2.0f,
                    agents[i].species == SPECIES_GRAM_POSITIVE ? UI_PURPLE : UI_PINK, 1.0f);
            }

            draw_treatment_marks(i);
            continue;
        }

        // hifa: um segmento septado de 16 px por vez, girado na direcao dela;
        // desenhada antes da colonia, que cobre a base (#229)
        if (agents[i].species == SPECIES_MOLD && agents[i].hypha_len > 0)
        {
            float tx;
            float ty;
            float angle = atan2f(agents[i].hypha_dy, agents[i].hypha_dx);
            // hifa para a esquerda: espelha na vertical para o brilho ficar em cima
            int flip = agents[i].hypha_dx < 0.0f ? ALLEGRO_FLIP_VERTICAL : 0;
            int k;

            for (k = 1; k <= agents[i].hypha_len; k++)
            {
                hypha_segment(i, k, &tx, &ty);
                al_draw_rotated_bitmap(hypha_sprite,
                    (float)al_get_bitmap_width(hypha_sprite) / 2.0f,
                    (float)al_get_bitmap_height(hypha_sprite) / 2.0f,
                    view_x(tx), view_y(ty), angle, flip);
            }
        }

        // fungos: silhueta ate a 1a leitura, depois o sprite no tamanho dele
        // (levedura 40, colonia 48, esporo 8); a hifa acima aparece desde o inicio (#232)
        if (species_is_fungus(agents[i].species))
        {
            if (agents[i].layers == 0)
            {
                draw_fungus_silhouette(i);
            }
            else
            {
                ALLEGRO_BITMAP* bmp = species_sprite(agents[i].species, agents[i].variant);

                al_draw_bitmap(bmp, view_x(agents[i].x) - (float)(al_get_bitmap_width(bmp) / 2),
                    view_y(agents[i].y) - (float)(al_get_bitmap_height(bmp) / 2), 0);
            }

            draw_fungus_marks(i);
            continue;
        }

        // virus e celula em escala biologica: 16 px o virus e 64 px a celula,
        // e o sprite ja tem esse tamanho (#235, #241)
        {
            ALLEGRO_BITMAP* bmp = species_sprite(agents[i].species, agents[i].variant);
            float size = 2.0f * agent_radius(i)
                + (agents[i].species == SPECIES_CELL ? 8.0f : 2.0f);
            ALLEGRO_COLOR tint = al_map_rgb_f(1.0f, 1.0f, 1.0f);

            al_draw_tinted_scaled_bitmap(bmp, tint, 0.0f, 0.0f,
                (float)al_get_bitmap_width(bmp), (float)al_get_bitmap_height(bmp),
                view_x(agents[i].x) - size / 2.0f, view_y(agents[i].y) - size / 2.0f,
                size, size, 0);
        }

        // celula infectada (#85, #87): o virus pisca la dentro, no ritmo de
        // LYSOGENIC_BLINK; no eclipse ele ja fez copias e a celula pulsa vermelha,
        // o aviso de que vai romper; depois da leitura, o arco ambar mostra o tempo
        if (agents[i].species == SPECIES_CELL && cell_virus(i) != -1)
        {
            int v = cell_virus(i);
            float cx = view_x(agents[i].x);
            float cy = view_y(agents[i].y);
            ALLEGRO_BITMAP* vb = species_sprite(SPECIES_VIRUS, agents[v].variant);
            float vw = (float)al_get_bitmap_width(vb);
            float vh = (float)al_get_bitmap_height(vb);
            bool eclipse = agents[v].virus_state == VIRUS_ECLIPSE;

            if (eclipse)
            {
                float a = 0.22f + 0.18f * sinf((float)match_time * 18.0f);

                al_draw_filled_circle(cx, cy, agent_radius(i) + 2.0f,
                    al_map_rgba_f(0.9f * a, 0.2f * a, 0.25f * a, a));
                al_draw_circle(cx, cy, agent_radius(i) + 4.0f, UI_RED, 2.0f);
                al_draw_scaled_bitmap(vb, 0.0f, 0.0f, vw, vh, cx + 2.0f, cy - 16.0f, 12.0f, 12.0f, 0);
                al_draw_scaled_bitmap(vb, 0.0f, 0.0f, vw, vh, cx - 14.0f, cy - 4.0f, 12.0f, 12.0f, 0);
                al_draw_scaled_bitmap(vb, 0.0f, 0.0f, vw, vh, cx + 4.0f, cy + 4.0f, 12.0f, 12.0f, 0);
            }
            else if ((int)(match_time / LYSOGENIC_BLINK) % 2 == 0)
            {
                // fundo escuro: o virus nao se perde entre as organelas
                al_draw_filled_circle(cx + 11.0f, cy - 7.0f, 9.0f, al_map_rgba_f(0.04f, 0.06f, 0.1f, 0.75f));
                al_draw_scaled_bitmap(vb, 0.0f, 0.0f, vw, vh, cx + 4.0f, cy - 14.0f, 14.0f, 14.0f, 0);
            }

            if (agents[i].layers > 0 && !eclipse)
            {
                float left = fminf(agents[v].lysogenic_timer / LYSOGENIC_TIME, 1.0f);

                al_draw_arc(cx, cy, agent_radius(i) + 6.0f, -ALLEGRO_PI / 2.0f,
                    left * 2.0f * ALLEGRO_PI, UI_AMBER, 2.0f);
            }
        }

        // celula colonizada: barra curta da vida dela, acima do sprite (#235)
        if (agents[i].species == SPECIES_CELL && agents[i].cell_health < CELL_HEALTH)
        {
            float bx = view_x(agents[i].x) - 16.0f;
            float by = view_y(agents[i].y) - agent_radius(i) - 8.0f;

            al_draw_filled_rectangle(bx, by, bx + 32.0f, by + 3.0f, UI_SLOT);
            al_draw_filled_rectangle(bx, by, bx + 32.0f * agents[i].cell_health / CELL_HEALTH,
                by + 3.0f, UI_RED);
        }
    }

    // dica do tutorial presa ao agente: some no primeiro scan, volta no tiro sem leitura (#219)
    if (tutorial_hint_target() != -1)
    {
        const char* hint = "[botão direito]: escanear";
        int hint_w = marked_text_width(hint);
        float hx = floorf(view_x(agents[tutorial_hint_target()].x) * WORLD_SCALE - (float)hint_w / 2.0f);
        float hy = (view_y(agents[tutorial_hint_target()].y) - agent_radius(tutorial_hint_target()))
            * WORLD_SCALE - 40.0f;

        // a placa e da interface: desenhada na tela, so com o tremor
        al_use_transform(&shake);

        // a tampa de tecla de 28 px cabe com folga; a moldura neutra nao briga
        // com a borda ciano da tecla
        draw_panel(hx - 8.0f, hy - 12.0f, (float)hint_w + 16.0f, 40.0f, UI_INK, UI_FRAME);
        draw_marked_text(hx, hy, UI_CYAN, hint);
        al_use_transform(&world);
    }

    // a sombra do nanobo fica sob o anel magnetico, que flutua embaixo (#220, #240)
    al_draw_filled_ellipse(view_x(player.x), view_y(player.y) + 21.0f,
        13.0f, 3.0f, CONTACT_SHADOW);

    // o nanobo e desenhado onde o mundo deixou ele, ja descontada a camera
    // a linha do spritesheet e a direcao quando a arte tem as quatro (#30)
    int nanobo_row = (al_get_bitmap_height(nanobo) >= 4 * PLAYER_FRAME) ? (int)player.facing : 0;

    // o nanobo pisca enquanto a invulnerabilidade dura (#221)
    if (invulnerability_left() > 0.0f && (int)(invulnerability_left() * 12.0f) % 2 == 1)
    {
        nanobo_tint = al_map_rgba_f(0.3f, 0.3f, 0.3f, 0.3f);
    }

    // quadro de 48 px centrado no nanobo: a cabeca e o corpo ficam no circulo
    // de colisao e o anel magnetico flutua logo abaixo (#240)
    al_draw_tinted_bitmap_region(nanobo, nanobo_tint, (float)(player_frame() * PLAYER_FRAME),
        (float)(nanobo_row * PLAYER_FRAME), (float)PLAYER_FRAME, (float)PLAYER_FRAME,
        view_x(player.x) - (float)PLAYER_FRAME / 2.0f, view_y(player.y) - (float)PLAYER_FRAME / 2.0f, 0);

    // anel da defesa: ciano enquanto protege, apagado durante a recarga (#17);
    // no raio do escudo que destroi os projeteis (#236)
    if (defense_state() == 1)
    {
        al_draw_circle(view_x(player.x), view_y(player.y), SHIELD_RADIUS, UI_CYAN, 2);
    }
    else if (defense_state() == 2)
    {
        al_draw_circle(view_x(player.x), view_y(player.y), SHIELD_RADIUS,
            UI_FRAME, 2);
    }

    // projetil de cor neutra, a forma mostra o mecanismo (RF-04, #242): cunha
    // da Ruptura, gota da Dissolucao, losango da Reserva, Y da Neutralizacao e
    // anel da Lise; o anel liso da Perfuracao e a 6a linha da folha; o desenho
    // aponta para a direita e gira na direcao do voo
    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        int frame = (int)(match_time / 0.08) % 2;

        if (!projectiles[i].active)
        {
            continue;
        }

        px = view_x(projectiles[i].x);
        py = view_y(projectiles[i].y);

        al_draw_tinted_scaled_rotated_bitmap_region(shots_sheet,
            (float)(frame * SHOT_FRAME), (float)((int)projectiles[i].laser * SHOT_FRAME),
            (float)SHOT_FRAME, (float)SHOT_FRAME, al_map_rgb_f(1.0f, 1.0f, 1.0f),
            (float)SHOT_FRAME / 2.0f, (float)SHOT_FRAME / 2.0f, px, py, 1.0f, 1.0f,
            atan2f(projectiles[i].dy, projectiles[i].dx), 0);
    }

    // projeteis inimigos: a toxina e uma gota verde-amarela com contorno (#236, #241)
    for (i = 0; i < MAX_ENEMY_SHOTS; i++)
    {
        if (!enemy_shots[i].active)
        {
            continue;
        }

        px = view_x(enemy_shots[i].x);
        py = view_y(enemy_shots[i].y);

        // virus da rajada: o proprio sprite do virus, pequeno, em voo (#86)
        if (enemy_shots[i].kind == SHOT_VIRION)
        {
            al_draw_scaled_bitmap(virus_sprite, 0.0f, 0.0f,
                (float)al_get_bitmap_width(virus_sprite), (float)al_get_bitmap_height(virus_sprite),
                px - 6.0f, py - 6.0f, 12.0f, 12.0f, 0);
            continue;
        }

        // esporo do bolor em voo: o conidio de 8x8, o mesmo do pousado (#229)
        if (enemy_shots[i].kind == SHOT_SPORE)
        {
            al_draw_bitmap(spore_sprite, px - 4.0f, py - 4.0f, 0);
            continue;
        }

        // toxina: a gota de 8x8 centrada no projetil (#241)
        al_draw_bitmap(toxin_sprite, px - 4.0f, py - 4.0f, 0);
    }

    draw_feel_world();

    // o resto da tela nao treme
    al_identity_transform(&shake);
    al_use_transform(&shake);

    // flash vermelho curto na faixa do mundo ao levar dano (#221)
    if (feel_flash_time > 0.0f)
    {
        float a = 0.35f * feel_flash_time / FEEL_FLASH_TIME;

        al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)PLAY_H,
            al_map_rgba_f(0.9f * a, 0.1f * a, 0.1f * a, a));
    }

    // ficha do scanner so com alvo (#243): calculada aqui para o aviso desviar dela
    card_target = scan_card_target(&card_from_cursor);
    card_y = card_target != -1 ? scan_card_y(card_target) : -1.0f;

    // feedback educacional curto da ultima decisao (#178), com placa de fundo
    message = feedback_text();

    if (message != NULL)
    {
        int text_w = marked_text_width(message);
        float box_x = floorf(((float)SCREEN_W - (float)text_w) / 2.0f);
        float box_y = (float)(PLAY_H - 60);

        // com a ficha embaixo, o aviso sobe para cima dela
        if (card_target != -1 && card_y > (float)PLAY_H / 2.0f)
        {
            box_y = card_y - 44.0f;
        }

        draw_panel(box_x - UI_PAD, box_y - 8.0f, (float)text_w + 2.0f * UI_PAD, 32.0f, UI_INK, UI_FRAME);
        draw_marked_text(box_x, box_y, UI_WARM, message);
    }

    // o aviso de objetivo fica no topo; o feedback continua embaixo (#246)
    draw_objective_notice(card_target, card_y);

    // marca o destino quando ele esta fora da faixa visivel (#19), sem passar
    // por baixo da ficha do scanner
    if (card_target == -1)
    {
        draw_destination_hint(0.0f, (float)PLAY_H);
    }
    else if (card_y > (float)PLAY_H / 2.0f)
    {
        draw_destination_hint(0.0f, card_y);
    }
    else
    {
        draw_destination_hint(card_y + (float)CARD_H, (float)PLAY_H);
    }

    // a ficha fica dentro do recorte do mundo: nada dela vaza para o HUD
    if (card_target != -1)
    {
        draw_scan_card(card_target, card_from_cursor, card_y);
    }

    // mira livre: o cursor do sistema some na fase e vira esta marca (#198);
    // o quadro diz se o tiro sai agora e a cor e a da arma (#246); com a caixa
    // do diario aberta ela some
    aim_x = to_logical_x(mouse_x);
    aim_y = to_logical_y(mouse_y);

    if (game_get_scene() == GAME_SCENE_GAME && pending_card() == CARD_NONE
        && aim_x >= 0.0f && aim_x < (float)SCREEN_W
        && aim_y >= 0.0f && aim_y < (float)PLAY_H)
    {
        ui_tinted_bitmap_region(aim_sprite, laser_color(player.laser),
            (float)((int)aim_frame() * AIM_FRAME_SIZE), 0.0f,
            (float)AIM_FRAME_SIZE, (float)AIM_FRAME_SIZE,
            floorf(aim_x) - (float)(AIM_CENTER * UI_SCALE),
            floorf(aim_y) - (float)(AIM_CENTER * UI_SCALE), 0);
    }

#ifdef _DEBUG
    // F1 mostra o estado e o tempo de cada virus da fase (#84)
    if (show_debug)
    {
        char line[96];
        int used = snprintf(line, sizeof(line), "QA virus");

        for (i = 0; i < agents_total() && used > 0 && used < (int)sizeof(line); i++)
        {
            if (agents[i].species != SPECIES_VIRUS || !agents[i].active)
            {
                continue;
            }

            used += snprintf(line + used, sizeof(line) - (size_t)used, " %d:%s%.1f",
                agents[i].id, agents[i].virus_state == VIRUS_LYSOGENIC ? "L"
                    : agents[i].virus_state == VIRUS_ECLIPSE ? "E" : "F",
                agents[i].lysogenic_timer);
        }

        ui_text(UI_DIM, 16, 16, 0, line);
    }
#endif

    // o clip do painel nao pode intersectar o clip do mundo, senao o HUD some
    al_reset_clipping_rectangle();

    draw_hud();
}
