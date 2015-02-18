
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
	highlightanimation.cpp
        scoresdialog.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/libKF5KDEGames/highscore)

ki18n_wrap_ui(ksquares_SRCS newgamewidget.ui scoreswidget.ui prefs_ai.ui prefs_display.ui)

kconfig_add_kcfg_files(ksquares_SRCS GENERATE_MOC settings.kcfgc) #GENERATE_MOC is not needed but it doesn't work without it for some reason :S

#kde4_add_app_icon(ksquares_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-apps-ksquares.png")
add_executable(ksquares ${ksquares_SRCS})

target_link_libraries(ksquares  KF5KDEGames KF5::I18n KF5::XmlGui KF5::Completion KF5::DBusAddons)

install(TARGETS ksquares ${INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install( PROGRAMS org.kde.ksquares.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES ksquares.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES ksquaresui.rc  DESTINATION  ${KXMLGUI_INSTALL_DIR}/ksquares )
ecm_install_icons(ICONS hi128-apps-ksquares.png  hi16-apps-ksquares.png  hi22-apps-ksquares.png  hi32-apps-ksquares.png  hi48-apps-ksquares.png  hi64-apps-ksquares.png DESTINATION ${ICON_INSTALL_DIR})
