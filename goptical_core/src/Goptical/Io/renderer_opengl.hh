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


#ifndef GOPTICAL_RENDERER_OPENGL_HH_
#define GOPTICAL_RENDERER_OPENGL_HH_

extern "C" {
#include <GL/gl.h>
}

#include "Goptical/common.hh"

#include "Goptical/Io/renderer_viewport.hh"

namespace _Goptical {

  namespace Io {

    /**
       @short OpenGL rendering driver
       @header Goptical/Io/RendererOpengl
       @module {Core}
       @main

       This class implements an Opengl graphic output driver. It needs
       the opengl library to compile.
     */
    class RendererOpengl : public RendererViewport
    {
    public:

      RendererOpengl(double near, double far,
                     double width = 800, double height = 600,
                     const Rgb &background = rgb_black);

      void set_z_range(double near, double far);

      static inline void glVertex(const Math::Vector2 &v);
      static inline void glVertex(const Math::Vector3 &v);
      static inline void glNormal(const Math::Vector3 &v);
      static inline void glColor(const Rgb &rgb);
      static void apply_transform(const Math::Transform<3> &t);
      static void get_transform(GLenum name, Math::Transform<3> &t);

    private:
      /** @override */
      void clear();
      /** @override */
      void flush();

      /** @override */
      void set_2d_size(double width, double height);

      /** @override */
      void set_camera_transform(const Math::Transform<3> & t);
      /** @override */
      Math::Transform<3> get_camera_transform() const;
      /** @override */
      void set_orthographic();
      /** @override */
      void set_perspective();

      /** @override */
      void draw_point(const Math::Vector2 &p, const Rgb &rgb, enum PointStyle s);
      /** @override */
      void draw_segment(const Math::VectorPair2 &l, const Rgb &rgb);

      /** @override */
      void draw_point(const Math::Vector3 &p, const Rgb &rgb, enum PointStyle s);
      /** @override */
      void draw_segment(const Math::VectorPair3 &l, const Rgb &rgb);
      /** @override */
      void draw_polygon(const Math::Vector3 *array, unsigned int count, const Rgb &rgb, bool filled, bool closed);
      /** @override */
      void draw_triangle(const Math::Triangle<3> &t, const Rgb &rgb);
      /** @override */
      void draw_triangle(const Math::Triangle<3> &t, const Math::Triangle<3> &gradient, const Rgb &rgb);

      /** @override */
      void draw_text(const Math::Vector3 &pos, const Math::Vector3 &dir,
                     const std::string &str, TextAlignMask a, int size, const Rgb &rgb);

      /** @override */
      void draw_text(const Math::Vector2 &pos, const Math::Vector2 &dir,
                     const std::string &str, TextAlignMask a, int size, const Rgb &rgb);

      double _near, _far;
    };

  }
}

#endif

