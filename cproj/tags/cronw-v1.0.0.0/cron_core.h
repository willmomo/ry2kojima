//-----------------------------------------------------------------------------
// cron ‚ÌƒƒCƒ“ˆ—
//-----------------------------------------------------------------------------
#pragma once

#include "kjmlib-20130512/kjmlib.h"

struct cron_options {
	int m_debug_notrun;
};

unsigned __stdcall crond_thread(void *parg);
void crond_thread_end();
