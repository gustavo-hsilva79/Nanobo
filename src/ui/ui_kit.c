// kit de interface: conversao do mouse, paineis, botoes, texto com marcas, tecla em caixinha e caixa de dialogo
#define _USE_MATH_DEFINES
#include "ui.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------------
// kit de interface: conversao, painel, botao, indicadores e linhas
// ---------------------------------------------------------------

// pixel da janela para pixel logico do canvas
float to_logical_x(float window_x)
{
    return (window_x - screen_off_x) / screen_scale;
}

float to_logical_y(float window_y)
{
    return (window_y - screen_off_y) / screen_scale;
}

// painel: fundo chapado e moldura de 1 px alinhada ao pixel
void draw_panel(float x, float y, float w, float h, ALLEGRO_COLOR fill, ALLEGRO_COLOR border)
{
    al_draw_filled_rectangle(x, y, x + w, y + h, fill);
    al_draw_rectangle(x + 0.5f, y + 0.5f, x + w - 0.5f, y + h - 0.5f, border, 1.0f);
}

// regua fina sob o titulo de uma tela
void draw_rule(float center_x, float y, float w, ALLEGRO_COLOR color)
{
    al_draw_line(floorf(center_x - w / 2.0f), y + 0.5f,
        floorf(center_x + w / 2.0f), y + 0.5f, color, 1.0f);
}

// 0 normal, 1 foco do teclado, 2 cursor em cima
static void draw_button(const UiButton* button, int state)
{
    ALLEGRO_COLOR border = UI_DIM;
    ALLEGRO_COLOR label = UI_DIM;

    if (state >= 1)
    {
        border = UI_AMBER;
        label = UI_TEXT;
    }

    if (state == 2)
    {
        al_draw_filled_rectangle(button->x, button->y, button->x + button->w,
            button->y + button->h, UI_HOVER);
    }

    al_draw_rectangle(button->x + 0.5f, button->y + 0.5f,
        button->x + button->w - 0.5f, button->y + button->h - 0.5f, border, 1.0f);
    al_draw_text(font, label, floorf(button->x + button->w / 2.0f),
        floorf(button->y + (button->h - (float)al_get_font_line_height(font)) / 2.0f),
        ALLEGRO_ALIGN_CENTRE, button->label);
}

// ponto em pixel logico dentro do retangulo do botao
int button_hit(const UiButton* button, float x, float y)
{
    return x >= button->x && x <= button->x + button->w
        && y >= button->y && y <= button->y + button->h;
}

// botoes da tela atual: desenho e clique leem a mesma lista
const UiButton* screen_buttons(int* count)
{
    static const UiButton menu_items[] = {
        { (float)BUTTON_X - 180.0f, (float)BUTTON_Y, (float)BUTTON_W, (float)BUTTON_H, "INICIAR" },
        { (float)BUTTON_X, (float)BUTTON_Y, (float)BUTTON_W, (float)BUTTON_H, "COMO JOGAR" },
        { (float)BUTTON_X + 180.0f, (float)BUTTON_Y, (float)BUTTON_W, (float)BUTTON_H, "RECORDES" }
    };
    static const UiButton pause_items[] = {
        { 152.0f, 222.0f, 160.0f, 28.0f, "CONTINUAR" },
        { 328.0f, 222.0f, 160.0f, 28.0f, "COMO JOGAR" }
    };
    static const UiButton results_items[] = {
        { 240.0f, 256.0f, 160.0f, 28.0f, "CONTINUAR" }
    };
    static const UiButton victory_items[] = {
        { 240.0f, 276.0f, 160.0f, 28.0f, "NOVA PARTIDA" }
    };
    static const UiButton over_items[] = {
        { 240.0f, 216.0f, 160.0f, 28.0f, "NOVA PARTIDA" }
    };

    *count = 1;

    switch (game_get_scene())
    {
        case GAME_SCENE_MENU:
            *count = 3;
            return menu_items;

        case GAME_SCENE_PAUSE:
            *count = 2;
            return pause_items;

        case GAME_SCENE_RESULTS:
            return results_items;

        case GAME_SCENE_VICTORY:
            return victory_items;

        case GAME_SCENE_GAME_OVER:
            return over_items;

        default:
            *count = 0;
            return NULL;
    }
}

