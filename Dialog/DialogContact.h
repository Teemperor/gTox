/**
    gTox a GTK-based tox-client - https://github.com/KoKuToru/gTox.git

    Copyright (C) 2014  Luca Béla Palkovics
    Copyright (C) 2014  Maurice Mohlek

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
**/
#ifndef DIALOGCONTACT_H
#define DIALOGCONTACT_H

#include <gtkmm.h>

#include "../Widget/WidgetChat.h"
#include "../Widget/WidgetContact.h"

#include "DialogChat.h"

//contact list with pinned chat
class DialogContact: public Gtk::Window {
    private:
        Gtk::Paned     m_header_paned;
        Gtk::HeaderBar m_headerbar_chat;
        Gtk::HeaderBar m_headerbar_contact;

        Gtk::Paned     m_paned;

        Gtk::Image     m_icon_attach;
        Gtk::Image     m_icon_detach;

        Gtk::Button    m_btn_xxtach;

        Gtk::Box       m_headerbar_btn_right;

        WidgetChat m_chat;
        WidgetContact m_contact;

        DialogChat     m_chat_dialog; //probably a list in the future
    public:
        DialogContact();
        ~DialogContact();

    protected:
        void detachChat();
};

#endif
