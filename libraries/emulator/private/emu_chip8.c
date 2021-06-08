#include "emu_chip8.h"

#include <stddef.h>

#include "emu_state.h"

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
 * Chip8 Inst Detection fct
 */
emu_exec_fct_t
chip8_is_sys(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x0) {
        return (chip8_exec_sys);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_cls(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x0 && inst.nibbles.n2 == 0x0 &&
        inst.nibbles.n3 == 0xE && inst.nibbles.n4 == 0x0) {
        return (chip8_exec_cls);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ret(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x0 && inst.nibbles.n2 == 0x0 &&
        inst.nibbles.n3 == 0xE && inst.nibbles.n4 == 0xE) {
        return (chip8_exec_ret);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_jp_addr(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x1) {
        return (chip8_exec_jp_addr);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_call(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x2) {
        return (chip8_exec_call);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_se_register_byte(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x3) {
        return (chip8_exec_se_register_byte);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_sne_register_byte(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x4) {
        return (chip8_exec_sne_register_byte);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_se_register_register(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x5) {
        return (chip8_exec_se_register_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_register_byte(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x6) {
        return (chip8_exec_ld_register_byte);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_add_register_byte(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x7) {
        return (chip8_exec_add_register_byte);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_register_register(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x8 && inst.nibbles.n4 == 0x0) {
        return (chip8_exec_ld_register_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_or(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x8 && inst.nibbles.n4 == 0x1) {
        return (chip8_exec_or);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_and(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x8 && inst.nibbles.n4 == 0x2) {
        return (chip8_exec_and);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_xor(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x8 && inst.nibbles.n4 == 0x3) {
        return (chip8_exec_xor);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_add_register_register(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x8 && inst.nibbles.n4 == 0x4) {
        return (chip8_exec_add_register_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_sub(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x8 && inst.nibbles.n4 == 0x5) {
        return (chip8_exec_sub);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_shr(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x8 && inst.nibbles.n4 == 0x6) {
        return (chip8_exec_shr);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_subn(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x8 && inst.nibbles.n4 == 0x7) {
        return (chip8_exec_subn);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_shl(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x8 && inst.nibbles.n4 == 0xE) {
        return (chip8_exec_shl);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_sne_register_register(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0x9 && inst.nibbles.n4 == 0x0) {
        return (chip8_exec_sne_register_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_addr_register_addr(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xA) {
        return (chip8_exec_ld_addr_register_addr);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_jp_v0_addr(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xB) {
        return (chip8_exec_jp_v0_addr);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_rnd(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xC) {
        return (chip8_exec_rnd);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_draw(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xD) {
        return (chip8_exec_draw);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_skp(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xE && inst.nibbles.n3 == 0x9 &&
        inst.nibbles.n4 == 0xE) {
        return (chip8_exec_skp);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_sknp(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xE && inst.nibbles.n3 == 0xA &&
        inst.nibbles.n4 == 0x1) {
        return (chip8_exec_sknp);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_register_delay(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xF && inst.nibbles.n3 == 0x0 &&
        inst.nibbles.n4 == 0x7) {
        return (chip8_exec_ld_register_delay);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_register_key(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xF && inst.nibbles.n3 == 0x0 &&
        inst.nibbles.n4 == 0xA) {
        return (chip8_exec_ld_register_key);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_delay_register(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xF && inst.nibbles.n3 == 0x1 &&
        inst.nibbles.n4 == 0x5) {
        return (chip8_exec_ld_delay_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_sound_register(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xF && inst.nibbles.n3 == 0x1 &&
        inst.nibbles.n4 == 0x8) {
        return (chip8_exec_ld_sound_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_add_addr_register_register(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xF && inst.nibbles.n3 == 0x1 &&
        inst.nibbles.n4 == 0xE) {
        return (chip8_exec_add_addr_register_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_font_addr_addr_register(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xF && inst.nibbles.n3 == 0x2 &&
        inst.nibbles.n4 == 0x9) {
        return (chip8_exec_ld_font_addr_addr_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_bcd_addr(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xF && inst.nibbles.n3 == 0x3 &&
        inst.nibbles.n4 == 0x3) {
        return (chip8_exec_ld_bcd_addr);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_store_register(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xF && inst.nibbles.n3 == 0x5 &&
        inst.nibbles.n4 == 0x5) {
        return (chip8_exec_ld_store_register);
    }
    return (NULL);
}

emu_exec_fct_t
chip8_is_ld_read_register(emu_inst_t inst)
{
    if (inst.nibbles.n1 == 0xF && inst.nibbles.n3 == 0x6 &&
        inst.nibbles.n4 == 0x5) {
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
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_cls(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_ret(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_jp_addr(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_call(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_se_register_byte(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_sne_register_byte(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_se_register_register(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_ld_register_byte(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_add_register_byte(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_ld_register_register(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_or(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_and(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_xor(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_add_register_register(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_sub(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_shr(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_subn(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_shl(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_sne_register_register(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_ld_addr_register_addr(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_jp_v0_addr(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_rnd(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_draw(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_skp(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_sknp(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_ld_register_delay(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_ld_register_key(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_ld_delay_register(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_ld_sound_register(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_add_addr_register_register(emu_inst_t inst,
                                      void *state,
                                      char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_ld_font_addr_addr_register(emu_inst_t inst,
                                      void *state,
                                      char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_ld_bcd_addr(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_ld_store_register(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}

int
chip8_exec_ld_read_register(emu_inst_t inst, void *state, char const **err)
{
    (void)inst;
    (void)state;
    (void)err;
    return (0);
}