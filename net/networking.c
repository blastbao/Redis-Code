/*
 * Copyright (c) 2009-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "redis.h"
#include <sys/uio.h>
#include <math.h>

static void setProtocolError(redisClient *c, int pos);

/* To evaluate the output buffer size of a client we need to get size of
 * allocated objects, however we can't used zmalloc_size() directly on sds
 * strings because of the trick they use to work (the header is before the
 * returned pointer), so we use this helper function. */
size_t zmalloc_size_sds(sds s) {
    return zmalloc_size(s-sizeof(struct sdshdr));
}

/* ------------ API ---------------------- */
void *dupClientReplyValue(void *o)	/* 复制value一份 */
int listMatchObjects(void *a, void *b) /* 比价2个obj是否相等 */
robj *dupLastObjectIfNeeded(list *reply) /* 返回回复列表中最后一个元素对象 */
void copyClientOutputBuffer(redisClient *dst, redisClient *src) /* 将源Client的输出buffer复制给目标Client */
static void acceptCommonHandler(int fd, int flags) /* 网络连接后的调用方法 */
void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask)
void acceptUnixHandler(aeEventLoop *el, int fd, void *privdata, int mask)
void disconnectSlaves(void) /* 使server的slave失去连接 */
void replicationHandleMasterDisconnection(void)
void flushSlavesOutputBuffers(void) /* 从方法将会在freeMemoryIfNeeded()，释放内存空间函数，将存在内存中数据操作结果刷新到磁盘中 */
int processEventsWhileBlocked(void)

/* ------------- addReply API -----------------   */
int _addReplyToBuffer(redisClient *c, char *s, size_t len) /* 往客户端缓冲区中添加内容 */
void _addReplyObjectToList(redisClient *c, robj *o) /* robj添加到reply的列表中 */
void _addReplySdsToList(redisClient *c, sds s) /* 在回复列表中添加Sds字符串对象 */
void _addReplyStringToList(redisClient *c, char *s, size_t len) /* 在回复列表中添加字符串对象,参数中已经给定字符的长度 */
void addReply(redisClient *c, robj *obj) /* 在redisClient的buffer中写入数据，数据存在obj->ptr的指针中 */
void addReplySds(redisClient *c, sds s) /* 在回复中添加Sds字符串,下面的额addReply()系列方法原理基本类似 */
void addReplyString(redisClient *c, char *s, size_t len)
void addReplyErrorLength(redisClient *c, char *s, size_t len)
void addReplyError(redisClient *c, char *err) /* 往Reply中添加error类的信息 */
void addReplyErrorFormat(redisClient *c, const char *fmt, ...)
void addReplyStatusLength(redisClient *c, char *s, size_t len)
void addReplyStatus(redisClient *c, char *status)
void addReplyStatusFormat(redisClient *c, const char *fmt, ...)
void *addDeferredMultiBulkLength(redisClient *c) /* 在reply list 中添加一个空的obj对象 */
void setDeferredMultiBulkLength(redisClient *c, void *node, long length)
void addReplyDouble(redisClient *c, double d) /* 在bulk reply中添加一个double类型值，bulk的意思为大块的，bulk reply的意思为大数据量的回复 */
void addReplyLongLongWithPrefix(redisClient *c, long long ll, char prefix)
void addReplyLongLong(redisClient *c, long long ll)
void addReplyMultiBulkLen(redisClient *c, long length)
void addReplyBulkLen(redisClient *c, robj *obj) /* 添加bulk 大块的数据的长度 */
void addReplyBulk(redisClient *c, robj *obj) /* 将一个obj的数据，拆分成大块数据的添加 */
void addReplyBulkCBuffer(redisClient *c, void *p, size_t len)
void addReplyBulkCString(redisClient *c, char *s)
void addReplyBulkLongLong(redisClient *c, long long ll)

/* ------------- Client API -----------------   */	
redisClient *createClient(int fd) /* 创建redisClient客户端，1.建立连接，2.设置数据库，3.属性设置 */
int prepareClientToWrite(redisClient *c) /* 此方法将会被调用于Client准备接受新数据之前调用，在fileEvent为客户端设定writer的handler处理事件 */
static void freeClientArgv(redisClient *c)
void freeClient(redisClient *c) /* 释放freeClient，要分为Master和Slave2种情况作不同的处理 */
void freeClientAsync(redisClient *c)
void freeClientsInAsyncFreeQueue(void) /* 异步的free客户端 */
void sendReplyToClient(aeEventLoop *el, int fd, void *privdata, int mask) /* 将Client中的reply数据存入文件中 */
void resetClient(redisClient *c)
int processInlineBuffer(redisClient *c) /* 处理redis Client的内链的buffer，就是c->querybuf */
static void setProtocolError(redisClient *c, int pos)
int processMultibulkBuffer(redisClient *c) /* 处理大块的buffer */
void processInputBuffer(redisClient *c) /* 处理redisClient的查询buffer */
void readQueryFromClient(aeEventLoop *el, int fd, void *privdata, int mask) /* 从Client获取查询query语句 */
void getClientsMaxBuffers(unsigned long *longest_output_list,
                          unsigned long *biggest_input_buffer) /* 获取Client中输入buffer和输出buffer的最大长度值 */
void formatPeerId(char *peerid, size_t peerid_len, char *ip, int port) /* 格式化ip,port端口号的输出，ip:port */
int genClientPeerId(redisClient *client, char *peerid, size_t peerid_len) /* 获取Client客户端的ip,port地址信息 */
char *getClientPeerId(redisClient *c) /* 获取c->peerid客户端的地址信息 */
sds catClientInfoString(sds s, redisClient *client) /* 格式化的输出客户端的属性信息，直接返回一个拼接好的字符串 */
sds getAllClientsInfoString(void) /* 获取所有Client客户端的属性信息，并连接成一个总的字符串并输出 */
void clientCommand(redisClient *c) /* 执行客户端的命令的作法 */
void rewriteClientCommandVector(redisClient *c, int argc, ...) /* 重写客户端的命令集合，旧的命令集合的应用计数减1，新的Command  Vector的命令集合增1 */
void rewriteClientCommandArgument(redisClient *c, int i, robj *newval) /* 重写Client中的第i个参数 */
unsigned long getClientOutputBufferMemoryUsage(redisClient *c) /* 获取Client中已经用去的输出buffer的大小 */
int getClientType(redisClient *c)
int getClientTypeByName(char *name) /* Client中的名字的3种类型，normal,slave，pubsub */
char *getClientTypeName(int class)
int checkClientOutputBufferLimits(redisClient *c) /* 判断Clint的输出缓冲区的已经占用大小是否超过软限制或是硬限制 */
void asyncCloseClientOnOutputBufferLimitReached(redisClient *c) /* 异步的关闭Client，如果缓冲区中的软限制或是硬限制已经到达的时候，缓冲区超出限制的结果会导致释放不安全， */

/* 复制value一份 */
void *dupClientReplyValue(void *o) {
	//增加对此obj的引用计数
    incrRefCount((robj*)o);
    return o;
}

/* 比价2个obj是否相等 */
int listMatchObjects(void *a, void *b) {
    return equalStringObjects(a,b);
}

/* 创建redisClient客户端，1.建立连接，2.设置数据库，3.属性设置 */
redisClient *createClient(int fd) {
    redisClient *c = zmalloc(sizeof(redisClient));

    /* passing -1 as fd it is possible to create a non connected client.
     * This is useful since all the Redis commands needs to be executed
     * in the context of a client. When commands are executed in other
     * contexts (for instance a Lua script) we need a non connected client. */
    //建立连接
    if (fd != -1) {
        anetNonBlock(NULL,fd);
        anetEnableTcpNoDelay(NULL,fd);
        if (server.tcpkeepalive)
            anetKeepAlive(NULL,fd,server.tcpkeepalive);
        if (aeCreateFileEvent(server.el,fd,AE_READABLE, readQueryFromClient, c) == AE_ERR){
            close(fd);
            zfree(c);
            return NULL;
        }
    }
	
	//选择数据库
    selectDb(c,0);
    //属性设置
    c->id = server.next_client_id++;
    c->fd = fd;
    c->name = NULL;
    c->bufpos = 0;
    c->querybuf = sdsempty();
    c->querybuf_peak = 0;
    c->reqtype = 0;
    c->argc = 0;
    c->argv = NULL;
    c->cmd = c->lastcmd = NULL;
    c->multibulklen = 0;
    c->bulklen = -1;
    c->sentlen = 0;
    c->flags = 0;
    c->ctime = c->lastinteraction = server.unixtime;
    c->authenticated = 0;
    c->replstate = REDIS_REPL_NONE;
    c->reploff = 0;
    c->repl_ack_off = 0;
    c->repl_ack_time = 0;
    c->slave_listening_port = 0;
    c->reply = listCreate();
    c->reply_bytes = 0;
    c->obuf_soft_limit_reached_time = 0;
    listSetFreeMethod(c->reply,decrRefCountVoid);
    listSetDupMethod(c->reply,dupClientReplyValue);
    c->bpop.keys = dictCreate(&setDictType,NULL);
    c->bpop.timeout = 0;
    c->bpop.target = NULL;
    c->watched_keys = listCreate();
    c->pubsub_channels = dictCreate(&setDictType,NULL);
    c->pubsub_patterns = listCreate();
    c->peerid = NULL;
    listSetFreeMethod(c->pubsub_patterns,decrRefCountVoid);
    listSetMatchMethod(c->pubsub_patterns,listMatchObjects);
    if (fd != -1) 
        listAddNodeTail(server.clients,c);
    initClientMultiState(c);
    return c;
}

/* This function is called every time we are going to transmit new data
 * to the client. The behavior is the following:
 *
 * If the client should receive new data (normal clients will) the function
 * returns REDIS_OK, and make sure to install the write handler in our event
 * loop so that when the socket is writable new data gets written.
 *
 * If the client should not receive new data, because it is a fake client,
 * a master, a slave not yet online, or because the setup of the write handler
 * failed, the function returns REDIS_ERR.
 *
 * Typically gets called every time a reply is built, before adding more
 * data to the clients output buffers. If the function returns REDIS_ERR no
 * data should be appended to the output buffers. */

/*
  此方法将会被调用于Client准备接受新数据之前。

  本函数中会判断client的flag，如果符合条件将当前client放入server.clients_pending_write链表。
  有几种情况不需要回复，会跳过：
     如果是执行lua脚本的client；
     如果client设置了CLIENT_REPLY_OFF|CLIENT_REPLY_SKIP；
     如果client是连接master的；
  
  本函数会设置client的socket写入事件处理函数sendReplyToClient（通过函数aeCreateFileEvent进行设置），
  这样一旦发送给client对应的socket缓存空间中有数据，即调用sendReplyToClient进行处理，将要发送的数
  据通过socket发送给client。
*/

