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

#include "nostokeeb/nostokeeb.hpp"
#include "nostokeeb/window.hpp"
#include "nostokeeb/style.hpp"
#include "nostokeeb/layout.hpp"

#include <iostream>
#include <string>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

void NK_Window::apply_css(const std::string css_data) {
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
    XTestFakeKeyEvent(m_display, keycode, false, CurrentTime);
  }

  XFlush(m_display);
  m_pressed_keys.clear();
}

bool toggle_button(Gtk::Button *button) {
  auto style_context = button->get_style_context();
  bool is_toggled = style_context->has_class("toggled");

  if (is_toggled) {
    style_context->remove_class("toggled");
  } else {
    style_context->add_class("toggled");
  }

  return !is_toggled;
}

void NK_Window::create_keys() {
  for (const auto& row : NK_Layout::LAYOUT) {
    auto row_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 5);

    for (const auto& [label, keysym] : row) {
      auto button = Gtk::make_managed<Gtk::Button>(label);
      bool expand_btn = NK_Layout::WIDE_KEYS.count(keysym);

      button->signal_pressed().connect([this, button, keysym]() {
        simulate_key(keysym, NK_Layout::TOGGLE_KEYS.count(keysym) ?
          toggle_button(button) : NK_KEY_PRESSED);

        // TODO: HANDLE CAPS_LOCK IN A BETTER WAY
        if (keysym == XK_Caps_Lock) {
          toggle_button(button);
        }
      });

      if (!NK_Layout::TOGGLE_KEYS.count(keysym)) {
        button->signal_released().connect([this, button, keysym]() {
          simulate_key(keysym, NK_KEY_RELEASED);
        });
      }

      if (label.size() > 1) {
        button->get_style_context()->add_class("button-wide");
      }

      row_box->pack_start(*button,
        expand_btn? Gtk::PACK_EXPAND_WIDGET : Gtk::PACK_SHRINK);
    }

    m_kb_box.pack_start(*row_box, Gtk::PACK_SHRINK);
  }
}

NK_Window::NK_Window() :
  m_button_drag(""),
  m_display(nullptr) {
  set_title(NostoKeeb::PROGRAM_NAME);
  set_icon(Gtk::IconTheme::get_default()->load_icon(
    NostoKeeb::PROGRAM_ICON, 128, Gtk::ICON_LOOKUP_USE_BUILTIN));
  set_decorated(false);
  set_resizable(false);
  set_keep_above(true);
  set_accept_focus(false);

  /* Abort if display server isn't X11 */
  m_display = XOpenDisplay(nullptr);
  if (!m_display) {
    Gtk::MessageDialog dialog(*this,
      "X11 Display not available!", false, Gtk::MESSAGE_ERROR);
    dialog.run();
    return;
  }

  get_style_context()->add_class("nk_keyboard");

  m_control_box.get_style_context()->add_class("control-box");
  m_control_box.pack_start(m_button_drag, Gtk::PACK_EXPAND_WIDGET);
  m_control_box.pack_end(m_button_close, Gtk::PACK_SHRINK);
  m_control_box.pack_end(m_button_about, Gtk::PACK_SHRINK);
  m_main_box.add(m_control_box);

  m_button_drag.signal_button_press_event().connect(
    [this](GdkEventButton* event) {
      begin_move_drag(event->button, event->x_root, event->y_root, event->time);
      return true;
    }
  );

  m_button_close.set_image(*Gtk::make_managed<Gtk::Image>(
    "window-close-symbolic", Gtk::ICON_SIZE_BUTTON));
  m_button_close.signal_clicked().connect([this]() { close(); });

  m_button_about.set_image(*Gtk::make_managed<Gtk::Image>(
    "help-about-symbolic", Gtk::ICON_SIZE_BUTTON));
  m_button_about.signal_clicked().connect(sigc::mem_fun(*this,
    &NK_Window::handle_sig_about));

  m_kb_box.set_margin_top(8);
  m_kb_box.set_margin_bottom(8);
  m_kb_box.set_margin_left(8);
  m_kb_box.set_margin_right(8);
  m_main_box.add(m_kb_box);

  add(m_main_box);

  create_keys();
  apply_css(NK_Style::CSS_DEFAULT);
  show_all_children();
}

NK_Window::~NK_Window() {
  release_all_keys();
}

void NK_Window::handle_sig_about() {
  m_about_dialog.set_transient_for(*this);
  m_about_dialog.run();
  m_about_dialog.hide();
}
