/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "authenticationinfo.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class KMessageWidget;
class KBusyIndicatorWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewServerCheckUrlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateNewServerCheckUrlWidget(QWidget *parent = nullptr);
    ~CreateNewServerCheckUrlWidget() override;

Q_SIGNALS:
    void serverUrlFound(const QString &url, const QVector<AuthenticationInfo> authenticationInfos, bool canResetPassword);

private:
    void slotTestConnection();
    void slotErrorConnection();
    QLineEdit *const mServerUrl;
    KBusyIndicatorWidget *const mBusyIndicatorWidget;
    KMessageWidget *const mFailedError;
};