int prepareClientToWrite(redisClient *c) {
    if (c->flags & REDIS_LUA_CLIENT) 
        return REDIS_OK;
    if ((c->flags & REDIS_MASTER) && !(c->flags & REDIS_MASTER_FORCE_REPLY)) 
        return REDIS_ERR;
    if (c->fd <= 0) 
        return REDIS_ERR; /* Fake client */

    //aeCreateFileEvent监听c->fd的可写事件，sendReplyToClient用于回复client c。
    if (c->bufpos == 0 &&  listLength(c->reply) == 0 && 
                           (c->replstate == REDIS_REPL_NONE || c->replstate == REDIS_REPL_ONLINE) && 
                           aeCreateFileEvent(server.el, c->fd, AE_WRITABLE, sendReplyToClient, c) == AE_ERR) 
        return REDIS_ERR;
    return REDIS_OK;
}

/* Create a duplicate of the last object in the reply list when
 * it is not exclusively owned by the reply list. */
/* 返回回复列表中最后一个元素对象 */
// 当回复列表中的最后一个对象并非属于回复的一部分时，创建该对象的一个复制品，并返回。
robj *dupLastObjectIfNeeded(list *reply) {
    robj *new, *cur;
    listNode *ln;
    redisAssert(listLength(reply) > 0);
    ln  = listLast(reply);
    cur = listNodeValue(ln);
    if (cur->refcount > 1) {
        new = dupStringObject(cur);
        decrRefCount(cur);
        listNodeValue(ln) = new;
    }
    return listNodeValue(ln);
}

/* -----------------------------------------------------------------------------
 * Low level functions to add more data to output buffers.
 * -------------------------------------------------------------------------- */
/* 往客户端缓冲区 c->buf 中添加内容 */
int _addReplyToBuffer(redisClient *c, char *s, size_t len) {
    size_t available = sizeof(c->buf) - c->bufpos;

    // 正准备关闭客户端，无须再发送内容
    if (c->flags & REDIS_CLOSE_AFTER_REPLY) 
        return REDIS_OK;

    /* If there already are entries in the reply list, we cannot
     * add anything more to the static buffer. */
    //回复链表里已经有内容，再添加内容到 c->buf 里面就是错误了
    if (listLength(c->reply) > 0) 
        return REDIS_ERR;

    /* Check that the buffer has enough space available for this string. */
    // 空间必须满足
    if (len > available) 
        return REDIS_ERR;
    // 复制内容到 c->buf 里面
    memcpy(c->buf+c->bufpos,s,len);
    c->bufpos+=len;
    return REDIS_OK;
}

/* robj添加到reply的列表中 
 * 将回复对象（一个 SDS ）添加到 c->reply 回复链表中
*/
void _addReplyObjectToList(redisClient *c, robj *o) {
    robj *tail;

    // 客户端即将被关闭，无须再发送回复
    if (c->flags & REDIS_CLOSE_AFTER_REPLY) 
        return;

    // 回复列表中无缓冲节点，直接新建链表节点将对象robj添加到链表中。
    if (listLength(c->reply) == 0) {
        incrRefCount(o);
        listAddNodeTail(c->reply,o);
        c->reply_bytes += zmalloc_size_sds(o->ptr);
    } else {
        tail = listNodeValue(listLast(c->reply));
        /* Append to this object when possible. */
        // 回复列表中已有缓冲块，尝试将回复添加到链表的尾节点内，若尾节点不能容纳obj的话，那么新建一个节点
        if (tail->ptr != NULL && sdslen(tail->ptr)+sdslen(o->ptr) <= REDIS_REPLY_CHUNK_BYTES)
        {
            //将tail->ptr和o->ptr合并成一个节点。
            c->reply_bytes -= zmalloc_size_sds(tail->ptr);
            tail            = dupLastObjectIfNeeded(c->reply);
            tail->ptr       = sdscatlen(tail->ptr,o->ptr,sdslen(o->ptr));
            c->reply_bytes += zmalloc_size_sds(tail->ptr);
        } else {
            // 直接新建节点将对象obj追加到回复列表尾
            incrRefCount(o);
            listAddNodeTail(c->reply,o);
            c->reply_bytes += zmalloc_size_sds(o->ptr);
        }
    }
    // 检查回复缓冲区的大小，如果超过系统限制的话，那么关闭客户端并释放相关资源。
    asyncCloseClientOnOutputBufferLimitReached(c);
}

/* This method takes responsibility over the sds. When it is no longer
 * needed it will be free'd, otherwise it ends up in a robj. */
// 和 _addReplyObjectToList 类似，只是把obj参数换作Sds字符串
// 内部实现上多了一步将Sds转换obj过程，且在必要时会负责释放参数Sds s
void _addReplySdsToList(redisClient *c, sds s) {
    robj *tail;

    // 客户端即将被关闭，无须再发送回复
    if (c->flags & REDIS_CLOSE_AFTER_REPLY) {
        sdsfree(s);
        return;
    }

    if (listLength(c->reply) == 0) {
        listAddNodeTail(c->reply,createObject(REDIS_STRING,s));
        c->reply_bytes += zmalloc_size_sds(s);
    } else {
        tail = listNodeValue(listLast(c->reply));

        /* Append to this object when possible. */
        if (tail->ptr != NULL && sdslen(tail->ptr)+sdslen(s) <= REDIS_REPLY_CHUNK_BYTES)
        {
            c->reply_bytes -= zmalloc_size_sds(tail->ptr);
            tail            = dupLastObjectIfNeeded(c->reply);
            tail->ptr       = sdscatlen(tail->ptr,s,sdslen(s));
            c->reply_bytes += zmalloc_size_sds(tail->ptr);
            sdsfree(s);
        } else {
            listAddNodeTail(c->reply,createObject(REDIS_STRING,s));
            c->reply_bytes += zmalloc_size_sds(s);
        }
    }
    asyncCloseClientOnOutputBufferLimitReached(c);
}

/* 在回复列表中添加字符串对象,参数中已经给定字符的长度 */
void _addReplyStringToList(redisClient *c, char *s, size_t len) {
    robj *tail;

    if (c->flags & REDIS_CLOSE_AFTER_REPLY) 
        return;

    if (listLength(c->reply) == 0) {
        // 为字符串创建字符串对象并追加到回复链表末尾
        robj *o = createStringObject(s,len);
        listAddNodeTail(c->reply,o);
        c->reply_bytes += zmalloc_size_sds(o->ptr);
    } else {
        tail = listNodeValue(listLast(c->reply));

        /* Append to this object when possible. */
        if (tail->ptr != NULL && sdslen(tail->ptr)+len <= REDIS_REPLY_CHUNK_BYTES)
        {
        	//调整replay中的字节数
            c->reply_bytes -= zmalloc_size_sds(tail->ptr);
            tail            = dupLastObjectIfNeeded(c->reply);
            tail->ptr       = sdscatlen(tail->ptr,s,len);
            c->reply_bytes += zmalloc_size_sds(tail->ptr);
        } else {
            robj *o = createStringObject(s,len);
            listAddNodeTail(c->reply,o);
            c->reply_bytes += zmalloc_size_sds(o->ptr);
        }
    }
    asyncCloseClientOnOutputBufferLimitReached(c);
}

/* -----------------------------------------------------------------------------
 * Higher level functions to queue data on the client output buffer.
 * The following functions are the ones that commands implementations will call.
 * -------------------------------------------------------------------------- */

/* 在redisClient的buffer中写入数据，数据存在obj->ptr的指针中 */
void addReply(redisClient *c, robj *obj) {

    // 将客户端可写的事件添加到事件循环中监听起来。
    if (prepareClientToWrite(c) != REDIS_OK) 
        return;

    /* This is an important place where we can avoid copy-on-write
     * when there is a saving child running, avoiding touching the
     * refcount field of the object if it's not needed.
     * 
     * 如果在使用子进程，那么尽可能地避免修改对象的 refcount 域。
     * 
     * If the encoding is RAW and there is room in the static buffer
     * we'll be able to send the object to the client without
     * messing with its page. 
     * 
     * 如果对象的编码为 RAW ，并且静态缓冲区中有空间
     * 那么就可以在不弄乱内存页的情况下，将对象发送给客户端。
     * */
    if (obj->encoding == REDIS_ENCODING_RAW) {
        // 首先尝试复制内容到 c->buf 中，这样可以避免内存分配
        if (_addReplyToBuffer(c,obj->ptr,sdslen(obj->ptr)) != REDIS_OK)
            // 如果 c->buf 中的空间不够，就复制到 c->reply 链表中，这可能会引起内存分配
            _addReplyObjectToList(c,obj);


    //如果obj的编码是Int类型，
    } else if (obj->encoding == REDIS_ENCODING_INT) {
        /* Optimization: if there is room in the static buffer for 32 bytes
         * (more than the max chars a 64 bit integer can take as string) we
         * avoid decoding the object and go for the lower level approach. 
         * 
         * 优化，如果 c->buf 中有等于或多于 32 个字节的剩余空间，能容纳int的32位
         * 字符，那么将整数直接以字符串的形式复制到 c->buf 中。
         * */
        if (listLength(c->reply) == 0 && (sizeof(c->buf) - c->bufpos) >= 32) {
            char buf[32];
            int len;

            len = ll2string(buf,sizeof(buf),(long)obj->ptr);
            if (_addReplyToBuffer(c,buf,len) == REDIS_OK)
                return;
            /* else... continue with the normal code path, but should never
             * happen actually since we verified there is room. */
        }

        // 执行到这里，代表对象是整数，并且缓存buffer长度小于32，将它转换为字符串
        obj = getDecodedObject(obj);
        // 首先尝试复制内容到 c->buf 中，这样可以避免内存分配
        if (_addReplyToBuffer(c,obj->ptr,sdslen(obj->ptr)) != REDIS_OK)
            // 如果 c->buf 中的空间不够，就复制到 c->reply 链表中，这可能会引起内存分配
            _addReplyObjectToList(c,obj);
        
        decrRefCount(obj);
    } else {
        redisPanic("Wrong obj->encoding in addReply()");
    }
}

/*
 * 将 SDS 中的内容复制到回复缓冲区
 */
void addReplySds(redisClient *c, sds s) {
	//在添加回复内容之前，都要先执行prepareClientToWrite(c)，设置文件事件的写事件。
    if (prepareClientToWrite(c) != REDIS_OK) {
        sdsfree(s);              /* The caller expects the sds to be free'd. */
        return;
    }
    if (_addReplyToBuffer(c,s,sdslen(s)) == REDIS_OK) {
        sdsfree(s);
    } else {
        _addReplySdsToList(c,s); /* This method free's the sds when it is no longer needed. */
    }
}

/*
 * 将 C 字符串中的内容复制到回复缓冲区
 */
void addReplyString(redisClient *c, char *s, size_t len) {
    if (prepareClientToWrite(c) != REDIS_OK) 
        return;
    if (_addReplyToBuffer(c,s,len) != REDIS_OK)
        _addReplyStringToList(c,s,len);
}

void addReplyErrorLength(redisClient *c, char *s, size_t len) {
    addReplyString(c,"-ERR ",5);
    addReplyString(c,s,len);
    addReplyString(c,"\r\n",2);
}

/*
 * 添加一个错误回复
 *
 * 例子 -ERR unknown command 'foobar'
 */
void addReplyError(redisClient *c, char *err) {
    addReplyErrorLength(c,err,strlen(err));
}

