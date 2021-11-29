/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "texttospeechtextinterface.h"

#include <KLocalizedString>

#include <QAction>
#include <QIcon>
#include <QMenu>

#include <QTextToSpeech>

TextToSpeechTextInterface::TextToSpeechTextInterface(QObject *parent)
    : PluginTextInterface(parent)
    , mTextToSpeech(new QTextToSpeech(this))
{
}

TextToSpeechTextInterface::~TextToSpeechTextInterface() = default;

void TextToSpeechTextInterface::addAction(QMenu *menu)
{
    menu->addSeparator();
    QAction *speakAction = menu->addAction(i18n("Speak Text"));
    speakAction->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-text-to-speech")));
    connect(speakAction, &QAction::triggered, this, &TextToSpeechTextInterface::slotSpeakText);
}

void TextToSpeechTextInterface::setSelectedText(const QString &str)
{
    mSelectedText = str;
}

void TextToSpeechTextInterface::slotSpeakText()
{
    mTextToSpeech->say(mSelectedText);
}
