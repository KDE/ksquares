
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

kde4_add_ui_files(ksquares_SRCS newgamewidget.ui scoreswidget.ui prefs_ai.ui prefs_display.ui)

kde4_add_kcfg_files(ksquares_SRCS GENERATE_MOC settings.kcfgc) #GENERATE_MOC is not needed but it doesn't work without it for some reason :S

kde4_add_app_icon(ksquares_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-app-ksquares.png")
kde4_add_executable(ksquares ${ksquares_SRCS})

target_link_libraries(ksquares ${KDE4_KDEUI_LIBS} kdegames)

install(TARGETS ksquares DESTINATION ${BIN_INSTALL_DIR})


########### install files ###############

install( FILES ksquares.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES ksquares.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES ksquaresui.rc  DESTINATION  ${DATA_INSTALL_DIR}/ksquares )
kde4_install_icons(${ICON_INSTALL_DIR})
