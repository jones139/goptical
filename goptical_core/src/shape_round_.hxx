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

#include <cstdlib>

#include <Goptical/Trace/Distribution>
#include <Goptical/Math/Triangle>

namespace _Goptical {

  namespace Shape {


    template <class X, bool hole>
    void Round<X, hole>::get_pattern(const Math::Vector2::put_delegate_t &f,
                                     const Trace::Distribution &d,
                                     bool unobstructed) const
    {
      static const double epsilon = 1e-8;
      const double      xyr = 1.0 / X::get_xy_ratio();
      const double      tr = X::get_external_xradius() * d.get_scaling();
      const bool        obstructed = hole && !unobstructed;
      const double      hr = obstructed ? X::get_internal_xradius() * (2.0 - d.get_scaling()) : 0.0;
      unsigned int rdens = floor((double)d.get_radial_density() - (d.get_radial_density() * (hr / tr)));
      rdens = std::max(1U, rdens);
      const double      step = (tr - hr) / rdens;

      Trace::Pattern p = d.get_pattern();

      switch (p)
        {
        case Trace::MeridionalDist: {

          if (!obstructed)
            f(Math::Vector2(0, 0));

          const double bound = obstructed ? hr - epsilon : epsilon;

          for (double r = tr; r > bound; r -= step)
            {
              f(Math::Vector2(0, r * xyr));
              f(Math::Vector2(0, -r * xyr));
            }

        } break;

        case Trace::SagittalDist: {

          if (!obstructed)
            f(Math::Vector2(0, 0));

          const double bound = obstructed ? hr - epsilon : epsilon;

          for (double r = tr; r > bound; r -= step)
            {
              f(Math::Vector2(r, 0));
              f(Math::Vector2(-r, 0));
            }

        } break;

        case Trace::CrossDist: {

          if (!obstructed)
            f(Math::Vector2(0, 0));

          const double bound = obstructed ? hr - epsilon : epsilon;

          for (double r = tr; r > bound; r -= step)
            {
              f(Math::Vector2(0, r * xyr));
              f(Math::Vector2(r, 0));
              f(Math::Vector2(0, -r * xyr));
              f(Math::Vector2(-r, 0));
            }

        } break;

        case Trace::RandomDist: {

          if (!obstructed)
            f(Math::Vector2(0, 0));

          const double bound = obstructed ? hr - epsilon : epsilon;

          for (double r = tr; r > bound; r -= step)
            {
              double astep = (M_PI / 3) / ceil(r / step);

              for (double a = 0; a < 2 * M_PI - epsilon; a += astep)
                {
                  Math::Vector2 v(sin(a) * r       + (drand48() - .5) * step,
                                    cos(a) * r * xyr + (drand48() - .5) * step);
                  double h = hypot(v.x(), v.y() / xyr);
                  if (h < tr && (h > hr || unobstructed))
                    f(v);
                }
            }

        } break;

        case Trace::DefaultDist:
        case Trace::HexaPolarDist: {

          if (!obstructed)
            f(Math::Vector2(0, 0));

          const double bound = obstructed ? hr - epsilon : epsilon;

          for (double r = tr; r > bound; r -= step)
            {
              double astep = (M_PI / 3) / ceil(r / step);

              for (double a = 0; a < 2 * M_PI - epsilon; a += astep)
                f(Math::Vector2(sin(a) * r, cos(a) * r * xyr));
            }

        } break;

        default: {

          DPP_DELEGATE3_OBJ(f2, void, (const Math::Vector2 &v),
                            const Math::Vector2::put_delegate_t &, f, // _0
                            double, xyr,                              // _1
                            double, tr,                               // _2
          {
            // unobstructed pattern must be inside external radius
            if (Math::square(v.x()) + Math::square(v.y() / _1) < Math::square(_2))
              _0(v);
          });

          Base::get_pattern(f2, d, unobstructed);
          break;
        }

        }
    }

    template <class X, bool hole>
    unsigned int Round<X, hole>::get_contour_count() const
    {
      return hole ? 2 : 1;
    }

    template <class X, bool hole>
    inline double Round<X, hole>::get_radial_step(double resolution) const
    {
      const double xyr = 1.0 / X::get_xy_ratio();
      double width = xyr <= 1.
        ? X::get_external_xradius() - X::get_internal_xradius()
        : X::get_external_xradius() * xyr - X::get_internal_xradius() * xyr;

      if (resolution < width / 30.)
        resolution = width / 30.;

      if (resolution > width)
        resolution = width;

      if (hole && resolution > X::get_internal_xradius())
        resolution = X::get_internal_xradius();

      return (X::get_external_xradius() - X::get_internal_xradius())
        / ceil(width / resolution);
    }

    template <class X, bool hole>
    void Round<X, hole>::get_contour(unsigned int contour, const Math::Vector2::put_delegate_t  &f, double resolution) const
    {
      static const double epsilon = 1e-8;
      const double xyr = 1.0 / X::get_xy_ratio();
      double r;

      assert(contour < get_contour_count());

      if (hole && contour == 1)
        r = X::get_internal_xradius();
      else
        r = X::get_external_xradius();

      double astep1 = (M_PI / 3.0) / round(r / get_radial_step(resolution));

      for (double a1 = 0; a1 < 2 * M_PI - epsilon; a1 += astep1)
        f(Math::Vector2(cos(a1) * r, sin(a1) * r * xyr));
    }



    template <class X, bool hole>
    void Round<X, hole>::get_triangles(const Math::Triangle<2>::put_delegate_t  &f, double resolution) const
    {
      static const double epsilon = 1e-8;
      const double xyr = 1.0 / X::get_xy_ratio();
      const double rstep = get_radial_step(resolution);

      double astep1;
      double r;

      if (!hole)
        {
          r = rstep;
          astep1 = M_PI / 3;

          // central hexagon

          for (double a1 = 0; a1 < M_PI - epsilon; a1 += astep1)
            {
              Math::Vector2 a(cos(a1) * rstep,          sin(a1) * rstep * xyr);
              Math::Vector2 b(cos(a1 + astep1) * rstep, sin(a1 + astep1) * rstep * xyr);
              Math::Vector2 z(0, 0);

              f(Math::Triangle<2>(b, a, z));
              f(Math::Triangle<2>(-b, -a, z));
            }
        }
      else
        {
          r = X::get_internal_xradius();
          astep1 = (M_PI / 3.0) / round(r / rstep);
        }

      // hexapolar distributed triangles

      for ( ; r < X::get_external_xradius() - epsilon; r += rstep)
        {
          double astep2 = (M_PI / 3.0) / round((r + rstep) / rstep);
          double a1 = 0, a2 = 0;

          while ((a1 < M_PI - epsilon) || (a2 < M_PI - epsilon))
            {
              Math::Vector2 a(cos(a1) * r,           sin(a1) * r * xyr);
              Math::Vector2 b(cos(a2) * (r + rstep), sin(a2) * (r + rstep) * xyr);
              Math::Vector2 c;

              if (a1 + epsilon > a2)
                {
                  a2 += astep2;
                  c = Math::Vector2(cos(a2) * (r + rstep), sin(a2) * (r + rstep) * xyr);
                }
              else
                {
                  a1 += astep1;
                  c = Math::Vector2(cos(a1) * r,           sin(a1) * r * xyr);
                }

              f(Math::Triangle<2>(a, c, b));
              f(Math::Triangle<2>(-a, -c, -b));
            }

          astep1 = astep2;
        }

    }

  }

}

