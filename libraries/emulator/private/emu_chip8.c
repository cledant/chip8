#include "emu_chip8.h"

/*
 * This implementation will follow CHIP8 technical refrence from
 * http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "emu_state.h"
#include "emu_fb.h"

/*
 * emu_chip8 variables
 */
emu_parse_fct_t g_chip8_parse_fcts[EMU_CHIP8_NB_INST] = {
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
    chip8_is_shr,
    chip8_is_subn,
    chip8_is_shl,
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
    chip8_is_ld_store_register,
    chip8_is_ld_read_register
};

/*
 * Error buffer
 */
#define CHIP8_ERROR_BUFFER_SIZE 1024
static char chip8_err_buffer[CHIP8_ERROR_BUFFER_SIZE];

/*
 * Chip8 Inst Detection fct
 */
emu_exec_fct_t
chip8_is_sys(emu_inst_t inst)
{
    if (inst.n1 == 0x0) {
        return (chip8_exec_sys);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_cls(emu_inst_t inst)
{
    if (inst.n1 == 0x0 && inst.n2 == 0x0 && inst.n3 == 0xE && inst.n4 == 0x0) {
        return (chip8_exec_cls);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ret(emu_inst_t inst)
{
    if (inst.n1 == 0x0 && inst.n2 == 0x0 && inst.n3 == 0xE && inst.n4 == 0xE) {
        return (chip8_exec_ret);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_jp_addr(emu_inst_t inst)
{
    if (inst.n1 == 0x1) {
        return (chip8_exec_jp_addr);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_call(emu_inst_t inst)
{
    if (inst.n1 == 0x2) {
        return (chip8_exec_call);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_se_register_byte(emu_inst_t inst)
{
    if (inst.n1 == 0x3) {
        return (chip8_exec_se_register_byte);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_sne_register_byte(emu_inst_t inst)
{
    if (inst.n1 == 0x4) {
        return (chip8_exec_sne_register_byte);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_se_register_register(emu_inst_t inst)
{
    if (inst.n1 == 0x5) {
        return (chip8_exec_se_register_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_register_byte(emu_inst_t inst)
{
    if (inst.n1 == 0x6) {
        return (chip8_exec_ld_register_byte);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_add_register_byte(emu_inst_t inst)
{
    if (inst.n1 == 0x7) {
        return (chip8_exec_add_register_byte);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_register_register(emu_inst_t inst)
{
    if (inst.n1 == 0x8 && inst.n4 == 0x0) {
        return (chip8_exec_ld_register_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_or(emu_inst_t inst)
{
    if (inst.n1 == 0x8 && inst.n4 == 0x1) {
        return (chip8_exec_or);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_and(emu_inst_t inst)
{
    if (inst.n1 == 0x8 && inst.n4 == 0x2) {
        return (chip8_exec_and);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_xor(emu_inst_t inst)
{
    if (inst.n1 == 0x8 && inst.n4 == 0x3) {
        return (chip8_exec_xor);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_add_register_register(emu_inst_t inst)
{
    if (inst.n1 == 0x8 && inst.n4 == 0x4) {
        return (chip8_exec_add_register_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_sub(emu_inst_t inst)
{
    if (inst.n1 == 0x8 && inst.n4 == 0x5) {
        return (chip8_exec_sub);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_shr(emu_inst_t inst)
{
    if (inst.n1 == 0x8 && inst.n4 == 0x6) {
        return (chip8_exec_shr);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_subn(emu_inst_t inst)
{
    if (inst.n1 == 0x8 && inst.n4 == 0x7) {
        return (chip8_exec_subn);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_shl(emu_inst_t inst)
{
    if (inst.n1 == 0x8 && inst.n4 == 0xE) {
        return (chip8_exec_shl);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_sne_register_register(emu_inst_t inst)
{
    if (inst.n1 == 0x9 && inst.n4 == 0x0) {
        return (chip8_exec_sne_register_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_addr_register_addr(emu_inst_t inst)
{
    if (inst.n1 == 0xA) {
        return (chip8_exec_ld_addr_register_addr);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_jp_v0_addr(emu_inst_t inst)
{
    if (inst.n1 == 0xB) {
        return (chip8_exec_jp_v0_addr);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_rnd(emu_inst_t inst)
{
    if (inst.n1 == 0xC) {
        return (chip8_exec_rnd);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_draw(emu_inst_t inst)
{
    if (inst.n1 == 0xD) {
        return (chip8_exec_draw);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_skp(emu_inst_t inst)
{
    if (inst.n1 == 0xE && inst.n3 == 0x9 && inst.n4 == 0xE) {
        return (chip8_exec_skp);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_sknp(emu_inst_t inst)
{
    if (inst.n1 == 0xE && inst.n3 == 0xA && inst.n4 == 0x1) {
        return (chip8_exec_sknp);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_register_delay(emu_inst_t inst)
{
    if (inst.n1 == 0xF && inst.n3 == 0x0 && inst.n4 == 0x7) {
        return (chip8_exec_ld_register_delay);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_register_key(emu_inst_t inst)
{
    if (inst.n1 == 0xF && inst.n3 == 0x0 && inst.n4 == 0xA) {
        return (chip8_exec_ld_register_key);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_delay_register(emu_inst_t inst)
{
    if (inst.n1 == 0xF && inst.n3 == 0x1 && inst.n4 == 0x5) {
        return (chip8_exec_ld_delay_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_sound_register(emu_inst_t inst)
{
    if (inst.n1 == 0xF && inst.n3 == 0x1 && inst.n4 == 0x8) {
        return (chip8_exec_ld_sound_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_add_addr_register_register(emu_inst_t inst)
{
    if (inst.n1 == 0xF && inst.n3 == 0x1 && inst.n4 == 0xE) {
        return (chip8_exec_add_addr_register_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_font_addr_addr_register(emu_inst_t inst)
{
    if (inst.n1 == 0xF && inst.n3 == 0x2 && inst.n4 == 0x9) {
        return (chip8_exec_ld_font_addr_addr_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_bcd_addr(emu_inst_t inst)
{
    if (inst.n1 == 0xF && inst.n3 == 0x3 && inst.n4 == 0x3) {
        return (chip8_exec_ld_bcd_addr);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_store_register(emu_inst_t inst)
{
    if (inst.n1 == 0xF && inst.n3 == 0x5 && inst.n4 == 0x5) {
        return (chip8_exec_ld_store_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_read_register(emu_inst_t inst)
{
    if (inst.n1 == 0xF && inst.n3 == 0x6 && inst.n4 == 0x5) {
        return (chip8_exec_ld_read_register);
    }
    return (NULL);
}

/*
 * Chip8 Inst Exec fct
 */
int
chip8_exec_sys(emu_inst_t inst, void *state, char const **err)
{
    /*
     * Ignored instruction
     */
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_cls(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)err;
    emu_state_t *es = state;

    memset(es->framebuffer, 0, EMU_FRAMEBUFFER_MAX_SIZE);
    return (0);
}

int
chip8_exec_ret(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    if (!rs->current_stack) {
        snprintf(chip8_err_buffer,
                 CHIP8_ERROR_BUFFER_SIZE,
                 "RET at %x on empty stack pointer",
                 rs->program_counter - 2);
        *err = chip8_err_buffer;
        return (1);
    }
    rs->program_counter = rs->stack_pointer[rs->current_stack--];
    return (0);
}

int
chip8_exec_jp_addr(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_inst_addr_t inst_addr = emu_inst_to_emu_inst_addr(inst);

    es->registers.program_counter = inst_addr.addr;
    return (0);
}

int
chip8_exec_call(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    (void)inst;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;
    emu_inst_addr_t inst_addr = emu_inst_to_emu_inst_addr(inst);

    rs->stack_pointer[++rs->current_stack] = rs->program_counter;
    rs->program_counter = inst_addr.addr;
    return (0);
}

int
chip8_exec_se_register_byte(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;
    emu_inst_reg_uint8_t inst_reg_uint8 = emu_inst_to_emu_inst_reg_uint8(inst);

    if (rs->general_registers[inst_reg_uint8.gen_reg] == inst_reg_uint8.value) {
        rs->program_counter += 2;
    }
    return (0);
}

int
chip8_exec_sne_register_byte(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;
    emu_inst_reg_uint8_t inst_reg_uint8 = emu_inst_to_emu_inst_reg_uint8(inst);

    if (rs->general_registers[inst_reg_uint8.gen_reg] != inst_reg_uint8.value) {
        rs->program_counter += 2;
    }
    return (0);
}

int
chip8_exec_se_register_register(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    if (rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] ==
        rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y]) {
        rs->program_counter += 2;
    }
    return (0);
}

int
chip8_exec_ld_register_byte(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;
    emu_inst_reg_uint8_t inst_reg_uint8 = emu_inst_to_emu_inst_reg_uint8(inst);

    rs->general_registers[inst_reg_uint8.gen_reg] = inst_reg_uint8.value;
    return (0);
}

int
chip8_exec_add_register_byte(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;
    emu_inst_reg_uint8_t inst_reg_uint8 = emu_inst_to_emu_inst_reg_uint8(inst);

    rs->general_registers[inst_reg_uint8.gen_reg] += inst_reg_uint8.value;
    return (0);
}

int
chip8_exec_ld_register_register(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] =
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y];
    return (0);
}

int
chip8_exec_or(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] |=
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y];
    return (0);
}

int
chip8_exec_and(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] &=
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y];
    return (0);
}

int
chip8_exec_xor(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] ^=
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y];
    return (0);
}

int
chip8_exec_add_register_register(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    uint16_t add =
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] +
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y];
    rs->general_registers[0xF] = (add > 255) ? 1 : 0;
    rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] =
      0x00FF & add;
    return (0);
}

int
chip8_exec_sub(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->general_registers[0xF] =
      (rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] >
       rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y])
        ? 1
        : 0;
    rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] -=
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y];
    return (0);
}

int
chip8_exec_shr(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->general_registers[0xF] =
      (rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] &
       0b00000001)
        ? 1
        : 0;
    rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] /= 2;
    return (0);
}

int
chip8_exec_subn(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->general_registers[0xF] =
      (rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y] >
       rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x])
        ? 1
        : 0;
    rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] =
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y] -
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x];
    return (0);
}

int
chip8_exec_shl(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->general_registers[0xF] =
      (rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] &
       0b10000000)
        ? 1
        : 0;
    rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] *= 2;
    return (0);
}

int
chip8_exec_sne_register_register(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    if (rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] !=
        rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_y]) {
        rs->program_counter += 2;
    }
    return (0);
}

int
chip8_exec_ld_addr_register_addr(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;
    emu_inst_addr_t inst_addr = emu_inst_to_emu_inst_addr(inst);

    rs->address_register = inst_addr.addr;
    return (0);
}

int
chip8_exec_jp_v0_addr(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;
    emu_inst_addr_t inst_addr = emu_inst_to_emu_inst_addr(inst);

    rs->program_counter = rs->general_registers[0] + inst_addr.addr;
    return (0);
}

int
chip8_exec_rnd(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;
    emu_inst_reg_uint8_t inst_reg_uint8 = emu_inst_to_emu_inst_reg_uint8(inst);

    uint8_t rnd_val = lrand48();
    rs->general_registers[inst_reg_uint8.gen_reg] =
      rnd_val & inst_reg_uint8.value;
    return (0);
}

static int
exec_draw_memory_location_checks(uint32_t ram_pos,
                                 uint32_t fb_pos,
                                 uint32_t max_ram_addr,
                                 uint32_t max_fb)
{
    if (ram_pos > max_ram_addr) {
        snprintf(chip8_err_buffer,
                 CHIP8_ERROR_BUFFER_SIZE,
                 "Instruction DXYN tried to load memory outside allowed "
                 "range at 0x%x. Max allowed=Ox%x",
                 ram_pos,
                 max_ram_addr);
        return (1);
    }
    if (fb_pos >= max_fb) {
        snprintf(chip8_err_buffer,
                 CHIP8_ERROR_BUFFER_SIZE,
                 "Instruction DXYN tried to write framebuffer outside allowed "
                 "range at 0x%x. Max allowed=Ox%x",
                 fb_pos,
                 max_fb);
        return (1);
    }
    return (0);
}

int
chip8_exec_draw(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    uint8_t size_to_copy = ((emu_inst_draw_t *)&inst)->size;
    uint8_t pos_w = rs->general_registers[((emu_inst_draw_t *)&inst)->x];
    uint8_t pos_h = rs->general_registers[((emu_inst_draw_t *)&inst)->y];
    rs->general_registers[0xF] = 0;
    for (uint8_t i = 0; i < size_to_copy; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            uint32_t write_pos_w = ((pos_w + j) % EMU_CHIP8_W);
            uint32_t write_pos_h = ((pos_h + i) % EMU_CHIP8_H);

            uint32_t fb_bit_pos = (write_pos_w + EMU_CHIP8_W * write_pos_h) / 8;
            uint32_t fb_bit_offset =
              (write_pos_w + EMU_CHIP8_W * write_pos_h) % 8;

            if (exec_draw_memory_location_checks(rs->address_register + i,
                                                 fb_bit_pos,
                                                 es->max_addr,
                                                 es->max_fb)) {
                *err = chip8_err_buffer;
                return (1);
            }
            uint8_t sprite_bit =
              (es->ram[rs->address_register + i] & (1 << (7 - j))) >> (7 - j);
            uint8_t fb_bit =
              (es->framebuffer[fb_bit_pos] & (1 << fb_bit_offset)) >>
              fb_bit_offset;

            if (sprite_bit && fb_bit) {
                rs->general_registers[0xF] = 1;
            }
            es->framebuffer[fb_bit_pos] ^= (sprite_bit << fb_bit_offset);
        }
    }
    return (0);
}

int
chip8_exec_skp(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    uint8_t key_val =
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x];
    if (key_val > EMU_NB_KEYS) {
        return (0);
    }
    if (es->keys_state[key_val]) {
        rs->program_counter += 2;
    }
    return (0);
}

int
chip8_exec_sknp(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    uint8_t key_val = rs->general_registers[inst.n2];
    if (key_val > EMU_NB_KEYS) {
        return (0);
    }
    if (!es->keys_state[key_val]) {
        rs->program_counter += 2;
    }
    return (0);
}

int
chip8_exec_ld_register_delay(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] =
      rs->delay_register;
    return (0);
}

int
chip8_exec_ld_register_key(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    for (uint8_t i = 0; i < EMU_NB_KEYS; ++i) {
        if (es->keys_state[i]) {
            es->skip_fetch = 0;
            rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] = i;
            return (0);
        }
    }
    es->skip_fetch = 1;
    return (0);
}

int
chip8_exec_ld_delay_register(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->delay_register =
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x];
    return (0);
}

int
chip8_exec_ld_sound_register(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->sound_register =
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x];
    return (0);
}

int
chip8_exec_add_addr_register_register(emu_inst_t inst,
                                      void *state,
                                      char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->address_register +=
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x];
    return (0);
}

int
chip8_exec_ld_font_addr_addr_register(emu_inst_t inst,
                                      void *state,
                                      char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->address_register =
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] *
      EMU_CHIP_8_FONT_HEIGHT;
    return (0);
}

int
chip8_exec_ld_bcd_addr(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    uint8_t divisor = 100;
    uint8_t count = 0;
    for (uint8_t i = 0; i < 3; ++i) {
        if (rs->address_register + i > es->max_addr) {
            snprintf(chip8_err_buffer,
                     CHIP8_ERROR_BUFFER_SIZE,
                     "Instruction FX33 tried to write memory outside allowed "
                     "range at 0x%x. Max allowed=Ox%x",
                     rs->address_register + i,
                     es->max_addr);
            *err = chip8_err_buffer;
            return (1);
        }
        es->ram[rs->address_register + i] =
          (rs->general_registers[inst.n2] - count) / divisor;
        count += es->ram[rs->address_register + i] * divisor;
        divisor /= 10;
    }
    return (0);
}

int
chip8_exec_ld_store_register(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    for (uint8_t i = 0; i <= inst.n2; ++i) {
        if (rs->address_register + i > es->max_addr) {
            snprintf(chip8_err_buffer,
                     CHIP8_ERROR_BUFFER_SIZE,
                     "Instruction FX55 tried to write memory outside allowed "
                     "range at 0x%x. Max allowed=Ox%x",
                     rs->address_register + i,
                     es->max_addr);
            *err = chip8_err_buffer;
            return (1);
        }
        es->ram[rs->address_register + i] = rs->general_registers[i];
    }
    return (0);
}

int
chip8_exec_ld_read_register(emu_inst_t inst, void *state, char const **err)
{
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    for (uint8_t i = 0; i <= inst.n2; ++i) {
        if (rs->address_register + i > es->max_addr) {
            snprintf(chip8_err_buffer,
                     CHIP8_ERROR_BUFFER_SIZE,
                     "Instruction FX65 tried to load memory outside allowed "
                     "range at 0x%x. Max allowed=Ox%x",
                     rs->address_register + i,
                     es->max_addr);
            *err = chip8_err_buffer;
            return (1);
        }
        rs->general_registers[i] = es->ram[rs->address_register + i];
    }
    return (0);
}