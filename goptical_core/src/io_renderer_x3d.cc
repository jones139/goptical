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

#include <fstream>

#include <Goptical/Io/RendererX3d>
#include <Goptical/Math/VectorPair>

namespace _Goptical {

  namespace Io {

    RendererX3d::RendererX3d(const Rgb &bg)
      : _filename(0),
        _xml_header(true),
        _x3d_header(true)
    {
      _out << std::fixed;
      _styles_color[StyleBackground] = bg;
      _styles_color[StyleForeground] = ~bg;
    }

    RendererX3d::RendererX3d(const char *filename, const Rgb &bg)
      : _filename(filename),
        _xml_header(true),
        _x3d_header(true)
    {
      _out << std::fixed;
      _styles_color[StyleBackground] = bg;
      _styles_color[StyleForeground] = ~bg;
    }

    RendererX3d::~RendererX3d()
    {
      flush();
    }

    void RendererX3d::write(std::ostream &s)
    {
      if (_xml_header)
        s << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;

      // FIXME set background color

      if (_x3d_header)
        {
          s << "<X3D xmlns=\"http://www.web3d.org/specifications/x3d-namespace\"" ;
          s << ">" << std::endl;
        }

      s << "<Scene>" << std::endl;
      s << _out.str();
      s << "</Scene>" << std::endl;

      if (_x3d_header)
        s << "</X3D>" << std::endl;
    }

    void RendererX3d::clear()
    {
      _out.str("");
    }

    void RendererX3d::flush()
    {
      if (_filename)
        {
          std::ofstream file(_filename);
          write(file);
        }
    }

    void RendererX3d::group_begin(const std::string &name)
    {
      _out << "<Group>\n";
    }

    void RendererX3d::group_end()
    {
      _out << "</Group>\n";
    }

    void RendererX3d::write_appearance(const Rgb &rgb, const char *type)
    {
      _out <<
        "    <Appearance>\n"
        "      <Material " << type << "=\"" << rgb.r << " " << rgb.g << " " << rgb.b << "\" />\n"
        "    </Appearance>\n";
    }

    void RendererX3d::draw_point(const Math::Vector2 &p, const Rgb &rgb, enum PointStyle s)
    {
      _out <<
        "  <Shape>\n";
      write_appearance(rgb, "emissiveColor");
      _out <<
        "    <Polypoint2D point=\"" << p.x() << " " << p.y() << "\" />\n"
        "  </Shape>\n";
    }

    void RendererX3d::draw_segment(const Math::VectorPair2 &l, const Rgb &rgb)
    {
      _out <<
        "  <Shape>\n";
      write_appearance(rgb, "emissiveColor");
      _out <<
        "    <Polyline2D lineSegments=\"" << l.x0() << " " << l.y0() << l.x1() << " " << l.y1() << "\" />\n"
        "  </Shape>\n";
    }

    void RendererX3d::draw_circle(const Math::Vector2 &v, double r, const Rgb &rgb, bool filled)
    {
      _out <<
        "  <Transform translation=\"" << v.x() << " " << v.y() << " 0\">\n"
        "    <Shape>\n";
      write_appearance(rgb, "emissiveColor");
      _out <<
        (filled ? "      <Disk2D radius=\"" : "      <Circle2D radius=\"") << r << "\" />\n"
        "    </Shape>\n"
        "  </Transform>\n";
    }

    void RendererX3d::draw_polygon(const Math::Vector2 *array, unsigned int count, const Rgb &rgb, bool filled, bool closed)
    {
      if (count < 3)
        return;

      _out <<
        "  <Shape>\n";
      write_appearance(rgb, "emissiveColor");
      _out <<
        "    <Polyline2D lineSegments=\"";

      for (unsigned int i = 0; i < count + closed; i++)
        {
          const Math::Vector2 &v = array[i];

          _out << v.x() << " " << v.y() << " ";
        }

      if (closed)
        {
          const Math::Vector2 &v0 = array[0];

          _out << v0.x() << " " << v0.y();
        }

      _out <<
        "\" />\n"
        "  </Shape>\n";
    }

