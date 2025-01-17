/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plugintextmessagewidget.h"
#include <QDesktopServices>
#include <QUrl>

PluginTextMessageWidget::PluginTextMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(true);
    connect(this, &KMessageWidget::linkActivated, this, [](const QString &contents) {
        QDesktopServices::openUrl(QUrl(contents));
    });
}

PluginTextMessageWidget::~PluginTextMessageWidget() = default;

void PluginTextMessageWidget::slotShareError(const QString &message)
{
    setMessageType(KMessageWidget::MessageType::Error);
    setText(message);
    animatedShow();
}

void PluginTextMessageWidget::slotShareSuccess(const QString &message)
{
    setMessageType(KMessageWidget::MessageType::Information);
    setText(message);
    animatedShow();
}

#include "moc_plugintextmessagewidget.cpp"
