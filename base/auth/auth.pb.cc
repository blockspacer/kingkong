// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: auth.proto

#include "auth.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace xauth {
class requestDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<request> _instance;
} _request_default_instance_;
class responseDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<response> _instance;
} _response_default_instance_;
}  // namespace xauth
static void InitDefaultsscc_info_request_auth_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::xauth::_request_default_instance_;
    new (ptr) ::xauth::request();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::xauth::request::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_request_auth_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_request_auth_2eproto}, {}};

static void InitDefaultsscc_info_response_auth_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::xauth::_response_default_instance_;
    new (ptr) ::xauth::response();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::xauth::response::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_response_auth_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_response_auth_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_auth_2eproto[2];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_auth_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_auth_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_auth_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::xauth::request, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::xauth::request, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::xauth::request, type_),
  PROTOBUF_FIELD_OFFSET(::xauth::request, version_),
  PROTOBUF_FIELD_OFFSET(::xauth::request, token_),
  PROTOBUF_FIELD_OFFSET(::xauth::request, deviceid_),
  PROTOBUF_FIELD_OFFSET(::xauth::request, randomid_),
  3,
  4,
  0,
  1,
  2,
  PROTOBUF_FIELD_OFFSET(::xauth::response, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::xauth::response, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::xauth::response, code_),
  PROTOBUF_FIELD_OFFSET(::xauth::response, signature_),
  PROTOBUF_FIELD_OFFSET(::xauth::response, desc_),
  PROTOBUF_FIELD_OFFSET(::xauth::response, rights_),
  3,
  0,
  1,
  2,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 10, sizeof(::xauth::request)},
  { 15, 24, sizeof(::xauth::response)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::xauth::_request_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::xauth::_response_default_instance_),
};

const char descriptor_table_protodef_auth_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\nauth.proto\022\005xauth\"k\n\007request\022\034\n\004type\030\001"
  " \002(\0162\016.xauth.product\022\017\n\007version\030\002 \002(\005\022\r\n"
  "\005token\030\003 \002(\t\022\020\n\010deviceid\030\004 \002(\t\022\020\n\010random"
  "id\030\005 \002(\t\"I\n\010response\022\014\n\004code\030\001 \002(\005\022\021\n\tsi"
  "gnature\030\002 \002(\t\022\014\n\004desc\030\003 \001(\t\022\016\n\006rights\030\004 "
  "\001(\t*\021\n\007product\022\006\n\002wa\020\000"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_auth_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_auth_2eproto_sccs[2] = {
  &scc_info_request_auth_2eproto.base,
  &scc_info_response_auth_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_auth_2eproto_once;
static bool descriptor_table_auth_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_auth_2eproto = {
  &descriptor_table_auth_2eproto_initialized, descriptor_table_protodef_auth_2eproto, "auth.proto", 222,
  &descriptor_table_auth_2eproto_once, descriptor_table_auth_2eproto_sccs, descriptor_table_auth_2eproto_deps, 2, 0,
  schemas, file_default_instances, TableStruct_auth_2eproto::offsets,
  file_level_metadata_auth_2eproto, 2, file_level_enum_descriptors_auth_2eproto, file_level_service_descriptors_auth_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_auth_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_auth_2eproto), true);
namespace xauth {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* product_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_auth_2eproto);
  return file_level_enum_descriptors_auth_2eproto[0];
}
bool product_IsValid(int value) {
  switch (value) {
    case 0:
      return true;
    default:
      return false;
  }
}


// ===================================================================

void request::InitAsDefaultInstance() {
}
class request::_Internal {
 public:
  using HasBits = decltype(std::declval<request>()._has_bits_);
  static void set_has_type(HasBits* has_bits) {
    (*has_bits)[0] |= 8u;
  }
  static void set_has_version(HasBits* has_bits) {
    (*has_bits)[0] |= 16u;
  }
  static void set_has_token(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_deviceid(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_randomid(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
};

request::request()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:xauth.request)
}
request::request(const request& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  token_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_token()) {
    token_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.token_);
  }
  deviceid_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_deviceid()) {
    deviceid_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.deviceid_);
  }
  randomid_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_randomid()) {
    randomid_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.randomid_);
  }
  ::memcpy(&type_, &from.type_,
    static_cast<size_t>(reinterpret_cast<char*>(&version_) -
    reinterpret_cast<char*>(&type_)) + sizeof(version_));
  // @@protoc_insertion_point(copy_constructor:xauth.request)
}

void request::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_request_auth_2eproto.base);
  token_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  deviceid_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  randomid_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(&type_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&version_) -
      reinterpret_cast<char*>(&type_)) + sizeof(version_));
}

request::~request() {
  // @@protoc_insertion_point(destructor:xauth.request)
  SharedDtor();
}

void request::SharedDtor() {
  token_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  deviceid_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  randomid_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void request::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const request& request::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_request_auth_2eproto.base);
  return *internal_default_instance();
}


