/*

      This file is part of the Goptical Core library.
  
      The Goptical library is free software; you can redistribute it
      and/or modify it under the terms of the GNU General Public
      License as published by the Free Software Foundation; either
      version 3 of the License, or (at your option) any later version.
  
      The Goptical library is distributed in the hope that it will be
      useful, but WITHOUT ANY WARRANTY; without even the implied
      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
      See the GNU General Public License for more details.
  
      You should have received a copy of the GNU General Public
      License along with the Goptical library; if not, write to the
      Free Software Foundation, Inc., 59 Temple Place, Suite 330,
      Boston, MA 02111-1307 USA
  
      Copyright (C) 2010-2011 Free Software Foundation, Inc
      Author: Alexandre Becoulet

*/

#ifndef GOPTICAL_GROUP_HH_
#define GOPTICAL_GROUP_HH_

#include "Goptical/common.hh"

#include "Goptical/Sys/element.hh"
#include "Goptical/Sys/container.hh"

namespace _Goptical {

  namespace Sys {

    /**
       @short Optical elements group element
       @header Goptical/Sys/Group
       @module {Core}
       @main

       This class is used to form a group of optical elements. A group
       is a special kind of optical element which can contains other
       elements. A group has its own local coordinate system.

       @xsee {tuto_group}
     */
    class Group : public Element, public Container
    {
    public:

      /** Create a new group at given position */
      inline Group(const Math::VectorPair3 &p);

      virtual ~Group();

      Math::VectorPair3 get_bounding_box() const;

    protected:
      /** @override */
      void draw_2d_e(Io::Renderer &r, const Element *ref) const;
      /** @override */
      void draw_3d_e(Io::Renderer &r, const Element *ref) const;

    private:
      void added(Element &e);
      void removed(Element &e);

      void system_register(System &s);
      void system_unregister();
      void system_moved();

    };

  }
}

#endif

