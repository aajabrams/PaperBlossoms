/*
 * *******************************************************************
 * This file is part of the Paper Blossoms application
 * (https://github.com/dashnine/PaperBlossoms).
 * Copyright (c) 2019 Kyle Hankins (dashnine)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * The Legend of the Five Rings Roleplaying Game is the creation
 * and property of Fantasy Flight Games.
 * *******************************************************************
 */

#ifndef APPVERSION_H
#define APPVERSION_H

#include <QString>

namespace AppVersion{
    constexpr uint MajorVersion = 1;
    constexpr uint MinorVersion = 0;
    constexpr uint PatchVersion = 0;

    static const QString VersionString = QString::asprintf("%i.%i.%i", MajorVersion, MinorVersion, PatchVersion);

    static const QString CompileDateTime = QString(__DATE__) + " " + QString(__TIME__);
}

#endif // APPVERSION_H
