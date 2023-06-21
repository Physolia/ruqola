/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdatawizard.h"
#include "exportdatafinishpage.h"
#include "exportdataselectaccountpage.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>

#include <QWindow>

namespace
{
static const char myConfigExportDataWizardGroupName[] = "ExportDataWizard";
}
ExportDataWizard::ExportDataWizard(QWidget *parent)
    : QWizard(parent)
    , mExportDataSelectAccountPage(new ExportDataSelectAccountPage(this))
    , mExportDataFinishPage(new ExportDataFinishPage(this))
{
    setWindowTitle(i18nc("@title:window", "Export Accounts"));

    mExportDataSelectAccountPage->setObjectName(QStringLiteral("mExportDataSelectAccountPage"));
    mExportDataFinishPage->setObjectName(QStringLiteral("mExportDataFinishPage"));

    setPage(SelectAccountPage, mExportDataSelectAccountPage);
    setPage(FinishPage, mExportDataFinishPage);

    readConfig();
}

ExportDataWizard::~ExportDataWizard()
{
    writeConfig();
}

void ExportDataWizard::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigExportDataWizardGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ExportDataWizard::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigExportDataWizardGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_exportdatawizard.cpp"
