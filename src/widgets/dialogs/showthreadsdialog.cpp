/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "showthreadsdialog.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>

namespace
{
static const char myShowThreadsDialogGroupName[] = "ShowThreadsDialog";
}
ShowThreadsDialog::ShowThreadsDialog(RocketChatAccount *account, QWidget *parent)
    : ShowListMessageBaseDialog(account, parent)
{
    setWindowTitle(i18nc("@title:window", "Show Threads Messages"));
    readConfig();
}

ShowThreadsDialog::~ShowThreadsDialog()
{
    writeConfig();
}

void ShowThreadsDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowThreadsDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ShowThreadsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowThreadsDialogGroupName);
    group.writeEntry("Size", size());
}
