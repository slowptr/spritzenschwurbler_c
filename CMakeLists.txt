cmake_minimum_required(VERSION 3.21)
project(spritzenschwurbler_c C)

set(CMAKE_C_STANDARD 99)

add_executable(spritzenschwurbler_c WIN32 src/main.c src/utils/injector.c src/ss_window.c src/utils/kippfenster/kf_utils.c src/utils/kippfenster/kf_wnd.c src/utils/kippfenster/kf_wnd_ctrls.c)