//回复格式化的错误信息
void addReplyErrorFormat(redisClient *c, const char *fmt, ...) {
    size_t l, j;
    va_list ap;
    va_start(ap,fmt);
    sds s = sdscatvprintf(sdsempty(),fmt,ap);
    va_end(ap);
    /* Make sure there are no newlines in the string, otherwise invalid protocol
     * is emitted. */
    l = sdslen(s);
    //消除回复内容中的\r\n，否则会扰乱格式。
    for (j = 0; j < l; j++) {
        if (s[j] == '\r' || s[j] == '\n') 
            s[j] = ' ';
    }
    addReplyErrorLength(c,s,sdslen(s));
    sdsfree(s);
}

void addReplyStatusLength(redisClient *c, char *s, size_t len) {
    addReplyString(c,"+",1);
    addReplyString(c,s,len);
    addReplyString(c,"\r\n",2);
}

/*
 * 返回一个状态回复
 *
 * 例子 +OK\r\n
 */
void addReplyStatus(redisClient *c, char *status) {
    addReplyStatusLength(c,status,strlen(status));
}

void addReplyStatusFormat(redisClient *c, const char *fmt, ...) {
    va_list ap;
    va_start(ap,fmt);
    sds s = sdscatvprintf(sdsempty(),fmt,ap);
    va_end(ap);
    addReplyStatusLength(c,s,sdslen(s));
    sdsfree(s);
}

/* Adds an empty object to the reply list that will contain the multi bulk
 * length, which is not known when this function is called. 
 * */
void *addDeferredMultiBulkLength(redisClient *c) {
    /* Note that we install the write event here even if the object is not
     * ready to be sent, since we are sure that before returning to the
     * event loop setDeferredMultiBulkLength() will be called. */
    if (prepareClientToWrite(c) != REDIS_OK) 
        return NULL;
    /* 在reply list 中添加一个空的obj对象 */
    listAddNodeTail(c->reply,createObject(REDIS_STRING,NULL));
    return listLast(c->reply);
}

/* Populate the length object and try gluing it to the next chunk. */
// 设置 Multi Bulk 回复的长度
void setDeferredMultiBulkLength(redisClient *c, void *node, long length) {
    listNode *ln = (listNode*)node;
    robj *len, *next;

    /* Abort when *node is NULL (see addDeferredMultiBulkLength). */
    if (node == NULL) return;

    len = listNodeValue(ln);
    len->ptr = sdscatprintf(sdsempty(),"*%ld\r\n",length);
    c->reply_bytes += zmalloc_size_sds(len->ptr);
    if (ln->next != NULL) {
        next = listNodeValue(ln->next);

        /* Only glue when the next node is non-NULL (an sds in this case) */
        if (next->ptr != NULL) {
            c->reply_bytes -= zmalloc_size_sds(len->ptr);
            c->reply_bytes -= zmalloc_size_sds(next->ptr);
            len->ptr = sdscatlen(len->ptr,next->ptr,sdslen(next->ptr));
            c->reply_bytes += zmalloc_size_sds(len->ptr);
            listDelNode(c->reply,ln->next);
        }
    }
    asyncCloseClientOnOutputBufferLimitReached(c);
}

/* Add a double as a bulk reply 
 *
 * 以 bulk 回复的形式，返回一个双精度浮点数
 * 
 * 例子 $4\r\n3.14\r\n
 * 
 * */

/* 在bulk reply中添加一个double类型值，bulk的意思为大块的，bulk reply的意思为大数据量的回复 */
void addReplyDouble(redisClient *c, double d) {
    char dbuf[128], sbuf[128];
    int dlen, slen;
    if (isinf(d)) {
        /* Libc in odd systems (Hi Solaris!) will format infinite in a
         * different way, so better to handle it in an explicit way. */
        addReplyBulkCString(c, d > 0 ? "inf" : "-inf");
    } else {
        dlen = snprintf(dbuf,sizeof(dbuf),"%.17g",d);
        slen = snprintf(sbuf,sizeof(sbuf),"$%d\r\n%s\r\n",dlen,dbuf);
        addReplyString(c,sbuf,slen);
    }
}

/* Add a long long as integer reply or bulk len / multi bulk count.
 * 
 * 添加一个 long long 为整数回复，或者 bulk 或 multi bulk 的数目
 *
 * Basically this is used to output <prefix><long long><crlf>. 
 *
 * 输出格式为 <prefix><long long><crlf>
 *
 * 例子:
 *
 * *5\r\n10086\r\n
 *
 * $5\r\n10086\r\n
 */
void addReplyLongLongWithPrefix(redisClient *c, long long ll, char prefix) {
    char buf[128];
    int len;

    /* Things like $3\r\n or *2\r\n are emitted very often by the protocol
     * so we have a few shared objects to use if the integer is small
     * like it is most of the times. */
    if (prefix == '*' && ll < REDIS_SHARED_BULKHDR_LEN) {
        addReply(c,shared.mbulkhdr[ll]);
        return;
    } else if (prefix == '$' && ll < REDIS_SHARED_BULKHDR_LEN) {
        addReply(c,shared.bulkhdr[ll]);
        return;
    }

    buf[0] = prefix;
    len = ll2string(buf+1,sizeof(buf)-1,ll); //将long long类型转化为String类型
    buf[len+1] = '\r';
    buf[len+2] = '\n';
    addReplyString(c,buf,len+3);
}
/*
 * 返回一个整数回复
 * 
 * 格式为 :10086\r\n
 */
void addReplyLongLong(redisClient *c, long long ll) {
    if (ll == 0)
        addReply(c,shared.czero);
    else if (ll == 1)
        addReply(c,shared.cone);
    else
        addReplyLongLongWithPrefix(c,ll,':');
}

void addReplyMultiBulkLen(redisClient *c, long length) {
    if (length < REDIS_SHARED_BULKHDR_LEN)
        addReply(c,shared.mbulkhdr[length]);
    else
        addReplyLongLongWithPrefix(c,length,'*');
}

/* Create the length prefix of a bulk reply, example: $2234 */
/* 添加bulk 大块的数据的长度 */
void addReplyBulkLen(redisClient *c, robj *obj) {
    size_t len;

    if (obj->encoding == REDIS_ENCODING_RAW) {
        len = sdslen(obj->ptr);
    } else {
        long n = (long)obj->ptr;
        /* Compute how many bytes will take this integer as a radix 10 string */
        len = 1;
        if (n < 0) {
            len++;
            n = -n;
        }
        while((n = n/10) != 0) {
            len++;
        }
    }
    if (len < REDIS_SHARED_BULKHDR_LEN)
        addReply(c,shared.bulkhdr[len]);
    else
        addReplyLongLongWithPrefix(c,len,'$');
}

/* Add a Redis Object as a bulk reply
 *
 * 返回一个 Redis 对象作为回复
 */
/* 将一个obj的数据，拆分成大块数据的添加 */
void addReplyBulk(redisClient *c, robj *obj) {
    addReplyBulkLen(c,obj);//reply添加长度
    addReply(c,obj);//reply添加对象
    addReply(c,shared.crlf);
}

/* Add a C buffer as bulk reply 
 *
 * 返回一个 C 缓冲区作为回复
 */
void addReplyBulkCBuffer(redisClient *c, void *p, size_t len) {
    addReplyLongLongWithPrefix(c,len,'$');
    addReplyString(c,p,len);
    addReply(c,shared.crlf);
}

/* Add a C nul term string as bulk reply 
 *
 * 返回一个 C 字符串作为回复
 */
void addReplyBulkCString(redisClient *c, char *s) {
    if (s == NULL) {
        addReply(c,shared.nullbulk);
    } else {
        addReplyBulkCBuffer(c,s,strlen(s));
    }
}

/* Add a long long as a bulk reply 
 *
 * 返回一个 long long 值作为回复
 */
void addReplyBulkLongLong(redisClient *c, long long ll) {
    char buf[64];
    int len;

    len = ll2string(buf,64,ll);
    addReplyBulkCBuffer(c,buf,len);
}

/* Copy 'src' client output buffers into 'dst' client output buffers.
 * The function takes care of freeing the old output buffers of the
 * destination client. */
/* 将源Client的输出buffer复制给目标Client，释放 dst 客户端原有的输出内容*/
void copyClientOutputBuffer(redisClient *dst, redisClient *src) {
    listRelease(dst->reply);                // 释放 dst 原有的回复链表
    dst->reply = listDup(src->reply);       // 复制新链表到 dst
    memcpy(dst->buf,src->buf,src->bufpos);  // 复制内容到回复 buf
    dst->bufpos      = src->bufpos;         // 同步偏移量和字节数
    dst->reply_bytes = src->reply_bytes;
}



/*
 * TCP 连接 accept 处理器
 */
#define MAX_ACCEPTS_PER_CALL 1000


/* 网络连接后的调用方法 */
static void acceptCommonHandler(int fd, int flags) {

    //创建客户端，createClient内部会监听fd上可读事件，由readQueryFromClient回调处理。
    redisClient *c;
    if ((c = createClient(fd)) == NULL) {
        redisLog(REDIS_WARNING, "Error registering fd event for the new client: %s (fd=%d)", strerror(errno),fd);
        close(fd); /* May be already closed, just ignore errors */
        return;
    }
    /* If maxclient directive is set and this is one client more... close the
     * connection. Note that we create the client instead to check before
     * for this condition, since now the socket is already set in non-blocking
     * mode and we can send an error for free using the Kernel I/O 
     * 
     * 如果新添加的客户端令服务器的超过最大客户端数量，那么向新客户端写入错误
     * 信息，并关闭新客户端。这里先创建客户端，再进行数量检查是为了方便地进行
     * 进行错误信息写入。
     * */
    if (listLength(server.clients) > server.maxclients) {
        char *err = "-ERR max number of clients reached\r\n";

        /* That's a best effort error message, don't check write errors */
        if (write(c->fd,err,strlen(err)) == -1) {
            /* Nothing to do, Just to avoid the warning... */
        }

        server.stat_rejected_conn++;// 更新拒绝连接数（做统计用）
        freeClient(c);
        return;
    }
    server.stat_numconnections++;   // 更新连接数
    c->flags |= flags;              // 设置 FLAG
}

/* 
 * 创建一个 TCP 连接处理器
 */
void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask) {
    int cport, cfd, max = MAX_ACCEPTS_PER_CALL;
    char cip[REDIS_IP_STR_LEN];
    REDIS_NOTUSED(el);
    REDIS_NOTUSED(mask);
    REDIS_NOTUSED(privdata);

    while(max--) {
    	//Accept并创建TCP连接，获取客户端id,port
        cfd = anetTcpAccept(server.neterr, fd, cip, sizeof(cip), &cport);
        if (cfd == ANET_ERR) {
            if (errno != EWOULDBLOCK)
                redisLog(REDIS_WARNING, "Accepting client connection: %s", server.neterr);
            return;
        }
        redisLog(REDIS_VERBOSE,"Accepted %s:%d", cip, cport);
        //为当前连接创建client结构体，监听可读事件。
        acceptCommonHandler(cfd,0);
    }
}

