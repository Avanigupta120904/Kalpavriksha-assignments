#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 2003      
#define MIN_CAPACITY 1
#define MAX_CAPACITY 1000
#define LINE_BUF_SIZE 1024

typedef struct lruNode {
    int key;
    char *value;
    struct lruNode *prev;
    struct lruNode *next;
} lruNode;

typedef struct hashNode {
    int key;
    lruNode *entry;
    struct hashNode *next;
} hashNode;

typedef struct lruCache {
    int capacity;
    int count;
    lruNode *head;                
    lruNode *tail;                
    hashNode *mapTable[TABLE_SIZE];
} lruCache;

static char *cloneString(const char *src) {
    if (!src) return NULL;
    size_t n = strlen(src) + 1;
    char *dst = malloc(n);
    if (!dst) return NULL;
    memcpy(dst, src, n);
    return dst;
}

static int isNumber(const char *txt) {
    if (!txt || !txt[0]) return 0;
    int i = 0;
    if (txt[0] == '-') {
        if (!txt[1]) return 0;
        i = 1;
    }
    for (; txt[i]; i++) {
        if (txt[i] < '0' || txt[i] > '9') return 0;
    }
    return 1;
}

static void stripLine(char *buf) {
    int len = (int)strlen(buf);
    while (len > 0) {
        char ch = buf[len - 1];
        if (ch == '\n' || ch == '\r' || ch == ' ' || ch == '\t') {
            buf[len - 1] = '\0';
            len--;
        } else break;
    }
}

static int hashCompute(int key) {
    unsigned int uk = (unsigned int) key;
    return (int)(uk % TABLE_SIZE);
}

static lruNode *mapFind(lruCache *cache, int key) {
    int idx = hashCompute(key);
    hashNode *cur = cache->mapTable[idx];
    while (cur) {
        if (cur->key == key) return cur->entry;
        cur = cur->next;
    }
    return NULL;
}

static bool mapInsert(lruCache *cache, int key, lruNode *entry) {
    int idx = hashCompute(key);
    hashNode *node = malloc(sizeof(hashNode));
    if (!node) return false;
    node->key = key;
    node->entry = entry;
    node->next = cache->mapTable[idx];
    cache->mapTable[idx] = node;
    return true;
}

