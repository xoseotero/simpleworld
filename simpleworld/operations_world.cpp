/**
 * @file simpleworld/operations_world.cpp
 * World operation of the Simple CPU.
 *
 * begin:     Mon, 27 Aug 2007 22:45:19 +0200
 * last:      $Date$
 *
 *  Copyright (C) 2007-2008  Xosé Otero <xoseotero@gmail.com>
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

#include <boost/format.hpp>

#include <simpleworld/cpu/types.hpp>

#include "types.hpp"
#include "actionerror.hpp"
#include "simpleworld.hpp"
#include "bug.hpp"
#include "movement.hpp"
#include "operations.hpp"

#define ACTION_TIME 16          /**< The actions take 16 (24) cycles */

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

/**
 * Make a action in the world.
 * @param isa the instruction set architecture.
 * @param regs the registers.
 * @param mem the memory.
 * @param interrupt interrupt.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
CPU::Update world(CPU::ISA& isa, CPU::Memory& regs, CPU::Memory& mem,
                  CPU::Interrupt& interrupt, CPU::Instruction inst)
{
  std::map<CPU::Memory*, Bug*>::iterator iter = Bug::bugs.find(&regs);
  assert(iter != Bug::bugs.end());
  Bug* bug = (*iter).second;
  const ::SimpleWorld::DB::Environment& env = bug->world->env();
  assert(bug->is_null("action_time") or bug->action_time >= env.time);

  // a action is not done in 1 cycle, it takes ACTION_TIME cycles to be
  // finished
  if (bug->is_null("action_time")) {
    // the action begins in this cycle
    // after ACTION_TIME cycles, the action can be finished

    bug->action_time = env.time + ACTION_TIME;
    bug->remove_null("action_time");

    bug->changed = true;

    // the PC can't be updated until the action is finished
    return CPU::UpdateNone;
  } else if (bug->action_time > env.time) {
    // the action can't be finished in this cycle

    // the PC can't be updated until the action is finished
    return CPU::UpdateNone;
  } else {
    // the action is finished in this cycle

    CPU::Word ypos;

    try {
      switch (inst.data) {
      case ACTION_NOTHING:
        bug->world->nothing(bug);
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;

      case ACTION_MYSELFID:
        regs.set_word(REGISTER(1),
                      static_cast<CPU::Word>(bug->world->myself(bug, InfoID,
                                                                NULL)));
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;
      case ACTION_MYSELFSIZE:
        regs.set_word(REGISTER(1),
                      static_cast<CPU::Word>(bug->world->myself(bug, InfoSize,
                                                                NULL)));
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;
      case ACTION_MYSELFENERGY:
        regs.set_word(REGISTER(1),
                      static_cast<CPU::Word>(bug->world->myself(bug,
                                                                InfoEnergy,
                                                                NULL)));
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;
      case ACTION_MYSELFPOSITION:
        regs.set_word(REGISTER(1), bug->world->myself(bug, InfoPosition,
                                                      &ypos));
        regs.set_word(REGISTER(2), ypos);
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;
      case ACTION_MYSELFORIENTATION:
        regs.set_word(REGISTER(1),
                      static_cast<CPU::Word>(bug->world->myself(bug,
                                                                InfoOrientation,
                                                                NULL)));
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;

      case ACTION_DETECT:
        regs.set_word(REGISTER(1),
                      static_cast<CPU::Word>(bug->world->detect(bug)));
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;

      case ACTION_INFOID:
        regs.set_word(REGISTER(1),
                      static_cast<CPU::Word>(bug->world->information(bug,
                                                                     InfoID,
                                                                     NULL)));
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;
      case ACTION_INFOSIZE:
        regs.set_word(REGISTER(1),
                      static_cast<CPU::Word>(bug->world->information(bug,
                                                                     InfoSize,
                                                                     NULL)));
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;
      case ACTION_INFOENERGY:
        regs.set_word(REGISTER(1),
                      static_cast<CPU::Word>(bug->world->information(bug,
                                                                     InfoEnergy,
                                                                     NULL)));
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;
      case ACTION_INFOPOSITION:
        regs.set_word(REGISTER(1),
                      bug->world->information(bug, InfoPosition, &ypos));
        regs.set_word(REGISTER(2), ypos);
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;
      case ACTION_INFOORIENTATION:
        regs.set_word(REGISTER(1),
                      static_cast<CPU::Word>(bug->world->information(bug,
                                                                     InfoOrientation,
                                                                     NULL)));
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;

      case ACTION_MOVEFORWARD:
        bug->world->move(bug, MoveForward);
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;
      case ACTION_MOVEBACKWARD:
        bug->world->move(bug, MoveBackward);
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;

      case ACTION_TURNLEFT:
        bug->world->turn(bug, TurnLeft);
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;
      case ACTION_TURNRIGHT:
        bug->world->turn(bug, TurnRight);
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;

      case ACTION_ATTACK:
        bug->world->attack(bug, static_cast<Energy>(regs[REGISTER(0)]));
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;

      case ACTION_EAT:
        regs.set_word(REGISTER(1), static_cast<CPU::Word>(bug->world->eat(bug)));
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;

      case ACTION_EGG:
        bug->world->egg(bug, static_cast<Energy>(regs[REGISTER(0)]));
        regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionSuccess));
        break;

      default:
        throw EXCEPTION(ActionError, boost::str(boost::format(\
"Unknown action (%04x)")
                                                % inst.data));
      }
    } catch (const ActionError& e) {
      regs.set_word(REGISTER(0), static_cast<CPU::Word>(ActionFailure));
    }


    // the action is finished
    bug->add_null("action_time");
    bug->changed = true;


    return CPU::UpdatePC;
  }
}

}
