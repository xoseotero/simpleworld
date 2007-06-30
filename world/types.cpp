/**
 * @file world/types.cpp
 * Types definition for the world in Simple World.
 *
 * begin:     Wed, 24 Jan 2007 00:12:37 +0100
 * last:      $Date$
 *
 *  Copyright (C) 2007, Xosé Otero <xoseotero@users.sourceforge.net>
 *
 *  License: See COPYING file that comes with this distribution
 */

#include "types.hpp"

namespace SimpleWorld
{
namespace World
{

Orientation operator !(Orientation orientation)
{
  return static_cast<Orientation>((static_cast<int>(orientation) + 2) % 4);
}

Orientation operator +(Orientation orientation, int value)
{
  return static_cast<Orientation>(std::abs((static_cast<int>(orientation) +
					    value + 4)) % 4);
}

Orientation operator -(Orientation orientation, int value)
{
  return static_cast<Orientation>(std::abs((static_cast<int>(orientation) -
					    value + 4)) % 4);
}


// /**
//  * Print the orientation.
//  * @param os Where to print the file.
//  * @param orientation Orientation to print.
//  * @return Reference to where to print.
//  */
// std::ostream& operator <<(std::ostream& os, Orientation orientation)
// {
//   static char* names[] = {"North", "East", "South", "West"};

//   return os << names[static_cast<int>(orientation)];
// }

}
}
