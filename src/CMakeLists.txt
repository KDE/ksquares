PROJECT(ksquares)
FIND_PACKAGE(KDE4 REQUIRED)
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(ksquares_SRCS
	ksquares.cpp
	main.cpp
	gameboardview.cpp
	gameboardscene.cpp
	ksquaresgame.cpp
	newgamedialog.cpp
	scoresdialog.cpp
)

kde4_automoc(${ksquares_SRCS})

kde4_add_ui_files(ksquares_SRCS newgamedialog.ui scoresdialog.ui prefs_base.ui)

kde4_add_kcfg_files(ksquares_SRCS GENERATE_MOC settings.kcfgc) #GENERATE_MOC is not needed but it doesn't work without it for some reason :S

kde4_add_executable(ksquares ${ksquares_SRCS})

target_link_libraries(ksquares ${KDE4_KDEUI_LIBS})

install(TARGETS ksquares DESTINATION ${BIN_INSTALL_DIR})


########### install files ###############

install( FILES ksquares.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES ksquares.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES ksquaresui.rc  DESTINATION  ${DATA_INSTALL_DIR}/ksquares )
