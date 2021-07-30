#include "emu_chip8.h"
#include "emu_chip8_cosmac_vip.h"

/*
 * This implementation will follow CHIP8 technical reference from
 * https://chip-8.github.io/extensions/#chip-8
 */

#include <string.h>
#include <stdio.h>

#include "emu_state.h"

/*
 * emu_chip8 variables
 */
emu_parse_fct_t g_chip8_cosmac_vip_parse_fcts[EMU_CHIP8_NB_INST] = {
    chip8_is_cls,
    chip8_is_ret,
    chip8_is_sys,
    chip8_is_jp_addr,
    chip8_is_call,
    chip8_is_se_register_byte,
    chip8_is_sne_register_byte,
    chip8_is_se_register_register,
    chip8_is_ld_register_byte,
    chip8_is_add_register_byte,
    chip8_is_ld_register_register,
    chip8_is_or,
    chip8_is_and,
    chip8_is_xor,
    chip8_is_add_register_register,
    chip8_is_sub,
    chip8_cosmac_vip_is_shr,
    chip8_is_subn,
    chip8_cosmac_vip_is_shl,
    chip8_is_sne_register_register,
    chip8_is_ld_addr_register_addr,
    chip8_is_jp_v0_addr,
    chip8_is_rnd,
    chip8_is_draw,
    chip8_is_skp,
    chip8_is_sknp,
    chip8_is_ld_register_delay,
    chip8_is_ld_register_key,
    chip8_is_ld_delay_register,
    chip8_is_ld_sound_register,
    chip8_is_add_addr_register_register,
    chip8_is_ld_font_addr_addr_register,
    chip8_is_ld_bcd_addr,
    chip8_cosmac_vip_is_ld_store_register,
    chip8_cosmac_vip_is_ld_read_register
};

/*
 * Error buffer
 */
#define CHIP8_COSMAC_ERROR_BUFFER_SIZE 1024
static char chip8_cosmac_err_buffer[CHIP8_COSMAC_ERROR_BUFFER_SIZE];

/*
 * Chip8 Cosmac Inst Detection fct
 */
emu_exec_fct_t
chip8_cosmac_vip_is_shr(emu_inst_t inst)
{
    if (inst.n1 == 0x8 && inst.n4 == 0x6) {
        return (chip8_cosmac_vip_exec_shr);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_cosmac_vip_is_shl(emu_inst_t inst)
{
    if (inst.n1 == 0x8 && inst.n4 == 0xE) {
        return (chip8_cosmac_vip_exec_shl);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_cosmac_vip_is_ld_store_register(emu_inst_t inst)
{
    if (inst.n1 == 0xF && inst.n3 == 0x5 && inst.n4 == 0x5) {
        return (chip8_cosmac_vip_exec_ld_store_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_cosmac_vip_is_ld_read_register(emu_inst_t inst)
{
    if (inst.n1 == 0xF && inst.n3 == 0x6 && inst.n4 == 0x5) {
        return (chip8_cosmac_vip_exec_ld_read_register);
    }
    return (NULL);
}

/*
 * Chip8 Cosmac Inst Exec fct
 */
int
chip8_cosmac_vip_exec_shr(emu_inst_t inst, void *state, char const **err)
{
    /*
     * Opcode 8XY6
     */
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->general_registers[0xF] =
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y] &
      0b00000001;
    rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] =
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y] >> 1;
    return (0);
}

int
chip8_cosmac_vip_exec_shl(emu_inst_t inst, void *state, char const **err)
{
    /*
     * Opcode 8XYE
     */
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->general_registers[0xF] =
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y] &
      0b10000000;
    rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] =
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y] << 1;
    return (0);
}

int
chip8_cosmac_vip_exec_ld_store_register(emu_inst_t inst,
                                        void *state,
                                        char const **err)
{
    /*
     * Opcode FX55
     */
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    for (uint8_t i = 0; i <= inst.n2; ++i) {
        if (rs->address_register + i > es->max_addr) {
            snprintf(chip8_cosmac_err_buffer,
                     CHIP8_COSMAC_ERROR_BUFFER_SIZE,
                     "Instruction FX55 tried to write memory outside allowed "
                     "range at 0x%x. Max allowed=Ox%x",
                     rs->address_register + i,
                     es->max_addr);
            *err = chip8_cosmac_err_buffer;
            return (1);
        }
        es->ram[rs->address_register + i] = rs->general_registers[i];
    }
    rs->address_register += (inst.n2 + 1);
    return (0);
}

int
chip8_cosmac_vip_exec_ld_read_register(emu_inst_t inst,
                                       void *state,
                                       char const **err)
{
    /*
     * Opcode FX65
     */
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    for (uint8_t i = 0; i <= inst.n2; ++i) {
        if (rs->address_register + i > es->max_addr) {
            snprintf(chip8_cosmac_err_buffer,
                     CHIP8_COSMAC_ERROR_BUFFER_SIZE,
                     "Instruction FX65 tried to load memory outside allowed "
                     "range at 0x%x. Max allowed=Ox%x",
                     rs->address_register + i,
                     es->max_addr);
            *err = chip8_cosmac_err_buffer;
            return (1);
        }
        rs->general_registers[i] = es->ram[rs->address_register + i];
    }
    rs->address_register += (inst.n2 + 1);
    return (0);
}