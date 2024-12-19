#include <gtkmm.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

class NostoKeeb : public Gtk::Window {
public:
  NostoKeeb();
  virtual ~NostoKeeb();

private:
  Gtk::Box m_vbox{Gtk::ORIENTATION_VERTICAL, 5};
  Gtk::Box m_control_box{Gtk::ORIENTATION_HORIZONTAL, 5};
  Gtk::Button m_button_drag;
  Display* m_display = nullptr;

  void create_keys();
  void apply_css();
  void simulate_key(KeySym keysym, bool pressed);
};
