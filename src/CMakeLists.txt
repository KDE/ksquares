
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

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*-apps-ksquares.png")
ecm_add_app_icon(ksquares_SRCS ICONS ${ICONS_SRCS})
add_executable(ksquares ${ksquares_SRCS})

target_link_libraries(ksquares
    KF5::Completion
    KF5::Crash
    KF5::DBusAddons
    KF5::I18n
    KF5::XmlGui
    KF5KDEGames
)

install(TARGETS ksquares ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install(PROGRAMS org.kde.ksquares.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES org.kde.ksquares.appdata.xml DESTINATION  ${KDE_INSTALL_METAINFODIR})
install(FILES ksquares.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR})
install(FILES ksquaresui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/ksquares)

ecm_install_icons(ICONS
    128-apps-ksquares.png
    16-apps-ksquares.png
    22-apps-ksquares.png
    32-apps-ksquares.png
    48-apps-ksquares.png
    64-apps-ksquares.png
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor
)
