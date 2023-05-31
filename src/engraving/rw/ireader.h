/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MU_ENGRAVING_IREADER_H
#define MU_ENGRAVING_IREADER_H

#include <memory>
#include <variant>

#include "engravingerrors.h"

namespace mu::engraving {
class XmlReader;
class Score;
class EngravingItem;

class ChordRest;
class Harmony;
class Tuplet;
}

namespace mu::engraving::rw {
struct ReadInOutData;
class IReader
{
public:
    virtual ~IReader() = default;

    virtual Err readScore(Score* score, XmlReader& xml, rw::ReadInOutData* out) = 0;

    using Supported = std::variant<std::monostate,
                                   ChordRest*,
                                   Harmony*,
                                   Tuplet*
                                   >;

    template<typename T>
    static void check_supported_static(T item)
    {
        if constexpr (std::is_same<T, EngravingItem*>::value) {
            // noop
        } else {
            Supported check(item);
            (void)check;
        }
    }

    template<typename T>
    void readItem(T item, XmlReader& xml)
    {
#ifndef NDEBUG
        check_supported_static(item);
#endif
        doReadItem(static_cast<EngravingItem*>(item), xml);
    }

private:
    virtual void doReadItem(EngravingItem* item, XmlReader& xml) = 0;
};

using IReaderPtr = std::shared_ptr<IReader>;
}

#endif // MU_ENGRAVING_IREADER_H