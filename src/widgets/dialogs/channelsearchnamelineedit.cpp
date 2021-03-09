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
#include "channelsearchnamelineedit.h"
#include "model/channelcompleterfilterproxymodel.h"
#include "model/channelcompletermodel.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "spotlightjob.h"

#include <QJsonObject>

ChannelSearchNameLineEdit::ChannelSearchNameLineEdit(QWidget *parent)
    : CompletionLineEdit(parent)
    , mChannelCompleterFilterProxyModel(new ChannelCompleterFilterProxyModel(this))
    , mChannelCompleterModel(new ChannelCompleterModel(this))
{
    mChannelCompleterFilterProxyModel->setSourceModel(mChannelCompleterModel);
    connect(this, &QLineEdit::textChanged, this, &ChannelSearchNameLineEdit::slotTextChanged);
    setCompletionModel(mChannelCompleterFilterProxyModel);
    connect(this, &ChannelSearchNameLineEdit::complete, this, &ChannelSearchNameLineEdit::slotComplete);
}

ChannelSearchNameLineEdit::~ChannelSearchNameLineEdit()
{
}

void ChannelSearchNameLineEdit::slotTextChanged(const QString &text)
{
    if (!text.trimmed().isEmpty()) {
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        auto job = new RocketChatRestApi::SpotlightJob(this);
        job->setSearchPattern(text);
        rcAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::SpotlightJob::spotlightDone, this, &ChannelSearchNameLineEdit::slotSpotlightDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start searchRoomUser job";
        }
    } else {
        mChannelCompleterModel->clear();
    }
}

void ChannelSearchNameLineEdit::slotSpotlightDone(const QJsonObject &obj)
{
    qDebug() << "slotSpotlightDone  " << obj;
    // TODO
    mChannelCompleterModel->insertChannels({});
    // TODO parse element
}

void ChannelSearchNameLineEdit::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(ChannelCompleterModel::RoomName).toString();
    const QString roomId = index.data(ChannelCompleterModel::ChannelId).toString();
    ChannelCompletionInfo info;
    info.channelName = completerName;
    info.channelId = roomId;
    //    mCompletionListView->hide();
    //    disconnect(this, &QLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotTextChanged);
    //    Q_EMIT newRoomName(info);
    //    clear();
    //    connect(this, &QLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotTextChanged);
}
