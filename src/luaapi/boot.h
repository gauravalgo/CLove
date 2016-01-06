#pragma once

#include "../3rdparty/lua/lualib.h"
#include "../bonding_config.h"

int l_boot(lua_State* state, bonding_Config *config);