void request::Clear() {
// @@protoc_insertion_point(message_clear_start:xauth.request)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    if (cached_has_bits & 0x00000001u) {
      token_.ClearNonDefaultToEmptyNoArena();
    }
    if (cached_has_bits & 0x00000002u) {
      deviceid_.ClearNonDefaultToEmptyNoArena();
    }
    if (cached_has_bits & 0x00000004u) {
      randomid_.ClearNonDefaultToEmptyNoArena();
    }
  }
  if (cached_has_bits & 0x00000018u) {
    ::memset(&type_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&version_) -
        reinterpret_cast<char*>(&type_)) + sizeof(version_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

const char* request::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // required .xauth.product type = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
          if (PROTOBUF_PREDICT_TRUE(::xauth::product_IsValid(val))) {
            _internal_set_type(static_cast<::xauth::product>(val));
          } else {
            ::PROTOBUF_NAMESPACE_ID::internal::WriteVarint(1, val, mutable_unknown_fields());
          }
        } else goto handle_unusual;
        continue;
      // required int32 version = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          _Internal::set_has_version(&has_bits);
          version_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required string token = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          auto str = _internal_mutable_token();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          #ifndef NDEBUG
          ::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "xauth.request.token");
          #endif  // !NDEBUG
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required string deviceid = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          auto str = _internal_mutable_deviceid();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          #ifndef NDEBUG
          ::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "xauth.request.deviceid");
          #endif  // !NDEBUG
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required string randomid = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 42)) {
          auto str = _internal_mutable_randomid();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          #ifndef NDEBUG
          ::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "xauth.request.randomid");
          #endif  // !NDEBUG
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* request::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:xauth.request)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required .xauth.product type = 1;
  if (cached_has_bits & 0x00000008u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      1, this->_internal_type(), target);
  }

  // required int32 version = 2;
  if (cached_has_bits & 0x00000010u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(2, this->_internal_version(), target);
  }

  // required string token = 3;
  if (cached_has_bits & 0x00000001u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_token().data(), static_cast<int>(this->_internal_token().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "xauth.request.token");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_token(), target);
  }

  // required string deviceid = 4;
  if (cached_has_bits & 0x00000002u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_deviceid().data(), static_cast<int>(this->_internal_deviceid().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "xauth.request.deviceid");
    target = stream->WriteStringMaybeAliased(
        4, this->_internal_deviceid(), target);
  }

  // required string randomid = 5;
  if (cached_has_bits & 0x00000004u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_randomid().data(), static_cast<int>(this->_internal_randomid().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "xauth.request.randomid");
    target = stream->WriteStringMaybeAliased(
        5, this->_internal_randomid(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:xauth.request)
  return target;
}

size_t request::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:xauth.request)
  size_t total_size = 0;

  if (_internal_has_token()) {
    // required string token = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_token());
  }

  if (_internal_has_deviceid()) {
    // required string deviceid = 4;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_deviceid());
  }

  if (_internal_has_randomid()) {
    // required string randomid = 5;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_randomid());
  }

  if (_internal_has_type()) {
    // required .xauth.product type = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_type());
  }

  if (_internal_has_version()) {
    // required int32 version = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_version());
  }

  return total_size;
}
size_t request::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:xauth.request)
  size_t total_size = 0;

  if (((_has_bits_[0] & 0x0000001f) ^ 0x0000001f) == 0) {  // All required fields are present.
    // required string token = 3;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_token());

    // required string deviceid = 4;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_deviceid());

    // required string randomid = 5;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_randomid());

    // required .xauth.product type = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_type());

    // required int32 version = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_version());

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void request::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:xauth.request)
  GOOGLE_DCHECK_NE(&from, this);
  const request* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<request>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:xauth.request)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:xauth.request)
    MergeFrom(*source);
  }
}

void request::MergeFrom(const request& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:xauth.request)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x0000001fu) {
    if (cached_has_bits & 0x00000001u) {
      _has_bits_[0] |= 0x00000001u;
      token_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.token_);
    }
    if (cached_has_bits & 0x00000002u) {
      _has_bits_[0] |= 0x00000002u;
      deviceid_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.deviceid_);
    }
    if (cached_has_bits & 0x00000004u) {
      _has_bits_[0] |= 0x00000004u;
      randomid_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.randomid_);
    }
    if (cached_has_bits & 0x00000008u) {
      type_ = from.type_;
    }
    if (cached_has_bits & 0x00000010u) {
      version_ = from.version_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void request::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:xauth.request)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void request::CopyFrom(const request& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:xauth.request)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool request::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000001f) != 0x0000001f) return false;
  return true;
}

void request::InternalSwap(request* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  token_.Swap(&other->token_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  deviceid_.Swap(&other->deviceid_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  randomid_.Swap(&other->randomid_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(type_, other->type_);
  swap(version_, other->version_);
}

::PROTOBUF_NAMESPACE_ID::Metadata request::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void response::InitAsDefaultInstance() {
}
class response::_Internal {
 public:
  using HasBits = decltype(std::declval<response>()._has_bits_);
  static void set_has_code(HasBits* has_bits) {
    (*has_bits)[0] |= 8u;
  }
  static void set_has_signature(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_desc(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static void set_has_rights(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
};

response::response()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:xauth.response)
}
response::response(const response& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  signature_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_signature()) {
    signature_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.signature_);
  }
  desc_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_desc()) {
    desc_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.desc_);
  }
  rights_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_rights()) {
    rights_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.rights_);
  }
  code_ = from.code_;
  // @@protoc_insertion_point(copy_constructor:xauth.response)
}