void acceptUnixHandler(aeEventLoop *el, int fd, void *privdata, int mask) {
    int cfd, max = MAX_ACCEPTS_PER_CALL;
    REDIS_NOTUSED(el);
    REDIS_NOTUSED(mask);
    REDIS_NOTUSED(privdata);

    while(max--) {
        cfd = anetUnixAccept(server.neterr, fd);
        if (cfd == ANET_ERR) {
            if (errno != EWOULDBLOCK)
                redisLog(REDIS_WARNING, "Accepting client connection: %s", server.neterr);
            return;
        }
        redisLog(REDIS_VERBOSE,"Accepted connection to %s", server.unixsocket);
        acceptCommonHandler(cfd,REDIS_UNIX_SOCKET);
    }
}

/*
 * 清空所有命令参数
 */
static void freeClientArgv(redisClient *c) {
    int j;
    for (j = 0; j < c->argc; j++)
        decrRefCount(c->argv[j]);
    c->argc = 0;
    c->cmd  = NULL;
}

/* Close all the slaves connections. This is useful in chained replication
 * when we resync with our own master and want to force all our slaves to
 * resync with us as well. */
/* 使server的所有从服务器slave断开连接，强制所有slave执行重同步*/
void disconnectSlaves(void) {
    while (listLength(server.slaves)) {
        listNode *ln = listFirst(server.slaves);
        freeClient((redisClient*)ln->value);
    }
}

/* This function is called when the slave lose the connection with the
 * master into an unexpected way. */
// 这个函数在从服务器slave意外地和主服务器server失去联系时调用。
void replicationHandleMasterDisconnection(void) {
    server.master           = NULL;
    server.repl_state       = REDIS_REPL_CONNECT;
    server.repl_down_since  = server.unixtime;
    /* We lost connection with our master, force our slaves to resync
     * with us as well to load the new data set.
     *
     * 和主服务器失联，强制所有这个服务器的从服务器 resync ，等待载入新数据。
     *
     * If server.masterhost is NULL the user called SLAVEOF NO ONE so
     * slave resync is not needed. 
     *
     * 如果 masterhost 不存在（怎么会这样呢？）
     * 那么调用 SLAVEOF NO ONE ，避免 slave resync
     */
    if (server.masterhost != NULL) 
        disconnectSlaves();
}

/* 释放freeClient，要分为Master和Slave2种情况作不同的处理 */
void freeClient(redisClient *c) {
    listNode *ln;

    /* If this is marked as current client unset it */
    if (server.current_client == c) 
        server.current_client = NULL;

    /* If it is our master that's beging disconnected we should make sure
     * to cache the state to try a partial resynchronization later.
     *
     * Note that before doing this we make sure that the client is not in
     * some unexpected state, by checking its flags. */
    if (server.master && c->flags & REDIS_MASTER) {
        redisLog(REDIS_WARNING,"Connection with master lost.");
        if (!(c->flags & (REDIS_CLOSE_AFTER_REPLY| REDIS_CLOSE_ASAP| REDIS_BLOCKED| REDIS_UNBLOCKED)))
        {
        	//如果是Master客户端，需要做缓存Client的处理，可以迅速重新启用
            replicationCacheMaster(c);
            return;
        }
    }

    /* Log link disconnection with slave */
    if ((c->flags & REDIS_SLAVE) && !(c->flags & REDIS_MONITOR)) {
        //获取连接客户端的 IP 和端口号
        char ip[REDIS_IP_STR_LEN];
        if (anetPeerToString(c->fd,ip,sizeof(ip),NULL) != -1) {
            redisLog(REDIS_WARNING,"Connection with slave %s:%d lost.", ip, c->slave_listening_port);
        }
    }

    /* Free the query buffer */
    sdsfree(c->querybuf);
    c->querybuf = NULL;

    /* Deallocate structures used to block on blocking ops. */
    if (c->flags & REDIS_BLOCKED)
        unblockClientWaitingData(c);
    dictRelease(c->bpop.keys);

    /* UNWATCH all the keys
     * 解除客户端所监听的key
     */
    unwatchAllKeys(c);
    listRelease(c->watched_keys);

    /* Unsubscribe from all the pubsub channels 
     *退订所有频道和模式
     */
    pubsubUnsubscribeAllChannels(c,0);
    pubsubUnsubscribeAllPatterns(c,0);
    dictRelease(c->pubsub_channels);
    listRelease(c->pubsub_patterns);

    /* Close socket, unregister events, and remove list of replies and
     * accumulated arguments. 
     * 
     * 关闭套接字，并从事件处理器中删除该套接字的事件，清空回复列表和参数数组。
     * */
    if (c->fd != -1) {
        aeDeleteFileEvent(server.el,c->fd,AE_READABLE);
        aeDeleteFileEvent(server.el,c->fd,AE_WRITABLE);
        close(c->fd);
    }
    listRelease(c->reply);  // 清空回复缓冲区
    freeClientArgv(c);      // 清空命令参数

    /* Remove from the list of clients 
     * 从服务器的客户端链表中删除自身
     */
    if (c->fd != -1) {
        ln = listSearchKey(server.clients, c);
        redisAssert(ln != NULL);
        listDelNode(server.clients,ln);
    }

    /* When client was just unblocked because of a blocking operation,
     * remove it from the list of unblocked clients. 
     * 
     * 删除客户端的阻塞信息
     * */
    if (c->flags & REDIS_UNBLOCKED) {
        ln = listSearchKey(server.unblocked_clients,c);
        redisAssert(ln != NULL);
        listDelNode(server.unblocked_clients,ln);
    }

    /* Master/slave cleanup Case 1:
     * we lost the connection with a slave. */
    if (c->flags & REDIS_SLAVE) {
        if (c->replstate == REDIS_REPL_SEND_BULK && c->repldbfd != -1)
            close(c->repldbfd);
        list *l = (c->flags & REDIS_MONITOR) ? server.monitors : server.slaves;
        ln = listSearchKey(l,c);
        redisAssert(ln != NULL);
        listDelNode(l,ln);
        /* We need to remember the time when we started to have zero
         * attached slaves, as after some time we'll free the replication
         * backlog. */
        if (c->flags & REDIS_SLAVE && listLength(server.slaves) == 0)
            server.repl_no_slaves_since = server.unixtime;
        refreshGoodSlavesCount();
    }

    /* Master/slave cleanup Case 2:
     * we lost the connection with the master. */
    if (c->flags & REDIS_MASTER) 
        replicationHandleMasterDisconnection();

    /* If this client was scheduled for async freeing we need to remove it
     * from the queue. */
    if (c->flags & REDIS_CLOSE_ASAP) {
        ln = listSearchKey(server.clients_to_close,c);
        redisAssert(ln != NULL);
        listDelNode(server.clients_to_close,ln);
    }

    /* Release other dynamically allocated client structure fields,
     * and finally release the client structure itself. 
     * */
    if (c->name) 
        decrRefCount(c->name);
    zfree(c->argv);             // 清除参数空间
    freeClientMultiState(c);    // 清除事务状态信息
    sdsfree(c->peerid);
    zfree(c);                   // 释放客户端 redisClient 结构本身
}

/* Schedule a client to free it at a safe time in the serverCron() function.
 * This function is useful when we need to terminate a client but we are in
 * a context where calling freeClient() is not possible, because the client
 * should be valid for the continuation of the flow of the program. 
 * 
 * 异步地释放给定的客户端。
 * */
void freeClientAsync(redisClient *c) {
    //若已设置REDIS_CLOSE_ASAP意味已经存在于待关闭链表中，直接返回。
    if (c->flags & REDIS_CLOSE_ASAP) 
        return;
    //设置REDIS_CLOSE_ASAP标识并添加到待关闭链表中。
    c->flags |= REDIS_CLOSE_ASAP;
    listAddNodeTail(server.clients_to_close,c);
}

/* 异步的free客户端 */
void freeClientsInAsyncFreeQueue(void) {
    // 遍历所有要关闭的客户端
    while (listLength(server.clients_to_close)) {
        listNode *ln   = listFirst(server.clients_to_close);
        redisClient *c = listNodeValue(ln);
        // 关闭客户端
        c->flags &= ~REDIS_CLOSE_ASAP;
        freeClient(c);
        // 从客户端链表中删除被关闭的客户端
        listDelNode(server.clients_to_close,ln);
    }
}

/* 将Client中的reply数据传输给套接字fd（socket） 
 * sendReplyToClient函数就是按顺序将响应buffer中的内容，发送出去；
 * 内部是一个while循环，条件是固定buffer或者响应链表不为空。
 * 循环内部首先是尝试发送固定buffer，totwritten记录一次sendReplyToClient函数发送数据的总大小。
 * redisClient->sentlen在发送固定buffer和链表时是复用的。
 * 在发送固定buffer时，如果sentlen等于bufpos，则固定buffer发送完，接下来发送reply链表。
 * 在发送reply链表时，会从链表头部开始依次发送，首先，取头结点，计算对象的长度，
 * 如果节点内容为空，则直接将该节点删除。
 **/
void sendReplyToClient(aeEventLoop *el, int fd, void *privdata, int mask) {
    redisClient *c = privdata;
    int nwritten = 0, totwritten = 0, objlen;
    size_t objmem;
    robj *o;
    REDIS_NOTUSED(el);
    REDIS_NOTUSED(mask);


    // 一直循环，直到回复缓冲区和回复链表为空为止。
    while(c->bufpos > 0 || listLength(c->reply)) {
        
        if (c->bufpos > 0) {

            // 写入内容到套接字
            nwritten = write(fd, c->buf + c->sentlen, c->bufpos - c->sentlen);

            // 写入出错则跳出while处理。
            if (nwritten <= 0)          
                break;
            c->sentlen += nwritten;
            totwritten += nwritten;

            /* If the buffer was sent, set bufpos to zero to continue with
             * the remainder of the reply. 
             * 如果缓冲区中的内容已经全部写入完毕，就重置客户端的两个计数器变量。
             * */
            if (c->sentlen == c->bufpos) {
                c->bufpos  = 0;
                c->sentlen = 0;
            }
        } else {  // listLength(c->reply) != 0

            // 取出位于链表最前面的对象
            o = listNodeValue(listFirst(c->reply));
            objlen = sdslen(o->ptr);
            objmem = zmalloc_size_sds(o->ptr);

            // 略过空对象
            if (objlen == 0) {
                listDelNode(c->reply,listFirst(c->reply));
                continue;
            }

            nwritten = write(fd, ((char*)o->ptr) + c->sentlen, objlen - c->sentlen);
            if (nwritten <= 0) 
                break;
            c->sentlen += nwritten;
            totwritten += nwritten;

            /* If we fully sent the object on head go to the next one 
             * 如果缓冲区内容全部写入完毕，那么删除已写入完毕的节点
             **/
            if (c->sentlen == objlen) {
                listDelNode(c->reply,listFirst(c->reply));
                c->sentlen      = 0;
                c->reply_bytes -= objmem;
            }
        }
        /* Note that we avoid to send more than REDIS_MAX_WRITE_PER_EVENT
         * bytes, in a single threaded server it's a good idea to serve
         * other clients as well, even if a very large request comes from
         * super fast link that is always able to accept data (in real world
         * scenario think about 'KEYS *' against the loopback interface).
         *
         * However if we are over the maxmemory limit we ignore that and
         * just deliver as much data as it is possible to deliver. 
         * 
         * 为了所有客户端的公平，为了避免一些量非常大的写回复操作独占服务器，
         * 当写入的总数量大于 REDIS_MAX_WRITE_PER_EVENT(64KB)时，中断当前while循环，
         * 结束本轮次循环写入，将处理时间让给其他客户端，剩余的内容等下次写入
         * 就绪（epoll触发）时，再继续写入。
         *
         * However if we are over the maxmemory limit we ignore that and
         * just deliver as much data as it is possible to deliver. 
         *
         * 不过，如果服务器的内存占用已经超过了限制，那么为了将回复缓冲区中的
         * 内容尽快写入给客户端，然后释放回复缓冲区的空间来回收内存，这时即使
         * 写入量超过了 REDIS_MAX_WRITE_PER_EVENT ，程序也继续进行写入，以尽快
         * 释放空间。
         * 
         * */
        if (totwritten > REDIS_MAX_WRITE_PER_EVENT &&
                (server.maxmemory == 0 || zmalloc_used_memory() < server.maxmemory)) 
            break;
    }
    //跳出while循环，首先会判断write是否出错，如果出错，记录日志并停止这个客户端。
    if (nwritten == -1) {
        if (errno == EAGAIN) {  //(nread == -1 && errno == EAGAIN) ==> 系统中断
            nwritten = 0;
        } else {                //(nread == -1 && errno != EAGAIN) ==> 读取出错
            redisLog(REDIS_VERBOSE, "Error writing to client: %s", strerror(errno));
            freeClient(c);
            return;
        }
    }
    //如果发送出数据，更新该客户端redisClient->lastinteraction（用于清理timeout的客户端）。   
    if (totwritten > 0) {
        /* For clients representing masters we don't count sending data
         * as an interaction, since we always send REPLCONF ACK commands
         * that take some time to just fill the socket output buffer.
         * We just rely on data / pings received for timeout detection. */
        if (!(c->flags & REDIS_MASTER)) 
            c->lastinteraction = server.unixtime;
    }

    //最后判断一下响应内容是不是全部发送完。如果发送完，则删除写事件。
    //否则，下一轮事件循环继续发送响应内容，直到完成为止。
    if (c->bufpos == 0 && listLength(c->reply) == 0) {
        //重置变量
        c->sentlen = 0;

        //写完成之后，删除写事件。
        aeDeleteFileEvent(server.el,c->fd,AE_WRITABLE); 

        /* Close connection after entire reply has been sent. */
        // 如果指定了写入之后关闭客户端的 FLAG ，那么发送完毕关闭客户端。
        if (c->flags & REDIS_CLOSE_AFTER_REPLY) 
            freeClient(c);
    }
}

