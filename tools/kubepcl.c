/** \file kubepcl.c
 * \author david.siorpaes@gmail.com
 * 
 * Decodes Custom KubeII PCL printer files
 * 
 * PCL file can be captured from real printer using tcpdump/wireshark
 * or can be dumped with netcat redirecting the printer to localhost
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

/* Returns the number of digits of a number */
int ndigits(int digit)
{
    char buf[16];
    sprintf(buf, "%u", digit);
    return strlen(buf);
}

int main(int argc, char** argv)
{
    char* fileaddr;
    int filefd, cursor, rasterlen, match, i;
    struct stat in_stat;

    if(argc != 2){
        fprintf(stderr, "Usage: %s <File name>\n", argv[0]);
        return -EINVAL;
    }

    if((filefd = open(argv[1], O_RDONLY)) < 0) {
        fprintf(stderr, "Failed to open %s: %s\n", argv[1], strerror(errno));
        return errno;
    }

    fstat(filefd, &in_stat);
    fileaddr = mmap(NULL, in_stat.st_size, PROT_READ, MAP_SHARED, filefd, 0);
    if(fileaddr == MAP_FAILED){
        fprintf(stderr, "Failed to mmap %s: %s\n", argv[2], strerror(errno));
        return errno;
    }

    /* Parse the PCL file */
    cursor = 0;
    while(cursor <= in_stat.st_size){
        if(fileaddr[cursor] == 0x1b){
            cursor++;
            match = sscanf(fileaddr+cursor, "*b%iW", &rasterlen);
            if((match == 1) && (fileaddr[cursor+2+ndigits(rasterlen)]) == 'W'){
                printf("%x: ", cursor);
                cursor += (2 + ndigits(rasterlen) + 1);
                for(i=0; i<rasterlen; i++){
                    printf("%02x ", fileaddr[cursor+i] & 0xff);
                }
                cursor += rasterlen;
                printf("\n");
            }
            else{
                cursor++;
            }
        }
        else{
            cursor++;
        }
    }

    return 0;
}