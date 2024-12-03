ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX KSQUARES
    VERSION_HEADER ksquares_version.h
)

add_executable(ksquares)

target_sources(ksquares PRIVATE
    aicontroller.cpp
    aicontroller.h
    gameboardscene.cpp
    gameboardscene.h
    gameboardview.cpp
    gameboardview.h
    highlightanimation.cpp
    highlightanimation.h
    ksquaresdemowindow.cpp
    ksquaresdemowindow.h
    ksquaresgame.cpp
    ksquaresgame.h
    ksquaresplayer.h
    ksquareswindow.cpp
    ksquareswindow.h
    main.cpp
    newgamedialog.cpp
    newgamedialog.h
    scoresdialog.cpp
    scoresdialog.h
    themegraphicsitems.cpp
    themegraphicsitems.h

    ksquares.qrc
)

if(APPLE)
    set_target_properties(ksquares PROPERTIES
	    MACOSX_BUNDLE_DISPLAY_NAME "KSquares"
        MACOSX_BUNDLE_BUNDLE_NAME "KSquares"
       MACOSX_BUNDLE_LONG_VERSION_STRING "KSquares ${RELEASE_SERVICE_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}"
        MACOSX_BUNDLE_BUNDLE_VERSION "${RELEASE_SERVICE_VERSION}"
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.kde.ksquares"
        MACOSX_BUNDLE_COPYRIGHT "2024 The KSquares Developers")
endif()



ki18n_wrap_ui(ksquares
    newgamewidget.ui
    scoreswidget.ui
    prefs_ai.ui
    prefs_display.ui
)

kconfig_add_kcfg_files(ksquares settings.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*-apps-ksquares.png")
ecm_add_app_icon(ksquares ICONS ${ICONS_SRCS})

target_link_libraries(ksquares
    KDEGames6
    KF6::CoreAddons
    KF6::Completion
    KF6::Crash
    KF6::DBusAddons
    KF6::I18n
    KF6::XmlGui
)

install(TARGETS ksquares ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(PROGRAMS org.kde.ksquares.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES org.kde.ksquares.appdata.xml DESTINATION  ${KDE_INSTALL_METAINFODIR})
install(FILES ksquares.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR})

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
