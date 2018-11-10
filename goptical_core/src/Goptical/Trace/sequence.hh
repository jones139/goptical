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

#ifndef GOPTICAL_SEQUENCE_HH_
#define GOPTICAL_SEQUENCE_HH_

#include "Goptical/common.hh"

#include <iostream>

namespace _Goptical {

  namespace Trace {

    /**
       @short Defines light propagation elemets order for sequential light propagation
       @header Goptical/Trace/Sequence
       @module {Core}
       @main

       There are two light propagation modes in common use: sequential
       and non sequential. When using the sequential mode, light only
       interacts with sequence elements in given sequence order.

       This class will hold the user defined ordered list of elements
       used by sequential light propagation algorithm implemented in
       the @ref Tracer class.
     */
    class Sequence : public ref_base<Sequence>
    {
      friend std::ostream & operator<<(std::ostream &o, const Sequence &s);
      friend class Tracer;

    public:
      /** Create a new empty sequence */
      Sequence();

      /** Create a new sequence and insert all elements present in the
          system. This is equivalent to calling add() on empty sequence. */
      Sequence(const Sys::System &system);

      /** Add all elements from the given system. Element are sorted
          in axis order starting from left; reflecting elements do reverse
          direction. */
      void add(const Sys::System &system);

      /** Insert an element at end of sequence.
          @return position of the element in the sequence
      */
      inline unsigned int append(const Sys::Element &element);

      /** Insert an element in sequence at given position */
      inline void insert(unsigned int index, const Sys::Element &element);

      /** Remove an element from sequence */
      inline void remove(unsigned int index);

      /** Remove all elements from sequence */
      inline void clear();

      /** Get a reference to an element in sequence */
      inline const Sys::Element &get_element(unsigned int index) const;

    private:
      void add(const Sys::Container &c);

      std::vector<const_ref<Sys::Element> > _list;
    };

    std::ostream & operator<<(std::ostream &o, const Sequence &s);

  }

}

#endif

