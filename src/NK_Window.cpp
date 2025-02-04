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

#include "nostokeeb/NostoKeeb.hpp"
#include "nostokeeb/NK_Window.hpp"
#include "nostokeeb/NK_Style.hpp"
#include "nostokeeb/NK_Layout.hpp"

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <glibmm/keyfile.h>
#include <giomm/file.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

namespace {
  constexpr bool KEY_PRESSED = true;
  constexpr bool KEY_RELEASED = false;
  constexpr int KB_BOX_MARGIN = 8;
  constexpr const char* CLOSE_ICON = "window-close-symbolic";
  constexpr const char* ABOUT_ICON = "help-about-symbolic";
}

NK_Window::NK_Window()
  : m_button_drag(""),
    m_display(nullptr) {

  set_title(NostoKeeb::PROGRAM_NAME);
  set_decorated(false);
  set_resizable(false);
  set_keep_above(true);
  set_accept_focus(false);
  set_skip_taskbar_hint(true);
  set_skip_pager_hint(false);
  
  // Set program icon 
  try {
    set_icon(Gtk::IconTheme::get_default()->load_icon(
      NostoKeeb::PROGRAM_ICON, 128, Gtk::ICON_LOOKUP_USE_BUILTIN));
  } catch (const Gtk::IconThemeError& e) {
    std::cerr << "Icon not found: " << NostoKeeb::PROGRAM_ICON << std::endl;
  }

  // Abort if display server isn't X11 
  m_display = XOpenDisplay(nullptr);
  if (!m_display) {
    Gtk::MessageDialog dialog(*this,
      "X11 Display not available!", false, Gtk::MESSAGE_ERROR);
    dialog.run();
    return;
  }

  // Settings file
  std::filesystem::path env_home = std::getenv("HOME");
  std::filesystem::path config_dir = env_home / ".config/nostokeeb/";
  m_settings_path = config_dir / "nostokeeb.ini";

  try {
    if (!std::filesystem::exists(config_dir)) {
      std::cerr << config_dir << " not found. Creating..." << std::endl;
      std::filesystem::create_directories(config_dir);
    }

    if (!std::filesystem::exists(m_settings_path)) {
      std::cerr << m_settings_path << " not found. Creating..." << std::endl;
      std::ofstream settings_file(m_settings_path);
    }
  } catch (const std::filesystem::filesystem_error& e) {
    std::cerr << "Filesystem error: " << e.what() << std::endl;
  }

  get_style_context()->add_class("nk_keyboard");

  // Control box
  m_control_box.get_style_context()->add_class("control-box");
  m_control_box.pack_start(m_button_drag, Gtk::PACK_EXPAND_WIDGET);
  m_control_box.pack_end(m_button_close, Gtk::PACK_SHRINK);
  m_control_box.pack_end(m_button_about, Gtk::PACK_SHRINK);
  m_main_box.add(m_control_box);

  // Drag button
  m_button_drag.signal_button_press_event().connect(
    [this](GdkEventButton* event) {
      begin_move_drag(event->button, event->x_root, event->y_root, event->time);
      return true;
    }
  );

  // Close button
  m_button_close.set_image(*Gtk::make_managed<Gtk::Image>(
    CLOSE_ICON, Gtk::ICON_SIZE_BUTTON));
  m_button_close.signal_clicked().connect([this]() { close(); });

  // About button
  m_button_about.set_image(*Gtk::make_managed<Gtk::Image>(
    ABOUT_ICON, Gtk::ICON_SIZE_BUTTON));
  m_button_about.signal_clicked().connect(sigc::mem_fun(*this,
    &NK_Window::handle_sig_about));

  // Keyboard container setup
  m_kb_box.set_margin_top(KB_BOX_MARGIN);
  m_kb_box.set_margin_bottom(KB_BOX_MARGIN);
  m_kb_box.set_margin_left(KB_BOX_MARGIN);
  m_kb_box.set_margin_right(KB_BOX_MARGIN);
  m_main_box.add(m_kb_box);

  add(m_main_box);

  create_keyboard();
  apply_css(NK_Style::CSS_DEFAULT);
  show_all_children();

  // Restore previous window position
  try {
    m_settings.load_from_file(m_settings_path);
    int window_x = m_settings.get_integer("Window", "PosX");
    int window_y = m_settings.get_integer("Window", "PosY");
    std::cout << "Restoring window position at " << window_x << " " << window_y << "\n";
    move(window_x, window_y);
  } catch (const Glib::KeyFileError& e) {
    std::cerr << e.what() << std::endl;
  }

  // Save window position before closing
  signal_delete_event().connect([this](GdkEventAny* event) {
    int window_x, window_y;
    get_position(window_x, window_y);
    
    m_settings.set_integer("Window", "PosX", window_x);
    m_settings.set_integer("Window", "PosY", window_y);
    
    try {
      m_settings.save_to_file(m_settings_path);
    } catch (const Glib::Error& e) {
      std::cerr << e.what() << std::endl;
    }
    
    return false; // Return false to proceed with window closing
  });
}

