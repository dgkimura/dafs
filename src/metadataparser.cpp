#include "dafs/metadata.hpp"
#include "dafs/metadataparser.hpp"


namespace dafs
{
    MetaDataParser::MetaDataParser(std::vector<dafs::MetaData> metadata)
        : metadata(metadata)
    {
    }
}
