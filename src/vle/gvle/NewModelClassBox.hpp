/**
 * @file vle/gvle/NewModelClassBox.hpp
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


#ifndef GUI_NEWMODELCLASSBOX_HPP
#define GUI_NEWMODELCLASSBOX_HPP

#include <libglademm.h>
#include <gtkmm.h>

namespace vle
{
namespace gvle {

class Modeling;

class NewModelClassBox
{
public:
    NewModelClassBox(Glib::RefPtr<Gnome::Glade::Xml> xml, Modeling* m);

    ~NewModelClassBox();

    void run();

protected:
class ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
    public:
        ModelColumns() {
            add(m_col_type);
        }

        Gtk::TreeModelColumn<Glib::ustring>  m_col_type;
    };

    ModelColumns                           mColumns;

private:
    Glib::RefPtr<Gnome::Glade::Xml>        mXml;
    Modeling*                              mModeling;

    Gtk::Dialog*                           mDialog;
    Gtk::Entry*                            mEntryClassName;
    Gtk::ComboBox*                         mComboTypeModel;
    Glib::RefPtr<Gtk::ListStore>           mRefTreeModel;
    Gtk::Entry*                            mEntryModelName;
    Gtk::Button*                           mButtonApply;

    void on_apply();
};

}
} // namespace vle gvle

#endif

