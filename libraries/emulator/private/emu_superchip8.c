#include "emu_superchip8.h"

/*
 * This implementation will follow SUPERCHIP8 1.1 technical reference from
 * http://devernay.free.fr/hacks/chip8/schip.txt
 */

#include <stdio.h>
#include <string.h>

#include "emu_inst.h"
#include "emu_state.h"

/*
 * emu_superchip8 variables
 */
emu_parse_fct_t g_superchip8_parse_fcts[EMU_SUPER_CHIP8_NB_INST] = {
    superchip8_is_scd,
    chip8_is_cls,
    chip8_is_ret,
    superchip8_is_scr,
    superchip8_is_scl,
    superchip8_is_exit,
    superchip8_is_low,
    superchip8_is_high,
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
    superchip8_is_draw_extended,
    superchip8_is_draw,
    chip8_is_skp,
    chip8_is_sknp,
    chip8_is_ld_register_delay,
    chip8_is_ld_register_key,
    chip8_is_ld_delay_register,
    chip8_is_ld_sound_register,
    chip8_is_add_addr_register_register,
    chip8_is_ld_font_addr_addr_register,
    superchip8_is_ld_digit_addr_addr_register,
    chip8_is_ld_bcd_addr,
    chip8_is_ld_store_register,
    chip8_is_ld_read_register,
    superchip8_is_ld_store_rpl,
    superchip8_is_ld_read_rpl,
};

/*
 * Error buffer
 */
#define SUPERCHIP8_ERROR_BUFFER_SIZE 1024
static char superchip8_err_buffer[SUPERCHIP8_ERROR_BUFFER_SIZE];

/*
 * Superchip8 Inst Detection fct
 */
emu_exec_fct_t
superchip8_is_scd(emu_inst_t inst)
{
    if (inst.n1 == 0x0 && inst.n2 == 0x0 && inst.n3 == 0xC) {
        return (superchip8_exec_scd);
    }
    return (NULL);
}

emu_exec_fct_t
superchip8_is_scr(emu_inst_t inst)
{
    if (inst.n1 == 0x0 && inst.n2 == 0x0 && inst.n3 == 0xF && inst.n4 == 0xB) {
        return (superchip8_exec_scr);
    }
    return (NULL);
}

emu_exec_fct_t
superchip8_is_scl(emu_inst_t inst)
{
    if (inst.n1 == 0x0 && inst.n2 == 0x0 && inst.n3 == 0xF && inst.n4 == 0xC) {
        return (superchip8_exec_scl);
    }
    return (NULL);
}

emu_exec_fct_t
superchip8_is_exit(emu_inst_t inst)
{
    if (inst.n1 == 0x0 && inst.n2 == 0x0 && inst.n3 == 0xF && inst.n4 == 0xD) {
        return (superchip8_exec_exit);
    }
    return (NULL);
}

emu_exec_fct_t
superchip8_is_low(emu_inst_t inst)
{
    if (inst.n1 == 0x0 && inst.n2 == 0x0 && inst.n3 == 0xF && inst.n4 == 0xE) {
        return (superchip8_exec_low);
    }
    return (NULL);
}
emu_exec_fct_t
superchip8_is_high(emu_inst_t inst)
{
    if (inst.n1 == 0x0 && inst.n2 == 0x0 && inst.n3 == 0xF && inst.n4 == 0xF) {
        return (superchip8_exec_high);
    }
    return (NULL);
}

emu_exec_fct_t
superchip8_is_draw_extended(emu_inst_t inst)
{
    if (inst.n1 == 0xD && inst.n4 == 0x0) {
        return (superchip8_exec_draw_extended);
    }
    return (NULL);
}

emu_exec_fct_t
superchip8_is_draw(emu_inst_t inst)
{
    if (inst.n1 == 0xD) {
        return (superchip8_exec_draw);
    }
    return (NULL);
}

emu_exec_fct_t
superchip8_is_ld_digit_addr_addr_register(emu_inst_t inst)
{
    if (inst.n1 == 0xF && inst.n3 == 0x3 && inst.n4 == 0x0) {
        return (superchip8_exec_ld_digit_addr_addr_register);
    }
    return (NULL);
}

emu_exec_fct_t
superchip8_is_ld_store_rpl(emu_inst_t inst)
{
    if (inst.n1 == 0xF && inst.n3 == 0x7 && inst.n4 == 0x5) {
        return (superchip8_exec_ld_store_rpl);
    }
    return (NULL);
}

emu_exec_fct_t
superchip8_is_ld_read_rpl(emu_inst_t inst)
{
    if (inst.n1 == 0xF && inst.n3 == 0x8 && inst.n4 == 0x5) {
        return (superchip8_exec_ld_read_rpl);
    }
    return (NULL);
}

