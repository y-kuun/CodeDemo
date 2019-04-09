#include <stdlib.h>
#include "include/utils.h"
#include "include/debug.h"
#include "zlib.h"
#include "lz4.h"

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
    return -1;
}

void lz4_test(int argc, char *argv[]){
    log_i("lz4_test =======================");
    char *src = argv[1];
    char *cdata;
    int src_len = strlen(src);
    int dst_len = LZ4_compressBound(src_len);
    char *dst = calloc(1, sizeof(char) * (dst_len + 1));
    
    dst_len = LZ4_compress_default(src, dst, src_len, dst_len);
    base64(dst, dst_len, (void**)&cdata);
    log_i("lz4 compress_test [%s] : [%s]", src, cdata);

    src = dst;
    dst = calloc(1, sizeof(char) * (src_len + 1));
    dst_len = src_len;
    src_len = strlen(src);
    dst_len = LZ4_decompress_safe(src, dst, src_len, dst_len);
    log_i("lz4 decompress_test [%s]", dst);

    log_i("lz4_test =======================");
    free(cdata);
    free(src);
    free(dst);
}

int split_file(char **lines, int *size, const char* filepath){
    FILE* fp = fopen(filepath, "r");
    size_t lnum;
    check(fp != NULL, "failed to open file %s", filepath);
    int rc = getline(lines, &lnum, fp);
    check(rc != -1, "failed to getline");
    *size = lnum;
    if(fp) free(fp);
    return 0;
error:
    if(fp) free(fp);
    if(*lines) free(*lines);
    return -1;
}

int base64_test(int argc, char *argv[]){
    if(argc < 2) return 1;
    char *data = "Hello World!";
    char *cdata = "Hello World!";
    base64(argv[1], strlen(argv[1]), (void**)&data);
    log_i("[%s] : [%s]", argv[1], data);
    
    uLongf dest_len = compressBound(strlen(argv[1]));
    Bytef *dest = calloc(1, sizeof(Bytef) * (dest_len + 1));
    compress(dest, &dest_len, (const Bytef*)argv[1], strlen(argv[1]));
    base64(dest, dest_len, (void**)&cdata);
    log_i("libz compress_test [%s] : [%s]", argv[1], cdata);

    Bytef *src = dest;
    uLongf src_len = dest_len;
    dest = calloc(1, sizeof(Bytef) * (strlen(argv[1]) + 1));
    uncompress(dest, &dest_len, src, src_len);
    log_i("libz uncompress_test [%s] : [%s]", argv[1], dest);

    lz4_test(argc, argv);
    free(src);
    free(dest);
    free(data);
    free(cdata);
    return 0;
}

int main(int argc, char *argv[]){
    if(argc < 2) return 1;
    FILE* fp = fopen(argv[1], "r");
    char *line = NULL;
    size_t size;
    int rc;
    while((rc = getline(&line, &size, fp)) != EOF){
        printf("%d:%lu:%s", rc, size, line); 
    }
    free(line);
    return 0;
}
