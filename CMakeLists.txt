PROJECT(ksquares)

#find_package(KDE4 REQUIRED)
#include (KDE4Defaults)
#include (MacroLibrary)

#find_library(KDEGAMES_LIB kdegames ${KDE4_LIB_DIR})
#link_directories (${KDE4_LIB_DIR})

#if (KDEGAMES_LIB)
#   add_subdirectory(src)
#endif (KDEGAMES_LIB)
add_subdirectory(src)
#add_subdirectory(doc)