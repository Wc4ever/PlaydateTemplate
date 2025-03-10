#include "utils.h"
#include "update.h"

struct Player {
	float x;
	float y;
    float angle;
	int width;
	int height;
	float speed;

	LCDBitmap* bitmap;
    LCDSprite* sprite;
};

struct GameCtx 
{
	struct Player player;

	int score;
} Game_ctx;

PlaydateAPI* PD;

SpriteCollisionResponseType playerCollisionResponse(LCDSprite* sprite, LCDSprite* other)
{
    return kCollisionTypeSlide;
}

void drawPlayer(LCDSprite* sprite, PDRect bounds, PDRect drawrect)
{
    struct Player* player = PD->sprite->getUserdata(sprite);

    PD->graphics->drawRotatedBitmap(player->bitmap, drawrect.x + drawrect.width / 2, drawrect.y + drawrect.height / 2, player->angle, 0.5, 0.5, 1, 1);
}

void playerInit(struct Player* player, PlaydateAPI* pd)
{
    PD = pd;

	int displayWidth = pd->display->getWidth();
    int displayHeight = pd->display->getHeight();

    player->speed = 100.0f;
    player->angle = 0.0f;

    player->bitmap = loadImageFrom("images/player", pd);
    player->sprite = pd->sprite->newSprite();
    pd->sprite->setDrawFunction(player->sprite, drawPlayer);
    pd->graphics->getBitmapData(player->bitmap, &player->width, &player->height, NULL, NULL, NULL);
    float y_start = displayHeight / 2.f;
    float x_start = displayWidth / 2.f;
    player->x = x_start;
    player->y = y_start;
    pd->sprite->setZIndex(player->sprite, 1);
    
    float rendering_padding = 10;
    float w = player->width + rendering_padding;
    float h = player->height + rendering_padding;
    PDRect playerRect = {x_start - w / 2, y_start - h / 2, w, h};
    pd->sprite->setBounds(player->sprite, playerRect);
    
    PDRect playerCollider = {0, 0, player->width, player->height};
    pd->sprite->setCollideRect(player->sprite, playerCollider);
    pd->sprite->setCollisionsEnabled(player->sprite, 1);
    pd->sprite->setCollisionResponseFunction(player->sprite, playerCollisionResponse);
    pd->sprite->setUserdata(player->sprite, player);
    pd->sprite->addSprite(player->sprite);
}

void playerUpdate(struct Player* player, struct UpdateCtx* ctx)
{
	float crank = ctx->pd->system->getCrankChange();

    PDButtons current;
    ctx->pd->system->getButtonState(&current, NULL, NULL);

    if(current & kButtonLeft) {
        player->x -= ctx->dt * player->speed;
    }
    if(current & kButtonRight) {
        player->x += ctx->dt * player->speed;
    }
    if(current & kButtonUp) {
        player->y -= ctx->dt * player->speed;
    }
    if(current & kButtonDown) {
        player->y += ctx->dt * player->speed;
    }
    
    player->angle += crank;

    float actualX = player->x;
    float actualY = player->y;
    int len = 0;
    SpriteCollisionInfo* info = ctx->pd->sprite->moveWithCollisions(player->sprite, player->x, player->y, &actualX, &actualY, &len);

    if(info) {
        // collision reaction
    }

    ctx->pd->sprite->markDirty(player->sprite);
}

void gameInit(PlaydateAPI* playdate)
{
	playerInit(&Game_ctx.player, playdate);
}

void gameUpdate(struct UpdateCtx* ctx)
{
    playerUpdate(&Game_ctx.player, ctx);
}