/* resetClient prepare the client to process the next command */
// 在客户端执行完命令之后执行：重置客户端以准备执行下个命令。
void resetClient(redisClient *c) {
    freeClientArgv(c);
    c->reqtype      = 0;
    c->multibulklen = 0;
    c->bulklen      = -1;
    /* We clear the ASKING flag as well if we are not inside a MULTI. */
    if (!(c->flags & REDIS_MULTI)) 
        c->flags &= (~REDIS_ASKING);
}

/*
 * 解析c->querybuf中的内联命令（和参数），设置c->argc和c->argv。
 *
 * 内联命令的各个参数以空格分割，并以 \r\n 结尾，
 * 例子：<arg0> <arg1> <arg...> <argN>\r\n
 * 
 * 把命令和参数保存在argv数组中，把参数个数保存在argc中，如：
 * argv[0] = arg0
 * argv[1] = arg1
 * argv[2] = arg2
 *
 * argc = 3
 * 
 */


这个函数是解析inline请求。格式上，inline请求就是诸如”+PING\r\n”。在解析时，只需要查找\n即可。
        首先，就是查找\n。如果没有找到，说明buffer中的内容不是完整的请求，然后判断一下querybuf的大小是否超过REDIS_INLINE_MAX_SIZE（64KB），超过则向客户端发送错误响应。否则，返回REDIS_ERR，继续read数据。



int processInlineBuffer(redisClient *c) {
    char *newline;
    int argc, j;
    sds *argv, aux;
    size_t querylen;

    /* Search for end of line */
    newline = strchr(c->querybuf,'\n');

    /* Nothing to do without a \r\n 
     *
     * 收到的查询内容不符合协议格式（不包含\n或者长度太长），出错
     */
    if (newline == NULL) {
        if (sdslen(c->querybuf) > REDIS_INLINE_MAX_SIZE) {
            addReplyError(c,"Protocol error: too big inline request");
            setProtocolError(c,0);
        }
        return REDIS_ERR;
    }

    /* Handle the \r\n case. **/
    if (newline && newline != c->querybuf && *(newline-1) == '\r')
        newline--;

    /* Split the input buffer up to the \r\n */
    // 根据空格，分割命令的参数
    // 比如说 SET msg hello \r\n 将分割为
    // argv[0] = SET
    // argv[1] = msg
    // argv[2] = hello
    // argc = 3
    querylen = newline-(c->querybuf);
    aux  = sdsnewlen(c->querybuf,querylen);
    argv = sdssplitargs(aux,&argc);
    sdsfree(aux);
    if (argv == NULL) {
        addReplyError(c,"Protocol error: unbalanced quotes in request");
        setProtocolError(c,0);
        return REDIS_ERR;
    }


    /* Newline from slaves can be used to refresh the last ACK time.
     * This is useful for a slave to ping back while loading a big
     * RDB file. */
    if (querylen == 0 && c->flags & REDIS_SLAVE)
        c->repl_ack_time = server.unixtime;

    /* Leave data after the first line of the query in the buffer */
    // 从缓冲区中删除已由 argv 解析获取的内容，剩余的内容是未读取的。
    sdsrange(c->querybuf,querylen+2,-1);

    /* Setup argv array on client structure */
    // 为客户端client的参数argv分配空间
    if (c->argv) 
        zfree(c->argv);
    c->argv = zmalloc(sizeof(robj*) * argc);


    /* Create redis objects for all arguments. */
    // 为每个参数创建一个字符串对象
    for (c->argc = 0, j = 0; j < argc; j++) {
        if (sdslen(argv[j])) {
            // argv[j] 已经是 SDS 了，所以创建的字符串对象直接指向该 SDS
            c->argv[c->argc] = createObject(REDIS_STRING,argv[j]);
            c->argc++;
        } else {
            // 无效argv[j]直接释放
            sdsfree(argv[j]);
        }
    }
    zfree(argv);
    return REDIS_OK;
}


/* Helper function. Trims query buffer to make the function that processes
 * multi bulk requests idempotent. 
 * 
 * 如果在读入协议内容时，发现内容不符合协议，那么异步地关闭这个客户端。
 * 
 * */
static void setProtocolError(redisClient *c, int pos) {
    if (server.verbosity >= REDIS_VERBOSE) {
        sds client = catClientInfoString(sdsempty(),c);
        redisLog(REDIS_VERBOSE, "Protocol error from client: %s", client);
        sdsfree(client);
    }
    c->flags |= REDIS_CLOSE_AFTER_REPLY;
    sdsrange(c->querybuf,pos,-1);
}

/*
 * 将 c->querybuf 中的协议内容转换成 c->argv 中的参数对象
 * 
 * 比如 *3\r\n$3\r\nSET\r\n$3\r\nMSG\r\n$5\r\nHELLO\r\n
 * 将被转换为：
 * argv[0] = SET
 * argv[1] = MSG
 * argv[2] = HELLO
 */

