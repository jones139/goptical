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


#include <typeinfo>
#include <cassert>

#include <Goptical/Sys/System>
#include <Goptical/Sys/Group>
#include <Goptical/Sys/Element>

#include <Goptical/Trace/Ray>

#include <Goptical/Math/Vector>
#include <Goptical/Math/VectorPair>
#include <Goptical/Math/Quaternion>

namespace _Goptical {

  namespace Sys {

    Element::Element(const Math::VectorPair3 &plane)
      : _system(0),
        _container(0),
        _enabled(true),
        _version(0),
        _system_id(0),
        _transform()
    {
      set_local_plane(plane);
    }

    Element::~Element()
    {
      if (_container)
        _container->remove(*this);
    }

    void Element::set_local_direction(const Math::Vector3 &v)
    {
      _transform.set_direction(v);
      system_moved();
    }

    Math::Vector3 Element::get_local_direction() const
    {
      return _transform.transform_linear(Math::vector3_001);
    }

    Math::Vector3 Element::get_position(const Element &e) const
    {
      return _system->get_transform(*this, e).transform(Math::vector3_0);
    }

    Math::Vector3 Element::get_direction(const Element &e) const
    {
      return _system->get_transform(*this, e).transform_linear(Math::vector3_001);
    }

    Math::Vector3 Element::get_position() const
    {
      return _system->get_global_transform(*this).transform(Math::vector3_0);
    }

    void Element::set_position(const Math::Vector3 &v)
    {
      if (Element *parent = get_parent())
        set_local_position(_system->get_local_transform(*parent).transform(v));
      else
        set_local_position(v);
    }

    Math::Vector3 Element::get_direction() const
    {
      return _system->get_global_transform(*this).transform_linear(Math::vector3_001);
    }

    void Element::set_direction(const Math::Vector3 &v)
    {
      if (Element *parent = get_parent())
        set_local_direction(_system->get_local_transform(*parent).transform_linear(v));
      else
        set_local_direction(v);
    }

    Math::VectorPair3 Element::get_plane() const
    {
      const Math::Transform<3> &t = _system->get_global_transform(*this);

      return Math::VectorPair3(t.transform(Math::vector3_0),
                         t.transform_linear(Math::vector3_001));
    }

    Math::VectorPair3 Element::get_plane(const Element &e) const
    {
      const Math::Transform<3> &t = _system->get_transform(*this, e);

      return Math::VectorPair3(t.transform(Math::vector3_0),
                         t.transform_linear(Math::vector3_001));
    }

    void Element::set_plane(const Math::VectorPair3 &p)
    {
      if (Element *parent = get_parent())
        {
          const Math::Transform<3> &t = _system->get_local_transform(*parent);
          _transform.set_translation(t.transform(p[0]));
          set_local_direction(t.transform_linear(p[1]));
        }
      else
        {
          set_local_plane(p);
        }
      // system_moved(); called in set_local_direction
    }

    Math::VectorPair3 Element::get_local_plane() const
    {
      return Math::VectorPair3(_transform.get_translation(),
                         _transform.transform_linear(Math::vector3_001));
    }

    void Element::set_local_plane(const Math::VectorPair3 &p)
    {
      _transform.set_translation(p[0]);
      set_local_direction(p[1]);
      // system_moved(); called in set_local_direction
    }

    Math::VectorPair3 Element::get_bounding_box() const
    {
      return Math::VectorPair3(Math::vector3_0, Math::vector3_0);
    }

    const Math::Transform<3> & Element::get_transform_to(const Element &e) const
    {
      assert(_system);
      return _system->get_transform(*this, e);
    }

    const Math::Transform<3> & Element::get_transform_from(const Element &e) const
    {
      assert(_system);
      return _system->get_transform(e, *this);
    }

    const Math::Transform<3> & Element::get_transform_to(const Element *e) const
    {
      assert(_system);
      return e ? _system->get_transform(*this, *e)
        : _system->get_global_transform(*this);
    }

    const Math::Transform<3> & Element::get_transform_from(const Element *e) const
    {
      assert(_system);
      return e ? _system->get_transform(*e, *this)
        : _system->get_local_transform(*this);
    }

    const Math::Transform<3> & Element::get_global_transform() const
    {
      assert(_system);
      return _system->get_global_transform(*this);
    }

    const Math::Transform<3> & Element::get_local_transform() const
    {
      assert(_system);
      return _system->get_local_transform(*this);
    }

    Group * Element::get_parent() const
    {
      return dynamic_cast<Group*>(_container);
    }

    void Element::process_rays_simple(Trace::Result &result,
                                     Trace::rays_queue_t *input) const
    {
      throw Error("this element is not designed to process incoming light rays in simple ray trace mode");
    }

    void Element::process_rays_intensity(Trace::Result &result,
                                        Trace::rays_queue_t *input) const
    {
      throw Error("this element is not designed to process incoming light rays in intensity ray trace mode");
    }

    void Element::process_rays_polarized(Trace::Result &result,
                                        Trace::rays_queue_t *input) const
    {
      throw Error("this element is not designed to process incoming light rays in polarized ray trace mode");
    }

    void Element::system_register(System &s)
    {
      assert(!_system);
      _system = &s;
      _system_id = s.index_get(*this);
    }

    void Element::system_unregister()
    {
      assert(_system);
      _system->index_put(*this);
      _system = 0;
      _system_id = 0;
    }

    void Element::system_moved()
    {
      if (_system)
        _system->transform_cache_flush(*this);
      update_version();
    }

    void Element::update_version()
    {
      Element *e;

      for (e = this; e; e = dynamic_cast<Element *>(e->_container))
        e->_version++;

      if (_system)
        _system->update_version();
    }

    void Element::draw_2d_e(Io::Renderer &r, const Element *ref) const
    {
    }

    void Element::draw_3d_e(Io::Renderer &r, const Element *ref) const
    {
    }

    void Element::print(std::ostream &o) const
    {
      o << " [" << id() << "]" << typeid(*this).name() << " at " << get_position();
    }

  }

}

