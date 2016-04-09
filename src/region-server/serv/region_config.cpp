#include "region_config.h"

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

#include <stdint.h>
#include <assert.h>
#include <stdio.h>

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

    auto iter = doc.FindMember("zk_host");
    if (iter == doc.MemberEnd() || !iter->value.IsString()) {return false;}
    zk_host = iter->value.GetString();

    iter = doc.FindMember("region_ppath");
    if (iter == doc.MemberEnd() || !iter->value.IsString()) {return false;}
    region_ppath = iter->value.GetString();

    iter = doc.FindMember("region_name");
    if (iter == doc.MemberEnd() || !iter->value.IsString()) {return false;}
    region_name = iter->value.GetString();

    iter = doc.FindMember("fs_addr");
    if (iter == doc.MemberEnd() || !iter->value.IsString()) {return false;}
    fs_addr = iter->value.GetString();

    iter = doc.FindMember("remote_itercache_size");
    if (iter == doc.MemberEnd() || !iter->value.IsInt()) {return false;}
    remote_itercache_size = iter->value.GetInt();

    iter = doc.FindMember("read_options");
    if (iter == doc.MemberEnd() || !iter->value.IsObject()) {return false;}
    auto r_opt = iter->value.GetObject();
    
    iter = r_opt.FindMember("verify_checksums");
    if (iter == r_opt.MemberEnd() || !iter->value.IsBool()) {return false;}
    db_ropt.verify_checksums = iter->value.GetBool();

    iter = r_opt.FindMember("fill_cache");
    if (iter == r_opt.MemberEnd() || !iter->value.IsBool()) {return false;}
    db_ropt.fill_cache = iter->value.GetBool();

    iter = doc.FindMember("write_options");
    if (iter == doc.MemberEnd() || !iter->value.IsObject()) {return false;}
    auto w_opt = iter->value.GetObject();

    iter = w_opt.FindMember("sync");
    if (iter == w_opt.MemberEnd() || !iter->value.IsBool()) {return false;}
    db_wopt.sync = iter->value.GetBool();
}
