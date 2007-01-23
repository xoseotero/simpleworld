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

#include <cpu/memory.h>
#include <cpu/instruction.h>

#define REGISTER(NUMBER) (NUMBER * sizeof(Uint32))
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
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update stop(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Restart the CPU (zeroing the registers).
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update restart(Memory& regs, Memory& mem, Instruction inst)
   throw (AddressOutOfRange);


/* Move operations: Move data from a register to other one. */
/**
 * Move the content of a register to a register.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update move(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Swap the high half-word and the low half-word of a word.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update swap(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);


/* Load operations: Move data from memory to a register. */
/**
 * Load a word from memory.
 *
 * REGISTERS[FIRST] = MEMORY[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update load(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Load a inmediate value.
 *
 * REGISTERS[FIRST] = ADDRESS (the upper 16 bits are cleared)
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadi(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Load a word from memory using two base registers.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + REGISTERS[ADDRESS]]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadrr(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Load a word from memory using a base register and a inmediate value.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadri(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);


/* Store operations: Move data from a register to memory. */
/**
 * Store a word to memory.
 *
 * MEMORY[ADDRESS] = REGISTERS[FIRST]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update store(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Store a word to memory using two base registers.
 *
 * MEMORY[REGISTERS[FIRST] + REGISTERS[ADDRESS]] = REGISTERS[SECOND]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storerr(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Store a word to memory using a base register and a inmediate value.
 *
 * MEMORY[REGISTERS[FIRST] + INMEDIATE] = REGISTERS[SECOND]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storeri(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);


/* Stack operations: Push and pop in the stack. */
/**
 * Move the content of the register to the top of the stack.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update push(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Move the top of the stack to a register.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update pop(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);


/* Branch operations: Change the program counter (PC) if the condition is 
   true */
/**
 * Branch always.
 *
 * PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update b(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Branch on equal.
 *
 * if REGISTERS[FIRST] == REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update beq(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Branch on not equal.
 *
 * if REGISTERS[FIRST] != REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bne(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Branch on less than (signed comparission).
 *
 * if REGISTERS[FIRST] < REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update blt(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Branch on less than (unsigned comparission).
 *
 * if REGISTERS[FIRST] < REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bltu(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Branch on great than (signed comparission).
 *
 * if REGISTERS[FIRST] > REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgt(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Branch on great than (unsigned comparission).
 *
 * if REGISTERS[FIRST] > REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgtu(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Branch on less or equal (signed comparission).
 *
 * if REGISTERS[FIRST] <= REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update ble(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Branch on less or equal (unsigned comparission).
 *
 * if REGISTERS[FIRST] <= REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bleu(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Branch on greater or equal (signed comparission).
 *
 * if REGISTERS[FIRST] >= REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bge(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Branch on greater or equal (unsigned comparission).
 *
 * if REGISTERS[FIRST] >= REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgeu(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);


/* Function operations: call and return from a function. */
/**
 * Call a function.
 *
 * PUSH(PC) and PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update call(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Return.
 *
 * POP(PC)
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update ret(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Return from exception.
 *
 * POP(ALL REGISTERS)
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rete(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Call a World function.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update world(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);


/* Arithmetic operations: execute a arithmetic operation. */
/**
 * Add two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] + REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update add(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Add a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] + ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update addi(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Substract two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] - REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sub(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Substract a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] - ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update subi(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Low 16bits from multiply two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multl(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Low 16bits from multiply a signed registers and a signed inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multli(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Low 16bits from multiply two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multlu(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Low 16bits from multiply a unsigned registers and a unsigned inmediate
 * value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multlui(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * High 16bits from multiply two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multh(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * High 16bits from multiply a signed registers and a signed inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhi(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * High 16bits from multiply two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhu(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * High 16bits from multiply a unsigned registers and a unsigned inmediate
 * value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhui(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Divide two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update div(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Divide a signed register and a signed inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divi(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Divide two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divu(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Divide a unsigned register and a unsigned inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divui(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Module of two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update mod(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Module of a signed register and a signed inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modi(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Module of two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modu(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Module of a unsigned register and a unsigned inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modui(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);


/* Logic operations: execute a logic operation. */
/**
 * NOT of a register.
 *
 * REGISTERS[FIRST] = ~REGISTERS[SECOND]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lnot(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * OR of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] | REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lor(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * OR of a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] | ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lori(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * AND of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] & REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update land(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * AND of a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] & ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update landi(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * EXOR of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] ^ REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lxor(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * EXOR of a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] ^ ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lxori(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);


/* Shift operations: execute a shift operation. */
/**
 * Logic shift left.
 *
 * Move the bits X positions to the left inserting zeros on the right.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sll(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Logic shift left.
 *
 * Move the bits X positions to the left inserting zeros on the right.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update slli(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Logic shift right.
 *
 * Move the bits X positions to the right inserting zeros on the left.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srl(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Logic shift right.
 *
 * Move the bits X positions to the right inserting zeros on the left.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srli(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Arithmetic shift left.
 *
 * Same as logic shift left.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sla(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Arithmetic shift left.
 *
 * Same as logic shift left.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update slai(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Arithmetic shift right.
 *
 * Same as logic shift right but performing sign extension.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sra(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Arithmetic shift right.
 *
 * Same as logic shift right but performing sign extension.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srai(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Rotate left.
 *
 * Same as logic shift left but inserting the removed bits on the right.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rl(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Rotate left.
 *
 * Same as logic shift left but inserting the removed bits on the right.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rli(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Rotate right.
 *
 * Same as logic shift right but inserting the removed bits on the left.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rr(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

/**
 * Rotate right.
 *
 * Same as logic shift right but inserting the removed bits on the left.
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rri(Memory& regs, Memory& mem, Instruction inst)
  throw (AddressOutOfRange);

}
}

#endif // __CPU_OPERATIONS_H__
