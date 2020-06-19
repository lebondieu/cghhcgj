#include "modppn.h"

STATIC MP_DEFINE_CONST_FUN_OBJ_1(modppn_col_lst2int_obj, modppn_colLst2Int);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(modppn_col_int2lst_obj, modppn_colInt2Lst);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(modppn_clearScreen_obj, modppn_clearScreen);
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(modppn_draw_string_obj, 3, 5, modppn_drawString);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(modppn_none_function_obj, modppn_noneFunction)

STATIC const mp_rom_map_elem_t modppn_module_globals_table[] = {
  { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ppn) },
  { MP_ROM_QSTR(MP_QSTR_ppn_w), MP_OBJ_NEW_SMALL_INT(screenWidth)},
  { MP_ROM_QSTR(MP_QSTR_ppn_h), MP_OBJ_NEW_SMALL_INT(screenHeight)},
  { MP_ROM_QSTR(MP_QSTR_ppn_y0), MP_OBJ_NEW_SMALL_INT(screenY0)},
  { MP_ROM_QSTR(MP_QSTR_ppn_has_color), mp_const_true}, // must be equal to modppn.h > hasColor
  { MP_ROM_QSTR(MP_QSTR_ppn_need_show), mp_const_false}, // must be equal to modppn.h > needShow
  { MP_ROM_QSTR(MP_QSTR_col_lst2int), MP_ROM_PTR(&modppn_col_lst2int_obj) },
  { MP_ROM_QSTR(MP_QSTR_col_int2lst), MP_ROM_PTR(&modppn_col_int2lst_obj) },
  { MP_ROM_QSTR(MP_QSTR_clear_screen), MP_ROM_PTR(&modppn_clearScreen_obj) },
  { MP_ROM_QSTR(MP_QSTR_cls), MP_ROM_PTR(&modppn_clearScreen_obj) },
  { MP_ROM_QSTR(MP_QSTR_draw_string), MP_ROM_PTR(&modppn_draw_string_obj) },
  { MP_ROM_QSTR(MP_QSTR_show_screen), MP_ROM_PTR(&modppn_none_function_obj) }
};

STATIC MP_DEFINE_CONST_DICT(modppn_module_globals, modppn_module_globals_table);

const mp_obj_module_t modppn_module = {
  .base = { &mp_type_module },
  .globals = (mp_obj_dict_t*)&modppn_module_globals,
};
