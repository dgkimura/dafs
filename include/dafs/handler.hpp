#pragma once

#include "dafs/metadataparser.hpp"
#include "dafs/node.hpp"
#include "dafs/sender.hpp"


namespace dafs
{
    void HandleOpenFile(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleReadFile(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleWriteFile(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleReadBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleWriteBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleAllocate(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleAllocated(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleRequestInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleProcessInitation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleConcludeInitation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);
}
