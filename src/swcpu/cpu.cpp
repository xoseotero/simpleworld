/**
 * @file src/swcpu/cpu.cpp
 * FakeCPU subclass that shows information about the execution.
 *
 *  Copyright (C) 2010  Xosé Otero <xoseotero@gmail.com>
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

#include <iostream>

#include <boost/format.hpp>

#include <simpleworld/ints.hpp>
#include <simpleworld/cpu/instruction.hpp>
#include <simpleworld/cpu/codeerror.hpp>

#include "cpu.hpp"
namespace cpu = simpleworld::cpu;


/**
 * Constructor.
 * @param filename filename from where to load the code.
 * @exception FileAccessError problem with the file.
 */
CPU::CPU(const std::string& filename) throw ()
  : FakeCPU(&this->registers_, &this->memory_),
    cpu::Object(cpu::CPU::isa_, filename),
    registers_(sizeof(cpu::Word) * 16), memory_(filename, &last_access)
{
}

/**
 * Execute the next instruction.
 * @exception CPUStopped A stop instruction was found
 */
void CPU::next()
{
  try {
    cpu::Instruction instruction = this->fetch_instruction_();
    std::cout
      << this->decompile(instruction.encode())
      << std::endl;
  } catch (const cpu::CPUException& e) {
    std::cout << boost::str(boost::format("Instruction[0x%08X]: 0x%08X")
                            % this->registers_[ADDRESS(REGISTER_PC)]
                            % this->memory_[this->registers_[ADDRESS(REGISTER_PC)]])
              << std::endl;
  }


  FakeCPU::next();

  sw::Uint8 i = 1;
  std::vector<sw::Uint8> regs_codes = cpu::CPU::isa_.register_codes();
  std::vector<sw::Uint8>::const_iterator reg = regs_codes.begin();
  while (reg != regs_codes.end()) {
    std::cout << boost::str(boost::format("%3s = 0x%08X")
                            % cpu::CPU::isa_.register_name(*reg)
                            % this->registers_[*reg * 4]);
    if (i % 4 == 0)
      std::cout << std::endl;
    else
      std::cout << ", ";

    ++reg;
    i++;
  }

  // Show the stack (the data between the fp and the sp).
  // If the sp is 0, then (suppose) the stack is not initialized.
  // If the fp is 0, then (suppose) the code is not in a function.
  if (this->registers_[REGISTER_SP * sizeof(cpu::Word)] != 0 and
      this->registers_[REGISTER_FP * sizeof(cpu::Word)] != 0 and
      this->registers_[REGISTER_SP * sizeof(cpu::Word)] !=
      this->registers_[REGISTER_FP * sizeof(cpu::Word)]) {
    std::cout << "Stack:" << std::endl;
    for (cpu::Address addr = this->registers_[REGISTER_SP * sizeof(cpu::Word)];
         addr > this->registers_[REGISTER_FP * sizeof(cpu::Word)];
         addr -= sizeof(cpu::Word))
      std::cout << boost::str(boost::format("0x%08X") %
                              this->memory_[addr - 4])
                << std::endl;
  }

  // Show the memory
  std::cout << "Memory:" << std::endl;
  const cpu::Address range = sizeof(cpu::Word) * 4;
  cpu::Address addr_ini = this->last_access / range * range;
  cpu::Address addr_end = addr_ini + range * 2;
  if (addr_ini >= range)
    addr_ini -= range;
  if (addr_end > this->memory_.size())
    addr_end = this->memory_.size();
  cpu::Address addr;
  for (addr = addr_ini; addr < addr_end; addr += sizeof(cpu::Word)) {
    if (addr % range == 0)
      std::cout << boost::str(boost::format("[0x%08X]\t") % addr);

    std::cout << boost::str(boost::format("0x%08X") % this->memory_[addr]);

    if (addr > 0 and (addr + sizeof(cpu::Word)) % range == 0)
      std::cout << std::endl;
    else
      std::cout << ' ';
  }
  if ((addr + sizeof(cpu::Word)) % range != 0)
    std::cout << std::endl;

  std::cout << std::endl;
}
