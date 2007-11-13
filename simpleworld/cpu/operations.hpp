/**
 * @file simpleworld/cpu/operations.hpp
 * Operations of the Simple CPU.
 *
 * begin:     Sat, 11 Nov 2006 19:15:19 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2006-2007  Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SIMPLEWORLD_CPU_OPERATIONS_HPP__
#define __SIMPLEWORLD_CPU_OPERATIONS_HPP__

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/interrupt.hpp>
#include <simpleworld/cpu/instruction.hpp>

#define REGISTER(NUMBER) (NUMBER * sizeof(Word))
#define REGISTER_PC REGISTER(0xd)
#define REGISTER_STP REGISTER(0xe)
#define REGISTER_ITP REGISTER(0xf)

namespace SimpleWorld
{
namespace CPU
{

/* Management operations. */
/**
 * Stop the CPU.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update stop(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Restart the CPU (zeroing the registers).
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update restart(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);


/* Move operations: Move data from a register to other one. */
/**
 * Move the content of a register to a register.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update move(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Swap the high half-word and the low half-word of a word.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update swap(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);


/* Load operations: Move data from memory to a register. */
/**
 * Load a word from memory.
 *
 * REGISTERS[FIRST] = MEMORY[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update load(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Load a inmediate value.
 *
 * REGISTERS[FIRST] = ADDRESS (the upper 32bits are cleared)
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadi(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Load a word from memory using two base registers.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + REGISTERS[ADDRESS]]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadrr(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Load a word from memory using a base register and a inmediate value.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadri(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);


/* Store operations: Move data from a register to memory. */
/**
 * Store a word to memory.
 *
 * MEMORY[ADDRESS] = REGISTERS[FIRST]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update store(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Store a word to memory using two base registers.
 *
 * MEMORY[REGISTERS[FIRST] + REGISTERS[ADDRESS]] = REGISTERS[SECOND]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storerr(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Store a word to memory using a base register and a inmediate value.
 *
 * MEMORY[REGISTERS[FIRST] + INMEDIATE] = REGISTERS[SECOND]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storeri(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);


/* Stack operations: Push and pop in the stack. */
/**
 * Move the content of the register to the top of the stack.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update push(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Move the top of the stack to a register.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update pop(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);


/* Branch operations: Change the program counter (PC) if the condition is 
   true */
/**
 * Branch always.
 *
 * PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update b(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Branch on equal.
 *
 * if REGISTERS[FIRST] == REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update beq(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Branch on not equal.
 *
 * if REGISTERS[FIRST] != REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bne(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Branch on less than (signed comparission).
 *
 * if REGISTERS[FIRST] < REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update blt(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Branch on less than (unsigned comparission).
 *
 * if REGISTERS[FIRST] < REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bltu(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Branch on great than (signed comparission).
 *
 * if REGISTERS[FIRST] > REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgt(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Branch on great than (unsigned comparission).
 *
 * if REGISTERS[FIRST] > REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgtu(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Branch on less or equal (signed comparission).
 *
 * if REGISTERS[FIRST] <= REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update ble(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Branch on less or equal (unsigned comparission).
 *
 * if REGISTERS[FIRST] <= REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bleu(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Branch on greater or equal (signed comparission).
 *
 * if REGISTERS[FIRST] >= REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bge(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Branch on greater or equal (unsigned comparission).
 *
 * if REGISTERS[FIRST] >= REGISTERS[SECOND] -> PC = ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgeu(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);


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
Update call(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Software interrupt.
 *
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update interrupt(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Return.
 *
 * POP(PC)
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update ret(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Return from exception.
 *
 * POP(ALL REGISTERS)
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update reti(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);


/* Arithmetic operations: execute a arithmetic operation. */
/**
 * Add two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] + REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update add(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Add a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] + ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update addi(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Substract two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] - REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sub(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Substract a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] - ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update subi(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Low 32bits from multiply two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multl(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Low 32bits from multiply a signed registers and a signed inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multli(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Low 32bits from multiply two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multlu(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Low 32bits from multiply a unsigned registers and a unsigned inmediate
 * value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multlui(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * High 32bits from multiply two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multh(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * High 32bits from multiply a signed registers and a signed inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhi(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * High 32bits from multiply two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhu(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * High 32bits from multiply a unsigned registers and a unsigned inmediate
 * value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhui(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Divide two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update div(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Divide a signed register and a signed inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divi(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Divide two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divu(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Divide a unsigned register and a unsigned inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divui(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Module of two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update mod(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Module of a signed register and a signed inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modi(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Module of two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modu(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Module of a unsigned register and a unsigned inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modui(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);


/* Logic operations: execute a logic operation. */
/**
 * NOT of a register.
 *
 * REGISTERS[FIRST] = ~REGISTERS[SECOND]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lnot(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * OR of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] | REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lor(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * OR of a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] | ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lori(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * AND of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] & REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update land(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * AND of a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] & ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update landi(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * EXOR of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] ^ REGISTERS[ADDRESS]
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lxor(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * EXOR of a register and a inmediate value.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] ^ ADDRESS
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lxori(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);


/* Shift operations: execute a shift operation. */
/**
 * Logic shift left.
 *
 * Move the bits X positions to the left inserting zeros on the right.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sll(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Logic shift left.
 *
 * Move the bits X positions to the left inserting zeros on the right.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update slli(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Logic shift right.
 *
 * Move the bits X positions to the right inserting zeros on the left.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srl(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Logic shift right.
 *
 * Move the bits X positions to the right inserting zeros on the left.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srli(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Arithmetic shift left.
 *
 * Same as logic shift left.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sla(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Arithmetic shift left.
 *
 * Same as logic shift left.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update slai(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Arithmetic shift right.
 *
 * Same as logic shift right but performing sign extension.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sra(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Arithmetic shift right.
 *
 * Same as logic shift right but performing sign extension.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srai(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Rotate left.
 *
 * Same as logic shift left but inserting the removed bits on the right.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rl(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Rotate left.
 *
 * Same as logic shift left but inserting the removed bits on the right.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rli(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Rotate right.
 *
 * Same as logic shift right but inserting the removed bits on the left.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rr(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

/**
 * Rotate right.
 *
 * Same as logic shift right but inserting the removed bits on the left.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rri(Memory& regs, Memory& mem, Interrupt& interrupt, Instruction inst);

}
}

#endif // __SIMPLEWORLD_CPU_OPERATIONS_HPP__
