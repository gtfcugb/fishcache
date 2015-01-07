dofile("/etc/fishnet.conf");
package.path 	= FISHNET_WORK_DIR.."libman/?.lua;"..FISHNET_WORK_DIR.."libman/frame/?.lua;"..FISHNET_WORK_DIR.."libman/lua/5.1/?.lua;"..FISHNET_WORK_DIR.."libman/lua/5.1/?/init.lua;"..package.path;
package.cpath   = FISHNET_WORK_DIR.."libman/lua/5.1/?.so;"..FISHNET_WORK_DIR.."libman/lua/5.1/?/?.so;"..package.cpath;

local redis   = require("redis");
local g_redis           = nil;

REDIS_PORT              = 6379;
REDIS_IP                = MONKEY_IP;

g_redis     =   redis.connect({host=REDIS_IP,port=REDIS_PORT});
for j=1,100000 do
    g_redis:set("testkey_"..j,"testvalue------------_"..j);
end