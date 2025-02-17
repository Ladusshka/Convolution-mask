#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h> // Используем чисто C

// Количество интервалов для гистограммы

typedef struct {
    uint8_t r, g, b;
} Pixel;

void read_ppm(const char *filename, int *width, int *height, Pixel **data) {
    FILE *file = fopen(filename, "rb");
    if (!file) { perror("File open error"); exit(1); }

    char header[3];
    fscanf(file, "%s\n%d %d\n255\n", header, width, height);

    *data = (Pixel*)malloc((*width) * (*height) * sizeof(Pixel));
    fread(*data, sizeof(Pixel), (*width) * (*height), file);
    fclose(file);
}

void convolution(Pixel *input, Pixel *output, int height, int width) {
    int mask[] = {0,-1,0,
                  -1,5,-1,
                  0,-1,0};

    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            int current_index = width * i + j;

            int newvalueR = 0;
            int newvalueG = 0;
            int newvalueB = 0;

            for (int rowindex = i - 1; rowindex <= i + 1; rowindex++) {
                for (int colomnindex = j - 1; colomnindex <= j + 1; colomnindex++) {
                    int mask_x = colomnindex - (j - 1);
                    int mask_y = rowindex - (i - 1);
                    int mask_index = mask_y * 3 + mask_x;

                    int input_index = width * rowindex + colomnindex;

                    newvalueR += input[input_index].r * mask[mask_index];
                    newvalueG += input[input_index].g * mask[mask_index];
                    newvalueB += input[input_index].b * mask[mask_index];
                }
            }


            if (newvalueR < 0) newvalueR = 0; else if (newvalueR > 255) newvalueR = 255;
            if (newvalueG < 0) newvalueG = 0; else if (newvalueG > 255) newvalueG = 255;
            if (newvalueB < 0) newvalueB = 0; else if (newvalueB > 255) newvalueB = 255;

            output[current_index].r = (uint8_t)newvalueR;
            output[current_index].g = (uint8_t)newvalueG;
            output[current_index].b = (uint8_t)newvalueB;
        }
    }


    for (int i = 0; i < height; i++) {
        int index_left = width * i;
        int index_right = width * i + (width - 1);
        output[index_left] = input[index_left];
        output[index_right] = input[index_right];
    }

    for (int j = 0; j < width; j++) {

        output[j] = input[j];

        int index_bottom = width * (height - 1) + j;
        output[index_bottom] = input[index_bottom];
    }
}

void write_ppm(const char *filename, int width, int height, Pixel *data) {
    FILE *file = fopen(filename, "wb");
    fprintf(file, "P6\n%d\n%d\n255\n", width, height);
    //fprintf(file, "P6\n");
    fwrite(data, sizeof(Pixel), width * height, file);
    fclose(file);
}

void count_histogram(Pixel * output, int height, int width, int * num){

    for (int i = 0; i < 5; i++) {
        num[i] = 0;
    }

    for(int i=0;i<height; i++){
        for(int j=0;j<width;j++){
            //0.2126*R + 0.7152*G + 0.0722*B
            int current_index = width * i + j;
            int Y = std::round(0.2126*output[current_index].r + 0.7152*output[current_index].g + 0.0722*output[current_index].b);
            if (Y >= 0 && Y <= 50) {
                num[0]++;
            } else if (Y >= 51 && Y <= 101) {
                num[1]++;
            } else if (Y >= 102 && Y <= 152) {
                num[2]++;
            } else if (Y >= 153 && Y <= 203) {
                num[3]++;
            } else if (Y >= 204 && Y <= 255) {
                num[4]++;
            }
        }
    }
}

void write_histogram_to_file(const char *filename, int *num) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("File open error");
        return;
    }
    fprintf(f, "%d %d %d %d %d", num[0],num[1],num[2],num[3],num[4]);


    fclose(f);
}


int main(int argc, char *argv[]) {
    if (argc < 2) { printf("Usage: %s input.ppm\n", argv[0]); return 1; }

    int width, height;
    Pixel *input, *output;

    read_ppm(argv[1], &width, &height, &input);
    output = (Pixel*)malloc(width * height * sizeof(Pixel));

    convolution(input, output, height,width);
    write_ppm("output.ppm", width, height, output);

    int histogram[5];
    count_histogram(output, height,width, histogram);
    write_histogram_to_file("output.txt",histogram);


    free(input);
    free(output);
    return 0;
}
