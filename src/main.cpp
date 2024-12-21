#include "window.cpp"

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create(argc, argv, "org.NostoKeeb");

  NK_Window window;
  return app->run(window);
}
