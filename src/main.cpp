/*  Niemeyer Top Wrapper
    
    Copyright (C) 2019 Pedro Henrique Lara Campos <root@pedrohlc.com>
    Distributed under the GPL v2. For full terms see the file LICENSE.
*/

#include <QtWidgets/QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include <QQmlComponent>
#include <QQuickView>
#include <QtQuickControls2/QQuickStyle>
#include <QTranslator>

#include "niemeyer.hpp"
#include "lib/translations.hpp"
#include "lib/keymap.hpp"
#include "lib/network.hpp"
#include "lib/mirrors.hpp"
#include "lib/locales.hpp"
#include "lib/user.hpp"
#include "lib/diskmgr.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QUrl appPath(app.applicationDirPath());

    // I coded this to use Material, but never enabled it.
    // When I did I decided to keep it in pure Qt.
    //QQuickStyle::setStyle("Material");
    
    // Get view and context
    QQuickView view;
    QQmlContext *context = view.rootContext();

    // Set translator
    QTranslator* translator = new QTranslator(&app);
    app.installTranslator(translator);
    
    // Components
    Translations * m_translations = new Translations(&app, view.engine(), &view);
    Keymap * m_keymap = new Keymap(&view);
    Network * m_net = new Network(&view);
    Mirrors * m_mirrors = new Mirrors(&view);
    Locales * m_locales = new Locales(&view);
    User * m_user = new User(&view);
    DiskMgr * m_disks = new DiskMgr(&view);

    // Context props
    context->setContextProperty(QStringLiteral("translations"), m_translations);
    context->setContextProperty(QStringLiteral("keymap"), m_keymap);
    context->setContextProperty(QStringLiteral("net"), m_net);
    context->setContextProperty(QStringLiteral("mirrors"), m_mirrors);
    context->setContextProperty(QStringLiteral("locales"), m_locales);
    context->setContextProperty(QStringLiteral("user"), m_user);
    context->setContextProperty(QStringLiteral("disks"), m_disks);
    context->setContextProperty(QStringLiteral("assetsPath"), appPath.resolved(QStringLiteral(ASSETS_PATH)));
    
    // Some connections
    QObject::connect(view.engine(), &QQmlApplicationEngine::quit, &QGuiApplication::quit);

    // Show view
    view.setSource(appPath.resolved(QStringLiteral(QML_PATH "MainContainer.qml")));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();

    return app.exec();
}
