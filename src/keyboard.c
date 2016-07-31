/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include <stdlib.h>
#include <string.h>
#include "keyboard.h"
#include "luaapi/keyboard.h"

typedef struct {
#ifdef UNIX
  SDL_Keycode keycode;
#else
  int32_t keycode;
#endif
  char const * name;
} KeyName;

static const KeyName keynames[] = {
  #ifdef UNIX
  {SDLK_UNKNOWN,      "unknown"},
  {SDLK_RETURN,       "return"},
  {SDLK_ESCAPE,       "escape"},
  {SDLK_ESCAPE,       "esc"},
  {SDLK_BACKSPACE,    "backspace"},
  {SDLK_TAB,          "tab"},
  {SDLK_SPACE,        " "},
  {SDLK_EXCLAIM,      "!"},
  {SDLK_QUOTEDBL,     "\""},
  {SDLK_HASH,         "#"},
  {SDLK_DOLLAR,       "$"},
  {SDLK_AMPERSAND,    "&"},
  {SDLK_QUOTE,        "\'"},
  {SDLK_LEFTPAREN,    "("},
  {SDLK_RIGHTPAREN,   ")"},
  {SDLK_ASTERISK,     "*"},
  {SDLK_PLUS,         "+"},
  {SDLK_COMMA,        ","},
  {SDLK_MINUS,        "-"},
  {SDLK_PERIOD,       "."},
  {SDLK_SLASH,        "/"},
  {SDLK_0,            "0"},
  {SDLK_1,            "1"},
  {SDLK_2,            "2"},
  {SDLK_3,            "3"},
  {SDLK_4,            "4"},
  {SDLK_5,            "5"},
  {SDLK_6,            "6"},
  {SDLK_7,            "7"},
  {SDLK_8,            "8"},
  {SDLK_9,            "9"},
  {SDLK_COLON,        ":"},
  {SDLK_SEMICOLON,    ";"},
  {SDLK_LESS,         "<"},
  {SDLK_EQUALS,       "="},
  {SDLK_GREATER,      ">"},
  {SDLK_QUESTION,     "?"},
  {SDLK_AT,           "@"},
  {SDLK_LEFTBRACKET,  "["},
  {SDLK_BACKSLASH,    "\\"},
  {SDLK_RIGHTBRACKET, "]"},
  {SDLK_CARET,        "^"},
  {SDLK_UNDERSCORE,   "_"},
  {SDLK_BACKQUOTE,    "`"},
  {SDLK_a,            "a"},
  {SDLK_b,            "b"},
  {SDLK_c,            "c"},
  {SDLK_d,            "d"},
  {SDLK_e,            "e"},
  {SDLK_f,            "f"},
  {SDLK_g,            "g"},
  {SDLK_h,            "h"},
  {SDLK_i,            "i"},
  {SDLK_j,            "j"},
  {SDLK_k,            "k"},
  {SDLK_l,            "l"},
  {SDLK_m,            "m"},
  {SDLK_n,            "n"},
  {SDLK_o,            "o"},
  {SDLK_p,            "p"},
  {SDLK_q,            "q"},
  {SDLK_r,            "r"},
  {SDLK_s,            "s"},
  {SDLK_t,            "t"},
  {SDLK_u,            "u"},
  {SDLK_v,            "v"},
  {SDLK_w,            "w"},
  {SDLK_x,            "x"},
  {SDLK_y,            "y"},
  {SDLK_z,            "z"},
  {SDLK_CAPSLOCK,     "capslock"},
  {SDLK_F1,           "f1"},
  {SDLK_F2,           "f2"},
  {SDLK_F3,           "f3"},
  {SDLK_F4,           "f4"},
  {SDLK_F5,           "f5"},
  {SDLK_F6,           "f6"},
  {SDLK_F7,           "f7"},
  {SDLK_F8,           "f8"},
  {SDLK_F9,           "f9"},
  {SDLK_F10,          "f10"},
  {SDLK_F11,          "f11"},
  {SDLK_F12,          "f12"},
  {SDLK_PAUSE,        "pause"},
  {SDLK_INSERT,       "insert"},
  {SDLK_HOME,         "home"},
  {SDLK_PAGEUP,       "pageup"},
  {SDLK_DELETE,       "delete"},
  {SDLK_END,          "end"},
  {SDLK_PAGEDOWN,     "pagedown"},
  {SDLK_RIGHT,        "right"},
  {SDLK_LEFT,         "left"},
  {SDLK_DOWN,         "down"},
  {SDLK_UP,           "up"},
  {SDLK_POWER,        "power"},
  {SDLK_KP_EQUALS,    "kp="},
  {SDLK_HELP,         "help"},
  {SDLK_MENU,         "menu"},
  {SDLK_UNDO,         "undo"},
  {SDLK_SYSREQ,       "sysreq"},
  {SDLK_CLEAR,        "clear"},
  {SDLK_LCTRL,        "lctrl"},
  {SDLK_LSHIFT,       "lshift"},
  {SDLK_LALT,         "lalt"},
  {SDLK_RCTRL,        "rctrl"},
  {SDLK_RSHIFT,       "rshift"},
  {SDLK_RALT,         "ralt"},
  {SDLK_MODE,         "mode"},
  {SDLK_RGUI,         "rgui"},
  {SDLK_LGUI,         "lgui"},
  {SDLK_CURRENCYUNIT, "currencyunit"},
  {SDLK_KP_DIVIDE,    "kp/"},
  {SDLK_KP_MULTIPLY,  "kp*"},
  {SDLK_KP_MINUS,     "kp-"},
  {SDLK_KP_PLUS,      "kp+"},
  {SDLK_KP_ENTER,     "kpenter"},
  {SDLK_KP_PERIOD,    "kp."},
  {SDLK_KP_COMMA,     "kp,"},
  {SDLK_PERCENT,      "%"},
  {SDLK_PRINTSCREEN,  "printscreen"},
  {SDLK_SCROLLLOCK,   "scrolllock"},
  {SDLK_APPLICATION,  "application"},
  {SDLK_F13,          "f13"},
  {SDLK_F14,          "f14"},
  {SDLK_F15,          "f15"},
  {SDLK_F16,          "f16"},
  {SDLK_F17,          "f17"},
  {SDLK_F18,          "f18"},
  {SDLK_F19,          "f19"},
  {SDLK_F20,          "f20"},
  {SDLK_F21,          "f21"},
  {SDLK_F22,          "f22"},
  {SDLK_F23,          "f23"},
  {SDLK_F24,          "f24"},
  {SDLK_KP_1,         "kp1"},
  {SDLK_KP_2,         "kp2"},
  {SDLK_KP_3,         "kp3"},
  {SDLK_KP_4,         "kp4"},
  {SDLK_KP_5,         "kp5"},
  {SDLK_KP_6,         "kp6"},
  {SDLK_KP_7,         "kp7"},
  {SDLK_KP_8,         "kp8"},
  {SDLK_KP_9,         "kp9"},
  {SDLK_KP_0,         "kp0"},
  {SDLK_WWW,          "www"},
  {SDLK_MAIL,         "mail"},
  {SDLK_CALCULATOR,   "calculator"},
  {SDLK_COMPUTER,     "computer"},
  {SDLK_AC_SEARCH,    "appsearch"},
  {SDLK_AC_HOME,      "apphome"},
  {SDLK_AC_BACK,      "appback"},
  {SDLK_AC_FORWARD,   "appforward"},
  {SDLK_AC_STOP,      "appstop"},
  {SDLK_AC_REFRESH,   "apprefresh"},
  {SDLK_AC_BOOKMARKS, "appbookmarks"},
  #endif
};

