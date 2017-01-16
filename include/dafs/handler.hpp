#pragma once

#include "dafs/metadataparser.hpp"
#include "dafs/partition.hpp"
#include "dafs/sender.hpp"


namespace dafs
{
    void HandleCreateFile(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleDeleteFile(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleOpenFile(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleReadFile(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleWriteFile(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleReadBlock(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleWriteBlock(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleAllocate(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleAllocated(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    void HandleInitiateCluster(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);
}
