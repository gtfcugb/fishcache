#include "mem.h"

typedef struct MemEleSt
{
    char*key;
    char*value;
}MemEle;

static int compareEle(void *data_one,void*data_two){
	return strcmp((((MemEle *)data_one)->key),  (((MemEle *)data_two)->key) );
}

/**这里的hash需要检测*/
static unsigned int hashEle(void *p_key,unsigned int bucket_num){
	return f_hash_string( ((MemEle *)p_key)->key,bucket_num);
}

F_Hashtable*sMems;
int memcache_engineDLInit(UpmanData * pUpmanData){
    sMems   =    f_hashtable_create(1024000,NULL,FMalloc,FRealloc,FFree,compareEle,hashEle);
    return 0;
}

void memcache_engineDLInput(UpmanData * pUpmanData,SocketClient*pSock,SocketPacket*pPacket){
    char* debug =pPacket->debug;

    if(strcmp(pPacket->mark,"cache_get") == 0){
        MemEle findMem;
        findMem.key         =   pPacket->type;
        MemEle*pMem         =   (MemEle*)f_hashtable_find(sMems,&findMem);
        char response[20480];
        if(pMem == NULL){
            sprintf(response,"%c%c%s%c%s%c%d%c%s%c",RESPONSE_TYPE_SOA,PACKET_SEP_CHAR,pPacket->mark,PACKET_SEP_CHAR,pPacket->type,PACKET_SEP_CHAR,0,PACKET_SEP_CHAR,debug,PACKET_HEAD_SEP);
        }
        else{
            sprintf(response,"%c%c%s%c%s%c%d%c%s%c%s",RESPONSE_TYPE_SOA,PACKET_SEP_CHAR,pPacket->mark,PACKET_SEP_CHAR,pPacket->type,PACKET_SEP_CHAR,(int)strlen(pMem->value),PACKET_SEP_CHAR,debug,PACKET_HEAD_SEP,pMem->value);
        }
        send(pSock->fd,response,strlen(response),0);
    }
    else if(strcmp(pPacket->mark,"cache_set") == 0){
        MemEle findMem;
        findMem.key         =   pPacket->type;
        MemEle*pMem         =   (MemEle*)f_hashtable_find(sMems,&findMem);
        if(pMem == NULL){
            int keyLen  =   strlen(pPacket->type);
            pMem        =   (MemEle*)FMalloc(sizeof(MemEle));
            pMem->key   =   (char*)FMalloc(keyLen+1);
            memcpy(pMem->key,pPacket->type,keyLen+1);
            pMem->value =   (char*)FMalloc(pPacket->bodyLen+1);
            memcpy(pMem->value , pPacket->rawBuffer + pPacket->ocuLen + pPacket->headLen,pPacket->bodyLen);
            pMem->value[pPacket->bodyLen] = '\0';
            f_hashtable_insert(sMems,pMem);
        }
        else{
            FFree(pMem->value);
            pMem->value =   (char*)FMalloc(pPacket->bodyLen+1);
            memcpy(pMem->value , pPacket->rawBuffer + pPacket->ocuLen + pPacket->headLen,pPacket->bodyLen);
            pMem->value[pPacket->bodyLen] = '\0';
        }
        char response[20480];
        sprintf(response,"%c%c%s%c%s%c%d%c%s%c",RESPONSE_TYPE_SOA,PACKET_SEP_CHAR,pPacket->mark,PACKET_SEP_CHAR,pPacket->type,PACKET_SEP_CHAR,0,PACKET_SEP_CHAR,debug,PACKET_HEAD_SEP);
        send(pSock->fd,response,strlen(response),0);
    }
    else if(strcmp(pPacket->mark,"cache_del") == 0){
        MemEle findMem;
        findMem.key         =   pPacket->type;
        MemEle*pMem         =   (MemEle*)f_hashtable_find(sMems,&findMem);
        char response[20480];
        if(pMem == NULL){
            sprintf(response,"%c%c%s%c%s%c%d%c%s%c%s",RESPONSE_TYPE_SOA,PACKET_SEP_CHAR,pPacket->mark,PACKET_SEP_CHAR,pPacket->type,PACKET_SEP_CHAR,1,PACKET_SEP_CHAR,debug,PACKET_HEAD_SEP,"0");
        }
        else{
            f_hashtable_erase(sMems,pMem) ;
            FFree(pMem->key);
            FFree(pMem->value);
            FFree(pMem);

            sprintf(response,"%c%c%s%c%s%c%d%c%s%c%s",RESPONSE_TYPE_SOA,PACKET_SEP_CHAR,pPacket->mark,PACKET_SEP_CHAR,pPacket->type,PACKET_SEP_CHAR,1,PACKET_SEP_CHAR,debug,PACKET_HEAD_SEP,"1");
        }
        send(pSock->fd,response,strlen(response),0);
    }
}

void memcache_engineDLDestroy(UpmanData * pUpmanData){
    F_H_Iter iter = f_hashtable_begin(sMems);
    while(f_hashtable_end(sMems,iter) != true){
        MemEle *pMem     = (MemEle*)f_hashtable_def(iter);
        FFree(pMem->key);
        FFree(pMem->value);
        FFree(pMem);
        iter                    = f_hashtable_next(sMems,iter);
    }
    f_hashtable_destroy(sMems);
}
