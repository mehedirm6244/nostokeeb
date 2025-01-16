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

#include "nostokeeb/aboutdialog.hpp"
#include "nostokeeb/nostokeeb.hpp"

NK_AboutDialog::NK_AboutDialog() {
  set_program_name(NostoKeeb::PROGRAM_NAME);
  set_version(NostoKeeb::VERSION);
  set_license(NostoKeeb::LICENSE);
  set_comments(NostoKeeb::COMMENT);
  set_website(NostoKeeb::WEBSITE);
  set_logo(Gtk::IconTheme::get_default()->load_icon(
    NostoKeeb::PROGRAM_ICON, 128, Gtk::ICON_LOOKUP_USE_BUILTIN));
}

NK_AboutDialog::~NK_AboutDialog() = default;