/*
首先对redis的命令协议进行简要介绍，以下是这个协议的一般形式：  
*<参数数量>CRLF  
$<参数1的字节数量>CRLF  
<参数1的数据>CRLF  
...
$<参数N的字节数量>CRLF  
<参数N的数据>CRLF  
...

举个例子，以下是一个命令协议的打印版本：  
*3  
$3  
SET  
$3  
foo  
$3  
bar  
这个命令的实际协议值如下："*3\r\n$3\r\nSET\r\n$3\r\foo\r\n$3\r\bar\r\n"  


那么下面的代码就非常好理解了：
    首先解析'*'后面的字符串转成long long类型，这样就知道了参数的个数。
    接着遍历参数个数解析$后面的数字为参数的长度，并且读取参数。
    最后，类似processInlineBuffer，将参数存入到了client的argv数组中。

经过以上几个步骤，querybuf中的数据算是解析完了。
最后，外部的processInputBuffer函数将会调用processCommand函数来处理解析完毕的命令行参数。
*/
int processMultibulkBuffer(redisClient *c) {
    char *newline = NULL;
    int pos = 0, ok;
    long long ll;

    // 读入命令的参数个数
    // 比如 *3\r\n$3\r\nSET\r\n... 将令 c->multibulklen = 3
    // 
    // 参数数目为0，表示这是新的请求指令  
    if (c->multibulklen == 0) {
        /* The client should have been reset */
        redisAssertWithInfo(c,NULL,c->argc == 0);

        /* Multi bulk length cannot be read without a \r\n */
        // 检查缓冲区的内容第一个 "\r\n"
        newline = strchr(c->querybuf,'\r');
        if (newline == NULL) {
            if (sdslen(c->querybuf) > REDIS_INLINE_MAX_SIZE) {
                addReplyError(c,"Protocol error: too big mbulk count string");
                setProtocolError(c,0);
            }
            return REDIS_ERR;
        }

        /* Buffer should also contain \n */
        if (newline-(c->querybuf) > ((signed)sdslen(c->querybuf)-2))
            return REDIS_ERR;

        /* We know for sure there is a whole line since newline != NULL,
         * so go ahead and find out the multi bulk length. */

        // 协议的第一个字符必须是 '*'
        redisAssertWithInfo(c,NULL,c->querybuf[0] == '*');


        // 将参数个数，也即是 * 之后， \r\n 之前的数字取出并保存到 ll 中
        // 比如对于 *3\r\n ，那么 ll 将等于 3
        ok = string2ll(c->querybuf+1,newline - (c->querybuf+1),&ll);
        // 参数的数量超出限制
        if (!ok || ll > 1024*1024) {
            addReplyError(c,"Protocol error: invalid multibulk length");
            setProtocolError(c,pos);
            return REDIS_ERR;
        }
        // 参数数量之后的位置
        // 比如对于 *3\r\n$3\r\n$SET\r\n... 来说，
        // pos 指向 *3\r\n$3\r\n$SET\r\n...
        //                ^
        //                |
        //               pos
        pos = (newline - c->querybuf)+2;
        // 如果 ll <= 0 ，那么这个命令是一个空白命令
        // 那么将这段内容从查询缓冲区中删除，只保留未阅读的那部分内容
        // 为什么参数可以是空的呢？
        // processInputBuffer 中有注释到 "Multibulk processing could see a <= 0 length"
        // 但并没有详细说明原因
        if (ll <= 0) {
            sdsrange(c->querybuf,pos,-1);
            return REDIS_OK;
        }

        // 设置参数数量
        c->multibulklen = ll;

        /* Setup argv array on client structure */
        // 根据参数数量，为各个参数对象分配空间
        if (c->argv) 
            zfree(c->argv);
        c->argv = zmalloc(sizeof(robj*)*c->multibulklen);
    }

    redisAssertWithInfo(c,NULL,c->multibulklen > 0);

    // 从 c->querybuf 中读入参数，并创建各个参数对象到 c->argv
    while(c->multibulklen) {

        /* Read bulk length if unknown */
        // 读入参数长度
        if (c->bulklen == -1) {

            // 确保 "\r\n" 存在
            newline = strchr(c->querybuf+pos,'\r');
            if (newline == NULL) {
                if (sdslen(c->querybuf) > REDIS_INLINE_MAX_SIZE) {
                    addReplyError(c,"Protocol error: too big bulk count string");
                    //设置协议错误
                    setProtocolError(c,0);
                    return REDIS_ERR;
                }
                break;
            }

            /* Buffer should also contain \n */
            if (newline-(c->querybuf) > ((signed)sdslen(c->querybuf)-2))
                break;

            // 确保协议符合参数格式，检查其中的 $...
            // 比如 $3\r\nSET\r\n
            if (c->querybuf[pos] != '$') {
                addReplyErrorFormat(c,"Protocol error: expected '$', got '%c'",c->querybuf[pos]);
                setProtocolError(c,pos);
                return REDIS_ERR;
            }

            // 读取长度
            // 比如 $3\r\nSET\r\n 将会让 ll 的值设置 3
            ok = string2ll(c->querybuf+pos+1,newline-(c->querybuf+pos+1),&ll);
            if (!ok || ll < 0 || ll > 512*1024*1024) {
                addReplyError(c,"Protocol error: invalid bulk length");
                setProtocolError(c,pos);
                return REDIS_ERR;
            }


            // 定位到参数的开头
            // 比如 
            // $3\r\nSET\r\n...
            //       ^
            //       |
            //      pos
            pos += newline-(c->querybuf+pos)+2;

            // 如果参数非常长，那么做一些预备措施来优化接下来的参数复制操作
            if (ll >= REDIS_MBULK_BIG_ARG) {
                size_t qblen;

                /* If we are going to read a large object from network
                 * try to make it likely that it will start at c->querybuf
                 * boundary so that we can optimize object creation
                 * avoiding a large copy of data. */
                sdsrange(c->querybuf,pos,-1);
                pos = 0;
                qblen = sdslen(c->querybuf);
                /* Hint the sds library about the amount of bytes this string is
                 * going to contain. */
                if (qblen < (size_t)ll+2)
                    c->querybuf = sdsMakeRoomFor(c->querybuf,ll+2-qblen);
            }

            // 参数的长度
            c->bulklen = ll;
        }

        /* Read bulk argument */
        // 读入参数
        if (sdslen(c->querybuf)-pos < (unsigned)(c->bulklen+2)) {

            /* Not enough data (+2 == trailing \r\n) */
            // 确保内容符合协议格式
            // 比如 $3\r\nSET\r\n 就检查 SET 之后的 \r\n
            /* Not enough data (+2 == trailing \r\n) */
            break;
        } else {
            /* Optimization: if the buffer contains JUST our bulk element
             * instead of creating a new object by *copying* the sds we
             * just use the current sds string. */
            
            // 为参数创建字符串对象  
            if (pos == 0 && c->bulklen >= REDIS_MBULK_BIG_ARG && (signed) sdslen(c->querybuf) == c->bulklen+2)
            {
                c->argv[c->argc++] = createObject(REDIS_STRING,c->querybuf);
                sdsIncrLen(c->querybuf,-2); /* remove CRLF */
                c->querybuf = sdsempty();
                /* Assume that if we saw a fat argument we'll see another one
                 * likely... */
                c->querybuf = sdsMakeRoomFor(c->querybuf,c->bulklen+2);
                pos = 0;
            } else {
                c->argv[c->argc++] = createStringObject(c->querybuf+pos,c->bulklen);
                pos += c->bulklen+2;
            }

            // 清空参数长度
            c->bulklen = -1;
            // 减少还需读入的参数个数
            c->multibulklen--;
        }
    }

    /* Trim to pos */
    // 从 querybuf 中删除已被读取的内容
    if (pos) 
        sdsrange(c->querybuf,pos,-1);

    /* We're done when c->multibulk == 0 */
    // 如果本条命令的所有参数都已读取完，那么返回
    if (c->multibulklen == 0) 
        return REDIS_OK;

    /* Still not read to process the command */
    // 如果还有参数未读取完，那么就协议内容有错
    return REDIS_ERR;
}

/* 解析并处理redisClient的查询querybuffer。
 *
 * readQueryFromClient()从客户端fd读取查询请求存到querybuf,processInputBuffer()对请求数据进行协议解析，
 * Redis支持两种协议，一种是inline，一种是multibulk。
 * inline协议是老协议，现在一般只在命令行下的redis客户端使用，其他情况一般是使用multibulk协议。
 * 如果客户端传送的数据的第一个字符时‘*’，那么传送数据将被当做multibulk协议处理，否则将被当做inline协议处理。
 * Inline协议的具体解析函数是processInlineBuffer，multibulk协议的具体解析函数是processMultibulkBuffer。
 * 当协议解析完毕，即客户端传送的数据已经解析出命令字段和参数字段，接下来进行命令处理，命令处理函数是processCommand。
 **/
void processInputBuffer(redisClient *c) {

    /* Keep processing while there is something in the input buffer */
    //只要querybuf中还包含请求数据就会一直while循环处理。
    while(sdslen(c->querybuf)) {

        /* Return if clients are paused. */
        // 如果客户端正处于暂停状态，那么直接返回
        if (!(c->flags & REDIS_SLAVE) && clientsArePaused()) 
            return;

        /* Immediately abort if the client is in the middle of something. */
        // REDIS_BLOCKED 状态表示客户端正在被阻塞
        if (c->flags & REDIS_BLOCKED) 
            return;

        /* REDIS_CLOSE_AFTER_REPLY closes the connection once the reply is
         * written to the client. Make sure to not let the reply grow after
         * this flag has been set (i.e. don't process more commands). */
        // 客户端已经设置了关闭 FLAG ，没有必要处理命令了
        if (c->flags & REDIS_CLOSE_AFTER_REPLY) 
            return;

        /* Determine request type when unknown. */
        // 判断请求的类型
        // 两种类型的区别可以在 Redis 的通讯协议上查到：
        // http://redis.readthedocs.org/en/latest/topic/protocol.html
        // 简单来说，多条查询是一般客户端发送来的，
        // 而内联查询则是 TELNET 发送来的
        if (!c->reqtype) {
            if (c->querybuf[0] == '*') {
                // 多条查询
                c->reqtype = REDIS_REQ_MULTIBULK;
            } else {
                // 内联查询
                c->reqtype = REDIS_REQ_INLINE;
            }
        }
	
		//根据Request请求的Type的不同，用不同的处理方法解析缓冲区querybuf中的内容，
        //将querybuf解析成请求参数数组，存于client->argv，client->argc中，
        //
        //注意，从网络读取client请求，不能保证当前querybuffer中的数据包含一个完整的  
        //命令，可能需要多次read。只有buffer中包含一个完整请求时，这两个函数才会解析
        //成功返回REDIS_OK，接下来会处理命令。否则，会跳出外部的while循环，等待下一次
        //事件循环再从socket读取剩余的数据，再进行解析。
        if (c->reqtype == REDIS_REQ_INLINE) {
            if (processInlineBuffer(c) != REDIS_OK) 
                break;
        } else if (c->reqtype == REDIS_REQ_MULTIBULK) {
            if (processMultibulkBuffer(c) != REDIS_OK) 
                break;
        } else {
            redisPanic("Unknown request type");
        }

        /* Multibulk processing could see a <= 0 length. */
        if (c->argc == 0) {
            resetClient(c);
        } else {
            /* Only reset the client when the command was executed. */
            //当上述两个函数返回REDIS_OK且c->argc!=0，即表示已经解析出一个命令，
            //接下来调用processCommand函数进行命令处理，执行结束重置客户端，记录precmd、释放参数列表等。
            if (processCommand(c) == REDIS_OK)
                resetClient(c);
        }
    }
}

/* 从Client fd读取查询请求query。
 *
 * 调用系统函数read来读取客户端传送过来的查询请求，
 * 调用read后对读取过程中被系统中断的情况（nread == -1 && errno == EAGAIN）、
 * 客户端关闭的情况（nread == 0），以及其他出错情况进行了判断处理。
 * 如果读取的数据超过限制（1GB）则报错并清空客户端结构。
 * 读取完后进入processInputBuffer进行协议解析。
 * */
void readQueryFromClient(aeEventLoop *el, int fd, void *privdata, int mask) {
    redisClient *c = (redisClient*) privdata;
    int nread, readlen;
    size_t qblen;
    REDIS_NOTUSED(el);
    REDIS_NOTUSED(mask);


    // 设置服务器的当前客户端
    server.current_client = c;

    // 读入长度（默认为 16 MB）
    readlen = REDIS_IOBUF_LEN;

    /* If this is a multi bulk request, and we are processing a bulk reply
     * that is large enough, try to maximize the probability that the query
     * buffer contains exactly the SDS string representing the object, even
     * at the risk of requiring more read(2) calls. This way the function
     * processMultiBulkBuffer() can avoid copying buffers to create the
     * Redis Object representing the argument. */
    //重新设置读取数据的大小，避免频繁拷贝数据。
    //如果当前请求是一个multi bulk类型的，并且要处理的bulk的大小大于REDIS_MBULK_BIG_ARG（32KB），
    //则将读取数据大小设置为该bulk剩余数据的大小。
    if (c->reqtype == REDIS_REQ_MULTIBULK && c->multibulklen && c->bulklen != -1 && c->bulklen >= REDIS_MBULK_BIG_ARG)
    {
        int remaining = (unsigned)(c->bulklen+2)-sdslen(c->querybuf);
        if (remaining < readlen) 
            readlen = remaining;
    }

    // 获取查询缓冲区当前内容的长度
    // 如果读取出现 short read ，那么可能会有内容滞留在读取缓冲区里面
    // 这些滞留内容也许不能完整构成一个符合协议的命令，
    qblen = sdslen(c->querybuf);

    // 如果有需要，更新缓冲区内容长度的峰值（peak）
    if (c->querybuf_peak < qblen) 
        c->querybuf_peak = qblen;

    //对querybuf的空间进行扩展  
    c->querybuf = sdsMakeRoomFor(c->querybuf, readlen);

    // 读取客户端发来的操作指令，将指令内容存储到c->querybuf
    nread = read(fd, c->querybuf+qblen, readlen);
    
    // 读入出错
    if (nread == -1) {
        if (errno == EAGAIN) {
            nread = 0;
        } else {
            redisLog(REDIS_VERBOSE, "Reading from client: %s",strerror(errno));
            freeClient(c);
            return;
        }

    // 遇到 EOF
    } else if (nread == 0) {
        redisLog(REDIS_VERBOSE, "Client closed connection");
        freeClient(c);
        return;
    }

    if (nread) {
        // 改变querybuf的实际长度和空闲长度，len += nread, free -= nread;  
        sdsIncrLen(c->querybuf,nread);
        // 记录服务器和客户端最后一次互动的时间
        c->lastinteraction = server.unixtime;
        // 如果客户端是 master 的话，更新它的复制偏移量
        if (c->flags & REDIS_MASTER) 
            c->reploff += nread;
    } else {
        // 在 nread == -1 且 errno == EAGAIN 时运行
        server.current_client = NULL;
        return;
    }

    //客户端请求的字符串长度大于服务器最大的请求长度值client_max_querybuf_len（1GB），
    //会拒绝服务，清空缓冲区并释放客户端。
    if (sdslen(c->querybuf) > server.client_max_querybuf_len) {
        sds ci = catClientInfoString(sdsempty(),c), bytes = sdsempty();
        bytes  = sdscatrepr(bytes,c->querybuf,64);
        redisLog(REDIS_WARNING,"Closing client that reached max query buffer length: %s (qbuf initial bytes: %s)", ci, bytes);
        sdsfree(ci);
        sdsfree(bytes);
        freeClient(c);
        return;
    }


    //解析请求  
    processInputBuffer(c);
    server.current_client = NULL;
}

