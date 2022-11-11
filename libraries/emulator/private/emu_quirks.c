#include "emu_quirks.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "emu_state.h"
#include "emu_fb.h"

/*
 * Error buffer
 */
#define CHIP8_QUIRK_ERROR_BUFFER_SIZE 1024
static char chip8_quirk_err_buffer[CHIP8_QUIRK_ERROR_BUFFER_SIZE];

/*
 * Chip8 Quirk Inst Detection fct
 */
emu_exec_fct_t
chip8_is_jp_v0_addr_quirk(emu_inst_t inst)
{
    if (inst.n1 == 0xB) {
        return (chip8_exec_jp_v0_addr_quirk);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_draw_wrap(emu_inst_t inst)
{
    if (inst.n1 == 0xD) {
        return (chip8_exec_draw_wrap);
    }
    return (NULL);
}

emu_exec_fct_t
superchip8_is_draw_wrap(emu_inst_t inst)
{
    if (inst.n1 == 0xD) {
        return (superchip8_exec_draw_wrap);
    }
    return (NULL);
}

emu_exec_fct_t
superchip8_is_draw_extended_wrap(emu_inst_t inst)
{
    if (inst.n1 == 0xD && inst.n4 == 0x0) {
        return (superchip8_exec_draw_extended_wrap);
    }
    return (NULL);
}

/*
 * Chip8 Quirk Inst Exec fct
 */
int
chip8_exec_jp_v0_addr_quirk(emu_inst_t inst, void *state, char const **err)
{
    /*
     * Opcode BXNN
     */
    (void)err;
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;
    emu_inst_reg_uint8_t inst_reg = emu_inst_to_emu_inst_reg_uint8(inst);
    emu_inst_addr_t inst_addr = emu_inst_to_emu_inst_addr(inst);

    rs->program_counter =
      rs->general_registers[inst_reg.gen_reg] + inst_addr.addr;
    return (0);
}

static int
exec_draw_memory_location_checks(uint32_t ram_pos,
                                 uint32_t fb_pos,
                                 uint32_t max_ram_addr,
                                 uint32_t max_fb)
{
    if (ram_pos > max_ram_addr) {
        snprintf(chip8_quirk_err_buffer,
                 CHIP8_QUIRK_ERROR_BUFFER_SIZE,
                 "Instruction DXYN tried to load memory outside allowed "
                 "range at 0x%x. Max allowed=Ox%x",
                 ram_pos,
                 max_ram_addr);
        return (1);
    }
    if (fb_pos >= max_fb) {
        snprintf(chip8_quirk_err_buffer,
                 CHIP8_QUIRK_ERROR_BUFFER_SIZE,
                 "Instruction DXYN tried to write framebuffer outside allowed "
                 "range at 0x%x. Max allowed=Ox%x",
                 fb_pos,
                 max_fb);
        return (1);
    }
    return (0);
}

int
chip8_exec_draw_wrap(emu_inst_t inst, void *state, char const **err)
{
    /*
     * Opcode DXYN
     */
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
                if (err) {
                    *err = chip8_quirk_err_buffer;
                }
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
superchip8_exec_draw_wrap(emu_inst_t inst, void *state, char const **err)
{
    /*
     * Opcode DXYN
     */
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    uint8_t size_to_copy = ((emu_inst_draw_t *)&inst)->size;
    uint8_t pos_w = rs->general_registers[((emu_inst_draw_t *)&inst)->x];
    uint8_t pos_h = rs->general_registers[((emu_inst_draw_t *)&inst)->y];
    rs->general_registers[0xF] = 0;
    for (uint8_t i = 0; i < size_to_copy; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            uint32_t write_pos_w =
              ((pos_w + j) % EMU_SUPER_CHIP_8_W) % es->current_mode_w;
            uint32_t write_pos_h =
              ((pos_h + i) % EMU_SUPER_CHIP_8_H) % es->current_mode_h;

            uint32_t fb_bit_pos =
              (write_pos_w + EMU_SUPER_CHIP_8_W * write_pos_h) / 8;
            uint32_t fb_bit_offset =
              (write_pos_w + EMU_SUPER_CHIP_8_W * write_pos_h) % 8;

            if (exec_draw_memory_location_checks(rs->address_register + i,
                                                 fb_bit_pos,
                                                 es->max_addr,
                                                 es->max_fb)) {
                if (err) {
                    *err = chip8_quirk_err_buffer;
                }
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
superchip8_exec_draw_extended_wrap(emu_inst_t inst,
                                   void *state,
                                   char const **err)
{
    /*
     * Opcode DXY0
     */
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    uint8_t size_to_copy = 16;
    uint8_t pos_w = rs->general_registers[((emu_inst_draw_t *)&inst)->x];
    uint8_t pos_h = rs->general_registers[((emu_inst_draw_t *)&inst)->y];
    rs->general_registers[0xF] = 0;
    for (uint8_t i = 0; i < size_to_copy; ++i) {
        for (uint8_t j = 0; j < 16; ++j) {
            uint32_t write_pos_w =
              ((pos_w + j) % EMU_SUPER_CHIP_8_W) % es->current_mode_w;
            uint32_t write_pos_h =
              ((pos_h + i) % EMU_SUPER_CHIP_8_H) % es->current_mode_h;

            uint32_t fb_bit_pos =
              (write_pos_w + EMU_SUPER_CHIP_8_W * write_pos_h) / 8;
            uint32_t fb_bit_offset =
              (write_pos_w + EMU_SUPER_CHIP_8_W * write_pos_h) % 8;

            if (exec_draw_memory_location_checks(rs->address_register +
                                                   +(i * 2),
                                                 fb_bit_pos,
                                                 es->max_addr,
                                                 es->max_fb)) {
                if (err) {
                    *err = chip8_quirk_err_buffer;
                }
                return (1);
            }
            uint16_t *sprite_data =
              (uint16_t *)&es->ram[rs->address_register + (i * 2)];
            uint16_t endian_sprite_data =
              (*sprite_data >> 8) | (*sprite_data << 8);
            uint16_t sprite_bit =
              (endian_sprite_data & (1 << (15 - j))) >> (15 - j);
            uint16_t fb_bit =
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
