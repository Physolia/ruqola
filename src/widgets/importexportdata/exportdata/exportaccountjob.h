/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "importexportdata/importexportutils.h"
#include "libruqolawidgets_private_export.h"

#include <QObject>
class KZip;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExportAccountJob : public QObject
{
    Q_OBJECT
public:
    explicit ExportAccountJob(const QString &fileName, QObject *parent = nullptr);
    ~ExportAccountJob() override;

    void start();

    Q_REQUIRED_RESULT QVector<ImportExportUtils::AccountImportExportInfo> listAccounts() const;
    void setListAccounts(const QVector<ImportExportUtils::AccountImportExportInfo> &newListAccounts);

    Q_REQUIRED_RESULT bool canStart() const;

Q_SIGNALS:
    void exportDone();
    void exportFailed(const QString &errorMsg);

private:
    void storeDirectory(const QString &subDirectory, const QString &subfolderPath);
    void exportAccount(const ImportExportUtils::AccountImportExportInfo &info);
    QVector<ImportExportUtils::AccountImportExportInfo> mListAccounts;
    KZip *const mArchive;
};