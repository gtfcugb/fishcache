#ifndef _DL_MEMCACHE_
#define _DL_MEMCACHE_
#include <jemalloc/jemalloc.h>

#include <fcalg/fcalg.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <assert.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/sem.h>

#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <sys/syscall.h>
#include <syslog.h>
#include <dlfcn.h>

#include "../../def.h"
#include "../../mem.h"

int memcache_engineDLInit(UpmanData * pUpmanData);
void memcache_engineDLInput(UpmanData * pUpmanData,SocketClient*,SocketPacket*);
void memcache_engineDLDestroy(UpmanData * pUpmanData);

#endif
