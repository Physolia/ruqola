/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "moderation/moderationreportinfos.h"
#include <QWidget>

class ModerationReportInfoWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationMessageInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModerationMessageInfoWidget(QWidget *parent = nullptr);
    ~ModerationMessageInfoWidget() override;

    void setReportInfos(const ModerationReportInfos &info);

private:
    ModerationReportInfoWidget *const mModerationReportInfoWidget;
};
