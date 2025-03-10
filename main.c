
#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"

#include "Code/update.h"
#include "Code/init.h"

#ifdef _WINDLL
__declspec(dllexport)
#endif

struct UpdateCtx update_ctx;

// recommended refresh rate for playdate
// possible to go up to 50 if requred but it will decrease battery life
const float refresh_rate = 30.0f;

// this template sets up the main loop in continious mode, everything redraws every update
// but playdate screen supports partial update, so you can redraw only nessesary parts of the screen and save battery life even more
int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
	//main loop setup
	if ( event == kEventInit )
	{
		update_ctx.pd = playdate;
		update_ctx.dt = 1.f / refresh_rate;

		playdate->display->setRefreshRate(refresh_rate);
		playdate->system->setUpdateCallback(mainLoop, &update_ctx);
		init(playdate);
	}
	
	return 0;
}
