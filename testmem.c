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

#include <libmemcached/memcached.h>

#define IP "127.0.0.1"
#define PORT 5000

int gLoopNum    =   1;
int gThreadNum  =   1;
int gSet        =   0;
int gGet        =   0;

void *processGo(void*arg){
    memcached_st *memc;
    memcached_return rc;
    memcached_server_st *server;
    time_t expiration=30000;
    uint32_t  flags=0;
    memc    = memcached_create(NULL);
    server  = memcached_server_list_append(NULL,"127.0.0.1",11211,&rc);
    rc      = memcached_server_push(memc,server);
    memcached_server_list_free(server);


    char sendMsg[1024];
    char key[1024];
    char value[1024];
    int i =0;
    
    struct timeval tv;
	struct timeval tv1;
    
    /*
    gettimeofday(&tv,0);
    for(i =0;i < 1000000;i++){
        sprintf(value,"testkey_%d",1);
        sprintf(sendMsg,"S*cache_get*cache_get*0*%d\n%s",strlen(value),value);
        //printf("%s\n",sendMsg);
        send(conn_fd,sendMsg,strlen(sendMsg),0);
        recv(conn_fd,sendMsg,1024,0);
        //printf("%s\n",sendMsg);
    }
    gettimeofday(&tv1,0);
    */

    gettimeofday(&tv,0);
    for(i =0;i < gLoopNum;i++){
        sprintf(key     ,"testkey_%d_%d",*(int*)arg,i);
        sprintf(value   ,"0000000000000000testvalue_%d",i);
        rc  =   memcached_set(memc,key,strlen(key),value,strlen(value),expiration,flags);
        if(MEMCACHED_SUCCESS == rc){
            
        }
        else{
            printf("rc fail \n");
        }
    }
    gettimeofday(&tv1,0);


    printf("gLoopNum %d  gThreadNum %d gSet %d gGet %d time %ld  \n",gLoopNum,gThreadNum ,
    gSet,gGet,(tv1.tv_sec-tv.tv_sec) * 1000000 + (tv1.tv_usec - tv.tv_usec));
    return NULL;
}

void test1(){
    gLoopNum    =   100000;
    gThreadNum  =   1;
    gSet        =   1;
    gGet        =   0;
}

void test2(){
    gLoopNum    =   100000;
    gThreadNum  =   1;
    gSet        =   1;
    gGet        =   0;
}

int i=0;
int main(){
    test1();
    for(i=0;i< gThreadNum;i++){
        int tid;
        int* num = malloc(sizeof(int));
        *num    = i;
        pthread_create(&tid, NULL, (void *)processGo, num);
    }
    printf("main thread go to sleep \n");
    sleep(10000);
    return 0;
}
