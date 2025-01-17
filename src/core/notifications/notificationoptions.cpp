/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationoptions.h"
#include "ruqola_debug.h"

NotificationOptions::NotificationOptions() = default;

void NotificationOptions::updateNotificationOptions(const QJsonObject &obj)
{
    parseNotificationOptions(obj);
}

void NotificationOptions::parseNotificationOptions(const QJsonObject &obj)
{
    // qDebug() << " parseNotificationOptions " << obj;
    mHideUnreadStatus = obj.value(QLatin1String("hideUnreadStatus")).toBool();
    mHideMentionStatus = obj.value(QLatin1String("hideMentionStatus")).toBool();
    mDisableNotifications = obj.value(QLatin1String("disableNotifications")).toBool();

    mAudioNotificationValue = obj.value(QLatin1String("audioNotificationValue")).toString();

    //"desktopNotificationDuration":0,"desktopNotifications":"mentions"
    mDesktopNotifications =
        NotificationValue{obj.value(QLatin1String("desktopNotifications")).toString(), obj.value(QLatin1String("desktopPrefOrigin")).toString()};
    //"mobilePushNotifications":"nothing"
    mMobilePushNotification =
        NotificationValue{obj.value(QLatin1String("mobilePushNotifications")).toString(), obj.value(QLatin1String("mobilePrefOrigin")).toString()};
    //"emailNotifications":"default"
    mEmailNotifications = NotificationValue{obj.value(QLatin1String("emailNotifications")).toString(), obj.value(QLatin1String("emailPrefOrigin")).toString()};
    //"unreadAlert":"nothing"
    mUnreadTrayIconAlert = obj.value(QLatin1String("unreadAlert")).toString();
    mMuteGroupMentions = obj.value(QLatin1String("muteGroupMentions")).toBool();
}

QString NotificationOptions::audioNotificationValue() const
{
    return mAudioNotificationValue;
}

void NotificationOptions::setAudioNotificationValue(const QString &audioNotificationValue)
{
    mAudioNotificationValue = audioNotificationValue;
}

QJsonObject NotificationOptions::serialize(const NotificationOptions &options)
{
    QJsonObject obj;
    obj[QLatin1String("audioNotificationValue")] = options.audioNotificationValue();
    obj[QLatin1String("disableNotifications")] = options.disableNotifications();
    obj[QLatin1String("desktopNotifications")] = options.desktopNotifications().currentValue();
    obj[QLatin1String("mobilePushNotifications")] = options.mobilePushNotification().currentValue();
    obj[QLatin1String("emailNotifications")] = options.emailNotifications().currentValue();
    obj[QLatin1String("unreadAlert")] = options.unreadTrayIconAlert();
    obj[QLatin1String("hideUnreadStatus")] = options.hideUnreadStatus();
    obj[QLatin1String("muteGroupMentions")] = options.muteGroupMentions();
    obj[QLatin1String("hideMentionStatus")] = options.hideMentionStatus();
    return obj;
}

NotificationOptions NotificationOptions::deserialize(const QJsonObject &o)
{
    qCWarning(RUQOLA_LOG) << "Not implemented yet";
    NotificationOptions options;
    options.setAudioNotificationValue(o[QLatin1String("audioNotificationValue")].toString());
    options.setDisableNotifications(o[QLatin1String("disableNotifications")].toBool());
    options.setUnreadTrayIconAlert(o[QLatin1String("unreadAlert")].toString());
    options.setHideUnreadStatus(o[QLatin1String("hideUnreadStatus")].toBool());
    options.setMuteGroupMentions(o[QLatin1String("muteGroupMentions")].toBool());
    options.setHideMentionStatus(o[QLatin1String("hideMentionStatus")].toBool());
    options.setDesktopNotifications(NotificationValue(o[QLatin1String("desktopNotifications")].toString(), QString()));
    options.setMobilePushNotification(NotificationValue(o[QLatin1String("mobilePushNotifications")].toString(), QString()));
    options.setEmailNotifications(NotificationValue(o[QLatin1String("emailNotifications")].toString(), QString()));
    return options;
}

bool NotificationOptions::hideUnreadStatus() const
{
    return mHideUnreadStatus;
}

void NotificationOptions::setHideUnreadStatus(bool value)
{
    mHideUnreadStatus = value;
}

