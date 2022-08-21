#ifndef IMAGES_CHANGE_H
#define IMAGES_CHANGE_H


class images_change
{
public:
    images_change();
    char const* NextImage(); //图片的路径
    char const* images[2] = {":/image/image/benbu.png",":/image/image/shahe.png"};
    int index;
};

#endif // IMAGES_CHANGE_H
