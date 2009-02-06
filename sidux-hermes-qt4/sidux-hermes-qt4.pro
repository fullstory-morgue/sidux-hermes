
TEMPLATE = app
CONFIG += qt

CODECFORSRC = UTF-8
CODECFORTR = ISO-8859-1

OBJECTS_DIR = build/
MOC_DIR = build/

DESTDIR = .

SOURCES += src/main.cpp \
 src/systray.cpp \
 src/upgradablePackages.cpp

TRANSLATIONS = translations/sidux-hermes-qt4_da.ts \
 translations/sidux-hermes-qt4_de.ts \
 translations/sidux-hermes-qt4_el.ts \
 translations/sidux-hermes-qt4_es.ts \
 translations/sidux-hermes-qt4_fr.ts \
 translations/sidux-hermes-qt4_hr.ts \
 translations/sidux-hermes-qt4_it.ts \
 translations/sidux-hermes-qt4_ja.ts \
 translations/sidux-hermes-qt4_nl.ts \
 translations/sidux-hermes-qt4_pl.ts \
 translations/sidux-hermes-qt4_pt.ts \
 translations/sidux-hermes-qt4_ro.ts \
 translations/sidux-hermes-qt4_ru.ts


target.path = /usr/bin
shortcuttarget.path = /usr/share/
shortcuttarget.files = applications
langtarget.path = /usr/share/sidux-hermes/translations/
langtarget.files = translations/*.qm

INSTALLS += target shortcuttarget langtarget

HEADERS += src/systray.h \
 src/upgradablePackages.h
TARGET = sidux-hermes-qt4

FORMS += src/upgradablePackages.ui