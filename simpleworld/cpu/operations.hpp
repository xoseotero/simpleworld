/**
 * @file simpleworld/cpu/operations.hpp
 * Operations of the Simple CPU.
 *
 * begin:     Sat, 11 Nov 2006 19:15:19 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2006-2008  Xosé Otero <xoseotero@users.sourceforge.net>
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

#ifndef SIMPLEWORLD_CPU_OPERATIONS_HPP
#define SIMPLEWORLD_CPU_OPERATIONS_HPP

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/cpu.hpp>
#include <simpleworld/cpu/instruction.hpp>

#define REGISTER_PC 0xd
#define REGISTER_SP 0xe
#define REGISTER_CS 0xf

#define INTERRUPT_TIMER (0x0)
#define INTERRUPT_SOFTWARE (0x1)
#define INTERRUPT_INSTRUCTION (0x2)
#define INTERRUPT_MEMORY (0x3)
#define INTERRUPT_DIVISION (0x4)

namespace SimpleWorld
{
namespace CPU
{

/* Management operations. */
/**
 * Stop the CPU.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update stop(CPU& cpu, Instruction inst);

/**
 * Restart the CPU (zeroing the registers).
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update restart(CPU& cpu, Instruction inst);


/* Move operations: Move data from a register to other one. */
/**
 * Move the content of a register to a register.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update move(CPU& cpu, Instruction inst);

/**
 * Swap the high half-word and the low half-word of a word.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update swap(CPU& cpu, Instruction inst);


/* Load operations: Move data from memory to a register. */
/**
 * Load a word from memory.
 *
 * REGISTERS[FIRST] = MEMORY[PC + OFFSET]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update load(CPU& cpu, Instruction inst);

/**
 * Load the data.
 * The higher 16 bits are set to 0.
 *
 * REGISTERS[FIRST] = DATA (the upper 32bits are cleared)
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadi(CPU& cpu, Instruction inst);

/**
 * Load the absolute address from the offset.
 *
 * REGISTERS[FIRST] = PC + OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loada(CPU& cpu, Instruction inst);

/**
 * Load a word from memory using two base registers.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + REGISTERS[DATA]]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadrr(CPU& cpu, Instruction inst);

/**
 * Load a word from memory using a base register and a offset.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + OFFSET]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadri(CPU& cpu, Instruction inst);


/* Store operations: Move data from a register to memory. */
/**
 * Store a word to memory.
 *
 * MEMORY[PC + OFFSET] = REGISTERS[FIRST]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update store(CPU& cpu, Instruction inst);

/**
 * Store a word to memory using two base registers.
 *
 * MEMORY[REGISTERS[FIRST] + REGISTERS[DATA]] = REGISTERS[SECOND]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storerr(CPU& cpu, Instruction inst);

/**
 * Store a word to memory using a base register and a offset.
 *
 * MEMORY[REGISTERS[FIRST] + OFFSET] = REGISTERS[SECOND]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storeri(CPU& cpu, Instruction inst);


/* Stack operations: Push and pop in the stack. */
/**
 * Move the content of the register to the top of the stack.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update push(CPU& cpu, Instruction inst);

/**
 * Move the top of the stack to a register.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update pop(CPU& cpu, Instruction inst);


/* Branch operations: Change the program counter (PC) if the condition is 
   true */