// desenha os botoes da tela com foco do teclado e cursor
void draw_buttons(const UiButton* buttons, int count)
{
    float lx = to_logical_x(mouse_x);
    float ly = to_logical_y(mouse_y);
    int i;

    if (buttons == NULL)
    {
        return;
    }

    for (i = 0; i < count; i++)
    {
        int state = i == ui_focus ? 1 : 0;

        if (button_hit(&buttons[i], lx, ly))
        {
            state = 2;
        }

        draw_button(&buttons[i], state);
    }
}

// anda o foco entre os botoes da tela, dando a volta nas pontas
void ui_move_focus(int step)
{
    int count;
    const UiButton* buttons = screen_buttons(&count);

    if (buttons == NULL || count <= 1)
    {
        return;
    }

    ui_focus = (ui_focus + step + count) % count;
}

// aciona o botao da tela atual: o efeito e da cena, nao do botao
void activate_button(int index)
{
    int count;
    const UiButton* buttons = screen_buttons(&count);

    if (buttons == NULL || index < 0 || index >= count)
    {
        return;
    }

    switch (game_get_scene())
    {
        case GAME_SCENE_MENU:
            // COMO JOGAR abre a cena propria, sempre na primeira pagina
            if (index == 1)
            {
                howto_page = 0;
                game_set_scene(GAME_SCENE_HOWTO);
                break;
            }

            // RECORDES abre o painel do Top 10 por cima do menu (#197)
            if (index == 2)
            {
                ranking_panel_open = true;
                break;
            }

            reset_match();
            start_opening();
            game_set_scene(GAME_SCENE_BRIEFING);
            break;

        case GAME_SCENE_PAUSE:
            // COMO JOGAR: a partida fica parada e a volta cai na pausa
            if (index == 1)
            {
                howto_page = 0;
                game_set_scene(GAME_SCENE_HOWTO);
                break;
            }

            game_set_scene(GAME_SCENE_GAME);
            break;

        case GAME_SCENE_RESULTS:
            // com duas fases, continuar avança; na última, conclui a run (#M4)
            if (has_next_phase())
            {
                start_next_phase();
                game_set_scene(GAME_SCENE_GAME);
            }
            else
            {
                // so a vitoria grava (#34): o nome e pedido quando a pontuacao
                // entra no Top 10 (#81)
                name_entry_active = ranking_qualifies(score);
                name_entry[0] = '\0';
                name_entry_chars = 0;
                ranking_highlight = -1;
                ranking_save_failed = false;
                game_set_scene(GAME_SCENE_VICTORY);
            }
            break;

        case GAME_SCENE_VICTORY:
        case GAME_SCENE_GAME_OVER:
            // briefing antes de toda partida, pulavel com ESC
            reset_match();
            start_opening();
            game_set_scene(GAME_SCENE_BRIEFING);
            break;

        default:
            break;
    }
}

