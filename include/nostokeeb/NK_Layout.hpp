/**
 * This file is from NostoKeeb - an on screen keyboard program for X11.
 * Copyright (C) 2024-2025 Mehedi Rahman Mahi
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef NK_LAYOUT
#define NK_LAYOUT

#include <X11/keysym.h>

namespace NK_Layout {
  const std::vector<std::vector<std::pair<std::string, KeySym>>> LAYOUT = {
    {
      {"`", XK_grave}, {"1", XK_1}, {"2", XK_2}, {"3", XK_3}, {"4", XK_4},
      {"5", XK_5}, {"6", XK_6}, {"7", XK_7}, {"8", XK_8}, {"9", XK_9}, {"0", XK_0},
      {"-", XK_minus}, {"=", XK_equal}, {"\u232B", XK_BackSpace}, {"Del", XK_Delete}
    },
    {
      {"Tab", XK_Tab}, {"Q", XK_Q}, {"W", XK_W}, {"E", XK_E}, {"R", XK_R},
      {"T", XK_T}, {"Y", XK_Y}, {"U", XK_U}, {"I", XK_I}, {"O", XK_O}, {"P", XK_P},
      {"[", XK_bracketleft}, {"]", XK_bracketright}, {"\\", XK_backslash}
    },
    {
      {"CapsLk", XK_Caps_Lock}, {"A", XK_A}, {"S", XK_S}, {"D", XK_D}, {"F", XK_F},
      {"G", XK_G}, {"H", XK_H}, {"J", XK_J}, {"K", XK_K}, {"L", XK_L},
      {";", XK_semicolon}, {"'", XK_apostrophe}, {"Enter", XK_Return}
    },
    {
      {"Shift", XK_Shift_L}, {"Z", XK_Z}, {"X", XK_X}, {"C", XK_C}, {"V", XK_V},
      {"B", XK_B}, {"N", XK_N}, {"M", XK_M}, {",", XK_comma}, {".", XK_period},
      {"/", XK_slash}, {"Shift", XK_Shift_R}
    },
    {
      {"Ctrl", XK_Control_L}, {"Win", XK_Super_L}, {"Alt", XK_Alt_L}, {" ", XK_space},
      {"Alt", XK_Alt_R}, {"Ctrl", XK_Control_R}, {"\u2190", XK_Left}, {"\u2192", XK_Right},
      {"\u2191", XK_Up}, {"\u2193", XK_Down}
    }
  };

  const std::set<KeySym> EXPANDED_KEYS = {
    XK_BackSpace, XK_Tab, XK_Caps_Lock, XK_Return, XK_Shift_L, XK_Shift_R, XK_space
  };

  const std::set<KeySym> TOGGLE_KEYS = {
    XK_Shift_L, XK_Shift_R, XK_Control_L, XK_Control_R, XK_Alt_L, XK_Alt_R
  };
} // namespace NK_Layout

#endif // NK_LAYOUT
