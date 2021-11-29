/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "threadmessagemodel.h"

#include "listmessages.h"

ThreadMessageModel::ThreadMessageModel(const QString &roomID, RocketChatAccount *account, Room *room, QObject *parent)
    : MessageModel(roomID, account, room, parent)
{
}

ThreadMessageModel::~ThreadMessageModel() = default;

QString ThreadMessageModel::threadMessageId() const
{
    return mThreadMessageId;
}

void ThreadMessageModel::setThreadMessageId(const QString &threadMessageId)
{
    mThreadMessageId = threadMessageId;
}

void ThreadMessageModel::parse(const QJsonObject &obj)
{
    ListMessages threadmessages;
    threadmessages.parseMessages(obj);
    mTotal = threadmessages.total();
    addMessages(threadmessages.listMessages());
}

void ThreadMessageModel::parseThreadMessages(const QJsonObject &obj)
{
    clear();
    parse(obj);
}

void ThreadMessageModel::loadMoreThreadMessages(const QJsonObject &obj)
{
    parse(obj);
}

int ThreadMessageModel::total() const
{
    return mTotal;
}

void ThreadMessageModel::setTotal(int total)
{
    mTotal = total;
}
