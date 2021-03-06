// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: region_server_proto.proto

#ifndef PROTOBUF_region_5fserver_5fproto_2eproto__INCLUDED
#define PROTOBUF_region_5fserver_5fproto_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/service.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace bolero {
namespace proto {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_region_5fserver_5fproto_2eproto();
void protobuf_AssignDesc_region_5fserver_5fproto_2eproto();
void protobuf_ShutdownFile_region_5fserver_5fproto_2eproto();

class HashRequest;
class HashResponse;

enum HashRequest_OpType {
  HashRequest_OpType_HGET = 0,
  HashRequest_OpType_HMGET = 1,
  HashRequest_OpType_HSET = 2,
  HashRequest_OpType_HMSET = 3,
  HashRequest_OpType_HDEL = 4,
  HashRequest_OpType_HMDEL = 5
};
bool HashRequest_OpType_IsValid(int value);
const HashRequest_OpType HashRequest_OpType_OpType_MIN = HashRequest_OpType_HGET;
const HashRequest_OpType HashRequest_OpType_OpType_MAX = HashRequest_OpType_HMDEL;
const int HashRequest_OpType_OpType_ARRAYSIZE = HashRequest_OpType_OpType_MAX + 1;

const ::google::protobuf::EnumDescriptor* HashRequest_OpType_descriptor();
inline const ::std::string& HashRequest_OpType_Name(HashRequest_OpType value) {
  return ::google::protobuf::internal::NameOfEnum(
    HashRequest_OpType_descriptor(), value);
}
inline bool HashRequest_OpType_Parse(
    const ::std::string& name, HashRequest_OpType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<HashRequest_OpType>(
    HashRequest_OpType_descriptor(), name, value);
}
enum HashResponse_ErrorCode {
  HashResponse_ErrorCode_OK = 0,
  HashResponse_ErrorCode_THREAD_ERROR = 1,
  HashResponse_ErrorCode_BAD_ARGS = 2,
  HashResponse_ErrorCode_DB_ERROR = 3
};
bool HashResponse_ErrorCode_IsValid(int value);
const HashResponse_ErrorCode HashResponse_ErrorCode_ErrorCode_MIN = HashResponse_ErrorCode_OK;
const HashResponse_ErrorCode HashResponse_ErrorCode_ErrorCode_MAX = HashResponse_ErrorCode_DB_ERROR;
const int HashResponse_ErrorCode_ErrorCode_ARRAYSIZE = HashResponse_ErrorCode_ErrorCode_MAX + 1;

const ::google::protobuf::EnumDescriptor* HashResponse_ErrorCode_descriptor();
inline const ::std::string& HashResponse_ErrorCode_Name(HashResponse_ErrorCode value) {
  return ::google::protobuf::internal::NameOfEnum(
    HashResponse_ErrorCode_descriptor(), value);
}
inline bool HashResponse_ErrorCode_Parse(
    const ::std::string& name, HashResponse_ErrorCode* value) {
  return ::google::protobuf::internal::ParseNamedEnum<HashResponse_ErrorCode>(
    HashResponse_ErrorCode_descriptor(), name, value);
}
// ===================================================================

class HashRequest : public ::google::protobuf::Message {
 public:
  HashRequest();
  virtual ~HashRequest();

  HashRequest(const HashRequest& from);

