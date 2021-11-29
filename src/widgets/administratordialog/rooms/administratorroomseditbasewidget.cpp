/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "administratorroomseditbasewidget.h"

AdministratorRoomsEditBaseWidget::AdministratorRoomsEditBaseWidget(QWidget *parent)
    : QWidget{parent}
{
}

AdministratorRoomsEditBaseWidget::~AdministratorRoomsEditBaseWidget()
{
}

QDebug operator<<(QDebug d, const AdministratorRoomsEditBaseWidget::RoomEditInfo &t)
{
    d << "Name " << t.name;
    d << "featured " << t.featured;
    d << "defaultRoom " << t.defaultRoom;
    d << "comment " << t.comment;
    d << "announcement " << t.announcement;
    d << "description " << t.description;
    d << "readOnly " << t.readOnly;
    d << "privateRoom " << t.privateRoom;
    d << "archived " << t.archived;
    return d;
}
