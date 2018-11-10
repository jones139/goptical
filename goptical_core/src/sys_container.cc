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

#include <Goptical/Sys/Container>
#include <Goptical/Sys/Element>
#include <Goptical/Math/VectorPair>
#include <Goptical/Math/Transform>
#include <Goptical/Io/RendererViewport>

#include <Goptical/Error>

#include <limits>

namespace _Goptical {

  namespace Sys {

    Container::Container()
      : _list()
    {
    }

    Container::~Container()
    {
      // all container elements become orphan
      GOPTICAL_FOREACH(i, _list)
        (*i)->_container = 0;
    }

    void Container::remove_all()
    {
      while (!_list.empty())
        remove(*_list.front());
    }

    void Container::add(const ref<Element> &e)
    {
      if (e->_container)
        e->_container->remove(*e);

      _list.push_back(e);

      e->_container = this;

      added(*e);
    }

    void Container::remove(Element &e)
    {
      removed(e);

      assert(e._container == this);

      e._container = 0;

      _list.remove(e);
    }

    Math::VectorPair3 Container::get_bounding_box() const
    {
      Math::Vector3 a(std::numeric_limits<double>::max());
      Math::Vector3 b(-std::numeric_limits<double>::max());

      GOPTICAL_FOREACH(i, get_element_list())
        {
          Math::VectorPair3 bi = (*i)->get_bounding_box();

          if (bi[0] == bi[1])
            continue;

          bi = (*i)->get_transform().transform_pair(bi);

          for (unsigned int j = 0; j < 3; j++)
            {
              if (bi[0][j] > bi[1][j])
                std::swap(bi[0][j], bi[1][j]);

              if (bi[0][j] < a[j])
                a[j] = bi[0][j];

              if (bi[1][j] > b[j])
                b[j] = bi[1][j];
            }
        }

      return Math::VectorPair3(a, b);
    }

    void Container::draw_2d_fit(Io::RendererViewport &r, bool keep_aspect) const
    {
      Math::VectorPair3 b = get_bounding_box();

      r.set_window(Math::VectorPair2(b, 2, 1), keep_aspect);
      r.set_camera_direction(Math::vector3_100);
      r.set_camera_position(Math::vector3_0);

      r.set_feature_size(b[1].y() - b[0].y() / 20.);
    }

    void Container::draw_2d(Io::Renderer &r) const
    {
      // optical axis
      Math::VectorPair3 b = get_bounding_box();
      r.draw_segment(Math::VectorPair2(b.z0(), 0., b.z1(), 0.),
                     Io::rgb_gray);

      if (const Element *e = dynamic_cast<const Element*>(this))
        return r.draw_element_2d(*e, e);

      GOPTICAL_FOREACH(i, get_element_list())
        r.draw_element_2d(**i, 0);
    }

    void Container::draw_3d_fit(Io::RendererViewport &r, double z_offset) const
    {
      Math::Transform<3> t;

      t.set_direction(-Math::vector3_100);
      t.translation_reset();

      Math::VectorPair3 b = get_bounding_box();
      std::swap(b[0].x(), b[0].z());
      std::swap(b[1].x(), b[1].z());

      Math::Vector3 center = (b[0] + b[1]) / 2.;
      double width = (center.x() - b[1].x()) / tan(Math::degree2rad(r.get_fov()) / 2.);
      double dist = width + (center.z() + b[0].z()) + z_offset;

      t.set_translation(t.transform(center) + Math::Vector3(dist, 0, 0));

      r.set_camera_transform(t.inverse());
      r.set_feature_size(fabs(width / 20.));
    }

    void Container::draw_3d(Io::Renderer &r) const
    {
      if (const Element *e = dynamic_cast<const Element*>(this))
        return r.draw_element_3d(*e, e);

      GOPTICAL_FOREACH(i, get_element_list())
        r.draw_element_3d(**i, 0);
    }

    std::ostream & operator<<(std::ostream &o, const Container &c)
    {
      GOPTICAL_FOREACH(i, c._list)
        o << "  " << **i << std::endl;

      return o;
    }

  }
}

