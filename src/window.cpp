#include <nostokeeb/window.hpp>

#include <iostream>
#include <string>
#include <csignal>
#include <X11/extensions/XTest.h>

/* Static instance for signal handling */
NK_Window* NK_Window::s_instance = nullptr;

void NK_Window::apply_css() {
  auto css_provider = Gtk::CssProvider::create();
  try {
    css_provider->load_from_data(CSS_DATA);
    auto screen = Gdk::Screen::get_default();
    Gtk::StyleContext::add_provider_for_screen(screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  } catch (const Glib::Error& e) {
    std::cerr << "Failed to load CSS: " << e.what() << std::endl;
  }
}

void NK_Window::simulate_key(KeySym keysym, bool pressed) {
  KeyCode keycode = XKeysymToKeycode(m_display, keysym);
  if (keycode == 0) {
    return;
  }

  /* Keep Track of The Pressed Keys */
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
    if (keycode != 0) {
      XTestFakeKeyEvent(m_display, keycode, false, CurrentTime);
    }
  }
  XFlush(m_display);
  m_pressed_keys.clear();
}

void NK_Window::handle_signal(int signal) {
  if (s_instance) {
    s_instance->release_all_keys();
  }
  std::cerr << "Signal received: " << signal << std::endl;
  exit(signal);
}

bool toggle_button(Gtk::Button *button) {
  auto btn_style_ctx = button->get_style_context();
  bool is_toggled = btn_style_ctx->has_class("toggled");

  if (is_toggled) {
    btn_style_ctx->remove_class("toggled");
  } else {
    btn_style_ctx->add_class("toggled");
  }

  return !is_toggled;
}

void NK_Window::create_keys() {
  /* Apparently this is the most optimal way I can think of */
  const std::vector<std::vector<std::pair<std::string, KeySym>>> keys = {
    {
      {"`", XK_grave}, {"1", XK_1}, {"2", XK_2}, {"3", XK_3}, {"4", XK_4}, {"5", XK_5},
      {"6", XK_6}, {"7", XK_7}, {"8", XK_8}, {"9", XK_9}, {"0", XK_0}, {"-", XK_minus},
      {"=", XK_equal}, {"Back", XK_BackSpace}, {"Del", XK_Delete}
    },
    {
      {"Tab", XK_Tab}, {"Q", XK_Q}, {"W", XK_W}, {"E", XK_E}, {"R", XK_R}, {"T", XK_T},
      {"Y", XK_Y}, {"U", XK_U}, {"I", XK_I}, {"O", XK_O}, {"P", XK_P}, {"[", XK_bracketleft},
      {"]", XK_bracketright}, {"\\", XK_backslash}
    },
    {
      {"CapsLk", XK_Caps_Lock}, {"A", XK_A}, {"S", XK_S}, {"D", XK_D}, {"F", XK_F},
      {"G", XK_G}, {"H", XK_H}, {"J", XK_J}, {"K", XK_K}, {"L", XK_L}, {";", XK_semicolon},
      {"'", XK_apostrophe}, {"Enter", XK_Return}
    },
    {
      {"Shift", XK_Shift_L}, {"Z", XK_Z}, {"X", XK_X}, {"C", XK_C}, {"V", XK_V}, {"B", XK_B},
      {"N", XK_N}, {"M", XK_M}, {",", XK_comma}, {".", XK_period}, {"/", XK_slash},
      {"Shift", XK_Shift_R}
    },
    {
      {"Ctrl", XK_Control_L}, {"Win", XK_Super_L}, {"Alt", XK_Alt_L}, {" ", XK_space},
      {"Ctrl", XK_Control_R}, {"Alt", XK_Alt_R}, {"←", XK_Left}, {"→", XK_Right}, {"↑", XK_Up},
      {"↓", XK_Down}
    }
  };

  const std::set<std::string> wide_keys = {
    " ", "Shift", "Tab", "Back", "Enter", "CapsLk", "Ctrl", "Alt", "Win"
  };

  for (const auto& row : keys) {
    auto current_row = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL, 4);

    for (const auto& [label, keysym] : row) {
      auto button = Gtk::make_managed<Gtk::Button>(label);
      bool expand_btn = false;

      if (label == "Shift" || label == "Ctrl" || label == "Alt") {
        button->signal_pressed().connect([this, button, keysym]() {
            simulate_key(keysym, toggle_button(button));
        });
      } else {
        if (label == "CapsLk") {
          XKeyboardState x;
          XGetKeyboardControl(m_display, &x);
          if (x.led_mask & 1) {
            toggle_button(button);
          }

          button->signal_pressed().connect([this, button, keysym]() {
            toggle_button(button);
            simulate_key(keysym, true);
          });
          button->signal_released().connect([this, keysym]() { simulate_key(keysym, false); });
        } else {
          button->signal_released().connect([this, keysym]() { simulate_key(keysym, true); });
          button->signal_released().connect([this, keysym]() { simulate_key(keysym, false); });
        }
      }

      if (wide_keys.count(label)) {
        button->get_style_context()->add_class("button-wide");
        if (label != "Ctrl" && label != "Alt" && label != "Win") {
          expand_btn = true;
        }
      }

      current_row->pack_start(*button, (expand_btn)? Gtk::PACK_EXPAND_WIDGET : Gtk::PACK_SHRINK);
    }

    m_vbox.pack_start(*current_row, Gtk::PACK_SHRINK);
  }
}

NK_Window::NK_Window() : m_button_drag("Drag"), m_button_close("Close") {
  /* Check if another instance of NostoKeeb is already running */
  if (s_instance) {
    throw std::runtime_error("Only one instance of NK_Window is allowed!");
  }
  s_instance = this;

  /* Set Window Properties */
  set_title("NostoKeeb");
  set_icon(Gtk::IconTheme::get_default()->load_icon("input-keyboard", 32, Gtk::ICON_LOOKUP_NO_SVG));
  set_decorated(false);
  set_resizable(false);
  set_keep_above(true);
  set_accept_focus(false);
  set_border_width(6);

  /* Abort if display server isn't X11 */
  m_display = XOpenDisplay(nullptr);
  if (!m_display) {
    Gtk::MessageDialog dialog(*this, "X11 Display not available!", false, Gtk::MESSAGE_ERROR);
    dialog.run();
    return;
  }

  m_control_box.get_style_context()->add_class("control-box");
  m_control_box.pack_start(m_button_drag, Gtk::PACK_EXPAND_WIDGET);
  m_control_box.pack_end(m_button_close, Gtk::PACK_SHRINK);
  m_vbox.add(m_control_box);

  m_button_drag.signal_button_press_event().connect([this](GdkEventButton* event) {
    begin_move_drag(event->button, event->x_root, event->y_root, event->time);
    return true;
  });

  m_button_close.signal_clicked().connect([this]() {
    close();
  });
  m_button_close.get_style_context()->add_class("close");

  add(m_vbox);

  create_keys();
  apply_css();
  show_all_children();

  /* Register Signal Handlers */
  std::signal(SIGINT, NK_Window::handle_signal);
  std::signal(SIGTERM, NK_Window::handle_signal);
}

NK_Window::~NK_Window() {
  release_all_keys();
  s_instance = nullptr;
}
