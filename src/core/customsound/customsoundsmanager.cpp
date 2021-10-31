/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "customsoundsmanager.h"

CustomSoundsManager::CustomSoundsManager(QObject *parent)
    : QObject{parent}
{
#if 0 // Core from RC
we had some default sound. No code for loading it directly. It's default list
                this.add({ _id: 'chime', name: 'Chime', extension: 'mp3', src: getURL('sounds/chime.mp3') });
                this.add({ _id: 'door', name: 'Door', extension: 'mp3', src: getURL('sounds/door.mp3') });
                this.add({ _id: 'beep', name: 'Beep', extension: 'mp3', src: getURL('sounds/beep.mp3') });
                this.add({ _id: 'chelle', name: 'Chelle', extension: 'mp3', src: getURL('sounds/chelle.mp3') });
                this.add({ _id: 'ding', name: 'Ding', extension: 'mp3', src: getURL('sounds/ding.mp3') });
                this.add({ _id: 'droplet', name: 'Droplet', extension: 'mp3', src: getURL('sounds/droplet.mp3') });
                this.add({ _id: 'highbell', name: 'Highbell', extension: 'mp3', src: getURL('sounds/highbell.mp3') });
                this.add({ _id: 'seasons', name: 'Seasons', extension: 'mp3', src: getURL('sounds/seasons.mp3') });

#endif
}

CustomSoundsManager::~CustomSoundsManager()
{
}

const CustomSoundsInfo &CustomSoundsManager::customSoundsInfo() const
{
    return mCustomSoundsInfo;
}

void CustomSoundsManager::setCustomSoundsInfo(const CustomSoundsInfo &newCustomSoundsInfo)
{
    mCustomSoundsInfo = newCustomSoundsInfo;
}

void CustomSoundsManager::parseCustomSounds(const QJsonObject &obj)
{
    // TODO
}
