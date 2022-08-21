#include "images_change.h"

images_change::images_change()
{
    index = -1;
}
char const* images_change::NextImage(){
    if(index>=1)
        index = 0;
    else
        index++;
    return(images[index]);
}
