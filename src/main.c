#include "core/game.h"

int main(void) {
    pd_game game;
    if(!pd_game_init(&game)) return 1;

    pd_game_run(&game);
    pd_game_deinit(&game);
    return 0;
}
