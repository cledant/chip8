#ifndef CHIP8_EMU_EMU_CHIP8_H
#define CHIP8_EMU_EMU_CHIP8_H

/*
 * This implementation will follow CHIP8 technical reference from
 * http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */
#include "emu_inst.h"

extern emu_parse_fct_t g_chip8_parse_fcts[EMU_CHIP8_NB_INST];

/*
 * Chip8 Inst Detection fct
 */
emu_exec_fct_t chip8_is_sys(emu_inst_t inst);
emu_exec_fct_t chip8_is_cls(emu_inst_t inst);
emu_exec_fct_t chip8_is_ret(emu_inst_t inst);
emu_exec_fct_t chip8_is_jp_addr(emu_inst_t inst);
emu_exec_fct_t chip8_is_call(emu_inst_t inst);
emu_exec_fct_t chip8_is_se_register_byte(emu_inst_t inst);
emu_exec_fct_t chip8_is_sne_register_byte(emu_inst_t inst);
emu_exec_fct_t chip8_is_se_register_register(emu_inst_t inst);
emu_exec_fct_t chip8_is_ld_register_byte(emu_inst_t inst);
emu_exec_fct_t chip8_is_add_register_byte(emu_inst_t inst);
emu_exec_fct_t chip8_is_ld_register_register(emu_inst_t inst);
emu_exec_fct_t chip8_is_or(emu_inst_t inst);
emu_exec_fct_t chip8_is_and(emu_inst_t inst);
emu_exec_fct_t chip8_is_xor(emu_inst_t inst);
emu_exec_fct_t chip8_is_add_register_register(emu_inst_t inst);
emu_exec_fct_t chip8_is_sub(emu_inst_t inst);
emu_exec_fct_t chip8_is_shr(emu_inst_t inst);
emu_exec_fct_t chip8_is_subn(emu_inst_t inst);
emu_exec_fct_t chip8_is_shl(emu_inst_t inst);
emu_exec_fct_t chip8_is_sne_register_register(emu_inst_t inst);
emu_exec_fct_t chip8_is_ld_addr_register_addr(emu_inst_t inst);
emu_exec_fct_t chip8_is_jp_v0_addr(emu_inst_t inst);
emu_exec_fct_t chip8_is_rnd(emu_inst_t inst);
emu_exec_fct_t chip8_is_draw(emu_inst_t inst);
emu_exec_fct_t chip8_is_skp(emu_inst_t inst);
emu_exec_fct_t chip8_is_sknp(emu_inst_t inst);
emu_exec_fct_t chip8_is_ld_register_delay(emu_inst_t inst);
emu_exec_fct_t chip8_is_ld_register_key(emu_inst_t inst);
emu_exec_fct_t chip8_is_ld_delay_register(emu_inst_t inst);
emu_exec_fct_t chip8_is_ld_sound_register(emu_inst_t inst);
emu_exec_fct_t chip8_is_add_addr_register_register(emu_inst_t inst);
emu_exec_fct_t chip8_is_ld_font_addr_addr_register(emu_inst_t inst);
emu_exec_fct_t chip8_is_ld_bcd_addr(emu_inst_t inst);
emu_exec_fct_t chip8_is_ld_store_register(emu_inst_t inst);
emu_exec_fct_t chip8_is_ld_read_register(emu_inst_t inst);

/*
 * Chip8 Inst Exec fct
 */
int chip8_exec_sys(emu_inst_t inst, void *state, char const **err);
int chip8_exec_cls(emu_inst_t inst, void *state, char const **err);
int chip8_exec_ret(emu_inst_t inst, void *state, char const **err);
int chip8_exec_jp_addr(emu_inst_t inst, void *state, char const **err);
int chip8_exec_call(emu_inst_t inst, void *state, char const **err);
int chip8_exec_se_register_byte(emu_inst_t inst, void *state, char const **err);
int chip8_exec_sne_register_byte(emu_inst_t inst,
                                 void *state,
                                 char const **err);
int chip8_exec_se_register_register(emu_inst_t inst,
                                    void *state,
                                    char const **err);
int chip8_exec_ld_register_byte(emu_inst_t inst, void *state, char const **err);
int chip8_exec_add_register_byte(emu_inst_t inst,
                                 void *state,
                                 char const **err);
int chip8_exec_ld_register_register(emu_inst_t inst,
                                    void *state,
                                    char const **err);
int chip8_exec_or(emu_inst_t inst, void *state, char const **err);
int chip8_exec_and(emu_inst_t inst, void *state, char const **err);
int chip8_exec_xor(emu_inst_t inst, void *state, char const **err);
int chip8_exec_add_register_register(emu_inst_t inst,
                                     void *state,
                                     char const **err);
int chip8_exec_sub(emu_inst_t inst, void *state, char const **err);
int chip8_exec_shr(emu_inst_t inst, void *state, char const **err);
int chip8_exec_subn(emu_inst_t inst, void *state, char const **err);
int chip8_exec_shl(emu_inst_t inst, void *state, char const **err);
int chip8_exec_sne_register_register(emu_inst_t inst,
                                     void *state,
                                     char const **err);
int chip8_exec_ld_addr_register_addr(emu_inst_t inst,
                                     void *state,
                                     char const **err);
int chip8_exec_jp_v0_addr(emu_inst_t inst, void *state, char const **err);
int chip8_exec_rnd(emu_inst_t inst, void *state, char const **err);
int chip8_exec_draw(emu_inst_t inst, void *state, char const **err);
int chip8_exec_skp(emu_inst_t inst, void *state, char const **err);
int chip8_exec_sknp(emu_inst_t inst, void *state, char const **err);
int chip8_exec_ld_register_delay(emu_inst_t inst,
                                 void *state,
                                 char const **err);
int chip8_exec_ld_register_key(emu_inst_t inst, void *state, char const **err);
int chip8_exec_ld_delay_register(emu_inst_t inst,
                                 void *state,
                                 char const **err);
int chip8_exec_ld_sound_register(emu_inst_t inst,
                                 void *state,
                                 char const **err);
int chip8_exec_add_addr_register_register(emu_inst_t inst,
                                          void *state,
                                          char const **err);
int chip8_exec_ld_font_addr_addr_register(emu_inst_t inst,
                                          void *state,
                                          char const **err);
int chip8_exec_ld_bcd_addr(emu_inst_t inst, void *state, char const **err);
int chip8_exec_ld_store_register(emu_inst_t inst,
                                 void *state,
                                 char const **err);
int chip8_exec_ld_read_register(emu_inst_t inst, void *state, char const **err);

#endif // CHIP8_EMU_EMU_CHIP8_H
