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


#ifndef GOPTICAL_CURVE_CONIC_BASE_HXX_
#define GOPTICAL_CURVE_CONIC_BASE_HXX_

#include "Goptical/Curve/rotational.hxx"
#include "curve_roc.hxx"

namespace _Goptical {

  namespace Curve {

    ConicBase::ConicBase(double roc, double sc)
      : CurveRoc(roc), _sh(sc + 1)
    {
    }

    double ConicBase::get_eccentricity() const
    {
      return sqrt(- _sh + 1.0);
    }

    double ConicBase::get_schwarzschild() const
    {
      return _sh - 1.0;
    }

  }
}
#endif

