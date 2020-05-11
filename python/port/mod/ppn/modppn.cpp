extern "C" {
#include "modppn.h"
}

mp_obj_t modppn_colLst2Int(mp_obj_t color) {
    mp_obj_t * rgb;
    mp_obj_get_array_fixed_n(color, 3, &rgb);
    int red = mp_obj_get_int(rgb[0]);
    int green = mp_obj_get_int(rgb[1]);
    int blue = mp_obj_get_int(rgb[2]);

    return mp_obj_new_int(red >> 3 << 11 | green >> 2 << 5 | blue >> 3);
}

mp_obj_t modppn_colInt2Lst(mp_obj_t color) {
    int int_color = mp_obj_get_int(color);
    mp_obj_t mp_color[3];

    mp_color[0] = mp_obj_new_int(int_color >> 11 << 3 & 255);
    mp_color[1] = mp_obj_new_int(int_color >> 5 << 2 & 255);
    mp_color[2] = mp_obj_new_int(int_color << 3 & 255);

    return mp_obj_new_tuple(3, mp_color);
}