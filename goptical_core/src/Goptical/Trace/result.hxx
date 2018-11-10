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


#ifndef GOPTICAL_TRACE_RESULT_HXX_
#define GOPTICAL_TRACE_RESULT_HXX_

#include <cassert>

#include "Goptical/error.hh"
#include "Goptical/Sys/element.hxx"
#include "Goptical/Sys/surface.hxx"
#include "Goptical/Trace/ray.hxx"

namespace _Goptical {

  namespace Trace {

    Result::element_result_s & Result::get_element_result(const Sys::Element &e)
    {
      return _elements[e.id() - 1];
    }

    const Result::element_result_s & Result::get_element_result(const Sys::Element &e) const
    {
      return _elements[e.id() - 1];
    }

    const Trace::rays_queue_t & Result::get_intercepted(const Sys::Surface &s) const
    {
      const struct element_result_s &er = get_element_result(s);

      if (!er._intercepted)
        throw Error("no such ray interception surface in ray trace result");

      return *er._intercepted;
    }

    const Trace::rays_queue_t & Result::get_generated(const Sys::Element &e) const
    {
      const struct element_result_s &er = get_element_result(e);

      if (!er._generated)
        throw Error("no such ray generator element in ray trace result");

      return *er._generated;
    }

    const Trace::Result::sources_t & Result::get_source_list() const
    {
      return _sources;
    }

    void Result::add_intercepted(const Sys::Surface &s, Ray &ray)
    {
      element_result_s &er = get_element_result(s);

      if (er._intercepted)
        er._intercepted->push_back(&ray);
    }

    void Result::add_generated(const Sys::Element &s, Ray &ray)
    {
      element_result_s &er = get_element_result(s);

      if (er._generated)
        er._generated->push_back(&ray);
    }

    void Result::add_ray_wavelen(double wavelen)
    {
      _wavelengths.insert(wavelen);
    }

    const std::set<double> & Result::get_ray_wavelen_set() const
    {
      return _wavelengths;
    }

    Trace::Ray & Result::new_ray()
    {
      Trace::Ray        &r = _rays.create();

      if (_generated_queue)
        _generated_queue->push_back(&r);

      return r;
    }

    Trace::Ray & Result::new_ray(const Light::Ray &ray)
    {
      Trace::Ray        &r = _rays.create(ray);

      if (_generated_queue)
        _generated_queue->push_back(&r);

      return r;
    }

    const Params & Result::get_params() const
    {
      assert(_params != 0);
      return *_params;
    }

  }
}

#endif

