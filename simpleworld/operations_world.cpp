/**
 * @file simpleworld/operations_world.cpp
 * World operation of the Simple CPU.
 *
 * begin:     Mon, 27 Aug 2007 22:45:19 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007  Xosé Antón Otero Ferreira <xoseotero@gmail.com>
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

#include <cassert>

#include <simpleworld/cpu/types.hpp>

#include "types.hpp"
#include "actionerror.hpp"
#include "simpleworld.hpp"
#include "bug.hpp"
#include "movement.hpp"
#include "operations.hpp"

#define ACTION_NOTHING 0x00
#define ACTION_MYSELFID 0x10
#define ACTION_MYSELFSIZE 0x11
#define ACTION_MYSELFENERGY 0x12
#define ACTION_MYSELFPOSITION 0x13
#define ACTION_MYSELFORIENTATION 0x14
#define ACTION_DETECT 0x20
#define ACTION_INFOID 0x30
#define ACTION_INFOSIZE 0x31
#define ACTION_INFOENERGY 0x32
#define ACTION_INFOPOSITION 0x33
#define ACTION_INFOORIENTATION 0x34
#define ACTION_MOVEFORWARD 0x40
#define ACTION_MOVEBACKWARD 0x41
#define ACTION_TURNLEFT 0x42
#define ACTION_TURNRIGHT 0x43
#define ACTION_ATTACK 0x50
#define ACTION_EAT 0x60
#define ACTION_EGG 0x70

namespace SimpleWorld
{

CPU::Update world(CPU::ISA& isa, CPU::Memory& regs, CPU::Memory& mem,
                  CPU::Interrupt& interrupt, CPU::Instruction inst)
{
  std::map<CPU::Memory*, Bug*>::iterator iter = Bug::bugs.find(&regs);
  assert(iter != Bug::bugs.end());
  Bug* bug = (*iter).second;

  CPU::Word ypos;

  switch (inst.address) {
  case ACTION_NOTHING:
    try {
      bug->world->nothing(bug);
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;

  case ACTION_MYSELFID:
    try {
      regs.set_word(REGISTER(1),
                    static_cast<CPU::Word>(bug->world->myself(bug, InfoID,
                      NULL)));
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;
  case ACTION_MYSELFSIZE:
    try {
      regs.set_word(REGISTER(1),
                    static_cast<CPU::Word>(bug->world->myself(bug, InfoSize,
                      NULL)));
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;
  case ACTION_MYSELFENERGY:
    try {
      regs.set_word(REGISTER(1),
                    static_cast<CPU::Word>(bug->world->myself(bug, InfoEnergy,
                      NULL)));
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;
  case ACTION_MYSELFPOSITION:
    try {
      regs.set_word(REGISTER(1), bug->world->myself(bug, InfoPosition, &ypos));
      regs.set_word(REGISTER(2), ypos);
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;
  case ACTION_MYSELFORIENTATION:
    try {
      regs.set_word(REGISTER(1),
                    static_cast<CPU::Word>(bug->world->myself(bug,
                      InfoOrientation, NULL)));
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;

  case ACTION_DETECT:
    try {
      regs.set_word(REGISTER(1),
                    static_cast<CPU::Word>(bug->world->detect(bug)));
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;

  case ACTION_INFOID:
    try {
      regs.set_word(REGISTER(1),
                    static_cast<CPU::Word>(bug->world->information(bug, InfoID,
                      NULL)));
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;
  case ACTION_INFOSIZE:
    try {
      regs.set_word(REGISTER(1),
                    static_cast<CPU::Word>(bug->world->information(bug,
                      InfoSize, NULL)));
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;
  case ACTION_INFOENERGY:
    try {
      regs.set_word(REGISTER(1),
                    static_cast<CPU::Word>(bug->world->information(bug,
                      InfoEnergy, NULL)));
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;
  case ACTION_INFOPOSITION:
    try {
      regs.set_word(REGISTER(1),
                    bug->world->information(bug, InfoPosition, &ypos));
      regs.set_word(REGISTER(2), ypos);
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;
  case ACTION_INFOORIENTATION:
    try {
      regs.set_word(REGISTER(1),
                    static_cast<CPU::Word>(bug->world->information(bug,
                      InfoOrientation, NULL)));
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;

  case ACTION_MOVEFORWARD:
    try {
      bug->world->move(bug, MoveForward);
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;
  case ACTION_MOVEBACKWARD:
    try {
      bug->world->move(bug, MoveBackward);
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;

  case ACTION_TURNLEFT:
    try {
      bug->world->turn(bug, TurnLeft);
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;
  case ACTION_TURNRIGHT:
    try {
      bug->world->turn(bug, TurnRight);
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;

  case ACTION_ATTACK:
    try {
      bug->world->attack(bug, static_cast<Energy>(regs[REGISTER(0)]));
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;

  case ACTION_EAT:
    try {
      regs.set_word(REGISTER(1), static_cast<CPU::Word>(bug->world->eat(bug)));
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;

  case ACTION_EGG:
    try {
      bug->world->egg(bug, static_cast<Energy>(regs[REGISTER(0)]));
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }
    break;

  default:
    regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    break;
  }

  return CPU::UpdatePC;
}

}
