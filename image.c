#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include "lodepng.h"

#include "grid.h"
#include "image.h"
#include "debug.h"


/*
typedef struct Image {
    unsigned width;
    unsigned height;
    char filename[32];
    unsigned char *data;
} Image;

typedef struct Color {
    unsigned char R, G, B, A;
} Color;
*/

Image initImage(unsigned width, unsigned height, const char* filename) {
    Image img;
    img.width = width;
    img.height = height;
    strcpy(img.filename, filename);
    img.data = (unsigned char *) calloc( width*height*4, sizeof(unsigned char) );
    return img;
}

void destroyImage(Image img) {
    free(img.data);
}

void encodeOneStep(const char* filename, const unsigned char* image, unsigned width, unsigned height) {
  /*Encode the image*/
  unsigned error = lodepng_encode32_file(filename, image, width, height);

  /*if there's an error, display it*/
  if(error) fprintf(stderr, "error %u: %s\n", error, lodepng_error_text(error));
}

void saveImage(struct Image img) {
    char* path;
    path = (char *) calloc( (strlen(img.filename) + 8), sizeof(char) );
    sprintf(path, "images/%s", img.filename);
    encodeOneStep(path, img.data, img.width, img.height);
    return;
}

void writePixel(Image *img, unsigned x, unsigned y, Color c){
    if (x > img->width || y > img->height) {
        fprintf(stderr, "writePixel: cannot write in position %u, %u: image has sizes %u, %u.\n", x, y, img->width, img->height);
        return;
    }
    img->data[(x+y*img->width)*4 + 0] = c.R;
    img->data[(x+y*img->width)*4 + 1] = c.G;
    img->data[(x+y*img->width)*4 + 2] = c.B;
    img->data[(x+y*img->width)*4 + 3] = c.A;
}

Color packColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
    Color c;
    c.R = R;
    c.G = G;
    c.B = B;
    c.A = A;
    return c;
}

void grid2PNG(Grid *g, const char *filename) {
    Image img = initImage(g->width, g->height, filename);
    double min;
    double max;
    Cell c;
    double buf;
    Color color;
    int x;
    int y;

    TRACE(("grid2PNG: Saving grid in image: %s\n", filename));

    c = getCell(g, 0, 0);
    min = c.data;
    max = c.data;

    /* Determine range */
    for( y = 0; y < g->height; y++) {
        for ( x = 0; x < g->width; x++) {
            c = getCell(g, x, y);
            buf = c.data;
            min = buf < min ? buf : min;
            max = buf > max ? buf : max;
        }
    }

    TRACE(("grid2PNG: min = %f, max = %f, median = %f\n", min, max, (min+max)/2 ));

    for( y = 0; y < g->height; y++) {
        for ( x = 0; x < g->width; x++) {
            c = getCell(g, x, y);
            color = hueGradient(min, max, c.data, 'r');
            writePixel(&img, x, y, color);
        }
    }
    saveImage(img);
    destroyImage(img);
    return;
}

Color hueGradient(double min, double max, double target, char type) {
    Color c;
    unsigned char col; /* Intensity of selected channel */
    unsigned char others; /* Intensity of other channels */
    int tmp;

    c.R = 0;
    c.G = 0;
    c.B = 0;
    c.A = 0;

    if (target < min || target > max) {
        fprintf(stderr, "hueGradient: target out of range\n");
        return c;
    }
    /* Essentially linearly interpolate */
    max = max-min;
    target = target - min;

    if (max == 0) {
        others = 0;
        col = 255;
    } else if ( target <= max/2 ) {
        others = 0;
        col = (target) * 510 / max;
        /* target is dark */
    } else {
        col = 255;
        tmp = (512*target/max - 256);
        if (tmp > 255) others = 255;
        else others = tmp;
    };

    switch ( type ) {
        case 'r' :
            c.R = col;
            c.G = others;
            c.B = others;
            c.A = 255;
            break;
        case 'g' :
            c.R = others;
            c.G = col;
            c.B = others;
            c.A = 255;
            break;
        case 'b' :
            c.R = others;
            c.G = others;
            c.B = col;
            c.A = 255;
            break;
        default :
            fprintf(stderr, "hueGradient: invalid format\n");
    }
    return c;

}