// anotacoes do diario e interludios (#222), registrados em textos-do-jogo.md §3.1
static const DialogEntry diary_entries[CARD_COUNT] =
{
    [CARD_INTERLUDE_2] =
    {
        "Enquanto isso, no hospital", 2,
        {
            {
                "A infecção no sangue de Léo cedeu. Mas, na enfermaria,",
                "ele pegou um {vírus respiratório}.",
                NULL
            },
            {
                "O Nanobô segue para as {vias aéreas} dele, onde o vírus",
                "infecta as células que forram os brônquios.",
                NULL
            }
        },
        false
    },
    [CARD_INTERLUDE_3] =
    {
        "Enquanto isso, no hospital", 3,
        {
            {
                "O vírus foi contido, mas Léo segue com as {defesas} fracas.",
                NULL,
                NULL
            },
            {
                "{Esporos de bolor}, que estão no ar,",
                "chegam ao pulmão dele.",
                NULL
            },
            {
                "O Nanobô desce até o {pulmão} para agir antes que",
                "o fungo se instale.",
                NULL
            }
        },
        false
    },
    [CARD_GRAM] =
    {
        "Coloração de Gram", 4,
        {
            {
                "Li uma bactéria até o fim! A forma não basta:",
                "existe coco <p:Gram+> e coco <r:Gram->. Quem mostra o grupo",
                "é a {coloração de Gram}, um corante de laboratório."
            },
            {
                "O corante roxo fica preso ou sai conforme a parede.",
                "<p:Gram+>: parede grossa de {peptidoglicano}, o roxo fica.",
                "<r:Gram->: parede fina e membrana externa; perde o roxo, fica rosa."
            },
            {
                "Anotado: no jogo, todas aparecem juntas no sangue para",
                "{simplificar}. A Moraxella vive nas vias respiratórias,",
                "e o Clostridium tetani entra por ferimentos."
            },
            {
                "Minhas armas são {regra do jogo}, inspiradas em antibióticos",
                "reais, como a penicilina. Na vida real há exceções:",
                "muitas cepas de S. aureus resistem à penicilina."
            }
        },
        true
    },
    [CARD_RESISTANCE] =
    {
        "Resistência e Reserva", 4,
        {
            {
                "Errei... anotado! Dei uma dose e demorei para dar a outra.",
                "Esta bactéria {resistiu}: o mesmo antibiótico não age",
                "mais nela nem nas filhas dela."
            },
            {
                "Regra do jogo: duas doses, a segunda em até 6 segundos.",
                "Na vida real, um tratamento mal feito faz {seleção}:",
                "morrem as sensíveis e sobram as que já resistiam."
            },
            {
                "As sobreviventes se multiplicam e passam a resistência",
                "para as filhas. Com antibiótico de verdade, a regra é:",
                "{siga a prescrição até o fim}."
            },
            {
                "A {Reserva} age na resistente, mas tem 2 cargas por fase.",
                "Até ela pode deixar de funcionar se for usada à toa.",
                "Vou guardar para quando precisar!"
            }
        },
        true
    },
    [CARD_FLAGELLUM] =
    {
        "Flagelo", 3,
        {
            {
                "Ei! Esse bacilo veio atrás de mim e me atacou!",
                "Ele tem {flagelo}, um chicote que gira e o faz nadar.",
                NULL
            },
            {
                "O coco não nada sozinho: ele {gruda} no tecido",
                "e fica ali, soltando toxinas que ferem as células.",
                NULL
            },
            {
                "Anotado: na natureza há {exceções}. Aqui, todo bacilo",
                "do jogo nada e todo coco gruda.",
                NULL
            }
        },
        true
    },
    [CARD_BIOFILM] =
    {
        "Biofilme", 3,
        {
            {
                "Fiquei preso numa gosma! Bactérias grudadas fazem",
                "uma camada pegajosa: o {biofilme}.",
                NULL
            },
            {
                "Dentro dele, o antibiótico {age menos}: entra devagar,",
                "e as bactérias de dentro funcionam mais devagar.",
                NULL
            },
            {
                "Regra do jogo: no biofilme a bactéria pede uma {dose} a mais,",
                "e eu ando devagar.",
                NULL
            }
        },
        true
    },
    [CARD_ENDOTOXIN] =
    {
        "Endotoxina", 3,
        {
            {
                "Ai, esta nuvem me fere! Ela veio de uma <r:Gram-> eliminada.",
                "A membrana externa da <r:Gram-> guarda uma {toxina}.",
                NULL
            },
            {
                "Quando a <r:Gram-> morre ou se divide, essa toxina se solta",
                "e causa {inflamação} e febre no corpo.",
                NULL
            },
            {
                "Regra do jogo: a nuvem me fere enquanto dura.",
                "Anotado: {sair da nuvem} bem rápido!",
                NULL
            }
        },
        true
    },
    [CARD_VIRUS] =
    {
        "Vírus não é célula", 4,
        {
            {
                "Um {vírus}! Ele não é célula: não tem parede,",
                "nem metabolismo, nem ribossomos próprios.",
                NULL
            },
            {
                "Ele só se multiplica {dentro de uma célula},",
                "usando os ribossomos dela.",
                NULL
            },
            {
                "{Antibiótico} não age nele: não trata gripe nem resfriado.",
                "Se gastei um tiro de antibiótico aqui, errei... anotado!",
                NULL
            },
            {
                "A <wN:Neutralização> leva {anticorpos}: eles se prendem ao vírus",
                "livre e o impedem de entrar nas células.",
                "No corpo, quem faz anticorpos são os linfócitos B."
            }
        },
        true
    },
    [CARD_CYCLE] =
    {
        "Lisogênico e lítico", 4,
        {
            {
                "Esta célula do Léo tem um vírus escondido dentro!",
                "{Lisogênico}: o vírus fica na célula sem destruí-la ainda.",
                NULL
            },
            {
                "{Lítico}: o vírus faz cópias, a célula se rompe",
                "e libera vírus novos, que infectam outras células.",
                NULL
            },
            {
                "O {Ataque T} destrói a célula infectada antes que ela",
                "se rompa, como faz o linfócito T no corpo.",
                "No jogo, ele custa um pouco dos vitais do Léo."
            },
            {
                "Anotado: lisogênico e lítico foram descritos em vírus de",
                "bactérias, os {bacteriófagos}. Aqui servem de comparação.",
                "Em nós, o mais parecido é o vírus da catapora, nos nervos."
            }
        },
        true
    },
    [CARD_WEAKENED] =
    {
        "Organismo enfraquecido", 3,
        {
            {
                "Estes {esporos} são de Aspergillus, um bolor comum no ar.",
                "A maioria das pessoas respira esporos e não adoece.",
                NULL
            },
            {
                "Fungos {oportunistas} se instalam com mais facilidade",
                "em quem está com a defesa enfraquecida.",
                "O risco vem da defesa fraca, não de respirar mais esporos."
            },
            {
                "O Léo está com a {defesa} fraca. Regra do jogo: aqui,",
                "defesa mais fraca aparece como mais esporos no chão.",
                NULL
            }
        },
        true
    },
    [CARD_FUNGUS] =
    {
        "Fungo", 4,
        {
            {
                "Um fungo! Ele tem núcleo, como as células do Léo:",
                "é um {eucarionte}, bem diferente de uma bactéria.",
                NULL
            },
            {
                "A parede dele é de {quitina}, não de peptidoglicano.",
                "Antibiótico de bactéria não tem alvo nele.",
                "Se gastei antibiótico aqui, errei... anotado!"
            },
            {
                "A membrana dele tem {ergosterol}; a nossa tem colesterol.",
                "Essa diferença é o alvo de muitos antifúngicos.",
                NULL
            },
            {
                "A {Perfuração} se inspira na anfotericina B: ela se prende",
                "ao ergosterol e abre poros na membrana do fungo.",
                "Esse remédio de verdade também pode fazer mal ao paciente."
            }
        },
        true
    }
};

