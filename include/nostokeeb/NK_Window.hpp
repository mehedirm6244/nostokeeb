/**
 * This file is from NostoKeeb - an on screen keyboard program for X11.
 * Copyright (C) 2024-2025 Mehedi Rahman Mahi
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef NK_WINDOW_HPP
#define NK_WINDOW_HPP

#include "nostokeeb/NK_AboutDialog.hpp"

#include <filesystem>
#include <gtkmm.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <set>

class NK_Window : public Gtk::Window {
private:
  // Main layout containers
  Gtk::Box m_main_box{Gtk::ORIENTATION_VERTICAL};
  Gtk::Box m_kb_box{Gtk::ORIENTATION_VERTICAL, 5};
  Gtk::Box m_control_box{Gtk::ORIENTATION_HORIZONTAL};

  // Control buttons
  Gtk::Button m_button_drag;
  Gtk::Button m_button_close;
  Gtk::Button m_button_about;

  // About dialog window
  NK_AboutDialog m_about_dialog;

  // Window settings file
  Glib::KeyFile m_settings;
  std::filesystem::path m_settings_path;

  // Track pressed keys
  std::set<KeySym> m_pressed_keys;

  // X display
  Display* m_display;

  void handle_sig_about();

  Gtk::ToggleButton* create_key(const std::string& label, KeySym keysym);
  void create_keyboard();

  void simulate_key(KeySym keysym, bool pressed);
  void release_all_keys();
  
  void apply_css(const std::string& css_data);

public:
  NK_Window();
  virtual ~NK_Window();
};

#endif // NK_WINDOW_HPP
