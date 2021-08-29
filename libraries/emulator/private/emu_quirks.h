#ifndef CHIP8_EMU_EMU_CHIP8_QUIRK_H
#define CHIP8_EMU_EMU_CHIP8_QUIRK_H

#include "emu_inst.h"

/*
 * Chip8 Quirks Inst Detection fct
 */
emu_exec_fct_t chip8_is_jp_v0_addr_quirk(emu_inst_t inst);
emu_exec_fct_t chip8_is_draw_wrap(emu_inst_t inst);
emu_exec_fct_t superchip8_is_draw_wrap(emu_inst_t inst);
emu_exec_fct_t superchip8_is_draw_extended_wrap(emu_inst_t inst);

/*
 * Chip8 Quirks Inst Exec fct
 */
int chip8_exec_jp_v0_addr_quirk(emu_inst_t inst, void *state, char const **err);
int chip8_exec_draw_wrap(emu_inst_t inst, void *state, char const **err);
int superchip8_exec_draw_wrap(emu_inst_t inst, void *state, char const **err);
int superchip8_exec_draw_extended_wrap(emu_inst_t inst,
                                       void *state,
                                       char const **err);

#endif // CHIP8_EMU_EMU_CHIP8_QUIRK_H
