#include "window.hpp"

#include <iostream>
#include <string>
#include <X11/extensions/XTest.h>

void NostoKeeb::apply_css() {
  auto css_provider = Gtk::CssProvider::create();
  css_provider->load_from_data(
    "* { all: unset; }"
    "background { background-color: black; } "
    "button { padding: 8px; border: 1px solid white; min-width: 24px; min-height: 24px; }"
    "button:active { color: black; background-color: white; }"
    ".control-box button { background-color: white; color: black; padding: 4px 12px; }"
  );

  auto screen = Gdk::Screen::get_default();
  Gtk::StyleContext::add_provider_for_screen(screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void NostoKeeb::simulate_key(KeySym keysym, bool pressed) {
  KeyCode keycode = XKeysymToKeycode(m_display, keysym);
  if (keycode == 0) {
    return;
  }

  XTestFakeKeyEvent(m_display, keycode, pressed, CurrentTime);
  XFlush(m_display);
}

void NostoKeeb::create_keys() {
  /* Apparently this is the most optimal way I can think of */
  std::vector<std::vector<std::pair<std::string, KeySym>>> keys = {
    {
      {"`", XK_grave}, {"1", XK_1}, {"2", XK_2}, {"3", XK_3}, {"4", XK_4}, {"5", XK_5},
      {"6", XK_6}, {"7", XK_7}, {"8", XK_8}, {"9", XK_9}, {"0", XK_0}, {"-", XK_minus},
      {"=", XK_equal}, {"Back", XK_BackSpace}, {"Del", XK_Delete}
    },
    {
      {"Tab", XK_Tab}, {"Q", XK_Q}, {"W", XK_W}, {"E", XK_E}, {"R", XK_R}, {"T", XK_T},
      {"Y", XK_Y}, {"U", XK_U}, {"I", XK_I}, {"O", XK_O}, {"P", XK_P}, {"[", XK_bracketleft},
      {"]", XK_bracketright}, {"\\", XK_backslash}}, {{"CapsLK", XK_Caps_Lock}, {"A", XK_A},
      {"S", XK_S}, {"D", XK_D}, {"F", XK_F}, {"G", XK_G}, {"H", XK_H}, {"J", XK_J},
      {"K", XK_K}, {"L", XK_L}, {";", XK_semicolon}, {"'", XK_apostrophe}, {"Enter", XK_Return}
    },
    {
      {"Shift", XK_Shift_L}, {"Z", XK_Z}, {"X", XK_X}, {"C", XK_C}, {"V", XK_V}, {"B", XK_B},
      {"N", XK_N}, {"M", XK_M}, {",", XK_comma}, {".", XK_period}, {"/", XK_slash},
      {"Shift", XK_Shift_R}
    },
    {
      {"Ctrl", XK_Control_L}, {"Space", XK_space}, {"Ctrl", XK_Control_R},
      {"←", XK_Left}, {"→", XK_Right}, {"↑", XK_Up}, {"↓", XK_Down}
    }
  };

  for (const auto& row : keys) {
    auto current_row = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 5);

    for (const auto& [label, keysym] : row) {
      auto button = Gtk::make_managed<Gtk::Button>(label);
      button->signal_pressed().connect([this, keysym, label]() {
        simulate_key(keysym, 1);
      });
      button->signal_released().connect([this, keysym, label]() {
        simulate_key(keysym, 0);
      });

      current_row->pack_start(*button, Gtk::PACK_EXPAND_WIDGET);
    }

    m_vbox.pack_start(*current_row, Gtk::PACK_SHRINK);
  }
}

NostoKeeb::NostoKeeb() : m_button_drag("Drag") {
  /* Set Window Properties */
  set_title("NostoKeeb");
  set_icon(Gtk::IconTheme::get_default()->
    load_icon("input-keyboard", 32, Gtk::ICON_LOOKUP_NO_SVG));
  set_decorated(false);
  set_resizable(false);
  set_keep_above(true);
  set_accept_focus(false);
  set_border_width(6);

  add(m_vbox);

  m_button_drag.signal_button_press_event().connect([this](GdkEventButton* event) {
    begin_move_drag(event->button, event->x_root, event->y_root, event->time);
    return true;
  });
  
  m_control_box.get_style_context()->add_class("control-box");
  m_control_box.pack_end(m_button_drag, Gtk::PACK_SHRINK);
  m_vbox.add(m_control_box);

  /* Abort if display server isn't X11 */
  m_display = XOpenDisplay(nullptr);
  if (!m_display) {
    std::cerr << "Unable to open X display!" << std::endl;
    return;
  }

  create_keys();
  apply_css();
  show_all_children();
}

NostoKeeb::~NostoKeeb() {
  if (m_display) {
    XCloseDisplay(m_display);
  }
}
