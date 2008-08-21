/**
 * @file vle/eov/Plugin.cpp
 * @author The VLE Development Team
 */

/*
 * VLE Environment - the multimodeling and simulation environment
 * This file is a part of the VLE environment (http://vle.univ-littoral.fr)
 * Copyright (C) 2003 - 2008 The VLE Development Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <vle/eov/Plugin.hpp>
#include <vle/eov/NetStreamReader.hpp>
#include <iostream>

namespace vle { namespace eov {

void Plugin::resize()
{
    Glib::Mutex::Lock lock(m_net->mutex());

    if (drawingSurface() and m_cairoplugin) {
        int width, height;
        drawingSurface()->get_size(width, height);
        m_cairoplugin->onSize(width, height);

        int preferredwidth, preferredheight;
        m_cairoplugin->preferredSize(preferredwidth, preferredheight);
        if (width != preferredwidth or height != preferredheight) {
            m_ctx.clear();
            m_buffer.clear();
            m_gc.clear();
        }
    }
}

void Plugin::redraw()
{
    Glib::Mutex::Lock lock(m_net->mutex());

    if (drawingSurface() and m_cairoplugin) {
        int width, height;
        m_cairoplugin->preferredSize(width, height);

        if (not m_ctx) {
            m_buffer = Gdk::Pixmap::create(drawingSurface(), width, height);
            m_gc = Gdk::GC::create(m_buffer);
            m_ctx = m_buffer->create_cairo_context();
        }

        m_ctx->save();
        m_ctx->set_source(m_cairoplugin->context()->get_target(), 0.0, 0.0);
        m_ctx->paint();
        m_ctx->restore();

        drawingSurface()->draw_drawable(m_gc, m_buffer, 0, 0, 0, 0, width, height);
    }
}

}} // namespace vle eov
