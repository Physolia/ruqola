/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportuserinfo.h"
#include "utils.h"
using namespace Qt::Literals::StringLiterals;
ModerationReportUserInfo::ModerationReportUserInfo() = default;

ModerationReportUserInfo::~ModerationReportUserInfo() = default;

bool ModerationReportUserInfo::operator==(const ModerationReportUserInfo &other) const
{
    return other.description() == mDescription && other.reportId() == mReportId && other.reportedUser() == mReportedUser && other.reportedBy() == mReportedBy
        && mTimeStamp == other.timeStamp();
}

void ModerationReportUserInfo::parseModerationReportUserInfo(const QJsonObject &o)
{
    mReportId = o["_id"_L1].toString().toLatin1();
    mDescription = o["description"_L1].toString();
    mReportedBy.parseUserRestApi(o["reportedBy"_L1].toObject(), {}); // TODO use role ???
    mReportedUser.parseUserRestApi(o["reportedUser"_L1].toObject(), {}); // TODO use role ???
    setTimeStamp(Utils::parseIsoDate(QStringLiteral("ts"), o));
}

QString ModerationReportUserInfo::description() const
{
    return mDescription;
}

void ModerationReportUserInfo::setDescription(const QString &newDescription)
{
    mDescription = newDescription;
}

QByteArray ModerationReportUserInfo::reportId() const
{
    return mReportId;
}

void ModerationReportUserInfo::setReportId(const QByteArray &newReportId)
{
    mReportId = newReportId;
}

User ModerationReportUserInfo::reportedBy() const
{
    return mReportedBy;
}

void ModerationReportUserInfo::setReportedBy(const User &newReportedBy)
{
    mReportedBy = newReportedBy;
}

User ModerationReportUserInfo::reportedUser() const
{
    return mReportedUser;
}

void ModerationReportUserInfo::setReportedUser(const User &newReportedUser)
{
    mReportedUser = newReportedUser;
}

qint64 ModerationReportUserInfo::timeStamp() const
{
    return mTimeStamp;
}

void ModerationReportUserInfo::setTimeStamp(qint64 newTimeStamp)
{
    mTimeStamp = newTimeStamp;
}

QDebug operator<<(QDebug d, const ModerationReportUserInfo &t)
{
    d << "description: " << t.description();
    d << "reportId: " << t.reportId();
    d << "reportedUser " << t.reportedUser();
    d << "reportedBy " << t.reportedBy();
    d << "timeStamp " << t.timeStamp();
    return d;
}
