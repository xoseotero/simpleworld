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

#ifndef SIMPLEWORLD_CPU_OPERATIONS_HPP
#define SIMPLEWORLD_CPU_OPERATIONS_HPP

#include <simpleworld/cpu/types.hpp>
#include <simpleworld/cpu/memory.hpp>
#include <simpleworld/cpu/interrupt.hpp>
#include <simpleworld/cpu/instruction.hpp>
#include <simpleworld/cpu/isa.hpp>

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
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update stop(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Restart the CPU (zeroing the registers).
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update restart(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
               Instruction inst);


/* Move operations: Move data from a register to other one. */
/**
 * Move the content of a register to a register.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update move(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Swap the high half-word and the low half-word of a word.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update swap(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);


/* Load operations: Move data from memory to a register. */
/**
 * Load a word from memory.
 *
 * REGISTERS[FIRST] = MEMORY[PC + OFFSET]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update load(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Load the data.
 * The higher 16 bits are set to 0.
 *
 * REGISTERS[FIRST] = DATA (the upper 32bits are cleared)
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst);

/**
 * Load the absolute address from the offset.
 *
 * REGISTERS[FIRST] = PC + OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loada(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst);

/**
 * Load a word from memory using two base registers.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + REGISTERS[DATA]]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadrr(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst);

/**
 * Load a word from memory using a base register and a offset.
 *
 * REGISTERS[FIRST] = MEMORY[REGISTERS[SECOND] + OFFSET]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update loadri(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst);


/* Store operations: Move data from a register to memory. */
/**
 * Store a word to memory.
 *
 * MEMORY[PC + OFFSET] = REGISTERS[FIRST]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update store(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst);

/**
 * Store a word to memory using two base registers.
 *
 * MEMORY[REGISTERS[FIRST] + REGISTERS[DATA]] = REGISTERS[SECOND]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storerr(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
               Instruction inst);

/**
 * Store a word to memory using a base register and a offset.
 *
 * MEMORY[REGISTERS[FIRST] + OFFSET] = REGISTERS[SECOND]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update storeri(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
               Instruction inst);


/* Stack operations: Push and pop in the stack. */
/**
 * Move the content of the register to the top of the stack.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update push(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Move the top of the stack to a register.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update pop(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);


/* Branch operations: Change the program counter (PC) if the condition is 
   true */
/**
 * Branch always.
 *
 * PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update b(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
         Instruction inst);

/**
 * Branch on equal.
 *
 * if REGISTERS[FIRST] == REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update beq(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Branch on not equal.
 *
 * if REGISTERS[FIRST] != REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bne(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Branch on less than (signed comparission).
 *
 * if REGISTERS[FIRST] < REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update blt(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Branch on less than (unsigned comparission).
 *
 * if REGISTERS[FIRST] < REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bltu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Branch on great than (signed comparission).
 *
 * if REGISTERS[FIRST] > REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgt(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Branch on great than (unsigned comparission).
 *
 * if REGISTERS[FIRST] > REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgtu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Branch on less or equal (signed comparission).
 *
 * if REGISTERS[FIRST] <= REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update ble(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Branch on less or equal (unsigned comparission).
 *
 * if REGISTERS[FIRST] <= REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bleu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Branch on greater or equal (signed comparission).
 *
 * if REGISTERS[FIRST] >= REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bge(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Branch on greater or equal (unsigned comparission).
 *
 * if REGISTERS[FIRST] >= REGISTERS[SECOND] -> PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update bgeu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);


/* Function operations: call and return from a function. */
/**
 * Call a function.
 *
 * PUSH(PC) and PC += OFFSET
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update call(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Software interrupt.
 *
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update interrupt(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
                 Instruction inst);

/**
 * Return.
 *
 * POP(PC)
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update ret(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Return from exception.
 *
 * POP(ALL REGISTERS)
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update reti(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);


/* Arithmetic operations: execute a arithmetic operation. */
/**
 * Add two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] + REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update add(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Add a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] + ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update addi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Substract two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] - REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sub(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Substract a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] - ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update subi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Low 32bits from multiply two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multl(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst);

/**
 * Low 32bits from multiply a signed registers and the data (signed value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multli(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst);

/**
 * Low 32bits from multiply two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multlu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst);

/**
 * Low 32bits from multiply a unsigned registers and the data (unsigned value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multlui(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
               Instruction inst);

/**
 * High 32bits from multiply two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multh(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst);

/**
 * High 32bits from multiply a signed registers and the data (signed value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst);

/**
 * High 32bits from multiply two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
              Instruction inst);

/**
 * High 32bits from multiply a unsigned registers and the data (unsigned value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] * ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update multhui(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
               Instruction inst);

/**
 * Divide two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update div(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Divide a signed register and the data (signed value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Divide two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Divide a unsigned register and the data (unsigned value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] / ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update divui(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst);

/**
 * Module of two signed registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update mod(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Module of a signed register and the data (signed value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Module of two unsigned registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modu(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Module of a unsigned register and the data (unsigned value).
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] % ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update modui(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst);


/* Logic operations: execute a logic operation. */
/**
 * NOT of a register.
 *
 * REGISTERS[FIRST] = ~REGISTERS[SECOND]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lnot(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * OR of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] | REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lor(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * OR of a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] | ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lori(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * AND of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] & REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update land(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * AND of a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] & ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update landi(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst);

/**
 * EXOR of two registers.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] ^ REGISTERS[ADDRESS]
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lxor(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * EXOR of a register and the data.
 *
 * REGISTERS[FIRST] = REGISTERS[SECOND] ^ ADDRESS
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update lxori(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
             Instruction inst);


/* Shift operations: execute a shift operation. */
/**
 * Logic shift left.
 *
 * Move the bits X positions to the left inserting zeros on the right.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sll(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Logic shift left.
 *
 * Move the bits X positions to the left inserting zeros on the right.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update slli(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Logic shift right.
 *
 * Move the bits X positions to the right inserting zeros on the left.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srl(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Logic shift right.
 *
 * Move the bits X positions to the right inserting zeros on the left.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srli(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Arithmetic shift left.
 *
 * Same as logic shift left.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sla(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Arithmetic shift left.
 *
 * Same as logic shift left.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update slai(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Arithmetic shift right.
 *
 * Same as logic shift right but performing sign extension.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update sra(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Arithmetic shift right.
 *
 * Same as logic shift right but performing sign extension.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update srai(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
            Instruction inst);

/**
 * Rotate left.
 *
 * Same as logic shift left but inserting the removed bits on the right.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rl(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
          Instruction inst);

/**
 * Rotate left.
 *
 * Same as logic shift left but inserting the removed bits on the right.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rli(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

/**
 * Rotate right.
 *
 * Same as logic shift right but inserting the removed bits on the left.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rr(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
          Instruction inst);

/**
 * Rotate right.
 *
 * Same as logic shift right but inserting the removed bits on the left.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
Update rri(ISA& isa, Memory& regs, Memory& mem, Interrupt& interrupt,
           Instruction inst);

}
}

#endif // SIMPLEWORLD_CPU_OPERATIONS_HPP
