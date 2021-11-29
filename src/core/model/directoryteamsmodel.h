/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "directorybasemodel.h"
#include "libruqolacore_export.h"
#include "roominfo/roomsinfo.h"

class LIBRUQOLACORE_EXPORT DirectoryTeamsModel : public DirectoryBaseModel
{
    Q_OBJECT
public:
    enum DirectoryTeamsRoles {
        TeamName,
        RoomsCount,
        TeamIdentifier,
        CreateTeamDateTime,
        CreateTeam,
        LastColumn = CreateTeam,
    };
    Q_ENUM(DirectoryTeamsRoles)
    explicit DirectoryTeamsModel(QObject *parent = nullptr);
    ~DirectoryTeamsModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent = {}) const override;

    void parseElements(const QJsonObject &obj) override;
    void addMoreElements(const QJsonObject &obj) override;

    Q_REQUIRED_RESULT int total() const override;

protected:
    Q_REQUIRED_RESULT QList<int> hideColumns() const override;

private:
    Q_DISABLE_COPY(DirectoryTeamsModel)
    void checkFullList();
    RoomsInfo mRoomsInfo;
};
