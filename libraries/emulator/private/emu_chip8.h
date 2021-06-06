#ifndef CHIP8_EMU_EMU_CHIP8_INST_H
#define CHIP8_EMU_EMU_CHIP8_INST_H

#include "emu_inst.h"
#include "emu_def.h"

extern emu_parse_fct g_emu_chip8_parse_fcts[EMU_CHIP8_NB_INST];
extern emu_exec_fct g_emu_chip8_exec_fcts[EMU_CHIP8_NB_INST];

/*
 * Chip8 Inst Parsing fct
 */
int emu_is_sys(emu_inst_t inst);
int emu_is_cls(emu_inst_t inst);
int emu_is_ret(emu_inst_t inst);
int emu_is_jp_addr(emu_inst_t inst);
int emu_is_call(emu_inst_t inst);
int emu_is_se_register_byte(emu_inst_t inst);
int emu_is_sne_register_byte(emu_inst_t inst);
int emu_is_se_register_register(emu_inst_t inst);
int emu_is_ld_register_byte(emu_inst_t inst);
int emu_is_add_register_byte(emu_inst_t inst);
int emu_is_ld_register_register(emu_inst_t inst);
int emu_is_or(emu_inst_t inst);
int emu_is_and(emu_inst_t inst);
int emu_is_xor(emu_inst_t inst);
int emu_is_add_register_register(emu_inst_t inst);
int emu_is_sub(emu_inst_t inst);
int emu_is_shr(emu_inst_t inst);
int emu_is_subn(emu_inst_t inst);
int emu_is_shl(emu_inst_t inst);
int emu_is_sne_register_register(emu_inst_t inst);
int emu_is_ld_addr_register_addr(emu_inst_t inst);
int emu_is_jp_v0_addr(emu_inst_t inst);
int emu_is_rnd(emu_inst_t inst);
int emu_is_draw(emu_inst_t inst);
int emu_is_skp(emu_inst_t inst);
int emu_is_ld_register_delay(emu_inst_t inst);
int emu_is_ld_register_key(emu_inst_t inst);
int emu_is_ld_delay_register(emu_inst_t inst);
int emu_is_ld_sound_register(emu_inst_t inst);
int emu_is_add_addr_register_register(emu_inst_t inst);
int emu_is_ld_fb_register(emu_inst_t inst);
int emu_is_ld_bcd_addr(emu_inst_t inst);
int emu_is_ld_store_register(emu_inst_t inst);
int emu_is_ld_read_register(emu_inst_t inst);

/*
 * Chip8 Inst Exec fct
 */
int emu_exec_sys(emu_inst_t inst);
int emu_exec_cls(emu_inst_t inst);
int emu_exec_ret(emu_inst_t inst);
int emu_exec_jp_addr(emu_inst_t inst);
int emu_exec_call(emu_inst_t inst);
int emu_exec_se_register_byte(emu_inst_t inst);
int emu_exec_sne_register_byte(emu_inst_t inst);
int emu_exec_se_register_register(emu_inst_t inst);
int emu_exec_ld_register_byte(emu_inst_t inst);
int emu_exec_add_register_byte(emu_inst_t inst);
int emu_exec_ld_register_register(emu_inst_t inst);
int emu_exec_or(emu_inst_t inst);
int emu_exec_and(emu_inst_t inst);
int emu_exec_xor(emu_inst_t inst);
int emu_exec_add_register_register(emu_inst_t inst);
int emu_exec_sub(emu_inst_t inst);
int emu_exec_shr(emu_inst_t inst);
int emu_exec_subn(emu_inst_t inst);
int emu_exec_shl(emu_inst_t inst);
int emu_exec_sne_register_register(emu_inst_t inst);
int emu_exec_ld_addr_register_addr(emu_inst_t inst);
int emu_exec_jp_v0_addr(emu_inst_t inst);
int emu_exec_rnd(emu_inst_t inst);
int emu_exec_draw(emu_inst_t inst);
int emu_exec_skp(emu_inst_t inst);
int emu_exec_ld_register_delay(emu_inst_t inst);
int emu_exec_ld_register_key(emu_inst_t inst);
int emu_exec_ld_delay_register(emu_inst_t inst);
int emu_exec_st_sound_register(emu_inst_t inst);
int emu_exec_add_addr_register_register(emu_inst_t inst);
int emu_exec_ld_fb_register(emu_inst_t inst);
int emu_exec_ld_bcd_addr(emu_inst_t inst);
int emu_exec_ld_store_register(emu_inst_t inst);
int emu_exec_ld_read_register(emu_inst_t inst);

#endif // CHIP8_EMU_EMU_CHIP8_INST_H
