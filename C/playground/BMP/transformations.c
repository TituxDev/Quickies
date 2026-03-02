#include "transformations.h"

size_t negative( file_t *file , size_t index ){
    file->bmp[index]= ~file->bmp[index];
    return 1;
}
size_t high_contrast( file_t *file ,  size_t index ){
    file->bmp[index]= file->bmp[index] > 125 ? 255 : 0;
    return 1;
}

int apply( enum transformations_e transformation , file_t *file ) {
    if( transformation < TOTAL_TRANFORMATIONS ) for( size_t i= 0 ; i < file->header.image_size ; i+= tranformatons_list[transformation].transformation( file , i ) );
    return transformation >= TOTAL_TRANFORMATIONS;
}