NK_Window::~NK_Window() {
  release_all_keys();
  if (m_display) {
    XCloseDisplay(m_display);
  }
}

void NK_Window::apply_css(const std::string& css_data) {
  auto css_provider = Gtk::CssProvider::create();
  try {
    css_provider->load_from_data(css_data);
    auto screen = Gdk::Screen::get_default();
    Gtk::StyleContext::add_provider_for_screen(
      screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  } catch (const Glib::Error& e) {
    std::cerr << "Failed to load CSS: " << e.what() << std::endl;
  }
}

void NK_Window::simulate_key(KeySym keysym, bool pressed) {
  KeyCode keycode = XKeysymToKeycode(m_display, keysym);

  if (pressed) {
    m_pressed_keys.insert(keysym);
  } else {
    m_pressed_keys.erase(keysym);
  }

  XTestFakeKeyEvent(m_display, keycode, pressed, CurrentTime);
  XFlush(m_display);
}

void NK_Window::release_all_keys() {
  for (const auto& keysym : m_pressed_keys) {
    KeyCode keycode = XKeysymToKeycode(m_display, keysym);
    XTestFakeKeyEvent(m_display, keycode, KEY_RELEASED, CurrentTime);
  }

  XFlush(m_display);
  m_pressed_keys.clear();
}

Gtk::ToggleButton* NK_Window::create_key(const std::string& label, KeySym keysym) {
  auto key = Gtk::make_managed<Gtk::ToggleButton>(label);

  if (keysym == XK_Caps_Lock) {
    key->signal_toggled().connect([this, keysym]() {
      simulate_key(keysym, KEY_PRESSED);
      simulate_key(keysym, KEY_RELEASED);
    });
  } else if (NK_Layout::TOGGLE_KEYS.count(keysym)) {
    key->signal_toggled().connect([this, keysym, key]() {
      simulate_key(keysym, key->get_active());
    });
  } else {
    key->signal_pressed().connect([this, keysym]() {
      simulate_key(keysym, KEY_PRESSED);
    });

    key->signal_released().connect([this, keysym, key]() {
      simulate_key(keysym, KEY_RELEASED);
      key->set_active(false);
    });
  }

  return key;
}

void NK_Window::create_keyboard() {
  for (const auto& row : NK_Layout::LAYOUT) {
    auto row_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 5);

    for (const auto& [label, keysym] : row) {
      auto key = create_key(label, keysym);
      bool is_expanded_key = NK_Layout::EXPANDED_KEYS.count(keysym);

      if (label.size() > 1) {
        key->get_style_context()->add_class("button-wide");
      }

      row_box->pack_start(*key,
        is_expanded_key? Gtk::PACK_EXPAND_WIDGET : Gtk::PACK_SHRINK);
    }

    m_kb_box.pack_start(*row_box, Gtk::PACK_SHRINK);
  }
}

void NK_Window::handle_sig_about() {
  m_about_dialog.set_transient_for(*this);
  m_about_dialog.run();
  m_about_dialog.hide();
}
