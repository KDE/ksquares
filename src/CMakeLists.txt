#macro_optional_find_package(LibKDEGames)
#macro_log_feature(KDEGAMES_FOUND "LibKDEGames" "KDE Games Library" "games.kde.org" FALSE "" "")

set(ksquares_SRCS
	main.cpp
	ksquareswindow.cpp
	gameboardview.cpp
	gameboardscene.cpp
	ksquaresgame.cpp
	aicontroller.cpp
	newgamedialog.cpp
	ksquaresdemowindow.cpp
	themegraphicsitems.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/libkdegames/highscore)

kde4_automoc(${ksquares_SRCS})

kde4_add_ui_files(ksquares_SRCS newgamedialog.ui scoresdialog.ui prefs_ai.ui prefs_display.ui)

kde4_add_kcfg_files(ksquares_SRCS GENERATE_MOC settings.kcfgc) #GENERATE_MOC is not needed but it doesn't work without it for some reason :S

kde4_add_executable(ksquares ${ksquares_SRCS})

target_link_libraries(ksquares ${KDE4_KDEUI_LIBS} kdegames)

install(TARGETS ksquares DESTINATION ${BIN_INSTALL_DIR})


########### install files ###############

install( FILES ksquares.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES ksquares.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES ksquaresui.rc  DESTINATION  ${DATA_INSTALL_DIR}/ksquares )
kde4_install_icons(${ICON_INSTALL_DIR})
