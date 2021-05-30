project(demo)

include_directories("./")

set(SOURCE_CPP
    main.cpp
    setting_button.cpp
    preview_button.cpp
    output_button.cpp
    region_button.cpp
    demo_button.cpp
)

add_executable(demo WIN32
    ${SOURCE_CPP}
    )

target_link_libraries(demo

)