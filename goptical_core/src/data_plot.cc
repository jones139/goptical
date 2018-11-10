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

#include <limits>

#include <Goptical/Error>

#include <Goptical/Data/Plot>
#include <Goptical/Data/PlotData>
#include <Goptical/Data/SampleSet>

#include <Goptical/Io/RendererViewport>

#include <Goptical/Math/Vector>
#include <Goptical/Math/VectorPair>

namespace _Goptical {

  namespace Data {

    Plot::Plot()
      : _title(),
        _plots(),
        _axes(),
        _xy_swap(false)
    {
    }

    void Plot::set_title(const std::string & title)
    {
      _title = title;
    }

    PlotData & Plot::add_plot_data(const Set &data, const Io::Rgb &color,
                                   const std::string &label, PlotStyleMask style)
    {
      _plots.push_back(PlotData(data));

      _plots.back().set_color(color);
      _plots.back().set_label(label);
      _plots.back().set_style(style);

      return _plots.back();
    }

    void Plot::add_plot_data(PlotData &data)
    {
      _plots.push_back(data);
    }

    void Plot::erase_plot_data()
    {
      _plots.clear();
    }

    Math::range_t Plot::get_x_data_range(unsigned int dimension) const
    {
      Math::range_t r(std::numeric_limits<double>::max(),
                      std::numeric_limits<double>::min());

      GOPTICAL_FOREACH(i, _plots)
        {
          Math::range_t ri = i->get_set().get_x_range(dimension);

          if (ri.first < r.first)
            r.first = ri.first;

          if (ri.second > r.second)
            r.second = ri.second;
        }

      return r;
    }

    Math::range_t Plot::get_y_data_range() const
    {
      Math::range_t r(std::numeric_limits<double>::max(),
                      std::numeric_limits<double>::min());

      GOPTICAL_FOREACH(i, _plots)
        {
          Math::range_t ri = i->get_set().get_y_range();

          if (ri.first < r.first)
            r.first = ri.first;

          if (ri.second > r.second)
            r.second = ri.second;
        }

      return r;
    }

    unsigned int Plot::get_dimensions() const
    {
      unsigned int dimension = 0;

      GOPTICAL_FOREACH(i, _plots)
        {
          unsigned int d = i->get_set().get_dimensions();

          if (dimension == 0)
            dimension = d;
          else
            if (dimension != d)
              return 0;
        }

      return dimension;
    }

    void Plot::set_color(const Io::Rgb & color)
    {
      GOPTICAL_FOREACH(i, _plots)
        i->set_color(color);
    }

    void Plot::set_different_colors()
    {
      Io::Rgb color;
      unsigned int n = 1;

      GOPTICAL_FOREACH(i, _plots)
        {
          color.r = (double)((n >> 0) & 0x01);
          color.g = (double)((n >> 1) & 0x01);
          color.b = (double)((n >> 2) & 0x01);

          i->set_color(color);
          n++;
        }
    }

    void Plot::set_style(PlotStyleMask style)
    {
      GOPTICAL_FOREACH(i, _plots)
        i->set_style(style);
    }

    void Plot::fit_axes_range()
    {
      switch (get_dimensions())
        {
        case 1:
          _axes.set_range(get_x_data_range(0), Io::RendererAxes::X);
          _axes.set_range(get_y_data_range(), Io::RendererAxes::Y);
          break;
        case 2:
          _axes.set_range(get_x_data_range(0), Io::RendererAxes::X);
          _axes.set_range(get_x_data_range(1), Io::RendererAxes::Y);
          _axes.set_range(get_y_data_range(), Io::RendererAxes::Z);
          break;
        default:
          throw Error("inconsistent dimensions of data sets in plot");
        }
    }

    void Plot::draw(Io::RendererViewport &r)
    {
      r.draw_plot(*this);
    }

  }

}

