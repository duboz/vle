/**
 * @file vle/gvle/TreeViewConditions.hpp
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


#ifndef GUI_TREEVIEW_CONDITIONS_HPP
#define GUI_TREEVIEW_CONDITIONS_HPP

#include <gtkmm.h>
#include <vle/gvle/Modeling.hpp>

namespace vle
{
namespace gvle {

class TreeViewConditions : public Gtk::TreeView
{
public:
    TreeViewConditions(vpz::Conditions& cond, const vpz::AtomicModel& atom);
    virtual ~TreeViewConditions();

    void makeTreeView();

protected:
    //Tree model columns:
class ModelColumnsCond : public Gtk::TreeModel::ColumnRecord
    {
    public:

        ModelColumnsCond() {
            add(m_col_name);
            add(m_col_activ);
        }

        Gtk::TreeModelColumn<Glib::ustring> m_col_name;
        Gtk::TreeModelColumn<bool> m_col_activ;
    };

    virtual void on_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
    virtual bool on_button_press_event(GdkEventButton* event);

public:
    ModelColumnsCond m_Columns;

    //The Tree model:
    Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

    std::vector<std::string>* get_conditions_checked();
private:
    vpz::Conditions* mCond;
    const vpz::AtomicModel* mAtom;
    Gtk::Menu mMenu;

    void on_menu_insert();
    void on_menu_remove();
};

}
} // namespace vle gvle
#endif
