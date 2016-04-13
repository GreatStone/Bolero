// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: region_server_proto.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "region_server_proto.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace bolero {
namespace proto {

namespace {

const ::google::protobuf::Descriptor* HashRequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  HashRequest_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* HashRequest_OpType_descriptor_ = NULL;
const ::google::protobuf::Descriptor* HashResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  HashResponse_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* HashResponse_ErrorCode_descriptor_ = NULL;
const ::google::protobuf::ServiceDescriptor* RegionServerProto_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_region_5fserver_5fproto_2eproto() {
  protobuf_AddDesc_region_5fserver_5fproto_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "region_server_proto.proto");
  GOOGLE_CHECK(file != NULL);
  HashRequest_descriptor_ = file->message_type(0);
  static const int HashRequest_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(HashRequest, operation_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(HashRequest, user_key_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(HashRequest, req_batch_),
  };
  HashRequest_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      HashRequest_descriptor_,
      HashRequest::default_instance_,
      HashRequest_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(HashRequest, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(HashRequest, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(HashRequest));
  HashRequest_OpType_descriptor_ = HashRequest_descriptor_->enum_type(0);
  HashResponse_descriptor_ = file->message_type(1);
  static const int HashResponse_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(HashResponse, user_key_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(HashResponse, res_batch_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(HashResponse, err_),
  };
  HashResponse_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      HashResponse_descriptor_,
      HashResponse::default_instance_,
      HashResponse_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(HashResponse, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(HashResponse, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(HashResponse));
  HashResponse_ErrorCode_descriptor_ = HashResponse_descriptor_->enum_type(0);
  RegionServerProto_descriptor_ = file->service(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_region_5fserver_5fproto_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    HashRequest_descriptor_, &HashRequest::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    HashResponse_descriptor_, &HashResponse::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_region_5fserver_5fproto_2eproto() {
  delete HashRequest::default_instance_;
  delete HashRequest_reflection_;
  delete HashResponse::default_instance_;
  delete HashResponse_reflection_;
}

void protobuf_AddDesc_region_5fserver_5fproto_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\031region_server_proto.proto\022\014bolero.prot"
    "o\"\233\001\n\013HashRequest\0223\n\toperation\030\001 \001(\0162 .b"
    "olero.proto.HashRequest.OpType\022\020\n\010user_k"
    "ey\030\002 \001(\t\022\021\n\treq_batch\030\003 \001(\014\"2\n\006OpType\022\010\n"
    "\004HGET\020\000\022\t\n\005HMGET\020\001\022\010\n\004HSET\020\002\022\t\n\005HMSET\020\003\""
    "\251\001\n\014HashResponse\022\020\n\010user_key\030\001 \001(\t\022\021\n\tre"
    "s_batch\030\002 \001(\014\0221\n\003err\030\003 \001(\0162$.bolero.prot"
    "o.HashResponse.ErrorCode\"A\n\tErrorCode\022\006\n"
    "\002OK\020\000\022\020\n\014THREAD_ERROR\020\001\022\014\n\010BAD_ARGS\020\002\022\014\n"
    "\010DB_ERROR\020\0032U\n\021RegionServerProto\022@\n\007hash"
    "_op\022\031.bolero.proto.HashRequest\032\032.bolero."
    "proto.HashResponseB\003\200\001\001", 463);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "region_server_proto.proto", &protobuf_RegisterTypes);
  HashRequest::default_instance_ = new HashRequest();
  HashResponse::default_instance_ = new HashResponse();
  HashRequest::default_instance_->InitAsDefaultInstance();
  HashResponse::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_region_5fserver_5fproto_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_region_5fserver_5fproto_2eproto {
  StaticDescriptorInitializer_region_5fserver_5fproto_2eproto() {
    protobuf_AddDesc_region_5fserver_5fproto_2eproto();
  }
} static_descriptor_initializer_region_5fserver_5fproto_2eproto_;

// ===================================================================

const ::google::protobuf::EnumDescriptor* HashRequest_OpType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return HashRequest_OpType_descriptor_;
}
bool HashRequest_OpType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const HashRequest_OpType HashRequest::HGET;
const HashRequest_OpType HashRequest::HMGET;
const HashRequest_OpType HashRequest::HSET;
const HashRequest_OpType HashRequest::HMSET;
const HashRequest_OpType HashRequest::OpType_MIN;
const HashRequest_OpType HashRequest::OpType_MAX;
const int HashRequest::OpType_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int HashRequest::kOperationFieldNumber;
const int HashRequest::kUserKeyFieldNumber;
const int HashRequest::kReqBatchFieldNumber;
#endif  // !_MSC_VER

HashRequest::HashRequest()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void HashRequest::InitAsDefaultInstance() {
}

HashRequest::HashRequest(const HashRequest& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void HashRequest::SharedCtor() {
  _cached_size_ = 0;
  operation_ = 0;
  user_key_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  req_batch_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

HashRequest::~HashRequest() {
  SharedDtor();
}

void HashRequest::SharedDtor() {
  if (user_key_ != &::google::protobuf::internal::kEmptyString) {
    delete user_key_;
  }
  if (req_batch_ != &::google::protobuf::internal::kEmptyString) {
    delete req_batch_;
  }
  if (this != default_instance_) {
  }
}

void HashRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* HashRequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return HashRequest_descriptor_;
}

const HashRequest& HashRequest::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_region_5fserver_5fproto_2eproto();
  return *default_instance_;
}

HashRequest* HashRequest::default_instance_ = NULL;

HashRequest* HashRequest::New() const {
  return new HashRequest;
}

void HashRequest::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    operation_ = 0;
    if (has_user_key()) {
      if (user_key_ != &::google::protobuf::internal::kEmptyString) {
        user_key_->clear();
      }
    }
    if (has_req_batch()) {
      if (req_batch_ != &::google::protobuf::internal::kEmptyString) {
        req_batch_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool HashRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .bolero.proto.HashRequest.OpType operation = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::bolero::proto::HashRequest_OpType_IsValid(value)) {
            set_operation(static_cast< ::bolero::proto::HashRequest_OpType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_user_key;
        break;
      }

      // optional string user_key = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_user_key:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_user_key()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->user_key().data(), this->user_key().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_req_batch;
        break;
      }

      // optional bytes req_batch = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_req_batch:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_req_batch()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void HashRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional .bolero.proto.HashRequest.OpType operation = 1;
  if (has_operation()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->operation(), output);
  }

  // optional string user_key = 2;
  if (has_user_key()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->user_key().data(), this->user_key().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->user_key(), output);
  }

  // optional bytes req_batch = 3;
  if (has_req_batch()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      3, this->req_batch(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* HashRequest::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional .bolero.proto.HashRequest.OpType operation = 1;
  if (has_operation()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->operation(), target);
  }

  // optional string user_key = 2;
  if (has_user_key()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->user_key().data(), this->user_key().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->user_key(), target);
  }

  // optional bytes req_batch = 3;
  if (has_req_batch()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        3, this->req_batch(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int HashRequest::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional .bolero.proto.HashRequest.OpType operation = 1;
    if (has_operation()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->operation());
    }

    // optional string user_key = 2;
    if (has_user_key()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->user_key());
    }

    // optional bytes req_batch = 3;
    if (has_req_batch()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->req_batch());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void HashRequest::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const HashRequest* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const HashRequest*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void HashRequest::MergeFrom(const HashRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_operation()) {
      set_operation(from.operation());
    }
    if (from.has_user_key()) {
      set_user_key(from.user_key());
    }
    if (from.has_req_batch()) {
      set_req_batch(from.req_batch());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void HashRequest::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void HashRequest::CopyFrom(const HashRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool HashRequest::IsInitialized() const {

  return true;
}

void HashRequest::Swap(HashRequest* other) {
  if (other != this) {
    std::swap(operation_, other->operation_);
    std::swap(user_key_, other->user_key_);
    std::swap(req_batch_, other->req_batch_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata HashRequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = HashRequest_descriptor_;
  metadata.reflection = HashRequest_reflection_;
  return metadata;
}


// ===================================================================

const ::google::protobuf::EnumDescriptor* HashResponse_ErrorCode_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return HashResponse_ErrorCode_descriptor_;
}
bool HashResponse_ErrorCode_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const HashResponse_ErrorCode HashResponse::OK;
const HashResponse_ErrorCode HashResponse::THREAD_ERROR;
const HashResponse_ErrorCode HashResponse::BAD_ARGS;
const HashResponse_ErrorCode HashResponse::DB_ERROR;
const HashResponse_ErrorCode HashResponse::ErrorCode_MIN;
const HashResponse_ErrorCode HashResponse::ErrorCode_MAX;
const int HashResponse::ErrorCode_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int HashResponse::kUserKeyFieldNumber;
const int HashResponse::kResBatchFieldNumber;
const int HashResponse::kErrFieldNumber;
#endif  // !_MSC_VER

HashResponse::HashResponse()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void HashResponse::InitAsDefaultInstance() {
}

HashResponse::HashResponse(const HashResponse& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void HashResponse::SharedCtor() {
  _cached_size_ = 0;
  user_key_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  res_batch_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  err_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

HashResponse::~HashResponse() {
  SharedDtor();
}

void HashResponse::SharedDtor() {
  if (user_key_ != &::google::protobuf::internal::kEmptyString) {
    delete user_key_;
  }
  if (res_batch_ != &::google::protobuf::internal::kEmptyString) {
    delete res_batch_;
  }
  if (this != default_instance_) {
  }
}

void HashResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* HashResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return HashResponse_descriptor_;
}

const HashResponse& HashResponse::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_region_5fserver_5fproto_2eproto();
  return *default_instance_;
}

HashResponse* HashResponse::default_instance_ = NULL;

HashResponse* HashResponse::New() const {
  return new HashResponse;
}

void HashResponse::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_user_key()) {
      if (user_key_ != &::google::protobuf::internal::kEmptyString) {
        user_key_->clear();
      }
    }
    if (has_res_batch()) {
      if (res_batch_ != &::google::protobuf::internal::kEmptyString) {
        res_batch_->clear();
      }
    }
    err_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool HashResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string user_key = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_user_key()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->user_key().data(), this->user_key().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_res_batch;
        break;
      }

      // optional bytes res_batch = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_res_batch:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_res_batch()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_err;
        break;
      }

      // optional .bolero.proto.HashResponse.ErrorCode err = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_err:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::bolero::proto::HashResponse_ErrorCode_IsValid(value)) {
            set_err(static_cast< ::bolero::proto::HashResponse_ErrorCode >(value));
          } else {
            mutable_unknown_fields()->AddVarint(3, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void HashResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string user_key = 1;
  if (has_user_key()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->user_key().data(), this->user_key().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->user_key(), output);
  }

  // optional bytes res_batch = 2;
  if (has_res_batch()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      2, this->res_batch(), output);
  }