/* 获取Client中输入buffer和输出buffer的最大长度值 */
void getClientsMaxBuffers(unsigned long *longest_output_list,
                          unsigned long *biggest_input_buffer) {
    redisClient *c;
    listNode *ln;
    listIter li;
    unsigned long lol = 0, bib = 0;

    listRewind(server.clients,&li);
    while ((ln = listNext(&li)) != NULL) {
        c = listNodeValue(ln);
		
		//通过遍历比较，算出最大值
        if (listLength(c->reply) > lol) lol = listLength(c->reply);   //输出buffer的最大值
        if (sdslen(c->querybuf) > bib) bib = sdslen(c->querybuf);   //输入的buffer即查询的buffer
    }
    *longest_output_list = lol;
    *biggest_input_buffer = bib;
}

/* This is a helper function for genClientPeerId().
 * It writes the specified ip/port to "peerid" as a null termiated string
 * in the form ip:port if ip does not contain ":" itself, otherwise
 * [ip]:port format is used (for IPv6 addresses basically). */
/* 格式化ip,port端口号的输出，ip:port */
void formatPeerId(char *peerid, size_t peerid_len, char *ip, int port) {
    if (strchr(ip,':'))
        snprintf(peerid,peerid_len,"[%s]:%d",ip,port);
    else
        snprintf(peerid,peerid_len,"%s:%d",ip,port);
}

/* A Redis "Peer ID" is a colon separated ip:port pair.
 * For IPv4 it's in the form x.y.z.k:pork, example: "127.0.0.1:1234".
 * For IPv6 addresses we use [] around the IP part, like in "[::1]:1234".
 * For Unix socekts we use path:0, like in "/tmp/redis:0".
 *
 * A Peer ID always fits inside a buffer of REDIS_PEER_ID_LEN bytes, including
 * the null term.
 *
 * The function returns REDIS_OK on succcess, and REDIS_ERR on failure.
 *
 * On failure the function still populates 'peerid' with the "?:0" string
 * in case you want to relax error checking or need to display something
 * anyway (see anetPeerToString implementation for more info). */
/* 获取Client客户端的ip,port地址信息 */
int genClientPeerId(redisClient *client, char *peerid, size_t peerid_len) {
    char ip[REDIS_IP_STR_LEN];
    int port;

    if (client->flags & REDIS_UNIX_SOCKET) {
        /* Unix socket client. */
        snprintf(peerid,peerid_len,"%s:0",server.unixsocket);
        return REDIS_OK;
    } else {
        /* TCP client. */
        int retval = anetPeerToString(client->fd,ip,sizeof(ip),&port);
        formatPeerId(peerid,peerid_len,ip,port);
        return (retval == -1) ? REDIS_ERR : REDIS_OK;
    }
}

/* This function returns the client peer id, by creating and caching it
 * if client->perrid is NULL, otherwise returning the cached value.
 * The Peer ID never changes during the life of the client, however it
 * is expensive to compute. */
/* 获取c->peerid客户端的地址信息 */
char *getClientPeerId(redisClient *c) {
    char peerid[REDIS_PEER_ID_LEN];

    if (c->peerid == NULL) {
        genClientPeerId(c,peerid,sizeof(peerid));
        c->peerid = sdsnew(peerid);
    }
    return c->peerid;
}

/* Concatenate a string representing the state of a client in an human
 * readable format, into the sds string 's'. */
// 获取客户端的各项属性信息，将它们储存到 sds 值 s 里面，并返回。
sds catClientInfoString(sds s, redisClient *client) {
    char flags[16], events[3], *p;
    int emask;

    //获取redis client的属性。
    p = flags;
    if (client->flags & REDIS_SLAVE) {
        if (client->flags & REDIS_MONITOR)
            *p++ = 'O';
        else
            *p++ = 'S';
    }
    if (client->flags & REDIS_MASTER)           *p++ = 'M';
    if (client->flags & REDIS_MULTI)            *p++ = 'x';
    if (client->flags & REDIS_BLOCKED)          *p++ = 'b';
    if (client->flags & REDIS_DIRTY_CAS)        *p++ = 'd';
    if (client->flags & REDIS_CLOSE_AFTER_REPLY)*p++ = 'c';
    if (client->flags & REDIS_UNBLOCKED)        *p++ = 'u';
    if (client->flags & REDIS_CLOSE_ASAP)       *p++ = 'A';
    if (client->flags & REDIS_UNIX_SOCKET)      *p++ = 'U';
    if (p == flags)                             *p++ = 'N';
    *p++ = '\0';

    //获取redis client的文件IO事件属性。
    emask = client->fd == -1 ? 0 : aeGetFileEvents(server.el,client->fd);
    p = events;
    if (emask & AE_READABLE) 
        *p++ = 'r';
    if (emask & AE_WRITABLE) 
        *p++ = 'w';
    *p = '\0';
    
    //最后格式化输出结果
    return sdscatfmt(s,
        "id=%U addr=%s fd=%i name=%s age=%I idle=%I flags=%s db=%i sub=%i psub=%i multi=%i qbuf=%U qbuf-free=%U obl=%U oll=%U omem=%U events=%s cmd=%s",
        (unsigned long long) client->id,
        getClientPeerId(client),
        client->fd,
        client->name ? (char*)client->name->ptr : "",
        (long long)(server.unixtime - client->ctime),
        (long long)(server.unixtime - client->lastinteraction),
        flags,
        client->db->id,
        (int) dictSize(client->pubsub_channels),
        (int) listLength(client->pubsub_patterns),
        (client->flags & REDIS_MULTI) ? client->mstate.count : -1,
        (unsigned long long) sdslen(client->querybuf),
        (unsigned long long) sdsavail(client->querybuf),
        (unsigned long long) client->bufpos,
        (unsigned long long) listLength(client->reply),
        (unsigned long long) getClientOutputBufferMemoryUsage(client),
        events,
        client->lastcmd ? client->lastcmd->name : "NULL");
}

/* 获取所有Client客户端的属性信息，并连接成一个总的字符串并输出 */
sds getAllClientsInfoString(void) {
    listNode *ln;
    listIter li;
    redisClient *client;
    sds o = sdsempty();
	
	//字符串重新调整空间
    o = sdsMakeRoomFor(o,200*listLength(server.clients));
    listRewind(server.clients,&li);
    while ((ln = listNext(&li)) != NULL) {
        client = listNodeValue(ln);
        o = catClientInfoString(o,client);
        //获取一个Client的属性信息，调用cat连接操作
        o = sdscatlen(o,"\n",1);
    }
    return o;
}

/* 执行客户端的命令的作法 */
void clientCommand(redisClient *c) {
    listNode *ln;
    listIter li;
    redisClient *client;

	//对客户端的命令进行解析对比
    if (!strcasecmp(c->argv[1]->ptr,"list") && c->argc == 2) {
        /* CLIENT LIST */
        sds o = getAllClientsInfoString();
        addReplyBulkCBuffer(c,o,sdslen(o));
        sdsfree(o);
    } else if (!strcasecmp(c->argv[1]->ptr,"kill")) {
        /* CLIENT KILL <ip:port>
         * CLIENT KILL <option> [value] ... <option> [value] */
        char *addr = NULL;
        int type = -1;
        uint64_t id = 0;
        int skipme = 1;
        int killed = 0, close_this_client = 0;

        if (c->argc == 3) {
            /* Old style syntax: CLIENT KILL <addr> */
            addr = c->argv[2]->ptr;
            skipme = 0; /* With the old form, you can kill yourself. */
        } else if (c->argc > 3) {
            int i = 2; /* Next option index. */

            /* New style syntax: parse options. */
            while(i < c->argc) {
                int moreargs = c->argc > i+1;

                if (!strcasecmp(c->argv[i]->ptr,"id") && moreargs) {
                    long long tmp;

                    if (getLongLongFromObjectOrReply(c,c->argv[i+1],&tmp,NULL)
                        != REDIS_OK) return;
                    id = tmp;
                } else if (!strcasecmp(c->argv[i]->ptr,"type") && moreargs) {
                    type = getClientTypeByName(c->argv[i+1]->ptr);
                    if (type == -1) {
                        addReplyErrorFormat(c,"Unknown client type '%s'",
                            (char*) c->argv[i+1]->ptr);
                        return;
                    }
                } else if (!strcasecmp(c->argv[i]->ptr,"addr") && moreargs) {
                    addr = c->argv[i+1]->ptr;
                } else if (!strcasecmp(c->argv[i]->ptr,"skipme") && moreargs) {
                    if (!strcasecmp(c->argv[i+1]->ptr,"yes")) {
                        skipme = 1;
                    } else if (!strcasecmp(c->argv[i+1]->ptr,"no")) {
                        skipme = 0;
                    } else {
                        addReply(c,shared.syntaxerr);
                        return;
                    }
                } else {
                	//将反馈结果输出到c->reply中
                    addReply(c,shared.syntaxerr);
                    return;
                }
                i += 2;
            }
        } else {
            addReply(c,shared.syntaxerr);
            return;
        }

        /* Iterate clients killing all the matching clients. */
        listRewind(server.clients,&li);
        //逐个kill Client
        while ((ln = listNext(&li)) != NULL) {
            client = listNodeValue(ln);
            if (addr && strcmp(getClientPeerId(client),addr) != 0) continue;
            if (type != -1 &&
                (client->flags & REDIS_MASTER ||
                 getClientType(client) != type)) continue;
            if (id != 0 && client->id != id) continue;
            if (c == client && skipme) continue;

            /* Kill it. */
            if (c == client) {
                close_this_client = 1;
            } else {
                freeClient(client);
            }
            killed++;
        }

        /* Reply according to old/new format. */
        if (c->argc == 3) {
            if (killed == 0)
                addReplyError(c,"No such client");
            else
                addReply(c,shared.ok);
        } else {
            addReplyLongLong(c,killed);
        }

        /* If this client has to be closed, flag it as CLOSE_AFTER_REPLY
         * only after we queued the reply to its output buffers. */
        if (close_this_client) c->flags |= REDIS_CLOSE_AFTER_REPLY;
    } else if (!strcasecmp(c->argv[1]->ptr,"setname") && c->argc == 3) {
        int j, len = sdslen(c->argv[2]->ptr);
        char *p = c->argv[2]->ptr;

        /* Setting the client name to an empty string actually removes
         * the current name. */
        if (len == 0) {
            if (c->name) decrRefCount(c->name);
            c->name = NULL;
            addReply(c,shared.ok);
            return;
        }

        /* Otherwise check if the charset is ok. We need to do this otherwise
         * CLIENT LIST format will break. You should always be able to
         * split by space to get the different fields. */
        for (j = 0; j < len; j++) {
            if (p[j] < '!' || p[j] > '~') { /* ASCII is assumed. */
                addReplyError(c,
                    "Client names cannot contain spaces, "
                    "newlines or special characters.");
                return;
            }
        }
        if (c->name) decrRefCount(c->name);
        c->name = c->argv[2];
        incrRefCount(c->name);
        addReply(c,shared.ok);
    } else if (!strcasecmp(c->argv[1]->ptr,"getname") && c->argc == 2) {
        if (c->name)
        	//获取客户端的name，属性
            addReplyBulk(c,c->name);
        else
            addReply(c,shared.nullbulk);
    } else {
        addReplyError(c, "Syntax error, try CLIENT (LIST | KILL ip:port | GETNAME | SETNAME connection-name)");
    }
}

