target_compile_options(pingpong PRIVATE /Wall /GR- /EHs-c-)

# Debug configuration
target_compile_options(pingpong PRIVATE
    $<$<CONFIG:Debug>:/MDd /Zi /Od /RTC1>
)

# Release configuration
target_compile_options(pingpong PRIVATE
    $<$<CONFIG:Release>:/MD /O2 /DNDEBUG>
)