// estado da caixa da fila de cards e a entrada que ela mostra
DialogState card_dialog;
ConceptCard card_dialog_entry = CARD_NONE;
// caractere que so marca o texto e nao e desenhado: {termo}, [tecla] e o fim
// da marca de cor <c:...>
static bool is_mark_char(char c)
{
    return c == '{' || c == '}' || c == '[' || c == ']' || c == '>';
}


// marca de cor <c:texto> (27/11): devolve os bytes de "<c:" (codigo de 1 ou
// 2 letras) ou 0 se ali nao comeca uma marca
static int color_mark_length(const char* c)
{
    int code = 0;

    if (*c != '<')
    {
        return 0;
    }

    while (code < 2 && c[1 + code] != '\0' && c[1 + code] != ':')
    {
        code = code + 1;
    }

    if (code == 0 || c[1 + code] != ':')
    {
        return 0;
    }

    return code + 2;
}

static ALLEGRO_COLOR color_mark_color(const char* c)
{
    if (c[1] == 'p')
    {
        return UI_PURPLE;
    }
    if (c[1] == 'r')
    {
        return UI_PINK;
    }
    if (c[1] == 'g')
    {
        return UI_GREEN;
    }
    if (c[1] == 'x')
    {
        return UI_RED;
    }

    switch (c[2])
    {
    case 'R':
        return laser_color(LASER_GRAM_POSITIVE);
    case 'D':
        return laser_color(LASER_GRAM_NEGATIVE);
    case 'S':
        return laser_color(LASER_RESERVE);
    case 'N':
        return laser_color(LASER_NEUTRALIZE);
    case 'T':
        return laser_color(LASER_LYSE);
    default:
        return laser_color(LASER_PERFORATE);
    }
}

