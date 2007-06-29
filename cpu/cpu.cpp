/**
 * @file cpu/cpu.cpp
 * Central Processing Unit big endian with 16 registers of 32bits and 16bits of
 * address space.
 *
 * begin:     Mon, 06 Nov 2006 13:39:29 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2006, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#ifdef DEBUG
#include <iostream>
#include <boost/format.hpp>
#endif

#include "cpu.hpp"
#include "operations.hpp"

namespace SimpleWorld
{
namespace CPU
{

CPU::CPU(Memory* memory)
  : registers_(sizeof(Word) * 16), memory_(memory), running_(true)
{
  // 16 registers
  this->set.add_register(0x0, "r0");
  this->set.add_register(0x1, "r1");
  this->set.add_register(0x2, "r2");
  this->set.add_register(0x3, "r3");
  this->set.add_register(0x4, "r4");
  this->set.add_register(0x5, "r5");
  this->set.add_register(0x6, "r6");
  this->set.add_register(0x7, "r7");
  this->set.add_register(0x8, "r8");
  this->set.add_register(0x9, "r9");
  this->set.add_register(0xa, "r10");
  this->set.add_register(0xb, "r11");
  this->set.add_register(0xc, "r12");
  this->set.add_register(0xd, "pc");
  this->set.add_register(0xe, "stp");
  this->set.add_register(0xf, "itp");

  // Instructions
  // Management operations
  this->set.add_instruction(0x00, "stop", 0, false, stop);
  this->set.add_instruction(0x01, "restart", 0, false, restart);

  // Move operations
  this->set.add_instruction(0x08, "move", 2, false, move);
  this->set.add_instruction(0x0a, "swap", 2, false, swap);

  // Stack operations
  this->set.add_instruction(0x0c, "push", 1, false, push);
  this->set.add_instruction(0x0e, "pop", 1, false, pop);

  // Load operations
  this->set.add_instruction(0x10, "load", 1, true, load);
  this->set.add_instruction(0x11, "loadi", 1, true, loadi);
  this->set.add_instruction(0x12, "loadrr", 3, false, loadrr);
  this->set.add_instruction(0x13, "loadri", 2, true, loadri);

  // Store operations
  this->set.add_instruction(0x18, "store", 1, true, store);
  this->set.add_instruction(0x1a, "storerr", 3, false, storerr);
  this->set.add_instruction(0x1b, "storeri", 2, true, storeri);

  // Branch operations
  this->set.add_instruction(0x20, "b", 0, true, b);
  this->set.add_instruction(0x21, "beq", 2, true, beq);
  this->set.add_instruction(0x22, "bne", 2, true, bne);
  this->set.add_instruction(0x23, "blt", 2, true, blt);
  this->set.add_instruction(0x24, "bltu", 2, true, bltu);
  this->set.add_instruction(0x25, "bgt", 2, true, bgt);
  this->set.add_instruction(0x26, "bgtu", 2, true, bgtu);
  this->set.add_instruction(0x27, "ble", 2, true, ble);
  this->set.add_instruction(0x28, "bleu", 2, true, bleu);
  this->set.add_instruction(0x29, "bge", 2, true, bge);
  this->set.add_instruction(0x2a, "bgeu", 2, true, bgeu);

  // Function operations
  this->set.add_instruction(0x30, "call", 0, true, call);
  this->set.add_instruction(0x31, "int", 0, true, interrupt);
  this->set.add_instruction(0x34, "ret", 0, false, ret);
  this->set.add_instruction(0x35, "reti", 0, false, reti);

  // Arithmetic operations
  this->set.add_instruction(0x40, "add", 3, false, add);
  this->set.add_instruction(0x41, "addi", 2, true, addi);
  this->set.add_instruction(0x42, "sub", 3, false, sub);
  this->set.add_instruction(0x43, "subi", 2, true, subi);
  this->set.add_instruction(0x44, "multl", 3, false, multl);
  this->set.add_instruction(0x45, "multli", 2, true, multli);
  this->set.add_instruction(0x46, "multlu", 3, false, multlu);
  this->set.add_instruction(0x47, "multlui", 2, true, multlui);
  this->set.add_instruction(0x48, "multh", 3, false, multh);
  this->set.add_instruction(0x49, "multhi", 2, true, multhi);
  this->set.add_instruction(0x4a, "multhu", 3, false, multhu);
  this->set.add_instruction(0x4b, "multhui", 2, true, multhui);
  this->set.add_instruction(0x4c, "div", 3, false, div);
  this->set.add_instruction(0x4d, "divi", 2, true, divi);
  this->set.add_instruction(0x4e, "divu", 3, false, divu);
  this->set.add_instruction(0x4f, "divui", 2, true, divui);
  this->set.add_instruction(0x50, "mod", 3, false, mod);
  this->set.add_instruction(0x51, "modi", 2, true, modi);
  this->set.add_instruction(0x52, "modu", 3, false, modu);
  this->set.add_instruction(0x53, "modui", 2, true, modui);

  // Logic operations
  this->set.add_instruction(0x60, "not", 2, false, lnot);
  this->set.add_instruction(0x68, "or", 3, false, lor);
  this->set.add_instruction(0x69, "ori", 2, true, lori);
  this->set.add_instruction(0x6a, "and", 3, false, land);
  this->set.add_instruction(0x6b, "andi", 2, true, landi);
  this->set.add_instruction(0x6c, "xor", 3, false, lxor);
  this->set.add_instruction(0x6d, "xori", 2, true, lxori);

  // Shift operations
  this->set.add_instruction(0x70, "sll", 3, false, sll);
  this->set.add_instruction(0x71, "slli", 2, true, slli);
  this->set.add_instruction(0x72, "srl", 3, false, srl);
  this->set.add_instruction(0x73, "srli", 2, true, srli);
  this->set.add_instruction(0x74, "sla", 3, false, sla);
  this->set.add_instruction(0x75, "slai", 2, true, slai);
  this->set.add_instruction(0x76, "sra", 3, false, sra);
  this->set.add_instruction(0x77, "srai", 2, true, srai);
  this->set.add_instruction(0x78, "rl", 3, false, rl);
  this->set.add_instruction(0x79, "rli", 2, true, rli);
  this->set.add_instruction(0x7a, "rr", 3, false, rr);
  this->set.add_instruction(0x7b, "rri", 2, true, rri);
}


const InstructionSet& CPU::instruction_set() const
{
  return this->set;
}

const Word CPU::memory(Address address) const
{
  return this->memory_->get_word(address, false);
}

const Word CPU::reg(Address address) const
{
  return this->registers_.get_word(address, false);
}


void CPU::execute()
{
  try {
    while (true)
      this->next();
  }
  catch (CPUStopped) {
    // nothing
  }
}

void CPU::next()
{
  if (not this->running_)
    throw CPUStopped(__FILE__, __LINE__);

  if (this->interrupt_request_) {
#ifdef DEBUG
    std::cout << boost::str(boost::format("Interrupt: 0x%8x")
                            % this->interrupt_)
	      << std::endl;
#endif
    this->interrupt_handler();
  } else {
    Instruction instruction = this->fetch_instruction();
    InstructionInfo info = this->set.instruction_info(instruction.code);
#ifdef DEBUG
    std::cout << "Instruction info:";
    std::cout
      << boost::str(boost::format("code: %d, name: %s, nregs: %d, has_i: %d")
		    % static_cast<int>(info.code)
		    % info.name
		    % static_cast<int>(info.nregs)
		    % static_cast<int>(info.has_inmediate))
      << std::endl;
#endif

    switch (info.func(this->registers_, *this->memory_, this->interrupt_,
		      instruction)) {
    case UpdateInterrupt:
      // Thrown a interrupt
      this->interrupt_request_ = true;
    case UpdatePC:
      // Update PC
      this->registers_.set_word(REGISTER_PC, this->registers_[REGISTER_PC] + 4);
      break;
    case Stop:
      this->running_ = false;
      break;
    }
  }
}


Instruction CPU::fetch_instruction() const
{
#ifdef DEBUG
  std::cout << boost::str(boost::format("Instruction[0x%8x]: 0x%8x")
    % this->registers_[REGISTER_PC]
    % this->memory_->get_word(this->registers_[REGISTER_PC], false))
    << std::endl;
#endif
  return InstructionSet::decode(this->memory_->get_word(this->registers_[REGISTER_PC],
							false));
}


void CPU::interrupt_handler()
{
  Word itp = this->registers_[REGISTER_ITP];
  Word handler = this->memory_->get_word(this->registers_[REGISTER_ITP +
    this->interrupt_.type * sizeof(Word)]);
  if (this->interrupt_request_ and itp != 0 and handler != 0) {
    // Save all the registers in the stack
    Sint8 i;
    for (i = 0; i < 16; i++) {
      // Store a register:
      // Save the register in the top of the stack
      this->registers_.set_word(REGISTER_STP, this->registers_[REGISTER(i)]);
      // Update stack pointer
      this->registers_.set_word(REGISTER_STP,
                                this->registers_[REGISTER_STP] - 4);
    }

    // Store the information of the interrupt
    this->registers_.set_word(REGISTER(0), this->interrupt_.r0);
    this->registers_.set_word(REGISTER(1), this->interrupt_.r1);
    this->registers_.set_word(REGISTER(2), this->interrupt_.r2);

    // Update the PC with the interrupt handler location
    this->registers_.set_word(REGISTER_PC, handler);
  }
}

}
}
