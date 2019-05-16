#include <stdio.h>
#include <stdlib.h>

#define WIDTH 32
#define HEIGTH 32

int main(int argc, char** argv)
{
    int x, y, pixel;

    /* Emit header */
    printf("P1\n");
    printf("%i %i\n", WIDTH, HEIGTH);

    for(y=0; y<HEIGTH; y++){
        for(x=0; x<WIDTH; x++){
            if(y <= x)
                pixel = 1;
            else
                pixel = 0;
            
            printf("%i ", pixel);
        }
        printf("\n");
    }

    return 0;
}