static struct {
  char const **keynames;
  bool *keystate;
  bool textActive;
  int numKeys;
} moduleData;

void keyboard_startText(void);

static int normalizeKeyCode(int key) {
  if(key & 0x40000000) {
      key = (key & 0x3ff) | 0x200;
    }
  return key;
}

#ifdef WINDOWS

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  DownKey[key] = action != GLFW_RELEASE;
  UpKey[key] = action != GLFW_PRESS;
  keyboardButton = key;
}

void keyboard_setcallback(void) {
  glfwSetKeyCallback(graphics_getWindow(), key_callback);
}

char const * keyboard_getKeyName(int key) {
  return moduleData.keynames[(key)];
}

int keyboard_getDownKeyGLFW(int key)
{
  if(key <= MAX_KEYS)
    return DownKey[key];
  else
    printf("%s \n","Error down key: You're trying to get an unknow key");
  return 0;
}

int keyboard_getUpKeyGLFW(int key)
{
  if(key <= MAX_KEYS)
    return UpKey[key];
  else
    printf("%s \n","Error key up: You're trying to get an unknow key");
  return 0;
}

//translate the key code number into actual keys
const char* returnGLFWKey(int key) {
  //NOTE: Lots of key tokens are missing on Windows because of GLFW, do not blame me! ^_^
  if (key == GLFW_KEY_UNKNOWN) return "unknown";
  if (key == GLFW_KEY_ENTER) return "return";
  if (key == GLFW_KEY_ESCAPE) return "esc";
  if (key == GLFW_KEY_ESCAPE) return "escape";
  if (key == GLFW_KEY_BACKSPACE) return "backspace";
  if (key == GLFW_KEY_TAB)    return      "tab";
  if (key == GLFW_KEY_SPACE)   return     " ";
  if (key == GLFW_KEY_BACKSLASH) return    "\"";
  if (key == GLFW_KEY_SLASH)   return     "\'";
  if (key == GLFW_KEY_KP_ADD)  return       "+";
  if (key == GLFW_KEY_COMMA)   return     ",";
  if (key == GLFW_KEY_MINUS)   return     "-";
  if (key == GLFW_KEY_PERIOD)  return     ".";
  if (key == GLFW_KEY_SLASH)   return     "/";
  if (key == GLFW_KEY_0)      return      "0";
  if (key == GLFW_KEY_1)     return       "1";
  if (key == GLFW_KEY_2)     return       "2";
  if (key == GLFW_KEY_3)     return       "3";
  if (key == GLFW_KEY_4)     return       "4";
  if (key == GLFW_KEY_5)    return        "5";
  if (key == GLFW_KEY_6)    return        "6";
  if (key == GLFW_KEY_7)    return        "7";
  if (key == GLFW_KEY_8)     return       "8";
  if (key == GLFW_KEY_9)     return       "9";
  if (key == GLFW_KEY_SEMICOLON)   return ";";
  if (key == GLFW_KEY_KP_EQUAL)  return     "=";
  if (key == GLFW_KEY_A)      return      "a";
  if (key == GLFW_KEY_B)      return      "b";
  if (key == GLFW_KEY_C)     return       "c";
  if (key == GLFW_KEY_D)     return       "d";
  if (key == GLFW_KEY_E)    return        "e";
  if (key == GLFW_KEY_F)     return       "f";
  if (key == GLFW_KEY_G)     return       "g";
  if (key == GLFW_KEY_H)     return       "h";
  if (key == GLFW_KEY_I)     return       "i";
  if (key == GLFW_KEY_J)      return      "j";
  if (key == GLFW_KEY_K)      return      "k";
  if (key == GLFW_KEY_L)      return      "l";
  if (key == GLFW_KEY_M)       return     "m";
  if (key == GLFW_KEY_N)       return     "n";
  if (key == GLFW_KEY_O)       return     "o";
  if (key == GLFW_KEY_P)       return     "p";
  if (key == GLFW_KEY_Q)        return    "q";
  if (key == GLFW_KEY_R)         return   "r";
  if (key == GLFW_KEY_S)         return   "s";
  if (key == GLFW_KEY_T)    return        "t";
  if (key == GLFW_KEY_U)      return      "u";
  if (key == GLFW_KEY_V)     return       "v";
  if (key == GLFW_KEY_W)     return       "w";
  if (key == GLFW_KEY_X)     return       "x";
  if (key == GLFW_KEY_Y)     return       "y";
  if (key == GLFW_KEY_Z)    return        "z";
  if (key == GLFW_KEY_CAPS_LOCK) return    "capslock";
  if (key == GLFW_KEY_F1)  return         "f1";
  if (key == GLFW_KEY_F2)  return         "f2";
  if (key == GLFW_KEY_F3)   return        "f3";
  if (key == GLFW_KEY_F4)   return        "f4";
  if (key == GLFW_KEY_F5)   return        "f5";
  if (key == GLFW_KEY_F6)   return        "f6";
  if (key == GLFW_KEY_F7)   return        "f7";
  if (key == GLFW_KEY_F8)   return        "f8";
  if (key == GLFW_KEY_F9)   return        "f9";
  if (key == GLFW_KEY_F10)  return        "f10";
  if (key == GLFW_KEY_F11)   return       "f11";
  if (key == GLFW_KEY_F12)   return       "f12";
  if (key == GLFW_KEY_PAUSE)  return      "pause";
  if (key == GLFW_KEY_INSERT) return     "insert";
  if (key == GLFW_KEY_HOME)    return     "home";
  if (key == GLFW_KEY_PAGE_UP)  return     "pageup";
  if (key == GLFW_KEY_DELETE)  return     "delete";
  if (key == GLFW_KEY_END)     return     "end";
  if (key == GLFW_KEY_PAGE_DOWN)  return   "pagedown";
  if (key == GLFW_KEY_RIGHT)   return     "right";
  if (key == GLFW_KEY_LEFT)   return      "left";
  if (key == GLFW_KEY_DOWN)    return     "down";
  if (key == GLFW_KEY_UP)     return      "up";
  if (key == GLFW_KEY_LEFT_CONTROL)  return      "lctrl";
  if (key == GLFW_KEY_LEFT_SHIFT)   return    "lshift";
  if (key == GLFW_KEY_LEFT_ALT)    return     "lalt";
  if (key == GLFW_KEY_RIGHT_SHIFT) return      "rshift";
  if (key == GLFW_KEY_RIGHT_ALT)   return      "ralt";
  if (key == GLFW_KEY_PRINT_SCREEN) return "printscreen";
  if (key == GLFW_KEY_SCROLL_LOCK) return  "scrolllock";
  if (key == GLFW_KEY_F13)   return       "f13";
  if (key == GLFW_KEY_F14)   return       "f14";
  if (key == GLFW_KEY_F15)   return       "f15";
  if (key == GLFW_KEY_F16)   return       "f16";
  if (key == GLFW_KEY_F17)   return       "f17";
  if (key == GLFW_KEY_F18)   return       "f18";
  if (key == GLFW_KEY_F19)   return       "f19";
  if (key == GLFW_KEY_F20)   return       "f20";
  if (key == GLFW_KEY_F21)   return       "f21";
  if (key == GLFW_KEY_F22)   return       "f22";
  if (key == GLFW_KEY_F23)   return       "f23";
  if (key == GLFW_KEY_F24)    return      "f24";
  if (key == GLFW_KEY_KP_1)   return      "kp1";
  if (key == GLFW_KEY_KP_2)   return      "kp2";
  if (key == GLFW_KEY_KP_3)    return     "kp3";
  if (key == GLFW_KEY_KP_4)    return     "kp4";
  if (key == GLFW_KEY_KP_5)  return       "kp5";
  if (key == GLFW_KEY_KP_6)  return       "kp6";
  if (key == GLFW_KEY_KP_7)   return      "kp7";
  if (key == GLFW_KEY_KP_8)   return      "kp8";
  if (key == GLFW_KEY_KP_9)   return      "kp9";
  if (key == GLFW_KEY_KP_0)   return      "kp0";
  return "unknown";
}

