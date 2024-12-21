#ifndef NK_WINDOW_HPP
#define NK_WINDOW_HPP

#include <gtkmm.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <string>
#include <set>
#include <csignal>

class NK_Window : public Gtk::Window {
private:
  /* Custom CSS for NostoKeeb window */
  const std::string CSS_DATA = R"(
* { all: unset; }
.background { background-color: #000; }
button {
  padding: 8px;
  border: 1px solid #eee;
  min-width: 24px;
  min-height: 24px;
  color: #eee;
  background-color: #000;
}
button:hover {
  background-color: #111;
}
button:active {
  color: #000;
  background-color: #eee;
}
.button-wide { padding: 8px 12px; }
.control-box button {
  padding: 3px 12px;
}
.control-box .close {
  background-color: #ff2222;
  border: none;
  color: #eee;
}
  )";

  Gtk::Box m_vbox{Gtk::ORIENTATION_VERTICAL,4};
  Gtk::Box m_control_box{Gtk::ORIENTATION_HORIZONTAL, 4};
  Gtk::Button m_button_drag;
  Gtk::Button m_button_close;
  std::set<KeySym> m_pressed_keys;
  Display* m_display = nullptr;

  /* Static reference to the application instance */
  static NK_Window* s_instance;

  /* Function to Create the Keyboard */
  void create_keys();

  /* Function to Apply CSS to NostoKeeb Window */
  void apply_css();

  /* Trigger Key Press and Release through X11 Events */
  void simulate_key(KeySym keysym, bool pressed);

  /* Function to release all the pressed keys */
  void release_all_keys();

public:
  /* Constructor for NK_Window */
  NK_Window();

  /* Deconstructor for NK_Window */
  virtual ~NK_Window();

  /* Static signal handler for cleanup */
  static void handle_signal(int signal);
};

#endif // NK_WINDOW_HPP