  // optional .bolero.proto.HashResponse.ErrorCode err = 3;
  if (has_err()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      3, this->err(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* HashResponse::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional string user_key = 1;
  if (has_user_key()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->user_key().data(), this->user_key().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->user_key(), target);
  }

  // optional bytes res_batch = 2;
  if (has_res_batch()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        2, this->res_batch(), target);
  }

  // optional .bolero.proto.HashResponse.ErrorCode err = 3;
  if (has_err()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      3, this->err(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int HashResponse::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string user_key = 1;
    if (has_user_key()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->user_key());
    }

    // optional bytes res_batch = 2;
    if (has_res_batch()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->res_batch());
    }

    // optional .bolero.proto.HashResponse.ErrorCode err = 3;
    if (has_err()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->err());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void HashResponse::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const HashResponse* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const HashResponse*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void HashResponse::MergeFrom(const HashResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_user_key()) {
      set_user_key(from.user_key());
    }
    if (from.has_res_batch()) {
      set_res_batch(from.res_batch());
    }
    if (from.has_err()) {
      set_err(from.err());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void HashResponse::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void HashResponse::CopyFrom(const HashResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool HashResponse::IsInitialized() const {

  return true;
}

void HashResponse::Swap(HashResponse* other) {
  if (other != this) {
    std::swap(user_key_, other->user_key_);
    std::swap(res_batch_, other->res_batch_);
    std::swap(err_, other->err_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata HashResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = HashResponse_descriptor_;
  metadata.reflection = HashResponse_reflection_;
  return metadata;
}


// ===================================================================

RegionServerProto::~RegionServerProto() {}

const ::google::protobuf::ServiceDescriptor* RegionServerProto::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return RegionServerProto_descriptor_;
}

const ::google::protobuf::ServiceDescriptor* RegionServerProto::GetDescriptor() {
  protobuf_AssignDescriptorsOnce();
  return RegionServerProto_descriptor_;
}

void RegionServerProto::hash_op(::google::protobuf::RpcController* controller,
                         const ::bolero::proto::HashRequest*,
                         ::bolero::proto::HashResponse*,
                         ::google::protobuf::Closure* done) {
  controller->SetFailed("Method hash_op() not implemented.");
  done->Run();
}

void RegionServerProto::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                             ::google::protobuf::RpcController* controller,
                             const ::google::protobuf::Message* request,
                             ::google::protobuf::Message* response,
                             ::google::protobuf::Closure* done) {
  GOOGLE_DCHECK_EQ(method->service(), RegionServerProto_descriptor_);
  switch(method->index()) {
    case 0:
      hash_op(controller,
             ::google::protobuf::down_cast<const ::bolero::proto::HashRequest*>(request),
             ::google::protobuf::down_cast< ::bolero::proto::HashResponse*>(response),
             done);
      break;
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      break;
  }
}

const ::google::protobuf::Message& RegionServerProto::GetRequestPrototype(
    const ::google::protobuf::MethodDescriptor* method) const {
  GOOGLE_DCHECK_EQ(method->service(), descriptor());
  switch(method->index()) {
    case 0:
      return ::bolero::proto::HashRequest::default_instance();
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      return *reinterpret_cast< ::google::protobuf::Message*>(NULL);
  }
}

const ::google::protobuf::Message& RegionServerProto::GetResponsePrototype(
    const ::google::protobuf::MethodDescriptor* method) const {
  GOOGLE_DCHECK_EQ(method->service(), descriptor());
  switch(method->index()) {
    case 0:
      return ::bolero::proto::HashResponse::default_instance();
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      return *reinterpret_cast< ::google::protobuf::Message*>(NULL);
  }
}

RegionServerProto_Stub::RegionServerProto_Stub(::google::protobuf::RpcChannel* channel)
  : channel_(channel), owns_channel_(false) {}
RegionServerProto_Stub::RegionServerProto_Stub(
    ::google::protobuf::RpcChannel* channel,
    ::google::protobuf::Service::ChannelOwnership ownership)
  : channel_(channel),
    owns_channel_(ownership == ::google::protobuf::Service::STUB_OWNS_CHANNEL) {}
RegionServerProto_Stub::~RegionServerProto_Stub() {
  if (owns_channel_) delete channel_;
}

void RegionServerProto_Stub::hash_op(::google::protobuf::RpcController* controller,
                              const ::bolero::proto::HashRequest* request,
                              ::bolero::proto::HashResponse* response,
                              ::google::protobuf::Closure* done) {
  channel_->CallMethod(descriptor()->method(0),
                       controller, request, response, done);
}

// @@protoc_insertion_point(namespace_scope)

}  // namespace proto
}  // namespace bolero

// @@protoc_insertion_point(global_scope)
