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

namespace simpleworld
{

/**
 * Make a action in the world.
 * @param cpu the CPU.
 * @param inst the instruction.
 * @return if the PC must be updated.
 */
cpu::Update world(cpu::CPU& cpu, cpu::Instruction inst)
{
  Bug* bug = dynamic_cast<Bug*>(&cpu);
  const db::Environment& env = bug->world->env();

  // a action is not done in 1 cycle, it takes ACTION_TIME cycles to be
  // finished
  // if action_time is NULL, then there is not a action in proccess
  // if action_time > time, then the action will be finished in the next cycles
  // if action_time == time, then the action will be finished in this cycle
  // if action_time < time, then a action was cancelled
  // a action can be cancelled if the code was mutated during the wait or if a
  // interrupt was thrown

  // return action-time to the normally
  if (bug->action_time < env.time) {
    bug->add_null("action_time");
    bug->changed = true;
  }

  // check the state of the action
  if (bug->is_null("action_time")) {
    // the action begins in this cycle
    // after ACTION_TIME cycles, the action can be finished

    bug->action_time = env.time + ACTION_TIME;
    bug->remove_null("action_time");

    bug->changed = true;

    // the PC can't be updated until the action is finished
    return cpu::UpdateNone;
  } else if (bug->action_time > env.time) {
    // the action can't be finished in this cycle

    // the PC can't be updated until the action is finished
    return cpu::UpdateNone;
  } else {
    // the action is finished in this cycle

    cpu::Word ypos;

    try {
      switch (inst.data) {
      case ACTION_NOTHING:
        bug->world->nothing(bug);
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_MYSELFID:
        cpu.set_reg(1, static_cast<cpu::Word>(bug->world->myself(bug, InfoID,
                                                                 NULL)));
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_MYSELFSIZE:
        cpu.set_reg(1, static_cast<cpu::Word>(bug->world->myself(bug, InfoSize,
                                                                 NULL)));
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_MYSELFENERGY:
        cpu.set_reg(1, static_cast<cpu::Word>(bug->world->myself(bug,
                                                                 InfoEnergy,
                                                                 NULL)));
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_MYSELFPOSITION:
        cpu.set_reg(1, bug->world->myself(bug, InfoPosition, &ypos));
        cpu.set_reg(2, ypos);
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_MYSELFORIENTATION:
        cpu.set_reg(1, static_cast<cpu::Word>(bug->world->myself(bug,
                                                                 InfoOrientation,
                                                                 NULL)));
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_DETECT:
        cpu.set_reg(1, static_cast<cpu::Word>(bug->world->detect(bug)));
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_INFOID:
        cpu.set_reg(1, static_cast<cpu::Word>(bug->world->information(bug,
                                                                      InfoID,
                                                                      NULL)));
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_INFOSIZE:
        cpu.set_reg(1, static_cast<cpu::Word>(bug->world->information(bug,
                                                                      InfoSize,
                                                                      NULL)));
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_INFOENERGY:
        cpu.set_reg(1, static_cast<cpu::Word>(bug->world->information(bug,
                                                                      InfoEnergy,
                                                                      NULL)));
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_INFOPOSITION:
        cpu.set_reg(1, bug->world->information(bug, InfoPosition, &ypos));
        cpu.set_reg(2, ypos);
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_INFOORIENTATION:
        cpu.set_reg(1,
                    static_cast<cpu::Word>(bug->world->information(bug,
                                                                   InfoOrientation,
                                                                   NULL)));
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_MOVEFORWARD:
        bug->world->move(bug, MoveForward);
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_MOVEBACKWARD:
        bug->world->move(bug, MoveBackward);
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_TURNLEFT:
        bug->world->turn(bug, TurnLeft);
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_TURNRIGHT:
        bug->world->turn(bug, TurnRight);
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_ATTACK:
        bug->world->attack(bug, static_cast<Energy>(cpu.get_reg(0)));
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_EAT:
        cpu.set_reg(1, static_cast<cpu::Word>(bug->world->eat(bug)));
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_EGG:
        bug->world->egg(bug, static_cast<Energy>(cpu.get_reg(0)));
        cpu.set_reg(0, static_cast<cpu::Word>(ActionSuccess));
        break;

      default:
        throw EXCEPTION(ActionError, boost::str(boost::format(\
"Unknown action (0x%04x)")
                                                % inst.data));
      }
    } catch (const ActionError& e) {
      cpu.set_reg(0, static_cast<cpu::Word>(ActionFailure));
    }


    // the action is finished
    bug->add_null("action_time");
    bug->changed = true;


    return cpu::UpdatePC;
  }
}

}
