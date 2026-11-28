// resposta visual aos eventos (#221): tremor, flash, anel, impacto, pop, particulas e hit-stop
#define _USE_MATH_DEFINES
#include "../ui/ui.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// tremor de cada nivel, em pixels do canvas
static const float feel_shake_power_by_level[3] = { 0.0f, 1.0f, 3.0f };

unsigned int feel_seen = 0;
float feel_shake_time = 0.0f;
float feel_shake_power = 0.0f;
float feel_flash_time = 0.0f;
float feel_ring_time = 0.0f;
float feel_ring_x = 0.0f;
float feel_ring_y = 0.0f;
float feel_impact_time = 0.0f;
float feel_impact_x = 0.0f;
float feel_impact_y = 0.0f;
bool feel_impact_dull = false;

// linha da folha de impactos: 0 dose certa, 1 sem efeito, 2 eliminado (#242)
int feel_impact_kind = 0;
float feel_pop_time = 0.0f;
int feel_pop_points = 0;
float feel_pop_x = 0.0f;
float feel_pop_y = 0.0f;
double feel_hit_stop = 0.0;
FeelParticle feel_particles[FEEL_PARTICLES];

// intensidade de cada evento: quanto mais decide a partida, mais forte
static FeelLevel feel_level(SfxEvent kind)
{
    if (kind == SFX_EVENT_DAMAGE || kind == SFX_EVENT_NEUTRALIZED || kind == SFX_EVENT_PHASE_DONE)
    {
        return FEEL_STRONG;
    }

    if (kind == SFX_EVENT_HIT || kind == SFX_EVENT_WRONG)
    {
        return FEEL_MEDIUM;
    }

    return FEEL_LIGHT;
}

// le o evento novo do gameplay e liga a resposta dele; nao usa rand(),
// para nao mexer na sequencia que o gameplay sorteia
void feel_trigger()
{
    SfxEvent kind;
    FeelLevel level;
    float x;
    float y;
    int i;

    if (sfx_event_id() == feel_seen)
    {
        return;
    }

    feel_seen = sfx_event_id();
    kind = sfx_event_kind();
    level = feel_level(kind);
    sfx_event_position(&x, &y);

    if (feel_shake_power_by_level[level] >= feel_shake_power || feel_shake_time <= 0.0f)
    {
        feel_shake_power = feel_shake_power_by_level[level];
        feel_shake_time = FEEL_SHAKE_TIME;
    }

    if (kind == SFX_EVENT_SCAN)
    {
        // anel do scan sai do nanobo ate o alcance do scanner
        feel_ring_time = FEEL_RING_TIME;
        feel_ring_x = player.x;
        feel_ring_y = player.y;
    }
    else if (kind == SFX_EVENT_HIT || kind == SFX_EVENT_WRONG)
    {
        // dose certa pisca claro; sem efeito pisca apagado
        feel_impact_time = FEEL_IMPACT_TIME;
        feel_impact_x = x;
        feel_impact_y = y;
        feel_impact_dull = kind == SFX_EVENT_WRONG;
        feel_impact_kind = feel_impact_dull ? 1 : 0;
    }
    else if (kind == SFX_EVENT_DAMAGE)
    {
        feel_flash_time = FEEL_FLASH_TIME;
    }
    else if (kind == SFX_EVENT_NEUTRALIZED)
    {
        feel_impact_time = FEEL_IMPACT_TIME;
        feel_impact_x = x;
        feel_impact_y = y;
        feel_impact_dull = false;
        feel_impact_kind = 2;
        feel_pop_time = FEEL_POP_TIME;
        feel_pop_points = last_kill_points();
        feel_pop_x = x;
        feel_pop_y = y;
        feel_hit_stop = FEEL_HIT_STOP;

        for (i = 0; i < FEEL_PARTICLES; i++)
        {
            float angle = (float)i * 2.0f * ALLEGRO_PI / (float)FEEL_PARTICLES;
            float speed = 70.0f + (float)(i % 3) * 25.0f;

            feel_particles[i].x = x;
            feel_particles[i].y = y;
            feel_particles[i].vx = cosf(angle) * speed;
            feel_particles[i].vy = sinf(angle) * speed;
            feel_particles[i].life = FEEL_PARTICLE_TIME;
        }
    }
}

