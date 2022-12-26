/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inputcompletermodel.h"
#include "ruqola_debug.h"

#include <QJsonArray>

#include <QIcon>
#include <QModelIndex>

InputCompleterModel::InputCompleterModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

InputCompleterModel::~InputCompleterModel() = default;

void InputCompleterModel::setDefaultUserCompletion()
{
    // Show here/all when we only use "@"
    QVector<Channel> customCompletion;

    Channel here;
    here.setUserName(QStringLiteral("here"));
    here.setStatus(QStringLiteral("online"));
    here.setType(Channel::ChannelType::DirectChannel);
    customCompletion.append(std::move(here));

    Channel all;
    all.setUserName(QStringLiteral("all"));
    all.setStatus(QStringLiteral("online"));
    all.setType(Channel::ChannelType::DirectChannel);
    customCompletion.append(std::move(all));

    setChannels(customCompletion);
}

void InputCompleterModel::setChannels(const QVector<Channel> &channels)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mChannel.count() - 1);
        mChannel.clear();
        endRemoveRows();
    }
    if (!channels.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, channels.count() - 1);
        mChannel = channels;
        endInsertRows();
    }
}

void InputCompleterModel::parseChannels(const QJsonObject &obj)
{
    QVector<Channel> channelList;
    const QJsonArray rooms = obj.value(QLatin1String("rooms")).toArray();
    channelList.reserve(rooms.size());
    for (int i = 0; i < rooms.size(); i++) {
        const QJsonObject o = rooms.at(i).toObject();
        Channel channel;
        channel.parseChannel(o, Channel::ChannelType::Room);
        // Verify that it's valid
        channelList.append(std::move(channel));
    }
    const QJsonArray users = obj.value(QLatin1String("users")).toArray();
    for (int i = 0; i < users.size(); i++) {
        const QJsonObject o = users.at(i).toObject();
        Channel channel;
        channel.parseChannel(o, Channel::ChannelType::DirectChannel);
        // Verify that it's valid
        channelList.append(std::move(channel));
    }
    setChannels(channelList);
}

void InputCompleterModel::clear()
{
    if (!mChannel.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, mChannel.count() - 1);
        mChannel.clear();
        endRemoveRows();
    }
}

int InputCompleterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mChannel.count();
}

QVariant InputCompleterModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mChannel.count()) {
        return {};
    }
    const Channel channel = mChannel.at(index.row());
    switch (role) {
    case InputCompleterModel::DisplayName:
    case Qt::DisplayRole:
        return channelName(channel);
    case InputCompleterModel::CompleterName:
        return completerName(channel);
    case InputCompleterModel::IconName:
    case Qt::DecorationRole:
        return channelIconName(channel);
    case InputCompleterModel::ChannelType:
        return channel.type();
    }
    return {};
}

QString InputCompleterModel::completerName(const Channel &channel) const
{
    // Specific channelId for opening room
    // For private channel we need to use username for channel we need roomId
    switch (channel.type()) {
    case Channel::ChannelType::DirectChannel:
        return channel.userName();
    case Channel::ChannelType::Room:
        return channel.roomName();
    case Channel::ChannelType::Unknown:
        qCWarning(RUQOLA_LOG) << "Unknown channel type!";
        return {};
    }

    return {};
}

QString InputCompleterModel::channelName(const Channel &channel) const
{
    switch (channel.type()) {
    case Channel::ChannelType::DirectChannel: {
        QString text = channel.userName();
        const QString name = channel.name();
        if (!name.isEmpty()) {
            text += QLatin1String(" (") + name + QLatin1Char(')');
        }
        return text;
    }
    case Channel::ChannelType::Room:
        return channel.roomName();
    case Channel::ChannelType::Unknown:
        return channel.name();
    }
    return {};
}

QIcon InputCompleterModel::channelIconName(const Channel &channel) const
{
    switch (channel.type()) {
    case Channel::ChannelType::DirectChannel:
        return QIcon::fromTheme(channel.iconFromStatus());
    case Channel::ChannelType::Room:
        if (channel.roomType() == QLatin1Char('c')) {
            return QIcon::fromTheme(QStringLiteral("irc-channel-inactive"));
        } else if (channel.roomType() == QLatin1Char('p')) {
            return QIcon::fromTheme(QStringLiteral("lock"));
        }
        qCWarning(RUQOLA_LOG) << "Unknown room type!" << channel.roomType();
        return {};
    case Channel::ChannelType::Unknown:
        return {};
    }
    return {};
}
