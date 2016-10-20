#include "metadata.hpp"
#include "metadataparser.hpp"


namespace dafs
{
    MetaDataParser::MetaDataParser(std::vector<dafs::MetaData> metadata_)
    {
        metadata = metadata_;
    }
}
