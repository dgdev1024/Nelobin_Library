///
/// \file   Entry.cpp
///

#include <Nelobin/Entry.hpp>

namespace Nelobin
{

    String stringifyEntryBase (EntryBase &a_entry)
    {
        switch (a_entry.getAlias())
        {
            case Datatypes::SignedByte:
            {
                auto* lp_cast = dynamic_cast<Entry<S8>*>(&a_entry);
                return std::to_string(lp_cast->get());
            }
            case Datatypes::SignedShort:
            {
                auto* lp_cast = dynamic_cast<Entry<S16>*>(&a_entry);
                return std::to_string(lp_cast->get());
            }
            case Datatypes::SignedInteger:
            {
                auto* lp_cast = dynamic_cast<Entry<S32>*>(&a_entry);
                return std::to_string(lp_cast->get());
            }
            case Datatypes::UnsignedByte:
            {
                auto* lp_cast = dynamic_cast<Entry<U8>*>(&a_entry);
                return std::to_string(lp_cast->get());
            }
            case Datatypes::UnsignedShort:
            {
                auto* lp_cast = dynamic_cast<Entry<U16>*>(&a_entry);
                return std::to_string(lp_cast->get());
            }
            case Datatypes::UnsignedInteger:
            {
                auto* lp_cast = dynamic_cast<Entry<U32>*>(&a_entry);
                return std::to_string(lp_cast->get());
            }
            case Datatypes::FloatingPoint:
            {
                auto* lp_cast = dynamic_cast<Entry<Float>*>(&a_entry);
                return std::to_string(lp_cast->get());
            }
            case Datatypes::StringLiteral:
            {
                auto* lp_cast = dynamic_cast<Entry<String>*>(&a_entry);
                return lp_cast->get();
            }
            default:
            {
                return "";
            }
        }
    }

}
