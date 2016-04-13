#include "proxy_config.h"

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
    bool ProxyConfig::init(const std::string& file_name) {
        FILE* fp = fopen(file_name.data(), "r");
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
        if (!GetMember(doc, "zk_region_ppath", &zk_region_ppath)) {return false;}

        auto iter = doc.FindMember("rpc_option");
        if (iter == doc.MemberEnd() || !iter->value.IsObject()) {return false;}
        auto opt = iter->value.GetObject();

        if (!GetMember(opt, "work_thread_num", &client_opts_.work_thread_num)) {return false;}
        if (!GetMember(opt, "callback_thread_num", &client_opts_.callback_thread_num)) {return false;}
        if (!GetMember(opt, "keep_alive_time", &client_opts_.keep_alive_time)) {return false;}
        if (!GetMember(opt, "max_pending_buffer_size", &client_opts_.max_pending_buffer_size)) {return false;}
        if (!GetMember(opt, "max_throughput_in", &client_opts_.max_throughput_in)) {return false;}
        if (!GetMember(opt, "max_throughput_out", &client_opts_.max_throughput_out)) {return false;}
        return true;
    }
}//namespace bolero
