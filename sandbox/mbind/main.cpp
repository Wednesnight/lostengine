#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

int main (int argc, char * const argv[]) {
    char buf[256];
    int error;
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    printf("%d\n",lua_gettop(L));
    lua_pushnumber(L,13);
    printf("%d\n",lua_gettop(L));
/*    while(fgets(buf,sizeof(buf),stdin) != NULL)
    {
      error = luaL_loadbuffer(L,buf,strlen(buf),"line") || lua_pcall(L,0,0,0);
      if(error)
      {
        fprintf(stderr,"%s",lua_tostring(L,-1));
        lua_pop(L,1);
      }
    }*/
    lua_close(L);
    return 0;
}