static bool mapDelete(lruCache *cache, int key) {
    int idx = hashCompute(key);
    hashNode *cur = cache->mapTable[idx];
    hashNode *prev = NULL;
    while (cur) {
        if (cur->key == key) {
            if (!prev) cache->mapTable[idx] = cur->next;
            else prev->next = cur->next;
            free(cur);
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

/* Free all hash nodes (used on cache destroy) */
static void mapClear(lruCache *cache) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashNode *cur = cache->mapTable[i];
        while (cur) {
            hashNode *nx = cur->next;
            free(cur);
            cur = nx;
        }
        cache->mapTable[i] = NULL;
    }
}

static void addToHead(lruCache *cache, lruNode *node) {
    node->prev = NULL;
    node->next = cache->head;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
    if (!cache->tail) cache->tail = node;
}

static void removeNode(lruCache *cache, lruNode *node) {
    if (node->prev) node->prev->next = node->next;
    else cache->head = node->next;

    if (node->next) node->next->prev = node->prev;
    else cache->tail = node->prev;

    node->prev = node->next = NULL;
}

static void moveToHead(lruCache *cache, lruNode *node) {
    if (cache->head == node) return;
    removeNode(cache, node);
    addToHead(cache, node);
}

static lruNode *removeTail(lruCache *cache) {
    if (!cache->tail) return NULL;
    lruNode *victim = cache->tail;
    removeNode(cache, victim);
    return victim;
}

static lruCache *createCache(int capacity) {
    if (capacity < MIN_CAPACITY || capacity > MAX_CAPACITY) return NULL;
    lruCache *cache = malloc(sizeof(lruCache));
    if (!cache) return NULL;
    cache->capacity = capacity;
    cache->count = 0;
    cache->head = cache->tail = NULL;
    for (int i = 0; i < TABLE_SIZE; i++) cache->mapTable[i] = NULL;
    return cache;
}

static void destroyCache(lruCache *cache) {
    if (!cache) return;
    lruNode *cur = cache->head;
    while (cur) {
        lruNode *nx = cur->next;
        free(cur->value);
        free(cur);
        cur = nx;
    }
    mapClear(cache);
    free(cache);
}

static char *lruGet(lruCache *cache, int key) {
    if (!cache) return NULL;
    lruNode *node = mapFind(cache, key);
    if (!node) return NULL;
    moveToHead(cache, node);
    return node->value;
}

static void lruPut(lruCache *cache, int key, const char *value) {
    if (!cache) return;
    if (!value || !value[0]) return;  // require non-empty string

    lruNode *existing = mapFind(cache, key);
    if (existing) {

        char *newVal = cloneString(value);
        if (!newVal) return; /* OOM: keep old value intact */
        free(existing->value);
        existing->value = newVal;
        moveToHead(cache, existing);
        return;
    }

    /* Create new node */
    lruNode *node = malloc(sizeof(lruNode));
    if (!node) return;
    node->key = key;
    node->value = cloneString(value);
    node->prev = node->next = NULL;
    if (!node->value) {
        free(node);
        return;
    }

    if (!mapInsert(cache, key, node)) {
        /* map insert failed (OOM) -> clean up */
        free(node->value);
        free(node);
        return;
    }

    addToHead(cache, node);
    cache->count++;

    if (cache->count > cache->capacity) {
        lruNode *evicted = removeTail(cache);
        if (evicted) {
            mapDelete(cache, evicted->key);
            free(evicted->value);
            free(evicted);
            cache->count--;
        }
    }
}

void showMenu() {
    printf("=========================================\n");
    printf("            LRU CACHE PROGRAM            \n");
    printf("=========================================\n");
    printf("Available Commands:\n");
    printf("  createCache <size>     : Create a new LRU cache\n");
    printf("  put <key> <value>      : Insert or update a key-value pair\n");
    printf("  get <key>              : Retrieve value for a key\n");
    printf("  exit                   : Print results & terminate program\n");
    printf("-----------------------------------------\n");
    printf("NOTE:\n");
    printf("  Keys must be integers (can be negative)\n");
    printf("  Values must be non-empty strings\n");
    printf("  Cache prints all GET results ONLY when you type 'exit'\n");
    printf("-----------------------------------------\n\n");
}

void storeResponse(char ***respArr, int *respCount, int *respCap, const char *txt) {
    if (*respCount + 1 > *respCap) {
        int newCap = (*respCap == 0 ? 8 : *respCap * 2);
        char **temp = malloc(newCap * sizeof(char *));
        if (!temp) return;

        for (int i = 0; i < *respCount; i++)
            temp[i] = (*respArr)[i];

        free(*respArr);
        *respArr = temp;
        *respCap = newCap;
    }

    (*respArr)[*respCount] = cloneString(txt);
    (*respCount)++;
}

bool processCommand(char *line, lruCache **cache,
                    char ***responses, int *respCount, int *respCap) {

    char *cmd = strtok(line, " \t");
    if (!cmd) return true;

    if (strcmp(cmd, "createCache") == 0) {
        char *sz = strtok(NULL, " \t");
        if (!sz || !isNumber(sz)) {
            printf("Invalid capacity\n");
            return true;
        }
        int cap = atoi(sz);
        if (cap < MIN_CAPACITY || cap > MAX_CAPACITY) {
            printf("capacity must be between 1 and 1000\n");
            return true;
        }
        if (*cache) destroyCache(*cache);
        *cache = createCache(cap);
        return true;
    }

    if (strcmp(cmd, "put") == 0) {
        char *k = strtok(NULL, " \t");
        char *val = strtok(NULL, "\n");
        if (!k || !val || !isNumber(k)) {
            printf("put requires: put <key> <value>\n");
            return true;
        }
        while (*val == ' ') val++;
        if (!*cache) {
            printf("cache not initialized\n");
            return true;
        }
        lruPut(*cache, atoi(k), val);
        return true;
    }

    if (strcmp(cmd, "get") == 0) {
        char *k = strtok(NULL, " \t");
        if (!k || !isNumber(k)) {
            printf("key must be an integer\n");
            return true;
        }
        char *res = *cache ? lruGet(*cache, atoi(k)) : NULL;
        storeResponse(responses, respCount, respCap, res ? res : "NULL");
        return true;
    }

    if (strcmp(cmd, "exit") == 0) {
        return false;
    }

    printf("unknown command\n");
    return true;
}

void runProgramLoop() {
    lruCache *cache = NULL;
    char line[LINE_BUF_SIZE];

    char **responses = NULL;
    int respCount = 0;
    int respCap = 0;

    while (1) {
        printf("Enter Command:\n");

        if (!fgets(line, sizeof(line), stdin)) break;

        stripLine(line);

        if (!processCommand(line, &cache, &responses, &respCount, &respCap))
            break;
    }

    for (int i = 0; i < respCount; i++)
        printf("%s\n", responses[i]);

    for (int i = 0; i < respCount; i++)
        free(responses[i]);

    free(responses);
    destroyCache(cache);
}


int main() {
    showMenu();
    runProgramLoop();
    return 0;
}