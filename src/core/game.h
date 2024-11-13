#ifndef PD_GAME_H
#define PD_GAME_H

#include "utils/defines.h"
#include "utils/types.h"
#include "core/window.h"

typedef struct pd_game pd_game;

struct pd_game {
    pd_window win;
};

PD_C_DECLS_BEGIN
b32 pd_game_init(pd_game* _game);
void pd_game_deinit(pd_game* _game);
void pd_game_run(pd_game* _game);
PD_C_DECLS_END

#endif // PD_GAME_H
