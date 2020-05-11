#include <py/obj.h>

const int screenWidth = 320;
const int screenHeight = 222;
const int screenY0 = 0;

const bool hasColor = true;
const bool needShow = false;

mp_obj_t modppn_colLst2Int(mp_obj_t color);
mp_obj_t modppn_colInt2Lst(mp_obj_t color);
