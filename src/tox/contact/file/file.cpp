/**
    gTox a GTK-based tox-client - https://github.com/KoKuToru/gTox.git

    Copyright (C) 2015  Luca Béla Palkovics

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
#include "file.h"
#include "manager.h"
#include "contact/contact.h"
#include "contact/manager.h"
#include "core.h"
#include "exception.h"

using namespace toxmm;

auto file::property_uuid()          -> PropProxy<uniqueId, false> {
    return proxy<false>(this, m_property_uuid);
}

auto file::property_id()           -> PropProxy<fileId, false> {
    return proxy<false>(this, m_property_id);
}
auto file::property_nr()           -> PropProxy<fileNr, false> {
    return proxy<false>(this, m_property_nr);
}
auto file::property_kind()         -> PropProxy<TOX_FILE_KIND, false> {
    return proxy<false>(this, m_property_kind);
}
auto file::property_position()     -> PropProxy<uint64_t, false> {
    return proxy<false>(this, m_property_position);
}
auto file::property_size()         -> PropProxy<uint64_t, false> {
    return proxy<false>(this, m_property_size);
}
auto file::property_name()         -> PropProxy<Glib::ustring, false> {
    return proxy<false>(this, m_property_name);
}
auto file::property_path()         -> PropProxy<Glib::ustring, false> {
    return proxy<false>(this, m_property_path);
}
auto file::property_state()        -> PropProxy<TOX_FILE_CONTROL> {
    return proxy(this, m_property_state);
}
auto file::property_state_remote() -> PropProxy<TOX_FILE_CONTROL, false> {
    return proxy<false>(this, m_property_state_remote);
}
auto file::property_progress()     -> PropProxy<double, false> {
    return proxy<false>(this, m_property_progress);
}
auto file::property_complete()     -> PropProxy<bool, false> {
    return proxy<false>(this, m_property_complete);
}
auto file::property_active()     -> PropProxy<bool, false> {
    return proxy<false>(this, m_property_active);
}

file::file():
    Glib::ObjectBase(typeid(file)),
    m_property_uuid (*this, "file-gid"),
    m_property_id  (*this, "file-id"),
    m_property_nr  (*this, "file-nr"),
    m_property_kind(*this, "file-kind"),
    m_property_position(*this, "file-position"),
    m_property_size (*this, "file-size"),
    m_property_name (*this, "file-name"),
    m_property_path (*this, "file-path"),
    m_property_state(*this, "file-state", TOX_FILE_CONTROL_PAUSE),
    m_property_state_remote(*this, "file-state-remote", TOX_FILE_CONTROL_RESUME),
    m_property_progress(*this, "file-progress", 0.0),
    m_property_complete(*this, "file-complete", false),
    m_property_active(*this, "file-active", false) {
    //set gid
    m_property_uuid = uniqueId::create_random();
}

file::file(std::shared_ptr<toxmm::file_manager> manager):
    file() {
    //set manager
    m_file_manager = manager;
}

void file::init() {
    property_state().signal_changed().connect(sigc::track_obj([this]() {
        switch (property_state().get_value()) {
            case TOX_FILE_CONTROL_RESUME:
                resume();
                break;
            case TOX_FILE_CONTROL_CANCEL:
                abort();
                m_property_complete = true;
                m_property_active = false;
                break;
            default:
                break;
        }
        //send changes
        auto c  = core();
        auto ct = contact();
        if (!c || !ct) {
            return;
        }
        TOX_ERR_FILE_CONTROL error = TOX_ERR_FILE_CONTROL_OK;
        if (property_active().get_value()) {
            tox_file_control(c->toxcore(),
                             ct->property_nr().get_value(),
                             property_nr().get_value(),
                             property_state(),
                             &error);
        }
        if (error != TOX_ERR_FILE_CONTROL_OK &&
                error != TOX_ERR_FILE_CONTROL_FRIEND_NOT_CONNECTED &&
                error != TOX_ERR_FILE_CONTROL_NOT_FOUND &&
                error != TOX_ERR_FILE_CONTROL_NOT_PAUSED &&
                error != TOX_ERR_FILE_CONTROL_DENIED &&
                error != TOX_ERR_FILE_CONTROL_ALREADY_PAUSED) {
            throw toxmm::exception(error);
        }
    }, *this));
    property_state_remote().signal_changed().connect(sigc::track_obj([this]() {
        if (property_state_remote().get_value() ==  TOX_FILE_CONTROL_CANCEL) {
            abort();
            m_property_complete = true;
        }
    }, *this));

    auto ct = contact();
    if (ct) {
        auto update_con = [this]() {
            auto  c = core();
            auto ct = contact();
            if (!c || !ct) {
                return;
            }
            if (ct->property_connection() == TOX_CONNECTION_NONE) {
                m_property_state_remote = TOX_FILE_CONTROL_PAUSE;
                m_property_active = false;
            } else if (!property_active().get_value()) {
                if (is_recv() == false) {
                    //resend offer
                    TOX_ERR_FILE_SEND error;

                    m_property_nr = tox_file_send(
                                        c->toxcore(),
                                        ct->property_nr().get_value(),
                                        property_kind(),
                                        property_size(),
                                        property_id().get_value(),
                                        (const uint8_t*)property_name().get_value().c_str(),
                                        property_name().get_value().bytes(),
                                        &error);
                    if (error != TOX_ERR_FILE_SEND_OK) {
                        throw toxmm::exception(error);
                    }
                    m_property_state = property_state();
                    m_property_active = true;
                }
                //recv files get handled in file_manager::init signal::recv_file section
            }
        };

        ct->property_connection()
                .signal_changed()
                .connect(sigc::track_obj(update_con, *this));
        update_con();
    }

    auto position_update = [this]() {
        m_property_progress = double(property_position().get_value()) /
                              double(property_size().get_value());
    };
    property_position().signal_changed().connect(sigc::track_obj(position_update, *this));
    position_update();
}

void file::pre_send_chunk_request(uint64_t position, size_t length) {
    if (length > 0) {
        send_chunk_request(position, length);
    }
    m_property_position = position + length;
    if (length == 0) {
        //upload complete
        m_property_complete = true;
        m_property_active = false;
    }
}

void file::pre_recv_chunk(uint64_t position, const std::vector<uint8_t>& data) {
    if (data.empty()) {
        //download complete
        m_property_complete = true;
        m_property_active = false;
    } else {
        recv_chunk(position, data);
    }
    m_property_position = position + data.size();
}

std::shared_ptr<toxmm::core> file::core() {
    auto m = file_manager();
    return m ? m->core() : nullptr;
}

std::shared_ptr<toxmm::file_manager> file::file_manager() {
    return m_file_manager.lock();
}

std::shared_ptr<toxmm::contact_manager> file::contact_manager() {
    auto m = file_manager();
    return m ? m->contact_manager() : nullptr;
}

std::shared_ptr<toxmm::contact> file::contact() {
    auto m = file_manager();
    return m ? m->contact() : nullptr;
}

void file::seek(uint64_t position) {
    auto  c = core();
    auto ct = contact();
    if (c && ct) {
        TOX_ERR_FILE_SEEK error;
        tox_file_seek(c->toxcore(),
                      ct->property_nr().get_value(),
                      property_nr().get_value(),
                      position,
                      &error);
        if (error != TOX_ERR_FILE_SEEK_OK) {
            throw toxmm::exception(error);
        }
    }
}