/**
 * Branch always.
 *
 * PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update b(CPU& cpu, Instruction inst);

/**
 * Branch if zero.
 *
 * if REGISTERS[FIRST] == 0 -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bz(CPU& cpu, Instruction inst);

/**
 * Branch if not zero.
 *
 * if REGISTERS[FIRST] != 0 -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bnz(CPU& cpu, Instruction inst);

/**
 * Branch on equal.
 *
 * if REGISTERS[FIRST] == REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update beq(CPU& cpu, Instruction inst);

/**
 * Branch on not equal.
 *
 * if REGISTERS[FIRST] != REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bne(CPU& cpu, Instruction inst);

/**
 * Branch on less than (signed comparission).
 *
 * if REGISTERS[FIRST] < REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update blt(CPU& cpu, Instruction inst);

/**
 * Branch on less than (unsigned comparission).
 *
 * if REGISTERS[FIRST] < REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bltu(CPU& cpu, Instruction inst);

/**
 * Branch on great than (signed comparission).
 *
 * if REGISTERS[FIRST] > REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgt(CPU& cpu, Instruction inst);

/**
 * Branch on great than (unsigned comparission).
 *
 * if REGISTERS[FIRST] > REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgtu(CPU& cpu, Instruction inst);

/**
 * Branch on less or equal (signed comparission).
 *
 * if REGISTERS[FIRST] <= REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update ble(CPU& cpu, Instruction inst);

/**
 * Branch on less or equal (unsigned comparission).
 *
 * if REGISTERS[FIRST] <= REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bleu(CPU& cpu, Instruction inst);

/**
 * Branch on greater or equal (signed comparission).
 *
 * if REGISTERS[FIRST] >= REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bge(CPU& cpu, Instruction inst);

/**
 * Branch on greater or equal (unsigned comparission).
 *
 * if REGISTERS[FIRST] >= REGISTERS[SECOND] -> PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgeu(CPU& cpu, Instruction inst);


/* Function operations: call and return from a function. */
/**
 * Call a function.
 *
 * PUSH(PC) and PC += OFFSET
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update call(CPU& cpu, Instruction inst);

/**
 * Software interrupt.
 *
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update interrupt(CPU& cpu, Instruction inst);

/**
 * Return.
 *
 * POP(PC)
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update ret(CPU& cpu, Instruction inst);

/**
 * Return from exception.
 *
 * POP(ALL REGISTERS)
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update reti(CPU& cpu, Instruction inst);


/* Arithmetic operations: execute a arithmetic operation. */
/**
 * Add two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] + REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update add(CPU& cpu, Instruction inst);

/**
 * Add a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] + ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update addi(CPU& cpu, Instruction inst);

/**
 * Substract two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] - REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sub(CPU& cpu, Instruction inst);

/**
 * Substract a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] - ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update subi(CPU& cpu, Instruction inst);

/**
 * Low 32bits from multiply two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multl(CPU& cpu, Instruction inst);

/**
 * Low 32bits from multiply a signed registers and the data (signed value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multli(CPU& cpu, Instruction inst);

/**
 * Low 32bits from multiply two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multlu(CPU& cpu, Instruction inst);

/**
 * Low 32bits from multiply a unsigned registers and the data (unsigned value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multlui(CPU& cpu, Instruction inst);

/**
 * High 32bits from multiply two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multh(CPU& cpu, Instruction inst);

/**
 * High 32bits from multiply a signed registers and the data (signed value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhi(CPU& cpu, Instruction inst);

/**
 * High 32bits from multiply two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhu(CPU& cpu, Instruction inst);

/**
 * High 32bits from multiply a unsigned registers and the data (unsigned value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhui(CPU& cpu, Instruction inst);

/**
 * Divide two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update div(CPU& cpu, Instruction inst);

/**
 * Divide a signed register and the data (signed value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divi(CPU& cpu, Instruction inst);

/**
 * Divide two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divu(CPU& cpu, Instruction inst);

/**
 * Divide a unsigned register and the data (unsigned value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divui(CPU& cpu, Instruction inst);

/**
 * Module of two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update mod(CPU& cpu, Instruction inst);

/**
 * Module of a signed register and the data (signed value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modi(CPU& cpu, Instruction inst);

/**
 * Module of two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modu(CPU& cpu, Instruction inst);

/**
 * Module of a unsigned register and the data (unsigned value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modui(CPU& cpu, Instruction inst);


/* Logic operations: execute a logic operation. */
/**
 * NOT of a register.
 *
 * REGISTERS[FIRST] = ~REGISTERS[SECOND]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lnot(CPU& cpu, Instruction inst);

/**
 * OR of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] | REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lor(CPU& cpu, Instruction inst);

/**
 * OR of a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] | ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lori(CPU& cpu, Instruction inst);

/**
 * AND of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] & REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update land(CPU& cpu, Instruction inst);

/**
 * AND of a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] & ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update landi(CPU& cpu, Instruction inst);

/**
 * EXOR of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] ^ REGISTERS[ADDRESS]
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lxor(CPU& cpu, Instruction inst);

/**
 * EXOR of a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] ^ ADDRESS
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lxori(CPU& cpu, Instruction inst);


/* Shift operations: execute a shift operation. */
/**
 * Logic shift left.
 *
 * Move the bits X positions to the left inserting zeros on the right.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sll(CPU& cpu, Instruction inst);

/**
 * Logic shift left.
 *
 * Move the bits X positions to the left inserting zeros on the right.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update slli(CPU& cpu, Instruction inst);

/**
 * Logic shift right.
 *
 * Move the bits X positions to the right inserting zeros on the left.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srl(CPU& cpu, Instruction inst);

/**
 * Logic shift right.
 *
 * Move the bits X positions to the right inserting zeros on the left.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srli(CPU& cpu, Instruction inst);

/**
 * Arithmetic shift left.
 *
 * Same as logic shift left.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sla(CPU& cpu, Instruction inst);

/**
 * Arithmetic shift left.
 *
 * Same as logic shift left.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update slai(CPU& cpu, Instruction inst);

/**
 * Arithmetic shift right.
 *
 * Same as logic shift right but performing sign extension.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sra(CPU& cpu, Instruction inst);

/**
 * Arithmetic shift right.
 *
 * Same as logic shift right but performing sign extension.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srai(CPU& cpu, Instruction inst);

/**
 * Rotate left.
 *
 * Same as logic shift left but inserting the removed bits on the right.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rl(CPU& cpu, Instruction inst);

/**
 * Rotate left.
 *
 * Same as logic shift left but inserting the removed bits on the right.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rli(CPU& cpu, Instruction inst);

/**
 * Rotate right.
 *
 * Same as logic shift right but inserting the removed bits on the left.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rr(CPU& cpu, Instruction inst);

/**
 * Rotate right.
 *
 * Same as logic shift right but inserting the removed bits on the left.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rri(CPU& cpu, Instruction inst);

}
}

#endif // SIMPLEWORLD_CPU_OPERATIONS_HPP
