/*

This file is from NostoKeeb - an on screen keyboard program for X11.
Copyright (C) 2024-2025 Mehedi Rahman Mahi

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef NK_WINDOW_HPP
#define NK_WINDOW_HPP

#include "nostokeeb/aboutdialog.hpp"

#include <gtkmm.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <set>

#define NK_KEY_PRESSED 1
#define NK_KEY_RELEASED 0

class NK_Window : public Gtk::Window {
private:
  Gtk::Box m_main_box{Gtk::ORIENTATION_VERTICAL};
  Gtk::Box m_kb_box{Gtk::ORIENTATION_VERTICAL, 5};
  Gtk::Box m_control_box{Gtk::ORIENTATION_HORIZONTAL};

  Gtk::Button m_button_drag;
  Gtk::Button m_button_close;
  Gtk::Button m_button_about;

  NK_AboutDialog m_about_dialog;

  std::set<KeySym> m_pressed_keys;
  Display* m_display;

  void handle_sig_about();

  void create_keys();
  void simulate_key(KeySym keysym, bool pressed);
  void release_all_keys();

public:
  NK_Window();
  virtual ~NK_Window();
  void apply_css(const std::string css_data);
};

#endif // NK_WINDOW_HPP
