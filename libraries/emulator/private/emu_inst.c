#include "emu_inst.h"

emu_inst_addr_t
emu_inst_to_emu_inst_addr(emu_inst_t base)
{
    emu_inst_addr_t cvrt = { base.n1, base.n2 << 8 | base.n3 << 4 | base.n4 };
    return (cvrt);
}

emu_inst_reg_uint8_t
emu_inst_to_emu_inst_reg_uint8(emu_inst_t base)
{
    emu_inst_reg_uint8_t cvrt = { base.n1, base.n2, base.n3 << 4 | base.n4 };
    return (cvrt);
}
