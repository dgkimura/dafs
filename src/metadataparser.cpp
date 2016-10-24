#include "metadata.hpp"
#include "metadataparser.hpp"


namespace dafs
{
    MetaDataParser::MetaDataParser(std::vector<dafs::MetaData> metadata_)
        : map_type
          {
              {typeid(dafs::FileInfo), dafs::FileInfoKey},
              {typeid(dafs::BlockInfo), dafs::BlockInfoKey},
              {typeid(dafs::Bytes), dafs::BytesKey},
          }
    {
        metadata = metadata_;
    }
}
