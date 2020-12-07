/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef TEXTTOSPEECHTEXTINTERFACE_H
#define TEXTTOSPEECHTEXTINTERFACE_H

#include "room/plugins/plugintextinterface.h"
class QTextToSpeech;
class TextToSpeechTextInterface : public PluginTextInterface
{
    Q_OBJECT
public:
    explicit TextToSpeechTextInterface(QObject *parent = nullptr);
    ~TextToSpeechTextInterface() override;

    void addAction(QMenu *menu) override;

    void setSelectedText(const QString &str) override;

private:
    void slotSpeakText();
    QString mSelectedText;
    QTextToSpeech *mTextToSpeech = nullptr;
};

#endif // TEXTTOSPEECHTEXTINTERFACE_H