/*
 * Superchip8 Inst Exec fct
 */
int
superchip8_exec_scd(emu_inst_t inst, void *state, char const **err)
{
    /*
     * Opcode 00CN
     */
    (void)inst;
    (void)err;

    emu_state_t *es = state;
    memmove(&es->framebuffer[((inst.n4 * EMU_SUPER_CHIP_8_W) / 8)],
            &es->framebuffer[0],
            EMU_SUPER_CHIP8_FRAMEBUFFER_SIZE -
              ((inst.n4 * EMU_SUPER_CHIP_8_W) / 8));
    memset(&es->framebuffer[0], 0, ((inst.n4 * EMU_SUPER_CHIP_8_W) / 8));
    return (0);
}

int
superchip8_exec_scr(emu_inst_t inst, void *state, char const **err)
{
    /*
     * Opcode 00FB
     */
    (void)inst;
    (void)err;

    emu_state_t *es = state;
    for (uint32_t i = 0; i < EMU_SUPER_CHIP8_FRAMEBUFFER_SIZE;
         i += (EMU_SUPER_CHIP_8_W / 8)) {
        uint64_t *ptr_left = (uint64_t *)&es->framebuffer[i];
        uint64_t shifted_left_part = *ptr_left
                                     << EMU_SUPER_CHIP_8_RIGHT_SCROLL_PX;
        uint64_t leftover_left =
          *ptr_left & (EMU_SUPER_CHIP_8_LATERAL_SCROLL_MASK
                       << (64 - EMU_SUPER_CHIP_8_RIGHT_SCROLL_PX));

        uint64_t *ptr_right =
          (uint64_t *)&es->framebuffer[i + (EMU_SUPER_CHIP_8_W / 16)];
        uint64_t shifted_right_part = *ptr_right
                                      << EMU_SUPER_CHIP_8_RIGHT_SCROLL_PX;

        *ptr_left = shifted_left_part;
        *ptr_right = shifted_right_part;
        *ptr_right |=
          (leftover_left >> (64 - EMU_SUPER_CHIP_8_RIGHT_SCROLL_PX));
    }
    return (0);
}

int
superchip8_exec_scl(emu_inst_t inst, void *state, char const **err)
{
    /*
     * Opcode 00FC
     */
    (void)inst;
    (void)err;

    emu_state_t *es = state;
    for (uint32_t i = 0; i < EMU_SUPER_CHIP8_FRAMEBUFFER_SIZE;
         i += (EMU_SUPER_CHIP_8_W / 8)) {
        uint64_t *ptr_left = (uint64_t *)&es->framebuffer[i];
        uint64_t shifted_left_part =
          *ptr_left >> EMU_SUPER_CHIP_8_LEFT_SCROLL_PX;

        uint64_t *ptr_right =
          (uint64_t *)&es->framebuffer[i + (EMU_SUPER_CHIP_8_W / 16)];
        uint64_t shifted_right_part =
          *ptr_right >> EMU_SUPER_CHIP_8_LEFT_SCROLL_PX;
        uint64_t leftover_right =
          *ptr_right & EMU_SUPER_CHIP_8_LATERAL_SCROLL_MASK;

        *ptr_left = shifted_left_part;
        *ptr_left |= (leftover_right << (64 - EMU_SUPER_CHIP_8_LEFT_SCROLL_PX));
        *ptr_right = shifted_right_part;
    }
    return (0);
}

int
superchip8_exec_exit(emu_inst_t inst, void *state, char const **err)
{
    /*
     * Opcode 00FD
     */
    (void)inst;
    (void)err;

    emu_state_t *es = state;
    es->should_exit = 1;
    return (0);
}

int
superchip8_exec_low(emu_inst_t inst, void *state, char const **err)
{
    /*
     * Opcode 00FE
     */
    (void)inst;
    (void)err;

    emu_state_t *es = state;
    es->res_mode = EMU_RS_CHIP8;
    es->current_mode_h = EMU_CHIP8_H;
    es->current_mode_w = EMU_CHIP8_W;
    memset(es->framebuffer, 0, EMU_FRAMEBUFFER_MAX_SIZE);
    return (0);
}

int
superchip8_exec_high(emu_inst_t inst, void *state, char const **err)
{
    /*
     * Opcode 00FF
     */
    (void)inst;
    (void)err;

    emu_state_t *es = state;
    es->res_mode = EMU_RS_SUPERCHIP8;
    es->current_mode_h = EMU_SUPER_CHIP_8_H;
    es->current_mode_w = EMU_SUPER_CHIP_8_W;
    memset(es->framebuffer, 0, EMU_FRAMEBUFFER_MAX_SIZE);
    return (0);
}

