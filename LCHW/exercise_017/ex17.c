#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include "simple_stack.h"

void test_stack();

void parser_cmd_line(char cmdv[][128], int *para_cnt){
    int max_data = 128;
    int max_argc = 10;

    int data_cnt = 0;
    *para_cnt = 0;
    char ch;
    while((ch = getchar())){
        if(strchr(" \n\t", ch)){
            if(data_cnt > 0){
                cmdv[*para_cnt][data_cnt] = '\0';
                /*printf("\nfinish read arg %d == %s\n", *para_cnt, cmdv[*para_cnt]);*/
                data_cnt = 0;
                *para_cnt += 1;
                assert(*para_cnt < max_argc);
            }        
            if(ch == '\n'){
                break;
            }
            continue;
        }
        /*printf("%c ", ch);*/
        cmdv[*para_cnt][data_cnt] = ch;
        data_cnt++;
        assert(data_cnt < max_data -1);
    }
}


int main(int argc, char* argv[])
{
    if(argc < 3) {
        die("USAGE: ex17 <dbfile> <MAX_DATA> <MAX_ROWS>", NULL); 
    }
    char *filename = argv[1];
    int max_data = atoi(argv[2]);
    int max_rows = atoi(argv[3]);
    Connection *conn = NULL;
    int id = 0;
    int loop = 1; 
    // cmd buff
    char cmdv[10][128];
    memset(cmdv, 0, 10 * 128 * sizeof(char));

    printf("Welcome to Simple Database !!! \n");
    while(loop){
        printf(">> ");
        int para_cnt = 0;
        char action = getchar();
        parser_cmd_line(cmdv, &para_cnt);
        if(conn == NULL){
            // if connection not initiated, then do it
            conn = Database_open(filename, action, max_data, max_rows); 
        }
        if(strchr("gsd", action)){
            id = atoi(cmdv[0]);
            if(id <  0 || id >= max_rows){
                printf("id: %d is not in [0, %d)\n", id, max_rows);
                continue;
            }
        }
        switch(action) {
            case 'c':
                Database_create(conn);
                Database_write(conn);
                break; 
            case 'g': 
                if(para_cnt != 1) die("Need an id to get", conn);
                Database_get(conn, id);
                break; 
            case 'f': 
                if(para_cnt != 2)  die("Need name, email to find", conn);
                Database_find(conn, cmdv[1], cmdv[2]);
                break; 
            case 's': 
                if(para_cnt != 3)  die("Need id, name, email to set", conn);
                Database_set(conn, id, cmdv[1], cmdv[2]);
                Database_write(conn);
                break; 
            case 'd': 
                if(para_cnt != 1) die("Need an id to delete", conn);
                Database_delete(conn, id);
                Database_write(conn);
                break; 
            case 'l': 
                Database_list(conn);
                break; 
            case 'x':
                loop = 0;
                break;
            case 't':
                test_stack();
                break;
            default:
                printf("Invalid action, only: c=create, g=get, f=find, s=set, d=del, l=list\n");
        }
    }
    Database_close(conn);
    return 0;
}

void test_stack(){
    init_stack(10);
    int a = 10;
    char *ch = "hello";
    char *dst = malloc(20 * sizeof(char));
    int b = 0;
    memset(dst, 0, 20 * sizeof(char));

    stack_print();
    stack_push(&a, sizeof(int));
    stack_print();
    stack_push(ch, sizeof(char) * strlen(ch));
    /*stack_push(ch, sizeof(char) * strlen(ch));*/
    stack_print();
    init_stack(20);
    stack_print();
    stack_pop(dst, sizeof(char) * strlen(ch));
    stack_print();
    stack_pop(&b, sizeof(int));
    /*stack_pop(&b, sizeof(int));*/
    stack_print();
    
    printf("get info from statck [%d] [%s]\n", b ,dst);
    free(dst);
    
    /*destroy_stack();*/
}
