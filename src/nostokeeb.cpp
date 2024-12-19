#include "window.cpp"

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create(argc, argv, "org.NostoKeeb");

  NostoKeeb nostokeeb;
  return app->run(nostokeeb);
}
