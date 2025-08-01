/*-------------images.c-------------------------------------------------------//

Purpose: This file is meant to define most of what is needed for 
         image and video creation

//----------------------------------------------------------------------------*/

#include "../include/quibble.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM 
#include "../extern/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../extern/stb_image_write.h"

/*----------------------------------------------------------------------------//
    IMAGE IO
//----------------------------------------------------------------------------*/

quibble_color_rgba8888 qb_zero_color_rgba8888(void){
    quibble_color_rgba8888 qp;
    qp.red = 0;
    qp.green = 0;
    qp.blue = 0;
    qp.alpha = 0;
    return qp;
}

quibble_color_rgb888 qb_zero_color_rgb888(void){
    quibble_color_rgb888 qp;
    qp.red = 0;
    qp.green = 0;
    qp.blue = 0;
    return qp;
}

quibble_pcolor_rgba8888 qb_zero_pcolor_rgba8888(void){
    quibble_pcolor_rgba8888 qp;
    qp.red = 0;
    qp.green = 0;
    qp.blue = 0;
    qp.alpha = 0;
    qp.priority = 0;
    return qp;
}

quibble_pcolor_rgb888 qb_zero_pcolor_rgb888(void){
    quibble_pcolor_rgb888 qp;
    qp.red = 0;
    qp.green = 0;
    qp.blue = 0;
    qp.priority = 0;
    return qp;
}

unsigned char qb_color_clamp(float value,
                             float min_value,
                             float max_value){
    if (value < min_value){
        return 0;
    }
    else if (value > max_value){
        return 255;
    }
    else{
        return floor(255*((value - min_value)/(max_value - min_value)));
    }
}

quibble_color_rgba8888 qb_color_rgba8888(float red,
                                         float green,
                                         float blue,
                                         float alpha){
    quibble_color_rgba8888 qp;
    qp.red = qb_color_clamp(red, 0, 1);
    qp.green = qb_color_clamp(green, 0, 1);
    qp.blue = qb_color_clamp(blue, 0, 1);
    qp.alpha = qb_color_clamp(alpha, 0, 1);
    return qp;
}

quibble_color_rgb888 qb_color_rgb888(float red, float green, float blue){
    quibble_color_rgb888 qp;
    qp.red = qb_color_clamp(red, 0, 1);
    qp.green = qb_color_clamp(green, 0, 1);
    qp.blue = qb_color_clamp(blue, 0, 1);
    return qp;
}

quibble_pcolor_rgba8888 qb_pcolor_rgba8888(float red,
                                           float green,
                                           float blue,
                                           float alpha,
                                           float priority){
    quibble_pcolor_rgba8888 qp;
    qp.red = qb_color_clamp(red, 0, 1);
    qp.green = qb_color_clamp(green, 0, 1);
    qp.blue = qb_color_clamp(blue, 0, 1);
    qp.alpha = qb_color_clamp(alpha, 0, 1);
    qp.priority = qb_color_clamp(priority, 0, 1);
    return qp;
}

quibble_pcolor_rgb888 qb_pcolor_rgb888(float red,
                                       float green,
                                       float blue,
                                       float priority){
    quibble_pcolor_rgb888 qp;
    qp.red = qb_color_clamp(red, 0, 1);
    qp.green = qb_color_clamp(green, 0, 1);
    qp.blue = qb_color_clamp(blue, 0, 1);
    qp.priority = qb_color_clamp(priority, 0, 1);
    return qp;
}


int qb_get_color_size(int color_type){
    if (color_type == RGBA8888){
        return 4;
    }
    else if (color_type == RGB888){
        return 3;
    }
    if (color_type == PRGBA8888){
        return 5;
    }
    else if (color_type == PRGB888){
        return 4;
    }
    else {
        fprintf(stderr, "Color type %d not found!", color_type);
        exit(1);
    }

}