// caracteres que aparecem na tela: conta por letra utf-8 e pula chaves,
// colchetes e as marcas de cor
static int dialog_visible_chars(const char* line)
{
    int count = 0;
    const unsigned char* c = (const unsigned char*)line;

    if (line == NULL)
    {
        return 0;
    }

    for (; *c != '\0'; c++)
    {
        int skip = color_mark_length((const char*)c);

        if (skip > 0)
        {
            c = c + skip - 1;
            continue;
        }

        if (!is_mark_char(*c) && (*c & 0xC0) != 0x80)
        {
            count = count + 1;
        }
    }

    return count;
}

// abre a caixa na pagina 1 com a contagem de letras de cada pagina
void dialog_open_entry(DialogState* d, const DialogEntry* e)
{
    int chars[DIALOG_MAX_PAGES] = { 0, 0, 0, 0 };
    int page;
    int line;

    for (page = 0; page < e->pages && page < DIALOG_MAX_PAGES; page++)
    {
        for (line = 0; line < 3; line++)
        {
            chars[page] = chars[page] + dialog_visible_chars(e->lines[page][line]);
        }
    }

    dialog_open(d, e->pages, chars);
}

// a entrada pendente mudou (nova, a seguinte da fila ou nenhuma): a caixa recomeca
void sync_card_dialog()
{
    ConceptCard card = pending_card();

    if (card == card_dialog_entry)
    {
        return;
    }

    card_dialog_entry = card;

    if (card != CARD_NONE)
    {
        dialog_open_entry(&card_dialog, &diary_entries[card]);
    }
}

// enter, espaco ou clique: completa, vira a pagina ou fecha a entrada
void press_card_dialog()
{
    sync_card_dialog();

    if (card_dialog_entry == CARD_NONE)
    {
        return;
    }

    dialog_press(&card_dialog);

    if (!card_dialog.open)
    {
        dismiss_card();
        sync_card_dialog();
    }
}

// esc fecha a entrada inteira; a seguinte da fila abre do inicio
void skip_card_dialog()
{
    // esc duplo nao pode fechar a entrada seguinte antes de ler
    if (card_dialog_entry != CARD_NONE && card_dialog.page_age < DIALOG_DEBOUNCE)
    {
        return;
    }

    dismiss_card();
    sync_card_dialog();
}

// cor com o alfa multiplicado (o blender do allegro usa alfa pre-multiplicado)
ALLEGRO_COLOR faded_color(ALLEGRO_COLOR color, float alpha)
{
    float r, g, b, a;

    al_unmap_rgba_f(color, &r, &g, &b, &a);
    return al_map_rgba_f(r * alpha, g * alpha, b * alpha, a * alpha);
}

// tampa de tecla: nome em ciano numa caixa de 14 px com borda ciano e fundo
// de tinta, KEY_CAP_PAD de folga de cada lado; devolve a largura ocupada
static float draw_key_cap(float x, float y, const char* name, float alpha)
{
    float w = (float)(al_get_text_width(font, name) + 2 * KEY_CAP_PAD);

    // 14 px de altura: a borda nao encosta nas letras nem nos acentos e cedilhas
    al_draw_filled_rectangle(x, y - 3.0f, x + w, y + 11.0f, faded_color(UI_INK, alpha));
    al_draw_rectangle(x + 0.5f, y - 2.5f, x + w - 0.5f, y + 10.5f, faded_color(UI_KEY, alpha), 1.0f);
    al_draw_text(font, faded_color(UI_KEY, alpha), x + (float)KEY_CAP_PAD, y, 0, name);

    return w;
}

