#include <chrono>
#include <iostream>
#include <thread>

#include <wu-syswin/syswin.hpp>

int main(int argc, char **args) {
  auto window = syswin::window_builder()
                    .with_title("Simple Window")
                    .with_pos(0, 0)
                    .with_size(640, 480)
                    .build();

  if (!window.has_value()) {
    std::cout << "Failed to create window.\n";
    return 1;
  }

  window->show();

  window->poll_events<syswin::button_press,
                      syswin::motion_event>(
      [](const auto &event) {
        using event_t = std::decay_t<decltype(event)>;
        if constexpr (std::is_same_v<
                          event_t, syswin::button_press>) {
          std::cout << "Closing\n";
          return false;
        }

        std::cout << "Event!\n";
        return true;
      });

  // while (1) {
  //   window->poll_event([](const auto &event) {
  //     using event_t = std::decay_t<decltype(event)>;

  //     if constexpr (std::is_same_v<event_t,
  //                                  syswin::key_release>)
  //                                  {
  //       std::cout << "Key release\n";
  //     }
  //   });
  // }
}