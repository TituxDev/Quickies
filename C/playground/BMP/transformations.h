#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "bmp.h"

typedef enum transformations_e {
    NEGATIVE,
    HIGH_CONTRAST,

    TOTAL_TRANFORMATIONS
} transformations_labels;

size_t negative( file_t * , size_t );
size_t high_contrast( file_t * , size_t );

static struct          { uint8_t *name  ; size_t (*transformation)( file_t * , size_t );}tranformatons_list
[TOTAL_TRANFORMATIONS]={
    [NEGATIVE        ]={ "negative"     , negative                                      },
    [HIGH_CONTRAST   ]={ "high contrast", high_contrast                                 },
};

extern int apply( enum transformations_e  , file_t * );

#endif // TRANFORMATIONS_H