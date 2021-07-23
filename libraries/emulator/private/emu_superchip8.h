#ifndef CHIP8_EMU_EMU_SUPERCHIP8_H
#define CHIP8_EMU_EMU_SUPERCHIP8_H

/*
 * This implementation will follow SUPERCHIP8 1.1 technical reference from
 * http://devernay.free.fr/hacks/chip8/schip.txt
 */
#include "emu_chip8.h"

extern emu_parse_fct_t g_superchip8_parse_fcts[EMU_SUPER_CHIP8_NB_INST];

/*
 * Super Chip8 Inst Detection fct
 */
emu_exec_fct_t superchip8_is_scd(emu_inst_t inst);
emu_exec_fct_t superchip8_is_scr(emu_inst_t inst);
emu_exec_fct_t superchip8_is_scl(emu_inst_t inst);
emu_exec_fct_t superchip8_is_exit(emu_inst_t inst);
emu_exec_fct_t superchip8_is_low(emu_inst_t inst);
emu_exec_fct_t superchip8_is_high(emu_inst_t inst);
emu_exec_fct_t superchip8_is_draw_extended(emu_inst_t inst);
emu_exec_fct_t superchip8_is_draw(emu_inst_t inst);
emu_exec_fct_t superchip8_is_ld_digit_addr_addr_register(emu_inst_t inst);
emu_exec_fct_t superchip8_is_ld_store_rpl(emu_inst_t inst);
emu_exec_fct_t superchip8_is_ld_read_rpl(emu_inst_t inst);

/*
 * Super Chip8 Inst Exec fct
 */
int superchip8_exec_scd(emu_inst_t inst, void *state, char const **err);
int superchip8_exec_scr(emu_inst_t inst, void *state, char const **err);
int superchip8_exec_scl(emu_inst_t inst, void *state, char const **err);
int superchip8_exec_exit(emu_inst_t inst, void *state, char const **err);
int superchip8_exec_low(emu_inst_t inst, void *state, char const **err);
int superchip8_exec_high(emu_inst_t inst, void *state, char const **err);
int superchip8_exec_draw_extended(emu_inst_t inst,
                                  void *state,
                                  char const **err);
int superchip8_exec_draw(emu_inst_t inst, void *state, char const **err);
int superchip8_exec_ld_digit_addr_addr_register(emu_inst_t inst,
                                                void *state,
                                                char const **err);
int superchip8_exec_ld_store_rpl(emu_inst_t inst,
                                 void *state,
                                 char const **err);
int superchip8_exec_ld_read_rpl(emu_inst_t inst, void *state, char const **err);

#endif // CHIP8_EMU_EMU_SUPERCHIP8_H
