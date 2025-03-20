target_compile_options(pingpong PRIVATE -Wall -Wextra -Wpedantic -fno-rtti -fno-exceptions)

# Debug configuration
target_compile_options(pingpong PRIVATE
    $<$<CONFIG:Debug>:-g -O0 -DDEBUG>
)

# Release configuration
target_compile_options(pingpong PRIVATE
    $<$<CONFIG:Release>:-O2 -DNDEBUG>
)