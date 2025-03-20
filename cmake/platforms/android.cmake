add_library(pingpong SHARED)

find_package(game-activity REQUIRED CONFIG)

target_link_libraries(pingpong
        # The game activity
        game-activity::game-activity

        # EGL and other dependent libraries required for drawing
        # and interacting with Android system
        EGL
        GLESv3
        jnigraphics
        android
        log)
