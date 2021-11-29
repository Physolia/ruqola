/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "webshortcuttextplugin.h"
#include "webshortcuttextinterface.h"
#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(WebShortcutTextPlugin, "ruqola_webshortcuttextplugin.json")

WebShortcutTextPlugin::WebShortcutTextPlugin(QObject *parent, const QVariantList &)
    : PluginText(parent)
{
}

WebShortcutTextPlugin::~WebShortcutTextPlugin() = default;

PluginTextInterface *WebShortcutTextPlugin::createInterface(QObject *parent)
{
    return new WebShortcutTextInterface(parent);
}

#include "webshortcuttextplugin.moc"
