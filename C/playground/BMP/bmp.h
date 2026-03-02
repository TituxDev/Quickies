#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>
#include <stddef.h>
#include <sys/ioctl.h>

#pragma pack(push, 1)
typedef struct bmp_header {
    uint16_t signature;
    uint32_t file_size;
    uint32_t reserved;
    uint32_t offset;
    uint32_t header_size;
    uint32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    uint32_t x_pixels_per_meter;
    uint32_t y_pixels_per_meter;
    uint32_t colors_used;
    uint32_t important_colors;
} header_t;

typedef struct bmp_file {
    header_t header;
    uint8_t *extra;
    uint8_t *bmp;
    FILE *file;
    uint8_t *name;
    uint8_t *out;
} file_t;
#pragma pack(pop)

typedef enum {
    SIGNATURE,
    FILE_SIZE,
    RESERVED,
    OFFSET,
    HEADER_SIZE,
    WIDTH,
    HEIGHT,
    PLANES,
    BITS_PER_PIXEL,
    COMPRESSION,
    IMAGE_SIZE,
    X_PIXELS_PER_METER,
    Y_PIXELS_PER_METER,
    COLORS_USED,
    IMPORTANT_COLORS,

    TOTAL_HEADER_ATTRIBUTES
} header_index;

static struct             { const char *field   ; size_t offset                           ; size_t size                                 ;}header_list
[TOTAL_HEADER_ATTRIBUTES]={
    [SIGNATURE          ]={ "signature"         , offsetof( header_t , signature         ), sizeof( ((header_t *)0)->signature         ) },
    [FILE_SIZE          ]={ "file size"         , offsetof( header_t , file_size         ), sizeof( ((header_t *)0)->file_size         ) },
    [RESERVED           ]={ "reserved"          , offsetof( header_t , reserved          ), sizeof( ((header_t *)0)->reserved          ) },
    [OFFSET             ]={ "offset"            , offsetof( header_t , offset            ), sizeof( ((header_t *)0)->offset            ) },
    [HEADER_SIZE        ]={ "header size"       , offsetof( header_t , header_size       ), sizeof( ((header_t *)0)->header_size       ) },
    [WIDTH              ]={ "width"             , offsetof( header_t , width             ), sizeof( ((header_t *)0)->width             ) },
    [HEIGHT             ]={ "height"            , offsetof( header_t , height            ), sizeof( ((header_t *)0)->height            ) },
    [PLANES             ]={ "planes"            , offsetof( header_t , planes            ), sizeof( ((header_t *)0)->planes            ) },
    [BITS_PER_PIXEL     ]={ "bits per pixel"    , offsetof( header_t , bits_per_pixel    ), sizeof( ((header_t *)0)->bits_per_pixel    ) },
    [COMPRESSION        ]={ "compression"       , offsetof( header_t , compression       ), sizeof( ((header_t *)0)->compression       ) },
    [IMAGE_SIZE         ]={ "image_size"        , offsetof( header_t , image_size        ), sizeof( ((header_t *)0)->image_size        ) },
    [X_PIXELS_PER_METER ]={ "x pixels per meter", offsetof( header_t , x_pixels_per_meter), sizeof( ((header_t *)0)->x_pixels_per_meter) },
    [Y_PIXELS_PER_METER ]={ "y pixels per meter", offsetof( header_t , y_pixels_per_meter), sizeof( ((header_t *)0)->y_pixels_per_meter) },
    [COLORS_USED        ]={ "colors used"       , offsetof( header_t , colors_used       ), sizeof( ((header_t *)0)->colors_used       ) },
    [IMPORTANT_COLORS   ]={ "important colors"  , offsetof( header_t , important_colors  ), sizeof( ((header_t *)0)->important_colors  ) },
};

#endif // BMP_h