void keyboard_keypressed(int key) {
  if(keyboard_getDownKeyGLFW(key) && keyboardButton != -1) {
      l_keyboard_keypressed(returnGLFWKey(key), false);
      key = 0;
      keyboardButton = -1;
    }
}

void keyboard_keyreleased(int key) {
  if(keyboard_getUpKeyGLFW(key) && keyboardButton != -1) {
      l_keyboard_keyreleased(returnGLFWKey(key));
      key = 0;
      keyboardButton = -1;
    }
}

bool keyboard_ispressed(int key) {
  if(keyboard_getDownKeyGLFW(key)) return true;
  return false;
}

#endif

void keyboard_init(void) {
  moduleData.numKeys = 0;
  for(int i = 0; i < sizeof(keynames) / sizeof(KeyName); ++i) {
      int key = normalizeKeyCode(keynames[i].keycode);
      if(key > moduleData.numKeys) {
          moduleData.numKeys = key;
        }
    }
  ++moduleData.numKeys;

  moduleData.keynames = malloc(moduleData.numKeys * sizeof(char*));
  moduleData.keystate = malloc(moduleData.numKeys * sizeof(bool));

  for(int i = 0; i < moduleData.numKeys; ++i) {
      moduleData.keynames[i] = "unknown";
    }

  for(int i = 0; i < sizeof(keynames) / sizeof(KeyName); ++i) {
      moduleData.keynames[normalizeKeyCode(keynames[i].keycode)] = keynames[i].name;
    }

  memset(moduleData.keystate, 0, sizeof(bool) * moduleData.numKeys);
  keyboard_startText();
}

