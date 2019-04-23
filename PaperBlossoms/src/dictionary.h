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

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QString>

namespace Dictionary {
    namespace Common {
        static const QString Clan = "Clan";
        static const QString Curriculum = "Curriculum";
        static const QString Family = "Family";
        static const QString Glory = "Glory";
        static const QString Honor = "Honor";
        static const QString Item = "Item";
        static const QString Ring = "Ring";
        static const QString Rings = "Rings";
        static const QString Role = "Role";
        static const QString School = "School";
        static const QString Skill = "Skill";
        static const QString Skills = "Skills";
        static const QString Strengths = "Strengths";
        static const QString Technique = "Technique";
        static const QString Title = "Title";
        static const QString Weaknesses = "Weaknesses";
    }

    namespace EquipmentRaritySpecialCases {
        static const QString One_Weapon_of_Rarity_6_or_Lower = "One Weapon of Rarity 6 or Lower";
        static const QString Two_Items_of_Rarity_4_or_Lower = "Two Items of Rarity 4 or Lower";
        static const QString Two_Weapons_of_Rarity_6_or_Lower = "Two Weapons of Rarity 6 or Lower";
        static const QString One_Sword_of_Rarity_7_or_Lower = "One Sword of Rarity 7 or Lower";
    }

    namespace Heritages {
        static const QString Wondrous_Work = "Wondrous Work";
        static const QString Dynasty_Builder = "Dynasty Builder";
        static const QString Discovery = "Discovery";
        static const QString Ruthless_Victor = "Ruthless Victor";
        static const QString Elevated_for_Service = "Elevated for Service";
        static const QString Infamous_Builder = "Infamous Builder";
        static const QString Lost_in_the_Darkness = "Lost in the Darkness";
        static const QString Vengeance_for_the_Fallen = "Vengeance for the Fallen";
        static const QString Tewnty_Goblin_Thief = "Tewnty Goblin Thief";
    }

    namespace Items {
        static const QString Armor = "Armor";
        static const QString Weapon = "Weapon";
    }

    namespace Question8Skills {
        static const QString Commerce = "Commerce";
        static const QString Labor = "Labor";
        static const QString Medicine = "Medicine";
        static const QString Seafaring = "Seafaring";
        static const QString Skulduggery = "Skulduggery";
        static const QString Survival = "Survival";
    }

    namespace Resistances {
        static const QString Physical = "Physical";
        static const QString Supernatural = "Supernatural";
    }

    namespace Rings {
        static const QString Void = "Void";
    }

    namespace Schools {
        static const QString KitsuneImpersonatorTradition = "Kitsune Impersonator Tradition";
    }

    namespace Techniques {
        static const QString Mahō = "Mahō";
    }

    namespace Weapons {
        static const QString Swords = "Swords";
    }
}

#endif // DICTIONARY_H