/* Rewrite the command vector of the client. All the new objects ref count
 * is incremented. The old command vector is freed, and the old objects
 * ref count is decremented. */
/* 重写客户端的命令集合，旧的命令集合的应用计数减1，新的Command  Vector的命令集合增1 */
void rewriteClientCommandVector(redisClient *c, int argc, ...) {
    va_list ap;
    int j;
    robj **argv; /* The new argument vector */

    argv = zmalloc(sizeof(robj*)*argc);
    va_start(ap,argc);
    for (j = 0; j < argc; j++) {
        robj *a;

        a = va_arg(ap, robj*);
        //从变长参数中获取robj
        argv[j] = a;
        //增加引用计数
        incrRefCount(a);
    }
    /* We free the objects in the original vector at the end, so we are
     * sure that if the same objects are reused in the new vector the
     * refcount gets incremented before it gets decremented. */
    for (j = 0; j < c->argc; j++) decrRefCount(c->argv[j]);
    zfree(c->argv);
    /* Replace argv and argc with our new versions. */
    c->argv = argv;
    c->argc = argc;
    c->cmd = lookupCommandOrOriginal(c->argv[0]->ptr);
    redisAssertWithInfo(c,NULL,c->cmd != NULL);
    va_end(ap);
}

/* Rewrite a single item in the command vector.
 * The new val ref count is incremented, and the old decremented. */
/* 重写Client中的第i个参数 */
void rewriteClientCommandArgument(redisClient *c, int i, robj *newval) {
    robj *oldval;

    redisAssertWithInfo(c,NULL,i < c->argc);
    oldval = c->argv[i];
    c->argv[i] = newval;
    //对象的引用计数的改变
    incrRefCount(newval);
    decrRefCount(oldval);

    /* If this is the command name make sure to fix c->cmd. */
    if (i == 0) {
        c->cmd = lookupCommandOrOriginal(c->argv[0]->ptr);
        redisAssertWithInfo(c,NULL,c->cmd != NULL);
    }
}

/* This function returns the number of bytes that Redis is virtually
 * using to store the reply still not read by the client.
 * It is "virtual" since the reply output list may contain objects that
 * are shared and are not really using additional memory.
 *
 * The function returns the total sum of the length of all the objects
 * stored in the output list, plus the memory used to allocate every
 * list node. The static reply buffer is not taken into account since it
 * is allocated anyway.
 *
 * Note: this function is very fast so can be called as many time as
 * the caller wishes. The main usage of this function currently is
 * enforcing the client output length limits. */
/* 获取Client中已经用去的输出buffer的大小 */
unsigned long getClientOutputBufferMemoryUsage(redisClient *c) {
    unsigned long list_item_size = sizeof(listNode)+sizeof(robj);

    return c->reply_bytes + (list_item_size*listLength(c->reply));
}

/* Get the class of a client, used in order to enforce limits to different
 * classes of clients.
 *
 * The function will return one of the following:
 * REDIS_CLIENT_TYPE_NORMAL -> Normal client
 * REDIS_CLIENT_TYPE_SLAVE  -> Slave or client executing MONITOR command
 * REDIS_CLIENT_TYPE_PUBSUB -> Client subscribed to Pub/Sub channels
 */
int getClientType(redisClient *c) {
    if ((c->flags & REDIS_SLAVE) && !(c->flags & REDIS_MONITOR))
        return REDIS_CLIENT_TYPE_SLAVE;
    if (c->flags & REDIS_PUBSUB)
        return REDIS_CLIENT_TYPE_PUBSUB;
    return REDIS_CLIENT_TYPE_NORMAL;
}

/* Client中的名字的3种类型，normal,slave，pubsub */
int getClientTypeByName(char *name) {
    if (!strcasecmp(name,"normal")) return REDIS_CLIENT_TYPE_NORMAL;
    else if (!strcasecmp(name,"slave")) return REDIS_CLIENT_TYPE_SLAVE;
    else if (!strcasecmp(name,"pubsub")) return REDIS_CLIENT_TYPE_PUBSUB;
    else return -1;
}

char *getClientTypeName(int class) {
    switch(class) {
    case REDIS_CLIENT_TYPE_NORMAL: return "normal";
    case REDIS_CLIENT_TYPE_SLAVE:  return "slave";
    case REDIS_CLIENT_TYPE_PUBSUB: return "pubsub";
    default:                       return NULL;
    }
}

/* The function checks if the client reached output buffer soft or hard
 * limit, and also update the state needed to check the soft limit as
 * a side effect.
 *
 * 这个函数检查客户端是否达到了输出缓冲区的软性（soft）限制或者硬性（hard）限制，
 * 并在到达软限制时，对客户端进行标记。
 *
 * Return value: non-zero if the client reached the soft or the hard limit.
 *               Otherwise zero is returned. 
 *
 * 返回值：到达软性限制或者硬性限制时，返回非 0 值，否则返回 0 。
 */
/* 判断Clint的输出缓冲区的已经占用大小是否超过软限制或是硬限制 */
int checkClientOutputBufferLimits(redisClient *c) {
    int soft = 0, hard = 0, class;

    //算出Client回复缓冲区已经占用的大小
    unsigned long used_mem = getClientOutputBufferMemoryUsage(c);

    class = getClientType(c);
    //比较Client的hard_limit_bytes的硬限制大小
    if (server.client_obuf_limits[class].hard_limit_bytes && 
            used_mem >= server.client_obuf_limits[class].hard_limit_bytes)
        hard = 1;
    //比较Client的soft_limit_bytes的软限制大小
    if (server.client_obuf_limits[class].soft_limit_bytes && 
            used_mem >= server.client_obuf_limits[class].soft_limit_bytes)
        soft = 1;

    /* We need to check if the soft limit is reached continuously for the
     * specified amount of seconds. */
    // 达到软性限制：还要检查连续软性限制的延续时间，超限才被认为真正达到软性限制。
    if (soft) {
        // 第一次达到软性限制
        if (c->obuf_soft_limit_reached_time == 0) {
            // 记录时间
            c->obuf_soft_limit_reached_time = server.unixtime;
            // 重新标记soft为0
            soft = 0;   /* First time we see the soft limit reached */
        
        // 再次达到软性限制
        } else {
            // 获得软性限制的连续时长
            time_t elapsed = server.unixtime - c->obuf_soft_limit_reached_time;
			
            // 如果没有超过最大连续时长soft_limit_seconds的话，那么关闭软性限制soft=0
            // 如果超过了最大连续时长的话，软性限制soft==1的状态就会被保留
            if (elapsed <= server.client_obuf_limits[class].soft_limit_seconds) {
                soft = 0; /* The client still did not reached the max number of
                             seconds for the soft limit to be considered reached. */
            }
        }
    } else {
        // 未达到软性限制，或者已脱离软性限制，那么清空软性限制的进入时间
        c->obuf_soft_limit_reached_time = 0;
    }
    return soft || hard;
}

/* Asynchronously close a client if soft or hard limit is reached on the
 * output buffer size. The caller can check if the client will be closed
 * checking if the client REDIS_CLOSE_ASAP flag is set.
 *
 * 如果客户端达到缓冲区大小的软性或者硬性限制，那么打开客户端的 ``REDIS_CLOSE_ASAP`` 状态，
 * 让服务器异步地关闭客户端。
 *
 * Note: we need to close the client asynchronously because this function is
 * called from contexts where the client can't be freed safely, i.e. from the
 * lower level functions pushing data inside the client output buffers. 
 *
 * 注意：
 * 我们不能直接关闭客户端，而要异步关闭的原因是客户端正处于一个不能被安全地关闭的上下文中。
 * 比如说，可能有底层函数正在推入数据到客户端的输出缓冲区里面。      
 */
/* 异步的关闭Client，如果缓冲区中的软限制或是硬限制已经到达的时候，缓冲区超出限制的结果会导致释放不安全*/
void asyncCloseClientOnOutputBufferLimitReached(redisClient *c) {
    redisAssert(c->reply_bytes < ULONG_MAX-(1024*64));

    // 已经被标记为REDIS_CLOSE_ASAP了。
    if (c->reply_bytes == 0 || c->flags & REDIS_CLOSE_ASAP) 
        return;

    // 检查限制输出缓冲区字节数是否超过软/硬性限制，超限返回1。
    if (checkClientOutputBufferLimits(c)) {
        //将client c的全部属性格式化存储到sdsempty创建的sds中，并返回该sds。
        sds client = catClientInfoString(sdsempty(),c);
        //调用freeClientAsync设置c的REDIS_CLOSE_ASAP标识，使守护进程后序能异步释放该client相关资源。
        freeClientAsync(c);
        redisLog(REDIS_WARNING,"Client %s scheduled to be closed ASAP for overcoming of output buffer limits.", client);
        sdsfree(client);
    }
}

/* Helper function used by freeMemoryIfNeeded() in order to flush slave
 * output buffers without returning control to the event loop. */
/* 从方法将会在freeMemoryIfNeeded()，释放内存空间函数，将存在内存中数据操作结果刷新到磁盘中 */
void flushSlavesOutputBuffers(void) {
    listIter li;
    listNode *ln;

    listRewind(server.slaves,&li);
    while((ln = listNext(&li))) {
        redisClient *slave = listNodeValue(ln);
        int events;

        events = aeGetFileEvents(server.el,slave->fd);
        if (events & AE_WRITABLE &&
            slave->replstate == REDIS_REPL_ONLINE &&
            listLength(slave->reply))
        {
        	//在这里调用了write的方法
            sendReplyToClient(server.el,slave->fd,slave,0);
        }
    }
}

/* This function is called by Redis in order to process a few events from
 * time to time while blocked into some not interruptible operation.
 * This allows to reply to clients with the -LOADING error while loading the
 * data set at startup or after a full resynchronization with the master
 * and so forth.
 *
 * It calls the event loop in order to process a few events. Specifically we
 * try to call the event loop for times as long as we receive acknowledge that
 * some event was processed, in order to go forward with the accept, read,
 * write, close sequence needed to serve a client.
 *
 * The function returns the total number of events processed. */
int processEventsWhileBlocked(void) {
    int iterations = 4; /* See the function top-comment. */
    int count = 0;
    while (iterations--) {
        int events = aeProcessEvents(server.el, AE_FILE_EVENTS|AE_DONT_WAIT);
        if (!events) break;
        count += events;
    }
    return count;
}
