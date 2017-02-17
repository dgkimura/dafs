#pragma once

#include "dafs/metadataparser.hpp"
#include "dafs/node.hpp"
#include "dafs/sender.hpp"


namespace dafs
{
    dafs::Message
    HandleReadBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    dafs::Message
    HandleWriteBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    dafs::Message
    HandleGetNodeDetails(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    dafs::Message
    HandleRequestInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    dafs::Message
    HandleProcessInitation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    dafs::Message
    HandleConcludeInitation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);
}
