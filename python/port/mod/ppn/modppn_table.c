#include "modppn.h"

STATIC MP_DEFINE_CONST_FUN_OBJ_1(modppn_colLst2Int_obj, modppn_colLst2Int);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(modppn_colInt2Lst_obj, modppn_colInt2Lst);

STATIC const mp_rom_map_elem_t modppn_module_globals_table[] = {
  { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ppn) },
  { MP_ROM_QSTR(MP_QSTR_ppn_w), MP_OBJ_NEW_SMALL_INT(screenWidth)},
  { MP_ROM_QSTR(MP_QSTR_ppn_h), MP_OBJ_NEW_SMALL_INT(screenHeight)},
  { MP_ROM_QSTR(MP_QSTR_ppn_y0), MP_OBJ_NEW_SMALL_INT(screenY0)},
  { MP_ROM_QSTR(MP_QSTR_ppn_has_color), hasColor ? mp_const_true : mp_const_false},
  { MP_ROM_QSTR(MP_QSTR_ppn_need_show), needShow ? mp_const_true : mp_const_false},
  { MP_ROM_QSTR(MP_QSTR_col_lst2int), MP_ROM_PTR(&modppn_colLst2Int_obj) },
  { MP_ROM_QSTR(MP_QSTR_col_int2lst), MP_ROM_PTR(&modppn_colInt2Lst_obj) },

};

STATIC MP_DEFINE_CONST_DICT(modppn_module_globals, modppn_module_globals_table);

const mp_obj_module_t modppn_module = {
  .base = { &mp_type_module },
  .globals = (mp_obj_dict_t*)&modppn_module_globals,
};