// uma linha com no maximo budget letras; o termo entre chaves sai em ambar,
// a tecla entre colchetes sai em tampa de tecla, os dois com o alfa da cor
// base, a marca <c:texto> na cor do codigo (color_mark_color) com o mesmo alfa,
// e o resto na cor base
static int draw_dialog_line(const char* line, float x, float y, ALLEGRO_COLOR base, int budget)
{
    char segment[192];
    int length = 0;
    bool term = false;
    bool key = false;
    bool span = false;
    const char* c;
    float r, g, b, a;
    ALLEGRO_COLOR term_color;
    ALLEGRO_COLOR span_color;

    al_unmap_rgba_f(base, &r, &g, &b, &a);
    term_color = faded_color(UI_AMBER, a);
    span_color = base;

    for (c = line; *c != '\0'; c++)
    {
        bool lead = (*c & 0xC0) != 0x80;
        int color_mark = color_mark_length(c);
        bool mark = is_mark_char(*c) || color_mark > 0;

        if (mark || (lead && budget == 0))
        {
            segment[length] = '\0';

            if (key)
            {
                // tecla pela metade na maquina de escrever: a caixa cresce com o texto
                if (length > 0)
                {
                    x = x + draw_key_cap(x, y, segment, a);
                }
            }
            else
            {
                al_draw_text(font, term ? term_color : (span ? span_color : base), x, y, 0, segment);
                x = x + (float)al_get_text_width(font, segment);
            }
            length = 0;

            if (!mark)
            {
                return 0;
            }

            if (color_mark > 0)
            {
                // a marca de cor comeca: pula "<c:" inteiro
                span = true;
                span_color = faded_color(color_mark_color(c), a);
                c = c + color_mark - 1;
            }
            else if (*c == '>')
            {
                span = false;
            }
            else if (*c == '{' || *c == '}')
            {
                term = *c == '{';
            }
            else
            {
                key = *c == '[';
            }
            continue;
        }

        if (lead)
        {
            budget = budget - 1;
        }

        if (length < (int)sizeof(segment) - 1)
        {
            segment[length] = *c;
            length = length + 1;
        }
    }

    segment[length] = '\0';

    if (key)
    {
        if (length > 0)
        {
            draw_key_cap(x, y, segment, a);
        }
    }
    else
    {
        al_draw_text(font, term ? term_color : (span ? span_color : base), x, y, 0, segment);
    }

    return budget;
}

// linha inteira com {termo} em ambar, [tecla] em tampa e <c:texto> na cor do codigo
// (aviso de objetivo, como jogar, rodapes)
void draw_marked_text(float x, float y, ALLEGRO_COLOR base, const char* text)
{
    if (text == NULL)
    {
        return;
    }

    draw_dialog_line(text, x, y, base, dialog_visible_chars(text));
}

// largura em px da linha sem chaves, colchetes e marcas de cor, com a folga
// de cada tampa de tecla
int marked_text_width(const char* text)
{
    char plain[192];
    int length = 0;
    int caps = 0;
    const char* c;

    if (text == NULL)
    {
        return 0;
    }

    for (c = text; *c != '\0' && length < (int)sizeof(plain) - 1; c++)
    {
        int skip = color_mark_length(c);

        if (skip > 0)
        {
            c = c + skip - 1;
            continue;
        }

        if (*c == '[')
        {
            caps = caps + 1;
        }

        if (!is_mark_char(*c))
        {
            plain[length] = *c;
            length = length + 1;
        }
    }

    plain[length] = '\0';
    return al_get_text_width(font, plain) + caps * 2 * KEY_CAP_PAD;
}

// linha marcada alinhada como o al_draw_text (centro ou direita em x)
void draw_marked_text_aligned(float x, float y, ALLEGRO_COLOR base, int flags, const char* text)
{
    float w = (float)marked_text_width(text);

    if (flags & ALLEGRO_ALIGN_CENTRE)
    {
        x = floorf(x - w / 2.0f);
    }
    else if (flags & ALLEGRO_ALIGN_RIGHT)
    {
        x = x - w;
    }

    draw_marked_text(x, y, base, text);
}

