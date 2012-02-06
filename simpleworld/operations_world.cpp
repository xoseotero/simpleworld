/**
 * @file simpleworld/operations_world.cpp
 * World operation of the Simple CPU.
 *
 *  Copyright (C) 2007-2012  Xosé Otero <xoseotero@gmail.com>
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
#include "cpu.hpp"
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
  Bug* bug = dynamic_cast<CPU*>(&cpu)->bug;;
  Time time = bug->world->env().time();

  // a action is not done in 1 cycle, it takes several cycles to be finished
  // if action_time is NULL, then there is not a action in proccess
  // if action_time > time, then the action will be finished in the next cycles
  // if action_time == time, then the action will be finished in this cycle
  // a action can be cancelled if the code was mutated during the wait or if a
  // interrupt was thrown

  // check the state of the action
  if (bug->is_null("action_time")) {
    // the action begins in this cycle and after some cycles the action can be
    // finished
    // every action last for a fixed number of cycles
    switch (inst.data) {
    case ACTION_NOTHING:
      bug->action_time(time + bug->world->env().time_nothing());
      break;

    case ACTION_MYSELFID:
    case ACTION_MYSELFSIZE:
    case ACTION_MYSELFENERGY:
    case ACTION_MYSELFPOSITION:
    case ACTION_MYSELFORIENTATION:
      bug->action_time(time + bug->world->env().time_myself());
      break;

    case ACTION_DETECT:
      bug->action_time(time + bug->world->env().time_detect());
      break;

    case ACTION_INFOID:
    case ACTION_INFOSIZE:
    case ACTION_INFOENERGY:
    case ACTION_INFOPOSITION:
    case ACTION_INFOORIENTATION:
      bug->action_time(time + bug->world->env().time_info());
      break;

    case ACTION_MOVEFORWARD:
    case ACTION_MOVEBACKWARD:
      bug->action_time(time + bug->world->env().time_move());
      break;

    case ACTION_TURNLEFT:
    case ACTION_TURNRIGHT:
      bug->action_time(time + bug->world->env().time_turn());
      break;

    case ACTION_ATTACK:
      bug->action_time(time + bug->world->env().time_attack());
      break;

    case ACTION_EAT:
      bug->action_time(time + bug->world->env().time_eat());
      break;

    case ACTION_EGG:
      bug->action_time(time + bug->world->env().time_egg());
      break;

    default:
      throw EXCEPTION(ActionError, boost::str(boost::format(\
"Unknown action (0x%04x)")
                                                % inst.data));
    }

    // the PC can't be updated until the action is finished
    return cpu::UpdateNone;
  } else if (bug->action_time() > time) {
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
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_MYSELFID:
        cpu.set_reg(REGISTER_G1,
                    static_cast<cpu::Word>(bug->world->myself(bug, InfoID,
                                                              NULL)));
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_MYSELFSIZE:
        cpu.set_reg(REGISTER_G1,
                    static_cast<cpu::Word>(bug->world->myself(bug, InfoSize,
                                                              NULL)));
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_MYSELFENERGY:
        cpu.set_reg(REGISTER_G1,
                    static_cast<cpu::Word>(bug->world->myself(bug, InfoEnergy,
                                                              NULL)));
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_MYSELFPOSITION:
        cpu.set_reg(REGISTER_G1, bug->world->myself(bug, InfoPosition, &ypos));
        cpu.set_reg(REGISTER_G2, ypos);
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_MYSELFORIENTATION:
        cpu.set_reg(REGISTER_G1,
                    static_cast<cpu::Word>(bug->world->myself(bug,
                                                              InfoOrientation,
                                                              NULL)));
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_DETECT:
        cpu.set_reg(REGISTER_G1,
                    static_cast<cpu::Word>(bug->world->detect(bug)));
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_INFOID:
        cpu.set_reg(REGISTER_G1,
                    static_cast<cpu::Word>(bug->world->information(bug, InfoID,
                                                                   NULL)));
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_INFOSIZE:
        cpu.set_reg(REGISTER_G1,
                    static_cast<cpu::Word>(bug->world->information(bug,
                                                                   InfoSize,
                                                                   NULL)));
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_INFOENERGY:
        cpu.set_reg(REGISTER_G1,
                    static_cast<cpu::Word>(bug->world->information(bug,
                                                                   InfoEnergy,
                                                                   NULL)));
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_INFOPOSITION:
        cpu.set_reg(REGISTER_G1, bug->world->information(bug, InfoPosition,
                                                         &ypos));
        cpu.set_reg(REGISTER_G2, ypos);
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_INFOORIENTATION:
        cpu.set_reg(REGISTER_G1,
                    static_cast<cpu::Word>(bug->world->information(bug,
                                                                   InfoOrientation,
                                                                   NULL)));
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_MOVEFORWARD:
        bug->world->move(bug, MoveForward);
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_MOVEBACKWARD:
        bug->world->move(bug, MoveBackward);
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_TURNLEFT:
        bug->world->turn(bug, TurnLeft);
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;
      case ACTION_TURNRIGHT:
        bug->world->turn(bug, TurnRight);
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_ATTACK:
        bug->world->attack(bug, static_cast<Energy>(cpu.get_reg(REGISTER_G0)));
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_EAT:
        cpu.set_reg(REGISTER_G1, static_cast<cpu::Word>(bug->world->eat(bug)));
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;

      case ACTION_EGG:
        bug->world->egg(bug, static_cast<Energy>(cpu.get_reg(REGISTER_G0)));
        cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionSuccess));
        break;

      default:
        throw EXCEPTION(ActionError, boost::str(boost::format(\
"Unknown action (0x%04x)")
                                                % inst.data));
      }
    } catch (const ActionError& e) {
      cpu.set_reg(REGISTER_G0, static_cast<cpu::Word>(ActionFailure));
    }


    // the action is finished
    bug->time_last_action(time);
    bug->set_null("action_time");

    return cpu::UpdatePC;
  }
}

}
