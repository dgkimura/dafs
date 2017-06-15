#pragma once

#include <limits>
#include <string>

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>


namespace dafs
{
    //
    // Associates stored objects with partitions.
    //
    struct Identity
    {
        boost::uuids::string_generator gen;

        std::string id;

        Identity()
        {
            id = "00000000-0000-0000-0000-000000000000";
        }

        Identity(std::string uuid)
        {
            id = uuid;
        }

        bool
        operator==(const Identity& rhs) const
        {
            return gen(id) == gen(rhs.id);
        }

        bool
        operator!=(const Identity& rhs) const
        {
            return gen(id) != gen(rhs.id);
        }

        bool
        operator<(const Identity& rhs) const
        {
            return gen(id) < gen(rhs.id);
        }

        bool
        operator>(const Identity& rhs) const
        {
            return gen(id) > gen(rhs.id);
        }

        bool
        operator<=(const Identity& rhs) const
        {
            return gen(id) <= gen(rhs.id);
        }

        bool
        operator>=(const Identity& rhs) const
        {
            return gen(id) >= gen(rhs.id);
        }

        Identity&
        operator+=(int value)
        {
            boost::uuids::uuid raw_id = gen(id);

            unsigned char uuid_data[raw_id.size()];
            std::memcpy(&uuid_data, &raw_id, raw_id.size());

            unsigned char carry = 0;
            for (int i=raw_id.size()-1; i>0; i--)
            {
                auto max = std::numeric_limits<unsigned char>::max();
                int updated = uuid_data[i] + carry + (max & value);
                uuid_data[i] = updated;
                carry = (updated > max ? 1 : 0);
                value = value >> std::numeric_limits<unsigned char>::digits;
            }

            boost::uuids::uuid u;
            std::memcpy(&u, uuid_data, raw_id.size());
            id = boost::uuids::to_string(u);

            return *this;
        }
    };


    bool IsLogicallyOrdered(dafs::Identity first,
                            dafs::Identity second,
                            dafs::Identity third);
}
