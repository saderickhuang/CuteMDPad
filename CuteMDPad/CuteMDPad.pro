QT       += core gui webchannel webenginewidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG(debug, debug|release) {
    LIBS += "$$PWD\libs\qscintilla2_qt6d.lib"
} else {
    LIBS += "$$PWD\libs\qscintilla2_qt6.lib"
}

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settings.cpp \
    webviewcontent.cpp

HEADERS += \
    Qsci/qsciabstractapis.h \
    Qsci/qsciapis.h \
    Qsci/qscicommand.h \
    Qsci/qscicommandset.h \
    Qsci/qscidocument.h \
    Qsci/qsciglobal.h \
    Qsci/qscilexer.h \
    Qsci/qscilexeravs.h \
    Qsci/qscilexerbash.h \
    Qsci/qscilexerbatch.h \
    Qsci/qscilexercmake.h \
    Qsci/qscilexercoffeescript.h \
    Qsci/qscilexercpp.h \
    Qsci/qscilexercsharp.h \
    Qsci/qscilexercss.h \
    Qsci/qscilexercustom.h \
    Qsci/qscilexerd.h \
    Qsci/qscilexerdiff.h \
    Qsci/qscilexeredifact.h \
    Qsci/qscilexerfortran.h \
    Qsci/qscilexerfortran77.h \
    Qsci/qscilexerhtml.h \
    Qsci/qscilexeridl.h \
    Qsci/qscilexerjava.h \
    Qsci/qscilexerjavascript.h \
    Qsci/qscilexerjson.h \
    Qsci/qscilexerlua.h \
    Qsci/qscilexermakefile.h \
    Qsci/qscilexermarkdown.h \
    Qsci/qscilexermatlab.h \
    Qsci/qscilexeroctave.h \
    Qsci/qscilexerpascal.h \
    Qsci/qscilexerperl.h \
    Qsci/qscilexerpo.h \
    Qsci/qscilexerpostscript.h \
    Qsci/qscilexerpov.h \
    Qsci/qscilexerproperties.h \
    Qsci/qscilexerpython.h \
    Qsci/qscilexerruby.h \
    Qsci/qscilexerspice.h \
    Qsci/qscilexersql.h \
    Qsci/qscilexertcl.h \
    Qsci/qscilexertex.h \
    Qsci/qscilexerverilog.h \
    Qsci/qscilexervhdl.h \
    Qsci/qscilexerxml.h \
    Qsci/qscilexeryaml.h \
    Qsci/qscimacro.h \
    Qsci/qsciprinter.h \
    Qsci/qsciscintilla.h \
    Qsci/qsciscintillabase.h \
    Qsci/qscistyle.h \
    Qsci/qscistyledtext.h \
    mainwindow.h \
    settings.h \
    webviewcontent.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource/Resource.qrc