bool NotificationOptions::disableNotifications() const
{
    return mDisableNotifications;
}

void NotificationOptions::setDisableNotifications(bool disableNotifications)
{
    mDisableNotifications = disableNotifications;
}

QString NotificationOptions::unreadTrayIconAlert() const
{
    return mUnreadTrayIconAlert;
}

void NotificationOptions::setUnreadTrayIconAlert(const QString &unreadTrayIconAlert)
{
    mUnreadTrayIconAlert = unreadTrayIconAlert;
}

NotificationOptions::NotificationValue NotificationOptions::emailNotifications() const
{
    return mEmailNotifications;
}

void NotificationOptions::setEmailNotifications(const NotificationValue &emailNotifications)
{
    mEmailNotifications = emailNotifications;
}

NotificationOptions::NotificationValue NotificationOptions::mobilePushNotification() const
{
    return mMobilePushNotification;
}

void NotificationOptions::setMobilePushNotification(const NotificationValue &mobilePushNotification)
{
    mMobilePushNotification = mobilePushNotification;
}

NotificationOptions::NotificationValue NotificationOptions::desktopNotifications() const
{
    return mDesktopNotifications;
}

void NotificationOptions::setDesktopNotifications(const NotificationValue &desktopNotifications)
{
    mDesktopNotifications = desktopNotifications;
}

bool NotificationOptions::operator==(const NotificationOptions &other) const
{
    return (mDesktopNotifications == other.desktopNotifications()) && (mMobilePushNotification == other.mobilePushNotification())
        && (mEmailNotifications == other.emailNotifications()) && (mUnreadTrayIconAlert == other.unreadTrayIconAlert())
        && (mDisableNotifications == other.disableNotifications()) && (mHideUnreadStatus == other.hideUnreadStatus())
        && (mAudioNotificationValue == other.audioNotificationValue()) && (mMuteGroupMentions == other.muteGroupMentions())
        && (mHideMentionStatus == other.hideMentionStatus());
}

bool NotificationOptions::operator!=(const NotificationOptions &other) const
{
    return !operator==(other);
}

bool NotificationOptions::muteGroupMentions() const
{
    return mMuteGroupMentions;
}

void NotificationOptions::setMuteGroupMentions(bool muteGroupMentions)
{
    mMuteGroupMentions = muteGroupMentions;
}

bool NotificationOptions::hideMentionStatus() const
{
    return mHideMentionStatus;
}

void NotificationOptions::setHideMentionStatus(bool newHideMentionStatus)
{
    mHideMentionStatus = newHideMentionStatus;
}

QDebug operator<<(QDebug d, const NotificationOptions &t)
{
    d << "mAudioNotificationValue: " << t.audioNotificationValue();
    d << "mDesktopNotifications: " << t.desktopNotifications();
    d << "mMobilePushNotification: " << t.mobilePushNotification();
    d << "mEmailNotifications: " << t.emailNotifications();
    d << "mDisableNotifications: " << t.disableNotifications();
    d << "hideUnreadStatus: " << t.hideUnreadStatus();
    d << "unreadTrayIconAlert: " << t.unreadTrayIconAlert();
    d << "mMuteGroupMentions: " << t.muteGroupMentions();
    d << "mHideMentionStatus: " << t.hideMentionStatus();
    return d;
}

QDebug operator<<(QDebug d, const NotificationOptions::NotificationValue &t)
{
    d << " value " << t.value;
    d << " preferenceOrigin " << t.preferenceOrigin;
    return d;
}

bool NotificationOptions::NotificationValue::operator==(const NotificationValue &other) const
{
    return other.preferenceOrigin == preferenceOrigin && other.value == value;
}

QString NotificationOptions::NotificationValue::currentValue() const
{
    if (preferenceOrigin == QLatin1String("subscription") && !value.isEmpty()) {
        return value;
    }
    // Keep compatibility
    if (preferenceOrigin.isEmpty() && !value.isEmpty()) {
        return value;
    }
    if (preferenceOrigin.isEmpty() && value.isEmpty()) {
        return QString();
    }
    return QStringLiteral("default");
}

bool NotificationOptions::NotificationValue::isEmpty() const
{
    return preferenceOrigin.isEmpty() && value.isEmpty();
}

#include "moc_notificationoptions.cpp"
