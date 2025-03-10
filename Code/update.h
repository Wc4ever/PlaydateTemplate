#pragma once
#include "pd_api.h"

struct UpdateCtx
{
	PlaydateAPI* pd;
    float dt;
};

int mainLoop(void *data);
