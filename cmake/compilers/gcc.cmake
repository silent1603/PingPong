target_compile_options(pingpong PRIVATE -Wall -Wextra -Wpedantic -fno-rtti -fno-exceptions -pthread -g -O0)

# Debug configuration
target_compile_options(pingpong PRIVATE
    $<$<CONFIG:Debug>:-g -O0 -DDEBUG -pthread>
)

# Release configuration
target_compile_options(pingpong PRIVATE
    $<$<CONFIG:Release>:-O2 -DNDEBUG -pthread>
)