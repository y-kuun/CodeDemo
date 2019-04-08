#include "db.h"
#include "dbg.h"


int main(int argc, char *argv[]){
    if(argc < 2){
        log_warn("one arg required!");
    }
    DB_init();
    DB_list();
    char *url = argv[1]; 
    if(DB_find(url)){
        log_info("ret 0"); 
    } else {
        log_info("ret !0"); 
    }
    DB_update(url);
    if(DB_find(url)){
        log_info("ret 0"); 
    } else {
        log_info("ret !0"); 
    }
    DB_list();
    return 0;
}
