#pragma once

#include "dafs/metadataparser.hpp"
#include "dafs/node.hpp"
#include "dafs/sender.hpp"


namespace dafs
{
    void HandleCreateFile(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleDeleteFile(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


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


    void HandleInitiateCluster(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);
}