quibble_pixels qb_create_blank_pixel_array(quibble_program qp,
                                           int width,
                                           int height,
                                           int color_type){
    if (!qp.configured){
        fprintf(stderr, "Cannot create quibble pixels because program has not been configured!\nPlease configure it first with:\n    qp_configure_program(&qp, int platform, int device).\nThe platform and device can be found with the associated `qbinfo` command.\n");
        exit(1);
    }
    quibble_pixels qps;
    qps.height = height;
    qps.width = width;

    qps.color_type = color_type;

    int color_size = qb_get_color_size(qps.color_type);
    qps.command_queue = qp.command_queue;
    int array_size = color_size*qps.width*qps.height*sizeof(unsigned char);

    cl_int err;
    qps.device_data =
        clCreateBuffer(qp.context,
                       CL_MEM_READ_WRITE,
                       array_size,
                       NULL,
                       &err);

    cl_check(err);

    qps.host_data = NULL;

    return qps;
}

quibble_pixels qb_create_pixel_array(quibble_program qp,
                                     int width,
                                     int height,
                                     int color_type){
    quibble_pixels qps =
        qb_create_blank_pixel_array(qp, width, height, color_type);
    int color_size = qb_get_color_size(qps.color_type);
    qps.host_data =
        (void *)calloc(color_size*qps.width*qps.height,
                       sizeof(unsigned char));
    qb_pixels_host_to_device(qps);
    return qps;

}
quibble_pixels qb_create_pixel_array_from_file(char *filename,
                                               quibble_program qp,
                                               int width,
                                               int height,
                                               int color_type){
    quibble_pixels qps =
        qb_create_blank_pixel_array(qp, width, height, color_type);
    qps.host_data = qb_read_file(filename, width, height, color_type);
    qb_pixels_host_to_device(qps);
    return qps;
}

void qb_pixels_host_to_device(quibble_pixels qps){
    int color_size = qb_get_color_size(qps.color_type);
    int array_size = color_size*qps.width*qps.height*sizeof(unsigned char);

    cl_check(clEnqueueWriteBuffer(qps.command_queue,
                                  qps.device_data,
                                  CL_TRUE,
                                  0,
                                  array_size,
                                  qps.host_data,
                                  0,
                                  NULL,
                                  NULL));

}

void qb_pixels_device_to_host(quibble_pixels qps){
    int color_size = qb_get_color_size(qps.color_type);
    int array_size = color_size*qps.width*qps.height*sizeof(unsigned char);

    cl_check(clEnqueueReadBuffer(qps.command_queue,
                                 qps.device_data,
                                 CL_TRUE,
                                 0,
                                 array_size,
                                 qps.host_data,
                                 0,
                                 NULL,
                                 NULL));

}

void qb_free_pixels(quibble_pixels qps){
    free(qps.host_data);
    cl_check(clReleaseMemObject(qps.device_data));
}

/*----------------------------------------------------------------------------//
    FILE FORMATS
//----------------------------------------------------------------------------*/

char *qb_find_file_extension(char* filename){
    int len = strlen(filename);
    bool iterate = true;
    int index = len-1;

    while (iterate){
        if (filename[index] == '.'){
            iterate = false;
        }
        index -= 1;
        if (index < 0){
            iterate = false;
        }
    }

    if (index < 0){
        return NULL;
    }
    else {
        return &filename[index+2];
    }
}

unsigned char *qb_read_file(char *filename,
                            int width,
                            int height,
                            int color_type){
    if (color_type == PRGBA8888 || color_type == PRGB888){
        int color_size = qb_get_color_size(color_type);

        unsigned char *output =
           (unsigned char *)malloc(color_size *
                                   width *
                                   height *
                                   sizeof(unsigned char));

        color_size--;

        unsigned char *data = stbi_load(filename,
                                        &width,
                                        &height,
                                        &color_size, 0);

        for (int i = 0; i < width * height; ++i){
            // sets rgb(a) channels
            for (int j = 0; j < color_size; ++j){
                output[i*(color_size+1)+j] = data[i+j];
            }

            // sets priority to 0
            output[i*(color_size+1)+color_size] = 0;
        }

        free(data);
        return output;
    }
    else{
        int color_size = qb_get_color_size(color_type);

        unsigned char *data = stbi_load(filename,
                                        &width,
                                        &height,
                                        &color_size, 0);
        return data;
    }

}