// idade da pagina desde que ficou completa (natural ou por toque): o triangulo pisca daqui
static float dialog_complete_age(const DialogEntry* e, const DialogState* d)
{
    static const DialogEntry* last_entry = NULL;
    static int last_page = -1;
    static float complete_at = -1.0f;
    float natural = (float)d->page_chars[d->page] / DIALOG_CHARS_PER_SECOND;

    if (e != last_entry || d->page != last_page || d->page_age < complete_at)
    {
        last_entry = e;
        last_page = d->page;
        complete_at = -1.0f;
    }

    if (complete_at < 0.0f)
    {
        complete_at = fminf(d->page_age, natural);
    }

    return d->page_age - complete_at;
}

// caixa sobre a fase parada: veu, retrato, cabecalho, linhas que aparecem
// letra a letra e o rodape com n/N, o triangulo ou o fechar
void draw_dialog(const DialogEntry* e, const DialogState* d)
{
    int budget = (int)d->shown;
    int line;
    char counter[16];
    float right = (float)(DIALOG_X + DIALOG_W - DIALOG_PAD);
    float footer_y = (float)(DIALOG_Y + DIALOG_H - DIALOG_PAD - 8);

    al_draw_filled_rectangle(0.0f, 0.0f, (float)SCREEN_W, (float)SCREEN_H,
        UI_VEIL(0.6f));
    draw_panel((float)DIALOG_X, (float)DIALOG_Y, (float)DIALOG_W, (float)DIALOG_H, UI_INK, UI_AMBER);

    // retrato: linha de frente do spritesheet, quadro ocioso 0
    if (e->portrait && nanobo)
    {
        float px = (float)(DIALOG_X + DIALOG_PAD);
        float py = (float)(DIALOG_Y + DIALOG_PAD);

        draw_panel(px - 1.0f, py - 1.0f, (float)PLAYER_FRAME + 2.0f, (float)PLAYER_FRAME + 2.0f,
            UI_PANEL, UI_FRAME);
        al_draw_bitmap_region(nanobo, 0.0f, (float)(PLAYER_FACING_DOWN * PLAYER_FRAME),
            (float)PLAYER_FRAME, (float)PLAYER_FRAME, px, py, 0);
    }

    al_draw_text(font, UI_DIM, (float)DIALOG_TEXT_X, (float)(DIALOG_Y + DIALOG_PAD), 0, e->header);

    for (line = 0; line < 3 && budget > 0; line++)
    {
        const char* text = e->lines[d->page][line];

        if (text == NULL)
        {
            break;
        }

        budget = draw_dialog_line(text, (float)DIALOG_TEXT_X,
            (float)(DIALOG_Y + DIALOG_PAD + DIALOG_LINE_H * (line + 1)), UI_TEXT, budget);
    }

    snprintf(counter, sizeof(counter), "%d/%d", d->page + 1, d->pages);
    al_draw_text(font, UI_DIM, right, footer_y, ALLEGRO_ALIGN_RIGHT, counter);

    if (!dialog_page_complete(d))
    {
        return;
    }

    // a ultima pagina fecha; as outras piscam o triangulo a cada 0,5 s
    if (dialog_last_page(d))
    {
        al_draw_text(font, UI_AMBER, right - 32.0f, footer_y, ALLEGRO_ALIGN_RIGHT, "fechar");
    }
    else if (fmodf(dialog_complete_age(e, d), 1.0f) < 0.5f)
    {
        float tx = right - 40.0f;

        al_draw_filled_triangle(tx, footer_y + 1.0f, tx + 8.0f, footer_y + 1.0f,
            tx + 4.0f, footer_y + 7.0f, UI_AMBER);
    }
}

// card pendente da fila na caixa de dialogo
void draw_concept_card()
{
    DialogEntry shown;
    char header[96];

    sync_card_dialog();

    if (card_dialog_entry == CARD_NONE)
    {
        return;
    }

    shown = diary_entries[card_dialog_entry];

    // a anotacao leva o numero da ordem em que abriu; o interludio fica sem numero
    if (card_note_number(card_dialog_entry) > 0)
    {
        snprintf(header, sizeof(header), "Nanobô · Anotação %d · %s",
            card_note_number(card_dialog_entry), diary_entries[card_dialog_entry].header);
        shown.header = header;
    }

    draw_dialog(&shown, &card_dialog);
}