static int
exec_draw_memory_location_checks(uint32_t ram_pos,
                                 uint32_t fb_pos,
                                 uint32_t max_ram_addr,
                                 uint32_t max_fb)
{
    if (ram_pos > max_ram_addr) {
        snprintf(superchip8_err_buffer,
                 SUPERCHIP8_ERROR_BUFFER_SIZE,
                 "Instruction DXYN tried to load memory outside allowed "
                 "range at 0x%x. Max allowed=Ox%x",
                 ram_pos,
                 max_ram_addr);
        return (1);
    }
    if (fb_pos >= max_fb) {
        snprintf(superchip8_err_buffer,
                 SUPERCHIP8_ERROR_BUFFER_SIZE,
                 "Instruction DXYN tried to write framebuffer outside allowed "
                 "range at 0x%x. Max allowed=Ox%x",
                 fb_pos,
                 max_fb);
        return (1);
    }
    return (0);
}

int
superchip8_exec_draw_extended(emu_inst_t inst, void *state, char const **err)
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
            if (((((pos_w + j) / es->current_mode_w) !=
                  ((pos_w / es->current_mode_w))) ||
                 (((pos_h + i) / es->current_mode_h) !=
                  ((pos_h / es->current_mode_h))))) {
                continue;
            }
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
                    *err = superchip8_err_buffer;
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

int
superchip8_exec_draw(emu_inst_t inst, void *state, char const **err)
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
            if (((((pos_w + j) / es->current_mode_w) !=
                  ((pos_w / es->current_mode_w))) ||
                 (((pos_h + i) / es->current_mode_h) !=
                  ((pos_h / es->current_mode_h))))) {
                continue;
            }
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
                    *err = superchip8_err_buffer;
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
superchip8_exec_ld_digit_addr_addr_register(emu_inst_t inst,
                                            void *state,
                                            char const **err)
{
    /*
     * Opcode FX30
     */
    if (inst.n2 > (EMU_SUPER_CHIP_8_FONT - 1)) {
        snprintf(superchip8_err_buffer,
                 SUPERCHIP8_ERROR_BUFFER_SIZE,
                 "Instruction FX30 tried to displayed hires font"
                 " outside the allowed range: %d. Max allowed=%d",
                 inst.n2,
                 (EMU_SUPER_CHIP_8_FONT - 1));
        if (err) {
            *err = superchip8_err_buffer;
        }
        return (1);
    }
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    rs->address_register =
      rs->general_registers[((emu_inst_reg_reg_t *)&inst)->gen_reg_x] *
        EMU_SUPER_CHIP_8_FONT_HEIGHT +
      (EMU_CHIP8_NB_FONTS * EMU_CHIP_8_FONT_HEIGHT);
    return (0);
}

int
superchip8_exec_ld_store_rpl(emu_inst_t inst, void *state, char const **err)
{
    /*
     * Opcode FX75
     */
    if (inst.n2 > (EMU_NB_FLAG_REGISTERS - 1)) {
        snprintf(superchip8_err_buffer,
                 SUPERCHIP8_ERROR_BUFFER_SIZE,
                 "Instruction FX75 tried to store flag register"
                 " outside the allowed range: %d. Max allowed=%d",
                 inst.n2,
                 EMU_NB_FLAG_REGISTERS);
        if (err) {
            *err = superchip8_err_buffer;
        }
        return (1);
    }
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    for (uint8_t i = 0; i <= inst.n2; ++i) {
        rs->flag_registers[i] = rs->general_registers[i];
    }
    return (0);
}

int
superchip8_exec_ld_read_rpl(emu_inst_t inst, void *state, char const **err)
{
    /*
     * Opcode FX85
     */
    if (inst.n2 > (EMU_NB_FLAG_REGISTERS - 1)) {
        snprintf(superchip8_err_buffer,
                 SUPERCHIP8_ERROR_BUFFER_SIZE,
                 "Instruction FX85 tried to load flag register"
                 " outside the allowed range: %d. Max allowed=%d",
                 inst.n2,
                 EMU_NB_FLAG_REGISTERS);
        if (err) {
            *err = superchip8_err_buffer;
        }
        return (1);
    }
    emu_state_t *es = state;
    emu_registers_state_t *rs = &es->registers;

    for (uint8_t i = 0; i <= inst.n2; ++i) {
        rs->general_registers[i] = rs->flag_registers[i];
    }
    return (0);
}
