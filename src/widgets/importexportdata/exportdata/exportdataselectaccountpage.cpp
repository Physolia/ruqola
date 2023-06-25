/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdataselectaccountpage.h"
#include "exportdataselectaccountlistwidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

ExportDataSelectAccountPage::ExportDataSelectAccountPage(QWidget *parent)
    : QWizardPage(parent)
    , mExportDataSelectAccountListWidget(new ExportDataSelectAccountListWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto label = new QLabel(i18n("Select Accounts:"), this);
    label->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(label);

    mExportDataSelectAccountListWidget->setObjectName(QStringLiteral("mExportDataSelectAccountListWidget"));
    mainLayout->addWidget(mExportDataSelectAccountListWidget);
}

ExportDataSelectAccountPage::~ExportDataSelectAccountPage() = default;

void ExportDataSelectAccountPage::setAccountList(const QVector<ImportExportUtils::AccountImportExportInfo> &lst)
{
    mExportDataSelectAccountListWidget->setAccountList(lst);
}

QVector<ImportExportUtils::AccountImportExportInfo> ExportDataSelectAccountPage::selectedAccounts() const
{
    return mExportDataSelectAccountListWidget->selectedAccounts();
}

#include "moc_exportdataselectaccountpage.cpp"