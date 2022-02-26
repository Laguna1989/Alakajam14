file(GLOB_RECURSE GAMELIBFILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*)

add_library(GameLib STATIC ${GAMELIBFILES} damage.cpp damage.hpp shroom_game_contact_listener.cpp shroom_game_contact_listener.hpp)
if (JT_ENABLE_WEB)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

target_link_libraries(GameLib PUBLIC JamTemplate)
target_include_directories(GameLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

