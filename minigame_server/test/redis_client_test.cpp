//
// Created by xinnjie on 2019-08-25.
// from hiredis/example
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <hiredis.h>

int main(int argc, char **argv) {
    unsigned int j, isunix = 0;
    redisContext *c;
    redisReply *reply;
    const char *hostname = (argc > 1) ? argv[1] : "127.0.0.1";

    if (argc > 2) {
        if (*argv[2] == 'u' || *argv[2] == 'U') {
            isunix = 1;
            /* in this case, host is the path to the unix socket */
            printf("Will connect to unix socket @%s\n", hostname);
        }
    }

    int port = (argc > 2) ? atoi(argv[2]) : 6379;

    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    if (isunix) {
        c = redisConnectUnixWithTimeout(hostname, timeout);
    } else {
        c = redisConnectWithTimeout(hostname, port, timeout);
    }
    if (c == nullptr || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }

    /* PING minigame_server */
    reply = static_cast<redisReply *>(redisCommand(c,"PING"));
    printf("PING: %s\n", reply->str);
    freeReplyObject(reply);

    /* Set a key */
    reply = static_cast<redisReply *>(redisCommand(c,"SET %s %s", "foo", "hello world"));
    printf("SET: %s\n", reply->str);
    freeReplyObject(reply);

    /* Set a key using binary safe API */
    reply = static_cast<redisReply *>(redisCommand(c,"SET %b %b", "bar", (size_t) 3, "hello", (size_t) 5));
    printf("SET (binary API): %s\n", reply->str);
    freeReplyObject(reply);

    /* Try a GET and two INCR */
    reply = static_cast<redisReply *>(redisCommand(c,"GET foo"));
    printf("GET foo: %s\n", reply->str);
    freeReplyObject(reply);

    reply =static_cast<redisReply *>( redisCommand(c,"INCR counter"));
    printf("INCR counter: %lld\n", reply->integer);
    freeReplyObject(reply);
    /* again ... */
    reply = static_cast<redisReply *>(redisCommand(c,"INCR counter"));
    printf("INCR counter: %lld\n", reply->integer);
    freeReplyObject(reply);

    /* Create a list of numbers, from 0 to 9 */
    reply = static_cast<redisReply *>(redisCommand(c,"DEL mylist"));
    freeReplyObject(reply);
    for (j = 0; j < 10; j++) {
        char buf[64];

        snprintf(buf,64,"%u",j);
        reply = static_cast<redisReply *>(redisCommand(c,"LPUSH mylist element-%s", buf));
        freeReplyObject(reply);
    }

    /* Let's check what we have inside the list */
    reply = static_cast<redisReply *>(redisCommand(c,"LRANGE mylist 0 -1"));
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (j = 0; j < reply->elements; j++) {
            printf("%u) %s\n", j, reply->element[j]->str);
        }
    }
    freeReplyObject(reply);

    /* Disconnects and frees the context */
    redisFree(c);

    return 0;
}
