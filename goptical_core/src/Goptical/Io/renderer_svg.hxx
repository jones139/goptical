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

#ifndef GOPTICAL_RENDERER_SVG_HXX_
#define GOPTICAL_RENDERER_SVG_HXX_

#include "Goptical/Io/renderer_2d.hxx"

namespace _Goptical {

  namespace Io {

    Math::Vector2 RendererSvg::trans_pos(const Math::Vector2 &v)
    {
      return Math::Vector2(x_trans_pos(v.x()), y_trans_pos(v.y()));
    }

    double RendererSvg::y_trans_pos(double y) const
    {
      return (((y - _page[1].y()) / (_page[0].y() - _page[1].y())) * _2d_output_res.y());
    }

  }
}

#endif

