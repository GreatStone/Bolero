#include "region_config.h"

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

#include <stdint.h>
#include <assert.h>
#include <stdio.h>

template <typename T>
static bool GetMember(T& root, const std::string& name, std::string* value) {
    auto iter = root.FindMember(name.data());
    if (iter == root.MemberEnd() || !iter->value.IsString()) {return false;}
    *value = iter->value.GetString();
    return true;
}

template <typename T>
static bool GetMember(T& root, const std::string& name, size_t* value) {
    auto iter = root.FindMember(name.data());
    if (iter == root.MemberEnd() || !iter->value.IsInt()) {return false;}
    *value = iter->value.GetInt();
    return true;
}

template <typename T>
static bool GetMember(T& root, const std::string& name, uint32_t* value) {
    auto iter = root.FindMember(name.data());
    if (iter == root.MemberEnd() || !iter->value.IsUint()) {return false;}
    *value = iter->value.GetUint();
    return true;
}

template <typename T>
static bool GetMember(T& root, const std::string& name, int* value) {
    auto iter = root.FindMember(name.data());
    if (iter == root.MemberEnd() || !iter->value.IsInt()) {return false;}
    *value = iter->value.GetInt();
    return true;
}

template <typename T>
static bool GetMember(T& root, const std::string& name, bool* value) {
    auto iter = root.FindMember(name.data());
    if (iter == root.MemberEnd() || !iter->value.IsBool()) {return false;}
    *value = iter->value.GetBool();
    return true;
}

namespace bolero {
    bool RegionConfig::read_from_file(const std::string& file_path) {
        FILE* fp = fopen(file_path.data(), "r");
        if (fp == nullptr) {
            return false;
        }
        char buffer[2048];
        rapidjson::FileReadStream in_stream(fp, buffer, sizeof(buffer));

        rapidjson::Document doc;
        doc.ParseStream(in_stream);
        fclose(fp);

        if (doc.HasParseError()) {
            return false;
        }
        assert(doc.IsObject());

        if (!GetMember(doc, "zk_host", &zk_host)) {return false;}
        if (!GetMember(doc, "region_ppath", &region_ppath)) {return false;}
        if (!GetMember(doc, "region_name", &region_name)) {return false;}
        if (!GetMember(doc, "fs_addr", &fs_addr)) {return false;}
        if (!GetMember(doc, "remote_itercache_size", &remote_itercache_size)) {return false;}
        if (!GetMember(doc, "db_location", &db_location)) {return false;}
        if (!GetMember(doc, "region_port", &region_port)) {return false;}

        auto iter = doc.FindMember("read_options");
        if (iter == doc.MemberEnd() || !iter->value.IsObject()) {return false;}
        auto r_opt = iter->value.GetObject();

        if (!GetMember(r_opt, "verify_checksums", &db_ropt.verify_checksums)) {return false;}
        if (!GetMember(r_opt, "fill_cache", &db_ropt.fill_cache)) {return false;}

        iter = doc.FindMember("write_options");
        if (iter == doc.MemberEnd() || !iter->value.IsObject()) {return false;}
        auto w_opt = iter->value.GetObject();

        if (!GetMember(w_opt, "sync", &db_wopt.sync)) {return false;}

        iter = doc.FindMember("db_options");
        if (iter == doc.MemberEnd() || !iter->value.IsObject()) {return false;}
        auto db_opt = iter->value.GetObject();

        if (!GetMember(db_opt, "create_if_missing", &opt.create_if_missing)) {return false;}
        if (!GetMember(db_opt, "error_if_exists", &opt.error_if_exists)) {return false;}
        if (!GetMember(db_opt, "paranoid_checks", &opt.paranoid_checks)) {return false;}
        if (!GetMember(db_opt, "write_buffer_size", &opt.write_buffer_size)) {return false;}
        if (!GetMember(db_opt, "max_open_files", &opt.max_open_files)) {return false;}
        if (!GetMember(db_opt, "block_size", &opt.block_size)) {return false;}
        if (!GetMember(db_opt, "block_restart_interval", &opt.block_restart_interval)) {return false;}
        if (!GetMember(db_opt, "reuse_logs", &opt.reuse_logs)) {return false;}
        return true;    }
}//namespace bolero
