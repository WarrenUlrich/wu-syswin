#pragma once

#include <optional>
#include <utility>

#include "detail/x11.hpp"

#ifdef WU_SYSWIN_VULKAN_SUPPORT
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xlib.h>
#endif

namespace syswin {
class key_event {
public:
  key_event(detail::x11::event &event) : _event(event) {}

private:
  detail::x11::event &_event;
};

class key_press : key_event {
public:
  static constexpr auto event_id = KeyPress;
  static constexpr auto event_mask = KeyPressMask;

  key_press(detail::x11::event &event) : key_event(event) {}
};

class key_release : key_event {
public:
  static constexpr auto event_id = KeyRelease;
  static constexpr auto event_mask = KeyReleaseMask;

  key_release(detail::x11::event &event)
      : key_event(event) {}
};

class button_event {
public:
  button_event(detail::x11::event &event) : _event(event) {}

private:
  detail::x11::event &_event;
};

class button_press : button_event {
public:
  static constexpr auto event_id = ButtonPress;
  static constexpr auto event_mask = ButtonPressMask;

  button_press(detail::x11::event &event)
      : button_event(event) {}
};

class button_release : button_event {
public:
  static constexpr auto event_id = ButtonRelease;
  static constexpr auto event_mask = ButtonReleaseMask;

  button_release(detail::x11::event &event)
      : button_event(event) {}
};

class motion_event {
public:
  static constexpr auto event_id = MotionNotify;
  static constexpr auto event_mask = PointerMotionMask;

  motion_event(detail::x11::event &event) : _event(event) {}

private:
  detail::x11::event &_event;
};

class window {
public:
  window(detail::x11::display *display,
         detail::x11::window window)
      : _display(display), _window(window) {}

  void poll_event(auto &&fn) noexcept {
    XNextEvent(_display, &_event_buffer);
  }

  template <typename... Events>
  void poll_events(auto &&fn) noexcept {
    static constexpr auto mask = (Events::event_mask | ...);
    XSelectInput(_display, _window, mask);
    while (true) {
      XNextEvent(_display, &_event_buffer);

      if (!(... && _handle_event<Events>(fn))) {
        break;
      }
    }
  }

#ifdef WU_SYSWIN_VULKAN_SUPPORT
  /*
    TODO: functions to support:
      create_vulkan_surface
      get_physical_device
  */
  VkResult
  create_vulkan_surface(VkInstance vk_instance,
                        VkSurfaceKHR *surface) const {
    VkXlibSurfaceCreateInfoKHR create_info = {
        .sType =
            VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        .dpy = _display,
        .window = _window};

    return vkCreateXlibSurfaceKHR(vk_instance, &create_info,
                                  nullptr, surface);
  }
#endif

  void show() const noexcept {
    XMapWindow(_display, _window);
  }

private:
  detail::x11::display *_display;
  detail::x11::window _window;
  detail::x11::event _event_buffer;

  template <typename Event>
  bool _handle_event(auto &&fn) noexcept {
    if (_event_buffer.type == Event::event_id) {
      return fn(Event(_event_buffer));
    }

    return true;
  }
};

class window_builder {
public:
  constexpr window_builder()
      : _width(640), _height(480), _x(0), _y(0) {}

  constexpr window_builder &
  with_title(std::string_view title) noexcept {
    _title = title;
    return *this;
  }

  constexpr window_builder &
  with_size(std::size_t width,
            std::size_t height) noexcept {
    _width = width;
    _height = height;
    return *this;
  }

  constexpr window_builder &
  with_pos(std::size_t x, std::size_t y) noexcept {
    _x = x;
    _y = y;
    return *this;
  }

  constexpr window_builder &
  with_border_width(std::size_t border_width) noexcept {
    _border_width = border_width;
    return *this;
  }

  std::optional<window> build() const noexcept {
    const auto _display = XOpenDisplay(NULL);
    if (_display == nullptr)
      return std::nullopt;

    const auto _screen = DefaultScreen(_display);

    const auto _window = XCreateSimpleWindow(
        _display, RootWindow(_display, _screen), _x, _y,
        _width, _height, _border_width,
        BlackPixel(_display, _screen),
        WhitePixel(_display, _screen));

    if (_window == 0) {
      return std::nullopt;
    }

    XStoreName(_display, _window, _title.data());
    return window(_display, _window);
  }

private:
  std::string_view _title;

  std::size_t _width;
  std::size_t _height;

  std::size_t _x;
  std::size_t _y;

  std::size_t _border_width;
};
} // namespace syswin