#ifdef UNIX
SDL_Keycode keyboard_getKeycode(char const* name) {
  for(int i = 0; i < moduleData.numKeys; ++i) {
      if(!strcmp(name, keynames[i].name)) {
          return keynames[i].keycode;
        }
    }
  return 0;
}

char const * keyboard_getKeyName(SDL_Keycode key) {
  return moduleData.keynames[normalizeKeyCode(key)];
}

void keyboard_keypressed(SDL_Keycode key) {
  int nk = normalizeKeyCode(key);
  if(nk < moduleData.numKeys) {
      bool repeat = moduleData.keystate[nk];
      moduleData.keystate[nk] = true;
      l_keyboard_keypressed(nk, repeat);
    }
}

void keyboard_keyreleased(SDL_Keycode key) {
  int nk = normalizeKeyCode(key);
  if(nk < moduleData.numKeys) {
      moduleData.keystate[nk] = false;
      l_keyboard_keyreleased(nk);
    }
}

bool keyboard_ispressed(SDL_Keycode key) {
  int nk = normalizeKeyCode(key);
  return nk < moduleData.numKeys && moduleData.keystate[nk];
}
#endif

void keyboard_startText(void) {
#ifdef UNIX
  SDL_StartTextInput();
#endif
  moduleData.textActive = true;
}

void keyboard_stopText(void) {
#ifdef UNIX
  SDL_StopTextInput();
#endif
  moduleData.textActive = false;
}

bool keyboard_isTextEnabled(void) {
  return moduleData.textActive;
}

void keyboard_textInput(char const* text) {
  l_keyboard_textInput(text);
}
