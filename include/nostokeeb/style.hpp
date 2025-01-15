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


#ifndef NK_STYLE_HPP
#define NK_STYLE_HPP

#include <string>

namespace NK_Style {
  const std::string CSS_DEFAULT = R"(
    .nk_keyboard button {
      padding: 8px;
      min-width: 24px;
      min-height: 24px;
      font-weight: 500;
    }
    .nk_keyboard button.toggled { background-color: red; }
    .nk_keyboard .button-wide { padding: 8px 12px; }
    .nk_keyboard .control-box button {
      background-color: transparent;
      border: none;
      box-shadow: none;
      padding: 6px;
      min-width: 20px;
      min-height: 20px;
      border-radius: 0px;
    }
  )";
}

#endif