void response::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_response_auth_2eproto.base);
  signature_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  desc_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  rights_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  code_ = 0;
}

response::~response() {
  // @@protoc_insertion_point(destructor:xauth.response)
  SharedDtor();
}

void response::SharedDtor() {
  signature_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  desc_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  rights_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void response::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const response& response::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_response_auth_2eproto.base);
  return *internal_default_instance();
}


void response::Clear() {
// @@protoc_insertion_point(message_clear_start:xauth.response)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    if (cached_has_bits & 0x00000001u) {
      signature_.ClearNonDefaultToEmptyNoArena();
    }
    if (cached_has_bits & 0x00000002u) {
      desc_.ClearNonDefaultToEmptyNoArena();
    }
    if (cached_has_bits & 0x00000004u) {
      rights_.ClearNonDefaultToEmptyNoArena();
    }
  }
  code_ = 0;
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

const char* response::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // required int32 code = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_code(&has_bits);
          code_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // required string signature = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          auto str = _internal_mutable_signature();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          #ifndef NDEBUG
          ::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "xauth.response.signature");
          #endif  // !NDEBUG
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // optional string desc = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          auto str = _internal_mutable_desc();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          #ifndef NDEBUG
          ::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "xauth.response.desc");
          #endif  // !NDEBUG
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // optional string rights = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          auto str = _internal_mutable_rights();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          #ifndef NDEBUG
          ::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "xauth.response.rights");
          #endif  // !NDEBUG
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* response::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:xauth.response)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required int32 code = 1;
  if (cached_has_bits & 0x00000008u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_code(), target);
  }

  // required string signature = 2;
  if (cached_has_bits & 0x00000001u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_signature().data(), static_cast<int>(this->_internal_signature().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "xauth.response.signature");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_signature(), target);
  }

  // optional string desc = 3;
  if (cached_has_bits & 0x00000002u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_desc().data(), static_cast<int>(this->_internal_desc().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "xauth.response.desc");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_desc(), target);
  }

  // optional string rights = 4;
  if (cached_has_bits & 0x00000004u) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::VerifyUTF8StringNamedField(
      this->_internal_rights().data(), static_cast<int>(this->_internal_rights().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::SERIALIZE,
      "xauth.response.rights");
    target = stream->WriteStringMaybeAliased(
        4, this->_internal_rights(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:xauth.response)
  return target;
}

size_t response::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:xauth.response)
  size_t total_size = 0;

  if (_internal_has_signature()) {
    // required string signature = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_signature());
  }

  if (_internal_has_code()) {
    // required int32 code = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_code());
  }

  return total_size;
}
size_t response::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:xauth.response)
  size_t total_size = 0;

  if (((_has_bits_[0] & 0x00000009) ^ 0x00000009) == 0) {  // All required fields are present.
    // required string signature = 2;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_signature());

    // required int32 code = 1;
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_code());

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000006u) {
    // optional string desc = 3;
    if (cached_has_bits & 0x00000002u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_desc());
    }

    // optional string rights = 4;
    if (cached_has_bits & 0x00000004u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
          this->_internal_rights());
    }

  }
  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void response::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:xauth.response)
  GOOGLE_DCHECK_NE(&from, this);
  const response* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<response>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:xauth.response)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:xauth.response)
    MergeFrom(*source);
  }
}

void response::MergeFrom(const response& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:xauth.response)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x0000000fu) {
    if (cached_has_bits & 0x00000001u) {
      _has_bits_[0] |= 0x00000001u;
      signature_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.signature_);
    }
    if (cached_has_bits & 0x00000002u) {
      _has_bits_[0] |= 0x00000002u;
      desc_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.desc_);
    }
    if (cached_has_bits & 0x00000004u) {
      _has_bits_[0] |= 0x00000004u;
      rights_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.rights_);
    }
    if (cached_has_bits & 0x00000008u) {
      code_ = from.code_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void response::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:xauth.response)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void response::CopyFrom(const response& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:xauth.response)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool response::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000009) != 0x00000009) return false;
  return true;
}

void response::InternalSwap(response* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  signature_.Swap(&other->signature_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  desc_.Swap(&other->desc_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  rights_.Swap(&other->rights_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(code_, other->code_);
}

::PROTOBUF_NAMESPACE_ID::Metadata response::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace xauth
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::xauth::request* Arena::CreateMaybeMessage< ::xauth::request >(Arena* arena) {
  return Arena::CreateInternal< ::xauth::request >(arena);
}
template<> PROTOBUF_NOINLINE ::xauth::response* Arena::CreateMaybeMessage< ::xauth::response >(Arena* arena) {
  return Arena::CreateInternal< ::xauth::response >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
