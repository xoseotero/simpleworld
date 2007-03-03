/**
 * @file cpu/types.h
 * Definition of the types used in the CPU.
 *
 * begin:     Thu, 01 Mar 2007 18:42:48 +0100
 * last:      $Date$ by $Author$
 *
 *  Copyright (C) 2007, Xosé Otero <xoseotero@users.sourceforge.net>
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

#ifndef __CPU_TYPES_H__
#define __CPU_TYPES_H__

#include <simple/types.h>
#include <world/types.h>
#include <cpu/types.h>

namespace SimpleWorld
{
namespace CPU
{

typedef Uint32 Word;
typedef Uint16 Address;

}
}

#endif // __CPU_TYPES_H__
