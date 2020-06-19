#include <py/obj.h>

const int screenWidth = 320;
const int screenHeight = 222;
const int screenY0 = 0;

const bool hasColor = true; // must be equal to modppn_table.c > hasColor table entry
const bool needShow = false; // must be equal to modppn_table.c > needShow table entry



mp_obj_t modppn_colLst2Int(mp_obj_t color);
mp_obj_t modppn_colInt2Lst(mp_obj_t color);
mp_obj_t modppn_clearScreen();
mp_obj_t modppn_drawString(size_t n_args, const mp_obj_t * args);
mp_obj_t modppn_noneFunction();
