/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "conferencecallwidget.h"
#include "libruqolawidgets_private_export.h"
#include "videoconference/videoconference.h"
#include <QDialog>
class RocketChatAccount;
class ConferenceCallWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConferenceDirectCallDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConferenceDirectCallDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ConferenceDirectCallDialog() override;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &newRoomId);

    [[nodiscard]] bool allowRinging() const;
    void setAllowRinging(bool newAllowRinging);

private:
    void cancelCall();
    void slotRejected();
    void slotStartVideoConference();
    void callUser();
    void slotVideoConferenceAccepted(const VideoConference &videoConference);
    void slotVideoConferenceCanceled(const VideoConference &videoConference);
    QString mRoomId;
    QString mCallId;
    ConferenceCallWidget *const mConferenceCallWidget;
    RocketChatAccount *const mRocketChatAccount;
    QPushButton *mOkButton = nullptr;
    int mNumberOfCall = 0;
    bool mAllowRinging = false;
    bool mWasAccepted = false;
};
