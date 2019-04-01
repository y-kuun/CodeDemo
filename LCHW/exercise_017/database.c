#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"


Connection* Database_conn(){
    if(conn){
        return conn; 
    } else {
        return NULL;
    }
}

void Address_free(Address *addr){
    if(addr->name){
        free(addr->name);
        addr->name = NULL;
    }
    if(addr->email){
        free(addr->email);
        addr->email = NULL;
    }
}

void Address_init(Connection *conn, Address *addr){
    if(addr->set) return;
    addr->name = malloc(sizeof(char) * conn->db->MAX_DATA); 
    addr->email = malloc(sizeof(char) * conn->db->MAX_DATA); 
    memset(addr->name, 0, sizeof(char) * conn->db->MAX_DATA);
    memset(addr->email, 0, sizeof(char) * conn->db->MAX_DATA);
}

void Database_create(Connection *conn){
    // create struct in memory first
    conn->db->rows = malloc(sizeof(Address) * conn->db->MAX_ROWS); 
    memset(conn->db->rows, 0, sizeof(Address) * conn->db->MAX_ROWS);
    int id;
    for(id = 0; id < conn->db->MAX_ROWS; id++){
        (conn->db->rows + id)->id = id;
    }
}

void Database_load(Connection *conn){
    Database* db = conn->db;
    int rc = fread(&db->MAX_DATA, sizeof(unsigned int), 1, conn->file);
    if(rc != 1){
        die("Failed to load MAX_DATA database.", conn);
    }
    rc = fread(&db->MAX_ROWS, sizeof(unsigned int), 1, conn->file);
    if(rc != 1){
        die("Failed to load MAX_ROWS database.", conn);
    }
    db->rows = malloc(sizeof(Address) * db->MAX_ROWS);
    memset(db->rows, 0, sizeof(Address) * db->MAX_ROWS); 
    int row_num = 0;
    for(row_num = 0; row_num < db->MAX_ROWS; row_num++){
        rc = 0;
        Address* addr = db->rows + row_num;
        rc += fread(&addr->id, sizeof(int), 1, conn->file);
        rc += fread(&addr->set, sizeof(int), 1, conn->file);
        assert(rc == 2);
        if(addr->set){
            addr->name = malloc(sizeof(char) * db->MAX_DATA);
            addr->email = malloc(sizeof(char) * db->MAX_DATA);
            rc += fread(addr->name, sizeof(char) * db->MAX_DATA, 1, conn->file);
            rc += fread(addr->email, sizeof(char) * db->MAX_DATA, 1, conn->file);
            assert(rc == 4);
        }
    }
}

Connection* Database_open(const char *filename, char mode, int max_data, int max_rows){
    if(conn) {
        return conn; 
    } else {
        conn = malloc(sizeof(Connection));
        if(!conn) die("Memory error", conn);

        conn->db = malloc(sizeof(Database));
        if(!conn->db) die("Memory error", conn);

        if(mode == 'c'){
            conn->file = fopen(filename, "w"); 
            conn->db->MAX_DATA = max_data;
            conn->db->MAX_ROWS = max_rows;
        } else {
            conn->file = fopen(filename, "r+"); 
            if(conn->file){
                Database_load(conn); 
            }
        }
        if(!conn->file) die("Failed to open the file", conn);
    }
    printf("Database Opened! MAX_DATA: %d, MAX_ROWS: %d\n", conn->db->MAX_DATA, conn->db->MAX_ROWS);
    return conn;
}

void Database_close(Connection *conn){
    if(conn){
        if(conn->file) fclose(conn->file);  
        int row_num;
        for(row_num = 0; row_num < conn->db->MAX_ROWS; row_num++){
            Address_free(conn->db->rows + row_num);
        }
        if(conn->db->rows) free(conn->db->rows);
        if(conn->db) free(conn->db);
        free(conn);
    }    
}

// modify
void Database_write(Connection *conn){
    rewind(conn->file);
    int rc = fwrite(&conn->db->MAX_DATA, sizeof(unsigned int), 1, conn->file);
    if(rc != 1){
        die("Failed to load MAX_DATA database.", conn);
    }
    rc = fwrite(&conn->db->MAX_ROWS, sizeof(unsigned int), 1, conn->file);
    if(rc != 1){
        die("Failed to load MAX_ROWS database.", conn);
    }
    int max_rows = conn->db->MAX_ROWS;
    int max_data = conn->db->MAX_DATA;
    int row_num = 0;
    for(row_num = 0; row_num < max_rows; row_num++){
        Address addr = conn->db->rows[row_num];
        rc = 0;
        rc += fwrite(&(addr.id), sizeof(int), 1, conn->file);
        rc += fwrite(&(addr.set), sizeof(int), 1, conn->file);
        assert(rc == 2);
        if(addr.set){
            rc += fwrite(addr.name, sizeof(char) * max_data, 1, conn->file);
            rc += fwrite(addr.email, sizeof(char) * max_data, 1, conn->file);
            assert(rc == 4);
        }
    }
    rc = fflush(conn->file);
    if(rc != 0) die("Cannot flush datebase.", conn);
    printf("Database write with %d Addresses!\n", max_rows);
}

void Database_set(Connection *conn, int id, const char *name, const char *email){
    Address *addr = &conn->db->rows[id];
    if(addr->set){
        die("Already set, delete it first", conn);
    } else {
        Address_init(conn, addr);
    }
    
    addr->set = 1;
    char *res = strncpy(addr->name, name, conn->db->MAX_DATA - 1);
    if(!res) die("Name copy failed", conn);
    addr->name[conn->db->MAX_DATA - 1] = '\0';

    res = strncpy(addr->email, email, conn->db->MAX_DATA - 1);
    if(!res) die("Email copy failed", conn);
    addr->email[conn->db->MAX_DATA - 1] = '\0';
}

void Database_delete(Connection *conn, int id){
    Address_free(conn->db->rows + id);
    (conn->db->rows + id)->set = 0;
}

// visit
void Database_find(Connection *conn, const char* name, const char* email){
    Address* cur_p = conn->db->rows;
    while(cur_p - conn->db->rows < conn->db->MAX_ROWS){
        if(cur_p->set){
            char* name_p = strstr(cur_p->name, name); 
            char* email_p = strstr(cur_p->email, email); 
            if(name_p && email_p){
                Database_get(conn, cur_p->id); 
            }
        } 
        cur_p++;
    }
}

void Database_get(Connection *conn, int id){
    Address *addr = &conn->db->rows[id];
    if(addr->set){
        Address_print(addr);
    } else {
        die("ID is not set", conn); 
    }
}

void Database_list(Connection *conn){
    int i = 0;
    Database *db = conn->db;
    for(i = 0; i < conn->db->MAX_ROWS; i++){
        Address *cur = &db->rows[i];
        if(cur->set){
            Address_print(cur); 
        }
    }
}

void Address_print(Address *addr){
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

// error handle
void die(const char *message, Connection *conn){
    if(errno){
        perror(message); 
    } else {
        printf("ERROR: %s\n", message);
    }
    Database_close(conn);
    exit(1);
}

