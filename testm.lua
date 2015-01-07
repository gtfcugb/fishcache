dofile("/etc/monkey.conf");
package.path 	= MONKEY_KING_WORK_DIR.."libman/?.lua;"..MONKEY_KING_WORK_DIR.."libman/frame/?.lua;"..MONKEY_KING_WORK_DIR.."libman/lua/5.1/?.lua;"..MONKEY_KING_WORK_DIR.."libman/lua/5.1/?/init.lua;"..package.path;
package.cpath   = MONKEY_KING_WORK_DIR.."libman/lua/5.1/?.so;"..MONKEY_KING_WORK_DIR.."libman/lua/5.1/?/?.so;"..package.cpath;

require("Memcached");

g_memcached = Memcached.Connect("127.0.0.1",11211);

for j=1,100000 do
    g_memcached:set("testkey_"..j,"testvalue------------_"..j);
end
