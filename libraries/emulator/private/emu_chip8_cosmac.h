#ifndef CHIP8_EMU_EMU_CHIP8_COSMAC_H
#define CHIP8_EMU_EMU_CHIP8_COSMAC_H

/*
 * This implementation will follow CHIP8 technical reference from
 * https://chip-8.github.io/extensions/#chip-8
 */
#include "emu_inst.h"

extern emu_parse_fct_t g_chip8_cosmac_parse_fcts[EMU_CHIP8_NB_INST];

/*
 * Chip8 Cosmac Inst Detection fct
 */
emu_exec_fct_t chip8_cosmac_is_shr(emu_inst_t inst);
emu_exec_fct_t chip8_cosmac_is_shl(emu_inst_t inst);
emu_exec_fct_t chip8_cosmac_is_ld_store_register(emu_inst_t inst);
emu_exec_fct_t chip8_cosmac_is_ld_read_register(emu_inst_t inst);

/*
 * Chip8 Cosmac Inst Exec fct
 */
int chip8_cosmac_exec_shr(emu_inst_t inst, void *state, char const **err);
int chip8_cosmac_exec_shl(emu_inst_t inst, void *state, char const **err);
int chip8_cosmac_exec_ld_store_register(emu_inst_t inst,
                                        void *state,
                                        char const **err);
int chip8_cosmac_exec_ld_read_register(emu_inst_t inst,
                                       void *state,
                                       char const **err);

#endif // CHIP8_EMU_EMU_CHIP8_COSMAC_H
