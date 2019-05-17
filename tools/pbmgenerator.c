/** \file pbmgenerator.c
 * \author david.siorpaes@gmail.com
 * 
 * Generates PBM image files with different patterns and sizes
 * Used to produce known patterns for PCL raster decoding
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Paint function: tells whether to draw pixel or not according to the desired pattern */
int paint(int x, int y, char* p)
{
    int pixel = 1;

    /* Top filled triangle */
    if(strcmp(p, "t1") == 0){
        if(y <= x)
            pixel = 1;
        else
            pixel = 0;
    }

    /* Bottom filled triangle */
    if(strcmp(p, "t2") == 0){
        if(y >= x)
            pixel = 1;
        else
            pixel = 0;
    }

    /* Line */
    if(strcmp(p, "l") == 0){
        if(y == x )
            pixel = 1;
        else
            pixel = 0;
    }

    /* Upper left 8x8 cube */
    if(strcmp(p, "c") == 0){
        if((x < 8) && (y < 8))
            pixel = 1;
        else
            pixel = 0;
    }

    /* Alternate vertical lines */
    if(strcmp(p, "vl") == 0){
        if( x & 1)
            pixel = 1;
        else
            pixel = 0;
    }

    /* Alternate horizontal lines */
    if(strcmp(p, "hl") == 0){
        if( y & 1)
            pixel = 1;
        else
            pixel = 0;
    }


    return pixel;
}


int main(int argc, char** argv)
{
    int x, y, pixel;
    int width, heigth;

    if(argc != 4){
        fprintf(stderr, "Usage: %s <pattern: t1, t2, l> <width> <heigth>\n", argv[0]);
        exit(-1);
    }

    width = atoi(argv[2]);
    heigth = atoi(argv[3]);

    /* Emit header */
    printf("P1\n");
    printf("%i %i\n", width, heigth);

    for(y=0; y<heigth; y++){
        for(x=0; x<width; x++){
            pixel = paint(x, y, argv[1]);
            printf("%i ", pixel);
        }

        printf("\n");
    }

    return 0;
}