// os efeitos decaem sozinhos; nada aqui toca no gameplay
void feel_update(float dt)
{
    int i;

    feel_shake_time = fmaxf(feel_shake_time - dt, 0.0f);
    feel_flash_time = fmaxf(feel_flash_time - dt, 0.0f);
    feel_ring_time = fmaxf(feel_ring_time - dt, 0.0f);
    feel_impact_time = fmaxf(feel_impact_time - dt, 0.0f);
    feel_pop_time = fmaxf(feel_pop_time - dt, 0.0f);

    for (i = 0; i < FEEL_PARTICLES; i++)
    {
        if (feel_particles[i].life <= 0.0f)
        {
            continue;
        }

        feel_particles[i].x = feel_particles[i].x + feel_particles[i].vx * dt;
        feel_particles[i].y = feel_particles[i].y + feel_particles[i].vy * dt;
        feel_particles[i].vx = feel_particles[i].vx * 0.92f;
        feel_particles[i].vy = feel_particles[i].vy * 0.92f;
        feel_particles[i].life = feel_particles[i].life - dt;
    }
}

// deslocamento do tremor: decai ate zero e so mexe no desenho do mundo
float feel_offset_x()
{
    if (feel_shake_time <= 0.0f)
    {
        return 0.0f;
    }

    return roundf(feel_shake_power * (feel_shake_time / FEEL_SHAKE_TIME) * sinf(feel_shake_time * 95.0f));
}

float feel_offset_y()
{
    if (feel_shake_time <= 0.0f)
    {
        return 0.0f;
    }

    return roundf(feel_shake_power * (feel_shake_time / FEEL_SHAKE_TIME) * cosf(feel_shake_time * 71.0f));
}

// efeitos presos ao mundo: anel do scan, impacto, particulas e "+10"
void draw_feel_world()
{
    int i;
    char pop[16];

    if (feel_ring_time > 0.0f)
    {
        float t = 1.0f - feel_ring_time / FEEL_RING_TIME;
        float a = feel_ring_time / FEEL_RING_TIME;
        float r = (float)PLAYER_RADIUS + t * ((float)SCAN_RANGE - (float)PLAYER_RADIUS);

        al_draw_circle(view_x(feel_ring_x), view_y(feel_ring_y), r,
            al_map_rgba_f(0.435f * a, 0.906f * a, 0.949f * a, a), 2.0f);
    }

    // impacto do resultado: dose certa, sem efeito ou eliminado, 4 quadros (#242)
    if (feel_impact_time > 0.0f)
    {
        int frame = (int)((1.0f - feel_impact_time / FEEL_IMPACT_TIME) * 4.0f);

        if (frame > 3)
        {
            frame = 3;
        }

        al_draw_bitmap_region(impacts_sheet, (float)(frame * IMPACT_FRAME),
            (float)(feel_impact_kind * IMPACT_FRAME), (float)IMPACT_FRAME, (float)IMPACT_FRAME,
            view_x(feel_impact_x) - (float)IMPACT_FRAME / 2.0f,
            view_y(feel_impact_y) - (float)IMPACT_FRAME / 2.0f, 0);
    }

    for (i = 0; i < FEEL_PARTICLES; i++)
    {
        float a;

        if (feel_particles[i].life <= 0.0f)
        {
            continue;
        }

        a = feel_particles[i].life / FEEL_PARTICLE_TIME;
        al_draw_filled_rectangle(view_x(feel_particles[i].x) - 1.0f, view_y(feel_particles[i].y) - 1.0f,
            view_x(feel_particles[i].x) + 1.0f, view_y(feel_particles[i].y) + 1.0f,
            al_map_rgba_f(1.0f * a, 0.906f * a, 0.659f * a, a));
    }

    if (feel_pop_time > 0.0f && feel_pop_points > 0)
    {
        float a = feel_pop_time / FEEL_POP_TIME;
        float rise = (1.0f - a) * 24.0f;

        snprintf(pop, sizeof(pop), "+%d", feel_pop_points);
        al_draw_text(font, al_map_rgba_f(0.373f * a, 0.839f * a, 0.627f * a, a),
            view_x(feel_pop_x), view_y(feel_pop_y) - (float)AGENT_RADIUS - 12.0f - rise,
            ALLEGRO_ALIGN_CENTRE, pop);
    }
}
