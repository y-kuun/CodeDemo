#include <stdlib.h>
#include "include/utils.h"
#include "include/debug.h"
#include "zlib.h"

int base64(const void *in, const int size, void **out){
    const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 
    int osize = (size / 3 + (size % 3 ? 1: 0)) * 4;
    *out = calloc(1, sizeof(char) * (osize + 1));
    memset(*out, '=', sizeof(char) * osize);
    debug("base64 target size %d %s", osize, *out);
    check(out != NULL, "failed to allocate mem");
    int in_idx = 0;
    int ot_idx = 0;

    Byte *ou_p = *out;
    const Byte *in_p = in;
    for(in_idx = 0; in_idx < size; in_idx++){
        ot_idx = (in_idx / 3) * 4 + in_idx % 3;
        switch(in_idx % 3){
            case 0: 
                ou_p[ot_idx] = in_p[in_idx] >> 2; 
                ou_p[ot_idx] = b64[ou_p[ot_idx]];
                ou_p[ot_idx + 1] = (in_p[in_idx] & 0x3) << 4;
                break;
            case 1:;
                ou_p[ot_idx] += in_p[in_idx] >> 4; 
                ou_p[ot_idx] = b64[ou_p[ot_idx]];
                ou_p[ot_idx + 1] = (in_p[in_idx] & 0xf) << 2;
                break;
            case 2: 
                ou_p[ot_idx] += in_p[in_idx] >> 6; 
                ou_p[ot_idx + 1] = in_p[in_idx] & 0x3f;
                ou_p[ot_idx] = b64[ou_p[ot_idx]];
                ou_p[ot_idx + 1] = b64[ou_p[ot_idx + 1]];
                break;
        } 
    }
    if(size % 3){
        ot_idx = (in_idx / 3) * 4 + in_idx % 3;
        ou_p[ot_idx] = b64[ou_p[ot_idx]];
    }
    return osize;
error:
    return -1;
}

int unbase64(const void *in, const int size, void **out){
    const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 
    int i = 0;
    /*while((Byte)in[size - 1 - i] == '=') i++;*/
    int osize = (size / 4 ) * 3 - i;
    debug("unbase64 target size %d", osize);
    *out = calloc(1, sizeof(char) * (osize + 1));
    check(out != NULL, "failed to allocate mem");

    int in_idx = 0;
    int ot_idx = 0;
    Byte *ou_p = *out;
    const Byte *in_p = in;
    for(in_idx = 0; in_idx < size; in_idx++){
        ot_idx = (in_idx / 3) * 4 + in_idx % 3;
        switch(in_idx % 3){
            case 0: 
                break;
            case 1:;
                break;
            case 2: 
                break;
            case 3: 
                break;
        } 
    }
    return osize;
error:
    return -1;
}


int main(int argc, char *argv[]){
    if(argc < 2) return 1;
    char *data = "Hello World!";
    char *cdata = "Hello World!";
    char *ucdata = "Hello World!";
    base64(argv[1], strlen(argv[1]), (void**)&data);
    log_i("[%s] : [%s]", argv[1], data);
    
    uLongf dest_len = compressBound(strlen(argv[1]));
    Bytef *dest = calloc(1, sizeof(Bytef) * (dest_len + 1));
    compress(dest, &dest_len, argv[1], strlen(argv[1]));
    base64(dest, dest_len, (void**)&cdata);
    log_i("compress_test [%s] : [%s]", argv[1], cdata);

    Bytef *src = dest;
    uLongf src_len = dest_len;
    dest = calloc(1, sizeof(Bytef) * (strlen(argv[1]) + 1));
    uncompress(dest, &dest_len, src, src_len);
    /*base64(dest, dest_len, (void**)&ucdata);*/
    log_i("uncompress_test [%s] : [%s]", argv[1], dest);

    free(src);
    free(dest);
    free(data);
    free(cdata);
    return 0;
}
