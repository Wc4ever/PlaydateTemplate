#include "update.h"
#include "game.h"

int mainLoop(void *data)
{
    struct UpdateCtx* ctx = data;

    //update
    gameUpdate(ctx);

    //draw
    ctx->pd->sprite->updateAndDrawSprites();

    return 1;
}
