extern "C" {
#include "modppn.h"
#include <py/runtime.h>
}
#include <python/port/port.h>
#include "../kandinsky/modkandinsky.h"
#include <kandinsky.h>
#include <ion.h>

static KDColor ColorForTuple(mp_obj_t tuple) {
    size_t len;
    mp_obj_t * elem;

    mp_obj_get_array(tuple, &len, &elem);
    if (len != 3) {
      mp_raise_TypeError("color needs 3 components");
    }

    return KDColor::RGB888(
      mp_obj_get_int(elem[0]),
      mp_obj_get_int(elem[1]),
      mp_obj_get_int(elem[2])
    );
}

void fillRect(int x, int y, int w, int h, KDColor color);

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

mp_obj_t modppn_clearScreen() {
    fillRect(0, 0, screenWidth, screenHeight, KDColorWhite);
    return mp_const_none;
}

mp_obj_t modppn_drawString(size_t n_args, const mp_obj_t * args) {
    const char * text = mp_obj_str_get_str(args[2]);
    KDPoint point(mp_obj_get_int(args[0]), mp_obj_get_int(args[1]));
    KDColor textColor = (n_args >= 4) ? ColorForTuple(args[3]) : KDColorBlack;
    KDColor backgroundColor = (n_args >= 5) ? ColorForTuple(args[4]) : KDColorWhite;
    MicroPython::ExecutionEnvironment::currentExecutionEnvironment()->displaySandbox();
    KDIonContext::sharedContext()->drawString(text, point, KDFont::LargeFont, textColor, backgroundColor);
    /* Before and after execution of "modkandinsky_draw_string",
    * "micropython_port_vm_hook_loop" is called by "mp_execute_bytecode" and will
    * call "micropython_port_interrupt_if_needed" every 20000 calls.
    * However, "drawString" function might take some time to execute leading to
    * drastically decrease the frequency of calls to
    * "micropython_port_vm_hook_loop" and thus to
    * "micropython_port_interrupt_if_needed". So we add an extra
    * check for user interruption here. This way the user can still interrupt an
    * infinite loop calling 'drawString' for instance. */
    micropython_port_interrupt_if_needed();
    return mp_const_none;
}

mp_obj_t modppn_noneFunction() {
    return mp_const_none;
}

void fillRect(int x, int y, int w, int h, KDColor color) {
    MicroPython::ExecutionEnvironment::currentExecutionEnvironment()->displaySandbox();
    KDIonContext::sharedContext()->fillRect(KDRect(x, y, w, h), color);
    // Cf comment on modkandinsky_draw_string
    micropython_port_interrupt_if_needed();
}