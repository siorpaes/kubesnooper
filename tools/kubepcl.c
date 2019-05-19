/** \file kubepcl.c
 * \author david.siorpaes@gmail.com
 * 
 * Decodes Custom KubeII PCL printer files, including proprietary ESC*b2M compression.
 * TODO
 * -Make it more rubust against overflows in compressed data decoding
 * -Compute PAGE_HEIGTH
 * -Support vertical absolute positioning command ESC*p#Y
 * -Enable debugging flags
 * -Add more documentation
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

#define PAGE_WIDTH 640

/* Returns the number of digits of a number */
int ndigits(int digit)
{
    char buf[16];
    sprintf(buf, "%u", digit);
    return strlen(buf);
}

/* Emit PBM data representation */
int sbit(char data)
{
    int i;

    for(i=0; i<8; i++){
        printf("%i ", (data>>(7-i)) & 1);
    }

    return 0;
}

/* Decodes PCL raster inclusing Custom KubeII compressed format */
int decodeRaster(char* pcl, int rasterlen, int compressed)
{
    int i, j, decodedColumns;

    decodedColumns = 0;

    /* Jus emit uncomressed raster */
    if(compressed == 0){
        for(i=0; i<rasterlen; i++){
            sbit(pcl[i]);
            decodedColumns+=8;
        }
    }
    /* Emit compressed raster */
    else{
        for(i=0; i<rasterlen; i++){
            sbit(pcl[i]);
            decodedColumns += 8;

            if(pcl[i+1] == pcl[i]){
                for(j=0; j<pcl[i+2]-1; j++){
                    sbit(pcl[i]);
                    decodedColumns += 8;
                }
                i+=2;
            }
        }
    }

    /* Pad row with zeroes */
    for(i=0; i<PAGE_WIDTH-decodedColumns; i++)
        printf("0 ");

    printf("\n");

    return 0;
}

int main(int argc, char** argv)
{
    char* fileaddr;
    int filefd, cursor, rasterlen, match;
    int compressed;
    struct stat in_stat;
    char argument, command;

    if(argc != 2){
        fprintf(stderr, "Usage: %s <File name>\n", argv[0]);
        return -EINVAL;
    }

    /* Open and mmap file */
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

    /* Emit PBM header */
    printf("P1\n");
    printf("%i %i\n", 640, 640);


    /* Parse the PCL file */
    cursor = 0;
    compressed = 0;
    while(cursor <= in_stat.st_size){

        /* Match ESC */
        if(fileaddr[cursor] == 0x1b){
            cursor++;

            /* Match PCL command */
            match = sscanf(fileaddr+cursor, "*%c%i%c", &command, &rasterlen, &argument);

            /* ESC*b#W command is matched: decode raster */
            if((match == 3) && (command == 'b') && (argument == 'W')){
                cursor += (2 + ndigits(rasterlen) + 1);
                decodeRaster(&fileaddr[cursor], rasterlen, compressed);
                cursor += rasterlen;
            }
            /* ESC*b#M is matched: change compression format */
            else if((match == 3) && (command == 'b') && (argument == 'M')){
                if(rasterlen == 2){
                    compressed = 1;
                    fprintf(stderr, "Compression activated\n");
                }
                else if(rasterlen == 0){
                    compressed = 0;
                    fprintf(stderr, "No compression\n");
                }
                else{
                    compressed = 1;
                    fprintf(stderr, "Warning!!! Unkown comression format %i\n", rasterlen);
                }

                cursor += (2 + ndigits(rasterlen) + 1);

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
