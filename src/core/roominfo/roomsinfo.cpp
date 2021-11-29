/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "roominfo/roomsinfo.h"
#include "ruqola_debug.h"
#include <QJsonArray>
#include <QJsonObject>

RoomsInfo::RoomsInfo() = default;

bool RoomsInfo::isEmpty() const
{
    return mRooms.isEmpty();
}

void RoomsInfo::clear()
{
    mRooms.clear();
}

int RoomsInfo::count() const
{
    return mRooms.count();
}

RoomInfo RoomsInfo::at(int index) const
{
    return mRooms.at(index);
}

void RoomsInfo::parseMoreRooms(const QJsonObject &obj, RoomsInfo::ParseType type)
{
    const int adminRoomsCount = obj[QStringLiteral("count")].toInt();
    mOffset = obj[QStringLiteral("offset")].toInt();
    mTotal = obj[QStringLiteral("total")].toInt();
    parseListRooms(obj, type);
    mRoomsCount += adminRoomsCount;
}

void RoomsInfo::parseListRooms(const QJsonObject &adminRoomsObj, RoomsInfo::ParseType type)
{
    QString jsonKeyType;
    switch (type) {
    case Administrator:
        jsonKeyType = QStringLiteral("rooms");
        break;
    case Directory:
        jsonKeyType = QStringLiteral("result");
        break;
    }

    const QJsonArray adminRoomsArray = adminRoomsObj[jsonKeyType].toArray();
    mRooms.reserve(mRooms.count() + adminRoomsArray.count());
    for (const QJsonValue &current : adminRoomsArray) {
        if (current.type() == QJsonValue::Object) {
            const QJsonObject adminRoomObject = current.toObject();
            RoomInfo m;
            m.parseRoomInfo(adminRoomObject);
            mRooms.append(m);
        } else {
            qCWarning(RUQOLA_LOG) << "Problem when parsing Rooms" << current;
        }
    }
}

int RoomsInfo::roomsCount() const
{
    return mRoomsCount;
}

void RoomsInfo::setRoomsCount(int count)
{
    mRoomsCount = count;
}

RoomInfo RoomsInfo::takeAt(int index)
{
    return mRooms.takeAt(index);
}

QVector<RoomInfo> RoomsInfo::rooms() const
{
    return mRooms;
}

void RoomsInfo::setRooms(const QVector<RoomInfo> &rooms)
{
    mRooms = rooms;
}

void RoomsInfo::parseRooms(const QJsonObject &obj, RoomsInfo::ParseType type)
{
    mRoomsCount = obj[QStringLiteral("count")].toInt();
    mOffset = obj[QStringLiteral("offset")].toInt();
    mTotal = obj[QStringLiteral("total")].toInt();
    mRooms.clear();
    parseListRooms(obj, type);
}

int RoomsInfo::offset() const
{
    return mOffset;
}

void RoomsInfo::setOffset(int offset)
{
    mOffset = offset;
}

int RoomsInfo::total() const
{
    return mTotal;
}

void RoomsInfo::setTotal(int total)
{
    mTotal = total;
}

QDebug operator<<(QDebug d, const RoomsInfo &t)
{
    d << "total " << t.total();
    d << "offset " << t.offset();
    d << "roomsCount " << t.roomsCount() << "\n";
    for (int i = 0, total = t.rooms().count(); i < total; ++i) {
        d << t.rooms().at(i) << "\n";
    }
    return d;
}
