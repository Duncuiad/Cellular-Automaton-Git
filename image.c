#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for srand(time(NULL))
#include <string.h> // for filename management
#include "lodepng.h"
#include <stdint.h> // for uintptr_t
#include "grid.h"

typedef struct Image {
    unsigned width;
    unsigned height;
    char filename[32];
    unsigned char *data;
} Image;

Image initImage(unsigned width, unsigned height, const char* filename) {
    Image img;
    img.width = width;
    img.height = height;
    strcpy(img.filename, filename);
    img.data = calloc( width*height*4, sizeof(unsigned char) );
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
    char path[ strlen(img.filename) + 8 ];
    sprintf(path, "images/%s", img.filename);
    encodeOneStep(path, img.data, img.width, img.height);
    return;
}

void writePixel(struct Image *img, unsigned x, unsigned y, unsigned char R, unsigned char G, unsigned char B, unsigned char A){
    if (x > img->width || y > img->height) {
        fprintf(stderr, "writePixel: cannot write in position %u, %u: image has sizes %u, %u.\n", x, y, img->width, img->height);
        return;
    }
    img->data[(x+y*img->width)*4 + 0] = R;
    img->data[(x+y*img->width)*4 + 1] = G;
    img->data[(x+y*img->width)*4 + 2] = B;
    img->data[(x+y*img->width)*4 + 3] = A;
}

void grid2PNG(Grid g, const char *filename) {
    Image img = initImage(g.width, g.height, filename);
    Cell c;
    unsigned char hue;
    for(unsigned y = 0; y < g.height; y++) {
        for (unsigned x = 0; x < g.width; x++) {
            c = getCell(g, x, y);
            hue = (unsigned char) c.data % 256;
            writePixel(&img, x, y, hue, hue, hue, 255);
        }
    }
    saveImage(img);
    destroyImage(img);
    return;
}

