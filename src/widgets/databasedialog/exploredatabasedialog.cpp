/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasedialog.h"
#include "exploredatabasewidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
const char myExploreDatabaseDialogConfigGroupName[] = "ExploreDatabaseDialog";
}
ExploreDatabaseDialog::ExploreDatabaseDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mExploreDatabaseWidget(new ExploreDatabaseWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Database Info"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mExploreDatabaseWidget->setObjectName(QStringLiteral("mExploreDatabaseWidget"));
    mainLayout->addWidget(mExploreDatabaseWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ExploreDatabaseDialog::reject);

    readConfig();
}

ExploreDatabaseDialog::~ExploreDatabaseDialog()
{
    writeConfig();
}

void ExploreDatabaseDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myExploreDatabaseDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ExploreDatabaseDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myExploreDatabaseDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_exploredatabasedialog.cpp"