  inline HashRequest& operator=(const HashRequest& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const HashRequest& default_instance();

  void Swap(HashRequest* other);

  // implements Message ----------------------------------------------

  HashRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const HashRequest& from);
  void MergeFrom(const HashRequest& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef HashRequest_OpType OpType;
  static const OpType HGET = HashRequest_OpType_HGET;
  static const OpType HMGET = HashRequest_OpType_HMGET;
  static const OpType HSET = HashRequest_OpType_HSET;
  static const OpType HMSET = HashRequest_OpType_HMSET;
  static const OpType HDEL = HashRequest_OpType_HDEL;
  static const OpType HMDEL = HashRequest_OpType_HMDEL;
  static inline bool OpType_IsValid(int value) {
    return HashRequest_OpType_IsValid(value);
  }
  static const OpType OpType_MIN =
    HashRequest_OpType_OpType_MIN;
  static const OpType OpType_MAX =
    HashRequest_OpType_OpType_MAX;
  static const int OpType_ARRAYSIZE =
    HashRequest_OpType_OpType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  OpType_descriptor() {
    return HashRequest_OpType_descriptor();
  }
  static inline const ::std::string& OpType_Name(OpType value) {
    return HashRequest_OpType_Name(value);
  }
  static inline bool OpType_Parse(const ::std::string& name,
      OpType* value) {
    return HashRequest_OpType_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // optional .bolero.proto.HashRequest.OpType operation = 1;
  inline bool has_operation() const;
  inline void clear_operation();
  static const int kOperationFieldNumber = 1;
  inline ::bolero::proto::HashRequest_OpType operation() const;
  inline void set_operation(::bolero::proto::HashRequest_OpType value);

  // optional bytes user_key = 2;
  inline bool has_user_key() const;
  inline void clear_user_key();
  static const int kUserKeyFieldNumber = 2;
  inline const ::std::string& user_key() const;
  inline void set_user_key(const ::std::string& value);
  inline void set_user_key(const char* value);
  inline void set_user_key(const void* value, size_t size);
  inline ::std::string* mutable_user_key();
  inline ::std::string* release_user_key();
  inline void set_allocated_user_key(::std::string* user_key);

  // optional bytes req_batch = 3;
  inline bool has_req_batch() const;
  inline void clear_req_batch();
  static const int kReqBatchFieldNumber = 3;
  inline const ::std::string& req_batch() const;
  inline void set_req_batch(const ::std::string& value);
  inline void set_req_batch(const char* value);
  inline void set_req_batch(const void* value, size_t size);
  inline ::std::string* mutable_req_batch();
  inline ::std::string* release_req_batch();
  inline void set_allocated_req_batch(::std::string* req_batch);

  // @@protoc_insertion_point(class_scope:bolero.proto.HashRequest)
 private:
  inline void set_has_operation();
  inline void clear_has_operation();
  inline void set_has_user_key();
  inline void clear_has_user_key();
  inline void set_has_req_batch();
  inline void clear_has_req_batch();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* user_key_;
  ::std::string* req_batch_;
  int operation_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_region_5fserver_5fproto_2eproto();
  friend void protobuf_AssignDesc_region_5fserver_5fproto_2eproto();
  friend void protobuf_ShutdownFile_region_5fserver_5fproto_2eproto();

  void InitAsDefaultInstance();
  static HashRequest* default_instance_;
};
// -------------------------------------------------------------------

class HashResponse : public ::google::protobuf::Message {
 public:
  HashResponse();
  virtual ~HashResponse();

  HashResponse(const HashResponse& from);

  inline HashResponse& operator=(const HashResponse& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const HashResponse& default_instance();

  void Swap(HashResponse* other);

  // implements Message ----------------------------------------------

  HashResponse* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const HashResponse& from);
  void MergeFrom(const HashResponse& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef HashResponse_ErrorCode ErrorCode;
  static const ErrorCode OK = HashResponse_ErrorCode_OK;
  static const ErrorCode THREAD_ERROR = HashResponse_ErrorCode_THREAD_ERROR;
  static const ErrorCode BAD_ARGS = HashResponse_ErrorCode_BAD_ARGS;
  static const ErrorCode DB_ERROR = HashResponse_ErrorCode_DB_ERROR;
  static inline bool ErrorCode_IsValid(int value) {
    return HashResponse_ErrorCode_IsValid(value);
  }
  static const ErrorCode ErrorCode_MIN =
    HashResponse_ErrorCode_ErrorCode_MIN;
  static const ErrorCode ErrorCode_MAX =
    HashResponse_ErrorCode_ErrorCode_MAX;
  static const int ErrorCode_ARRAYSIZE =
    HashResponse_ErrorCode_ErrorCode_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  ErrorCode_descriptor() {
    return HashResponse_ErrorCode_descriptor();
  }
  static inline const ::std::string& ErrorCode_Name(ErrorCode value) {
    return HashResponse_ErrorCode_Name(value);
  }
  static inline bool ErrorCode_Parse(const ::std::string& name,
      ErrorCode* value) {
    return HashResponse_ErrorCode_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // optional bytes user_key = 1;
  inline bool has_user_key() const;
  inline void clear_user_key();
  static const int kUserKeyFieldNumber = 1;
  inline const ::std::string& user_key() const;
  inline void set_user_key(const ::std::string& value);
  inline void set_user_key(const char* value);
  inline void set_user_key(const void* value, size_t size);
  inline ::std::string* mutable_user_key();
  inline ::std::string* release_user_key();
  inline void set_allocated_user_key(::std::string* user_key);

  // optional bytes res_batch = 2;
  inline bool has_res_batch() const;
  inline void clear_res_batch();
  static const int kResBatchFieldNumber = 2;
  inline const ::std::string& res_batch() const;
  inline void set_res_batch(const ::std::string& value);
  inline void set_res_batch(const char* value);
  inline void set_res_batch(const void* value, size_t size);
  inline ::std::string* mutable_res_batch();
  inline ::std::string* release_res_batch();
  inline void set_allocated_res_batch(::std::string* res_batch);

  // optional .bolero.proto.HashResponse.ErrorCode err = 3;
  inline bool has_err() const;
  inline void clear_err();
  static const int kErrFieldNumber = 3;
  inline ::bolero::proto::HashResponse_ErrorCode err() const;
  inline void set_err(::bolero::proto::HashResponse_ErrorCode value);

  // @@protoc_insertion_point(class_scope:bolero.proto.HashResponse)
 private:
  inline void set_has_user_key();
  inline void clear_has_user_key();
  inline void set_has_res_batch();
  inline void clear_has_res_batch();
  inline void set_has_err();
  inline void clear_has_err();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* user_key_;
  ::std::string* res_batch_;
  int err_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_region_5fserver_5fproto_2eproto();
  friend void protobuf_AssignDesc_region_5fserver_5fproto_2eproto();
  friend void protobuf_ShutdownFile_region_5fserver_5fproto_2eproto();

  void InitAsDefaultInstance();
  static HashResponse* default_instance_;
};
// ===================================================================

class RegionServerProto_Stub;

class RegionServerProto : public ::google::protobuf::Service {
 protected:
  // This class should be treated as an abstract interface.
  inline RegionServerProto() {};
 public:
  virtual ~RegionServerProto();

  typedef RegionServerProto_Stub Stub;

  static const ::google::protobuf::ServiceDescriptor* descriptor();

  virtual void hash_op(::google::protobuf::RpcController* controller,
                       const ::bolero::proto::HashRequest* request,
                       ::bolero::proto::HashResponse* response,
                       ::google::protobuf::Closure* done);

  // implements Service ----------------------------------------------

  const ::google::protobuf::ServiceDescriptor* GetDescriptor();
  void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                  ::google::protobuf::RpcController* controller,
                  const ::google::protobuf::Message* request,
                  ::google::protobuf::Message* response,
                  ::google::protobuf::Closure* done);
  const ::google::protobuf::Message& GetRequestPrototype(
    const ::google::protobuf::MethodDescriptor* method) const;
  const ::google::protobuf::Message& GetResponsePrototype(
    const ::google::protobuf::MethodDescriptor* method) const;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(RegionServerProto);
};

class RegionServerProto_Stub : public RegionServerProto {
 public:
  RegionServerProto_Stub(::google::protobuf::RpcChannel* channel);
  RegionServerProto_Stub(::google::protobuf::RpcChannel* channel,
                   ::google::protobuf::Service::ChannelOwnership ownership);
  ~RegionServerProto_Stub();

  inline ::google::protobuf::RpcChannel* channel() { return channel_; }

  // implements RegionServerProto ------------------------------------------

  void hash_op(::google::protobuf::RpcController* controller,
                       const ::bolero::proto::HashRequest* request,
                       ::bolero::proto::HashResponse* response,
                       ::google::protobuf::Closure* done);
 private:
  ::google::protobuf::RpcChannel* channel_;
  bool owns_channel_;
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(RegionServerProto_Stub);
};


// ===================================================================


// ===================================================================

// HashRequest

// optional .bolero.proto.HashRequest.OpType operation = 1;
inline bool HashRequest::has_operation() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void HashRequest::set_has_operation() {
  _has_bits_[0] |= 0x00000001u;
}
inline void HashRequest::clear_has_operation() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void HashRequest::clear_operation() {
  operation_ = 0;
  clear_has_operation();
}
inline ::bolero::proto::HashRequest_OpType HashRequest::operation() const {
  return static_cast< ::bolero::proto::HashRequest_OpType >(operation_);
}
inline void HashRequest::set_operation(::bolero::proto::HashRequest_OpType value) {
  assert(::bolero::proto::HashRequest_OpType_IsValid(value));
  set_has_operation();
  operation_ = value;
}

// optional bytes user_key = 2;
inline bool HashRequest::has_user_key() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void HashRequest::set_has_user_key() {
  _has_bits_[0] |= 0x00000002u;
}
inline void HashRequest::clear_has_user_key() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void HashRequest::clear_user_key() {
  if (user_key_ != &::google::protobuf::internal::kEmptyString) {
    user_key_->clear();
  }
  clear_has_user_key();
}
inline const ::std::string& HashRequest::user_key() const {
  return *user_key_;
}
inline void HashRequest::set_user_key(const ::std::string& value) {
  set_has_user_key();
  if (user_key_ == &::google::protobuf::internal::kEmptyString) {
    user_key_ = new ::std::string;
  }
  user_key_->assign(value);
}
inline void HashRequest::set_user_key(const char* value) {
  set_has_user_key();
  if (user_key_ == &::google::protobuf::internal::kEmptyString) {
    user_key_ = new ::std::string;
  }
  user_key_->assign(value);
}
inline void HashRequest::set_user_key(const void* value, size_t size) {
  set_has_user_key();
  if (user_key_ == &::google::protobuf::internal::kEmptyString) {
    user_key_ = new ::std::string;
  }
  user_key_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* HashRequest::mutable_user_key() {
  set_has_user_key();
  if (user_key_ == &::google::protobuf::internal::kEmptyString) {
    user_key_ = new ::std::string;
  }
  return user_key_;
}
inline ::std::string* HashRequest::release_user_key() {
  clear_has_user_key();
  if (user_key_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = user_key_;
    user_key_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void HashRequest::set_allocated_user_key(::std::string* user_key) {
  if (user_key_ != &::google::protobuf::internal::kEmptyString) {
    delete user_key_;
  }
  if (user_key) {
    set_has_user_key();
    user_key_ = user_key;
  } else {
    clear_has_user_key();
    user_key_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional bytes req_batch = 3;
inline bool HashRequest::has_req_batch() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void HashRequest::set_has_req_batch() {
  _has_bits_[0] |= 0x00000004u;
}
inline void HashRequest::clear_has_req_batch() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void HashRequest::clear_req_batch() {
  if (req_batch_ != &::google::protobuf::internal::kEmptyString) {
    req_batch_->clear();
  }
  clear_has_req_batch();
}
inline const ::std::string& HashRequest::req_batch() const {
  return *req_batch_;
}
inline void HashRequest::set_req_batch(const ::std::string& value) {
  set_has_req_batch();
  if (req_batch_ == &::google::protobuf::internal::kEmptyString) {
    req_batch_ = new ::std::string;
  }
  req_batch_->assign(value);
}
inline void HashRequest::set_req_batch(const char* value) {
  set_has_req_batch();
  if (req_batch_ == &::google::protobuf::internal::kEmptyString) {
    req_batch_ = new ::std::string;
  }
  req_batch_->assign(value);
}
inline void HashRequest::set_req_batch(const void* value, size_t size) {
  set_has_req_batch();
  if (req_batch_ == &::google::protobuf::internal::kEmptyString) {
    req_batch_ = new ::std::string;
  }
  req_batch_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* HashRequest::mutable_req_batch() {
  set_has_req_batch();
  if (req_batch_ == &::google::protobuf::internal::kEmptyString) {
    req_batch_ = new ::std::string;
  }
  return req_batch_;
}
inline ::std::string* HashRequest::release_req_batch() {
  clear_has_req_batch();
  if (req_batch_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = req_batch_;
    req_batch_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void HashRequest::set_allocated_req_batch(::std::string* req_batch) {
  if (req_batch_ != &::google::protobuf::internal::kEmptyString) {
    delete req_batch_;
  }
  if (req_batch) {
    set_has_req_batch();
    req_batch_ = req_batch;
  } else {
    clear_has_req_batch();
    req_batch_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// -------------------------------------------------------------------

// HashResponse

// optional bytes user_key = 1;
inline bool HashResponse::has_user_key() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void HashResponse::set_has_user_key() {
  _has_bits_[0] |= 0x00000001u;
}
inline void HashResponse::clear_has_user_key() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void HashResponse::clear_user_key() {
  if (user_key_ != &::google::protobuf::internal::kEmptyString) {
    user_key_->clear();
  }
  clear_has_user_key();
}
inline const ::std::string& HashResponse::user_key() const {
  return *user_key_;
}
inline void HashResponse::set_user_key(const ::std::string& value) {
  set_has_user_key();
  if (user_key_ == &::google::protobuf::internal::kEmptyString) {
    user_key_ = new ::std::string;
  }
  user_key_->assign(value);
}
inline void HashResponse::set_user_key(const char* value) {
  set_has_user_key();
  if (user_key_ == &::google::protobuf::internal::kEmptyString) {
    user_key_ = new ::std::string;
  }
  user_key_->assign(value);
}
inline void HashResponse::set_user_key(const void* value, size_t size) {
  set_has_user_key();
  if (user_key_ == &::google::protobuf::internal::kEmptyString) {
    user_key_ = new ::std::string;
  }
  user_key_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* HashResponse::mutable_user_key() {
  set_has_user_key();
  if (user_key_ == &::google::protobuf::internal::kEmptyString) {
    user_key_ = new ::std::string;
  }
  return user_key_;
}
inline ::std::string* HashResponse::release_user_key() {
  clear_has_user_key();
  if (user_key_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = user_key_;
    user_key_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void HashResponse::set_allocated_user_key(::std::string* user_key) {
  if (user_key_ != &::google::protobuf::internal::kEmptyString) {
    delete user_key_;
  }
  if (user_key) {
    set_has_user_key();
    user_key_ = user_key;
  } else {
    clear_has_user_key();
    user_key_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional bytes res_batch = 2;
inline bool HashResponse::has_res_batch() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void HashResponse::set_has_res_batch() {
  _has_bits_[0] |= 0x00000002u;
}
inline void HashResponse::clear_has_res_batch() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void HashResponse::clear_res_batch() {
  if (res_batch_ != &::google::protobuf::internal::kEmptyString) {
    res_batch_->clear();
  }
  clear_has_res_batch();
}
inline const ::std::string& HashResponse::res_batch() const {
  return *res_batch_;
}
inline void HashResponse::set_res_batch(const ::std::string& value) {
  set_has_res_batch();
  if (res_batch_ == &::google::protobuf::internal::kEmptyString) {
    res_batch_ = new ::std::string;
  }
  res_batch_->assign(value);
}
inline void HashResponse::set_res_batch(const char* value) {
  set_has_res_batch();
  if (res_batch_ == &::google::protobuf::internal::kEmptyString) {
    res_batch_ = new ::std::string;
  }
  res_batch_->assign(value);
}
inline void HashResponse::set_res_batch(const void* value, size_t size) {
  set_has_res_batch();
  if (res_batch_ == &::google::protobuf::internal::kEmptyString) {
    res_batch_ = new ::std::string;
  }
  res_batch_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* HashResponse::mutable_res_batch() {
  set_has_res_batch();
  if (res_batch_ == &::google::protobuf::internal::kEmptyString) {
    res_batch_ = new ::std::string;
  }
  return res_batch_;
}
inline ::std::string* HashResponse::release_res_batch() {
  clear_has_res_batch();
  if (res_batch_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = res_batch_;
    res_batch_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void HashResponse::set_allocated_res_batch(::std::string* res_batch) {
  if (res_batch_ != &::google::protobuf::internal::kEmptyString) {
    delete res_batch_;
  }
  if (res_batch) {
    set_has_res_batch();
    res_batch_ = res_batch;
  } else {
    clear_has_res_batch();
    res_batch_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional .bolero.proto.HashResponse.ErrorCode err = 3;
inline bool HashResponse::has_err() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void HashResponse::set_has_err() {
  _has_bits_[0] |= 0x00000004u;
}
inline void HashResponse::clear_has_err() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void HashResponse::clear_err() {
  err_ = 0;
  clear_has_err();
}
inline ::bolero::proto::HashResponse_ErrorCode HashResponse::err() const {
  return static_cast< ::bolero::proto::HashResponse_ErrorCode >(err_);
}
inline void HashResponse::set_err(::bolero::proto::HashResponse_ErrorCode value) {
  assert(::bolero::proto::HashResponse_ErrorCode_IsValid(value));
  set_has_err();
  err_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace proto
}  // namespace bolero

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::bolero::proto::HashRequest_OpType>() {
  return ::bolero::proto::HashRequest_OpType_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::bolero::proto::HashResponse_ErrorCode>() {
  return ::bolero::proto::HashResponse_ErrorCode_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_region_5fserver_5fproto_2eproto__INCLUDED
