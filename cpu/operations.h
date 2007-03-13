/**
 * @file cpu/operations.h
 * Operations of the Simple CPU.
 *
 * begin:     Sat, 11 Nov 2006 19:15:19 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __CPU_OPERATIONS_H__
#define __CPU_OPERATIONS_H__

#include <cpu/types.h>
#include <cpu/memory.h>
#include <cpu/instruction.h>
#include <cpu/operation.h>

#define REGISTER(NUMBER) (NUMBER * sizeof(Word))
#define REGISTER_PC REGISTER(0xc)
#define REGISTER_SGP REGISTER(0xd)
#define REGISTER_STP REGISTER(0xe)
#define REGISTER_ETP REGISTER(0xf)

namespace SimpleWorld
{
namespace CPU
{

/* Management operations. */
/**
 * Stop the CPU.
 */
class stop: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  stop(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Restart the CPU (zeroing the registers).
 */
class restart: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  restart(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};


/* Move operations: Move data from a register to other one. */
/**
 * Move the content of a register to a register.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND]
 */
class move: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  move(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Swap the high half-word and the low half-word of a word.
 */
class swap: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  swap(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};


/* Load operations: Move data from memory to a register. */
/**
 * Load a word from memory.
 *
 * REGISTERS[FIRST] = MEMORY[ADDRESS]
 */
class load: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  load(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Load a inmediate value.
 *
 * REGISTERS[FIRST] = ADDRESS (the upper 16 bits are cleared)
 */
class loadi: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  loadi(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Load a word from memory using two base registers.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + REGISTERS[ADDRESS]]
 */
class loadrr: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  loadrr(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Load a word from memory using a base register and a inmediate value.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + ADDRESS]
 */
class loadri: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  loadri(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};


/* Store operations: Move data from a register to memory. */
/**
 * Store a word to memory.
 *
 * MEMORY[ADDRESS] = REGISTERS[FIRST]
 */
class store: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  store(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Store a word to memory using two base registers.
 *
 * MEMORY[REGISTERS[FIRST] + REGISTERS[ADDRESS]] = REGISTERS[SECOND]
 */
class storerr: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  storerr(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Store a word to memory using a base register and a inmediate value.
 *
 * MEMORY[REGISTERS[FIRST] + INMEDIATE] = REGISTERS[SECOND]
 */
class storeri: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  storeri(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};


/* Stack operations: Push and pop in the stack. */
/**
 * Move the content of the register to the top of the stack.
 */
class push: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  push(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Move the top of the stack to a register.
 */
class pop: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  pop(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};


/* Branch operations: Change the program counter (PC) if the condition is 
   true */
/**
 * Branch always.
 *
 * PC = ADDRESS
 */
class b: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  b(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Branch on equal.
 *
 * if REGISTERS[FIRST] == REGISTERS[SECOND] -> PC = ADDRESS
 */
class beq: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  beq(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Branch on not equal.
 *
 * if REGISTERS[FIRST] != REGISTERS[SECOND] -> PC = ADDRESS
 */
class bne: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  bne(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Branch on less than (signed comparission).
 *
 * if REGISTERS[FIRST] < REGISTERS[SECOND] -> PC = ADDRESS
 */
class blt: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  blt(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Branch on less than (unsigned comparission).
 *
 * if REGISTERS[FIRST] < REGISTERS[SECOND] -> PC = ADDRESS
 */
class bltu: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  bltu(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Branch on great than (signed comparission).
 *
 * if REGISTERS[FIRST] > REGISTERS[SECOND] -> PC = ADDRESS
 */
class bgt: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  bgt(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Branch on great than (unsigned comparission).
 *
 * if REGISTERS[FIRST] > REGISTERS[SECOND] -> PC = ADDRESS
 */
class bgtu: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  bgtu(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Branch on less or equal (signed comparission).
 *
 * if REGISTERS[FIRST] <= REGISTERS[SECOND] -> PC = ADDRESS
 */
class ble: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  ble(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Branch on less or equal (unsigned comparission).
 *
 * if REGISTERS[FIRST] <= REGISTERS[SECOND] -> PC = ADDRESS
 */
class bleu: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  bleu(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Branch on greater or equal (signed comparission).
 *
 * if REGISTERS[FIRST] >= REGISTERS[SECOND] -> PC = ADDRESS
 */
class bge: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  bge(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Branch on greater or equal (unsigned comparission).
 *
 * if REGISTERS[FIRST] >= REGISTERS[SECOND] -> PC = ADDRESS
 */
class bgeu: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  bgeu(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};


/* Function operations: call and return from a function. */
/**
 * Call a function.
 *
 * PUSH(PC) and PC = ADDRESS
 */
class call: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  call(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Return.
 *
 * POP(PC)
 */
class ret: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  ret(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Return from exception.
 *
 * POP(ALL REGISTERS)
 */
class rete: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  rete(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Call a World function.
 */
class world: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  world(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};


/* Arithmetic operations: execute a arithmetic operation. */
/**
 * Add two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] + REGISTERS[ADDRESS]
 */
class add: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  add(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Add a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] + ADDRESS
 */
class addi: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  addi(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Substract two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] - REGISTERS[ADDRESS]
 */
class sub: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  sub(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Substract a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] - ADDRESS
 */
class subi: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  subi(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Low 16bits from multiply two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 */
class multl: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  multl(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Low 16bits from multiply a signed registers and a signed inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 */
class multli: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  multli(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Low 16bits from multiply two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 */
class multlu: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  multlu(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Low 16bits from multiply a unsigned registers and a unsigned inmediate
 * value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 */
class multlui: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  multlui(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * High 16bits from multiply two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 */
class multh: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  multh(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * High 16bits from multiply a signed registers and a signed inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 */
class multhi: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  multhi(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * High 16bits from multiply two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 */
class multhu: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  multhu(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * High 16bits from multiply a unsigned registers and a unsigned inmediate
 * value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 */
class multhui: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  multhui(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Divide two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / REGISTERS[ADDRESS]
 */
class div: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  div(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Divide a signed register and a signed inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / ADDRESS
 */
class divi: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  divi(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Divide two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / REGISTERS[ADDRESS]
 */
class divu: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  divu(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Divide a unsigned register and a unsigned inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / ADDRESS
 */
class divui: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  divui(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Module of two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % REGISTERS[ADDRESS]
 */
class mod: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  mod(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Module of a signed register and a signed inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % ADDRESS
 */
class modi: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  modi(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Module of two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % REGISTERS[ADDRESS]
 */
class modu: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  modu(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Module of a unsigned register and a unsigned inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % ADDRESS
 */
class modui: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  modui(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};


/* Logic operations: execute a logic operation. */
/**
 * NOT of a register.
 *
 * REGISTERS[FIRST] = ~REGISTERS[SECOND]
 */
class lnot: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  lnot(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * OR of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] | REGISTERS[ADDRESS]
 */
class lor: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  lor(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * OR of a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] | ADDRESS
 */
class lori: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  lori(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * AND of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] & REGISTERS[ADDRESS]
 */
class land: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  land(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * AND of a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] & ADDRESS
 */
class landi: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  landi(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * EXOR of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] ^ REGISTERS[ADDRESS]
 */
class lxor: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  lxor(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * EXOR of a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] ^ ADDRESS
 */
class lxori: public operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  lxori(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};


/* Shift operations: execute a shift operation. */
/**
 * Logic shift left.
 *
 * Move the bits X positions to the left inserting zeros on the right.
 */
class sll: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  sll(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Logic shift left.
 *
 * Move the bits X positions to the left inserting zeros on the right.
 */
class slli: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  slli(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Logic shift right.
 *
 * Move the bits X positions to the right inserting zeros on the left.
 */
class srl: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  srl(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Logic shift right.
 *
 * Move the bits X positions to the right inserting zeros on the left.
 */
class srli: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  srli(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Arithmetic shift left.
 *
 * Same as logic shift left.
 */
class sla: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  sla(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Arithmetic shift left.
 *
 * Same as logic shift left.
 */
class slai: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  slai(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Arithmetic shift right.
 *
 * Same as logic shift right but performing sign extension.
 */
class sra: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  sra(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Arithmetic shift right.
 *
 * Same as logic shift right but performing sign extension.
 */
class srai: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  srai(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Rotate left.
 *
 * Same as logic shift left but inserting the removed bits on the right.
 */
class rl: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  rl(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Rotate left.
 *
 * Same as logic shift left but inserting the removed bits on the right.
 */
class rli: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  rli(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Rotate right.
 *
 * Same as logic shift right but inserting the removed bits on the left.
 */
class rr: public Operation
{
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  rr(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

/**
 * Rotate right.
 *
 * Same as logic shift right but inserting the removed bits on the left.
 */
class rri: public Operation
{
public:
  /**
   * Constructor.
   * @param regs the registers.
   * @param mem the memory.
   */
  rri(Memory& regs, Memory& mem);

  Update operator()(Instruction inst);
};

}
}

#endif // __CPU_OPERATIONS_H__
