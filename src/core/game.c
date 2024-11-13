#include "utils/expect.h"
#include "core/game.h"

b32 pd_game_init(pd_game* _game) {
    PD_expect_nonnull(_game);
    return pd_window_init(&_game->win, "Parrydice", 800, 600);
}

void pd_game_deinit(pd_game* _game) {
    PD_expect_nonnull(_game);
    pd_window_deinit(&_game->win);
}

void pd_game_run(pd_game* _game) {
    PD_expect_nonnull(_game);
    while(pd_window_is_running(&_game->win)) {
        pd_window_poll_events(&_game->win);
    }
}