void qb_write_file(char *filename, quibble_pixels qps){

    char *file_extension = qb_find_file_extension(filename);

    if (file_extension == NULL){
        printf("Warning, no file extension found!\nDefaulting to BMP!\n");
        qb_write_bmp_file(filename, qps);
    }
    else if (strcmp(file_extension, "png") == 0 ||
             strcmp(file_extension, "PNG") == 0){
        qb_write_png_file(filename, qps);
    }
    else if (strcmp(file_extension, "jpg") == 0 ||
             strcmp(file_extension, "JPG") == 0 ||
             strcmp(file_extension, "jpeg") == 0 ||
             strcmp(file_extension, "JPEG") == 0){
        printf("Warning: no quality set for jpg file. Defaulting to 100!\n");
        printf("To set quality, run `qb_write_jpg_file(filename, pixels, quality)` instead!\n");
        qb_write_jpg_file(filename, qps, 100);
    }
    else if (strcmp(file_extension, "bmp") == 0 ||
             strcmp(file_extension, "BMP") == 0){
        qb_write_bmp_file(filename, qps);
    }
    else {
        printf("Warning, file extension %s is not supported!\n",
               file_extension);
        printf("Defaulting to BMP!\n");
        qb_write_bmp_file(filename, qps);
    }
}

unsigned char *qb_deprioritize_array(quibble_pixels qps){
    int color_size = qb_get_color_size(qps.color_type) - 1;

    unsigned char *output =
        (unsigned char *)malloc(sizeof(unsigned char) *
                                color_size * 
                                qps.width *
                                qps.height);

    unsigned char *host_data = (unsigned char *)qps.host_data;

    for (int i = 0; i < qps.width * qps.height; ++i){
        // sets rgb(a) channels
        for (int j = 0; j < color_size; ++j){
            output[i*color_size+j] = host_data[i*(color_size+1) + j];
        }
    }

    return output;
}

void qb_write_png_file(char *filename, quibble_pixels qps){
    unsigned char *output = qps.host_data;
    int color_size = qb_get_color_size(qps.color_type);

    if (qps.color_type == PRGBA8888 || qps.color_type == PRGB888){
        output = qb_deprioritize_array(qps);
        color_size--;
    }

    stbi_write_png(filename,
                   qps.width,
                   qps.height,
                   color_size,
                   output,
                   qps.width*color_size);


    if (qps.color_type == PRGBA8888 || qps.color_type == PRGB888){
        free(output);
    }
}

void qb_write_bmp_file(char *filename, quibble_pixels qps){
    unsigned char *output = qps.host_data;
    int color_size = qb_get_color_size(qps.color_type);

    if (qps.color_type == PRGBA8888 || qps.color_type == PRGB888){
        output = qb_deprioritize_array(qps);
        color_size--;
    }

    stbi_write_bmp(filename,
                   qps.width,
                   qps.height,
                   color_size,
                   output);

    if (qps.color_type == PRGBA8888 || qps.color_type == PRGB888){
        free(output);
    }
}

void qb_write_jpg_file(char *filename, quibble_pixels qps, int quality){
    unsigned char *output = qps.host_data;
    int color_size = qb_get_color_size(qps.color_type);

    if (qps.color_type == PRGBA8888 || qps.color_type == PRGB888){
        output = qb_deprioritize_array(qps);
        color_size--;
    }

    stbi_write_jpg(filename,
                   qps.width,
                   qps.height,
                   color_size,
                   output,
                   quality);

    if (qps.color_type == PRGBA8888 || qps.color_type == PRGB888){
        free(output);
    }
}