    void RendererX3d::draw_point(const Math::Vector3 &p, const Rgb &rgb, enum PointStyle s)
    {
      _out <<
        "  <Shape>\n";
      write_appearance(rgb, "emissiveColor");
      _out <<
        "    <PointSet>\n"
        "      <Coordinate point=\"" << p.x() << " " << p.y() << " " << p.z() << "\" />\n"
        "    </PointSet>\n"
        "  </Shape>\n";
    }

    void RendererX3d::draw_segment(const Math::VectorPair3 &l, const Rgb &rgb)
    {
      _out <<
        "  <Shape>\n";
      write_appearance(rgb, "emissiveColor");
      _out <<
        "    <LineSet vertexCount=\"2\">\n"
        "      <Coordinate point=\"" << l.x0() << " " << l.y0() << " " << l.z0() << " "
                                   << l.x1() << " " << l.y1() << " " << l.z1() << "\" />\n"
        "    </LineSet>\n"
        "  </Shape>\n";
    }

    void RendererX3d::draw_polygon(const Math::Vector3 *array, unsigned int count, const Rgb &rgb, bool filled, bool closed)
    {
      if (count < 3)
        return;

      _out <<
        "  <Shape>\n";
      write_appearance(rgb, "emissiveColor");
      _out <<
        "    <LineSet vertexCount=\"" << count + closed << "\">\n"
        "      <Coordinate point=\"";

      for (unsigned int i = 0; i < count; i++)
        {
          const Math::Vector3 &v = array[i];

          _out << v.x() << " " << v.y() << " " << v.z() << " ";
        }

      if (closed)
        {
          const Math::Vector3 &v0 = array[0];

          _out << v0.x() << " " << v0.y() << " " << v0.z();
        }

      _out <<
        "\" />\n"
        "    </LineSet>\n"
        "  </Shape>\n";
    }

    void RendererX3d::draw_triangle(const Math::Triangle<3> &t, const Rgb &rgb)
    {
      _out <<
        "  <Shape>\n";
      write_appearance(rgb, "diffuseColor");
      _out <<
        "    <TriangleSet solid=\"false\">\n"
        "      <Coordinate point=\"";

      for (unsigned int i = 0; i < 3; i++)
        for (unsigned int j = 0; j < 3; j++)
          _out << t[i][j] << " ";

      _out <<
        "\" />\n"
        "    </TriangleSet>\n"
        "  </Shape>\n";
    }

    void RendererX3d::draw_triangle(const Math::Triangle<3> &t, const Math::Triangle<3> &g, const Rgb &rgb)
    {
      _out <<
        "  <Shape>\n";
      write_appearance(rgb, "diffuseColor");
      _out <<
        "    <TriangleSet solid=\"false\">\n"
        "      <Coordinate point=\"";

      for (unsigned int i = 0; i < 3; i++)
        for (unsigned int j = 0; j < 3; j++)
          _out << t[i][j] << " ";
#if 0
      // FIXME
      _out <<
        "\" />\n"
        "      <Normal point=\"";

      for (unsigned int i = 0; i < 3; i++)
        for (unsigned int j = 0; j < 3; j++)
          _out << g[i][j] << " ";
#endif

      _out <<
        "\" />\n"
        "    </TriangleSet>\n"
        "  </Shape>\n";
    }

    void RendererX3d::draw_text(const Math::Vector3 &pos, const Math::Vector3 &dir,                   
                                const std::string &str, TextAlignMask a, int size, const Rgb &rgb)
    {
      // FIXME
    }

    void RendererX3d::draw_text(const Math::Vector2 &pos, const Math::Vector2 &dir,                   
                                const std::string &str, TextAlignMask a, int size, const Rgb &rgb)
    {
      // FIXME
    }

  }

}

