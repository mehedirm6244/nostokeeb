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
  const std::string CSS_LIGHT = R"(
    .nk_keyboard * { all: unset; }
    .nk_keyboard { background-color: #d0d3d9; color: #222; }
    .nk_keyboard button {
      padding: 8px;
      min-width: 24px;
      min-height: 24px;
      color: #333;
      font-weight: 500;
      background-color: #fff;
      border-radius: 4px;
      box-shadow: 0 3px 2px -3px rgba(0, 0, 0, 0.2);
    }
    .nk_keyboard button:hover { background-color: #eee; }
    .nk_keyboard button:active,
    .nk_keyboard button.toggled { background-color: #dedede; }
    .nk_keyboard .button-wide { padding: 8px 12px; }
    .nk_keyboard .control-box button {
      background-color: transparent;
      box-shadow: none;
      padding: 6px;
      min-width: 20px;
      min-height: 20px;
      border-radius: 0px;
    }
    .nk_keyboard .control-box button:hover { background-color: #eee; }
  )";

  const std::string CSS_DARK = R"(
    .nk_keyboard * { all: unset; }
    .nk_keyboard { background-color: #202020; color: #eee; }
    .nk_keyboard button {
      padding: 8px;
      min-width: 24px;
      min-height: 24px;
      color: #eee;
      font-weight: 500;
      background-color: #323232;
      border-radius: 4px;
      box-shadow: 0 3px 2px -3px rgba(0, 0, 0, 0.2);
    }
    .nk_keyboard button:hover { background-color: #3c3c3c; }
    .nk_keyboard button:active,
    .nk_keyboard button.toggled { background-color: #4d4d4d; }
    .nk_keyboard .button-wide { padding: 8px 12px; }
    .nk_keyboard .control-box button {
      background-color: transparent;
      box-shadow: none;
      padding: 6px;
      min-width: 20px;
      min-height: 20px;
      border-radius: 0px;
    }
    .nk_keyboard .control-box button:hover { background-color: #323232; }
  )";
}

#endif
