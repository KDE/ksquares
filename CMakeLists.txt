PROJECT(ksquares)

macro_optional_find_package(LibKDEGames QUIET)


if (KDEGAMES_FOUND)
add_subdirectory(src)
#add_subdirectory(doc)
endif (KDEGAMES_FOUND)
