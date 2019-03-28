#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    int set;
    char *name;
    char *email;
} Address;

typedef struct {
    unsigned int MAX_DATA;
    unsigned int MAX_ROWS;
    Address *rows;
} Database;

typedef struct {
    FILE *file;
    Database *db;
} Connection;

// Address Utils
void Address_free(Address*);

// status
void Database_create(Connection *conn);
void Database_load(Connection *conn);
Connection* Database_open(const char *filename, char mode, int max_data, int max_rows);
void Database_close(Connection *conn);

// modify
void Database_write(Connection *conn);
void Database_set(Connection *conn, int id, const char *name, const char *email);
void Database_delete(Connection *conn, int id);

// visit
void Database_find(Connection *conn, const char* name, const char* email);
void Database_get(Connection *conn, int id);
void Database_list(Connection *conn);
void Address_print(Address *addr);

// error handle
void die(const char *message, Connection *conn);
