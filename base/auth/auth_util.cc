#include "auth_util.h"
#include "auth.pb.h"
#include <boost/serialization/singleton.hpp> 
#include <base/net/http/http_client.h>
#include <base/crypto_util.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

BEGIN_NAMESPACE_AUTH

struct AuthInfo {
  std::string url;
  std::string random_id;
  std::string device_id;
  int32_t product_type;
  int32_t version;
  std::string token;
  AuthResponseDelegate delegate;
  std::shared_ptr<BASE_NET::HttpClient> http_client;
  std::string response_data;
};

class AuthManager :public boost::serialization::singleton<AuthManager> ,
  public BASE_NET::HttpClient::HttpClientDelegate {
public:
  int32_t AuthRequest(AuthInfo info) {
    //构造一个PB 数据包
    xauth::request request;
    request.set_type((xauth::product)info.product_type);
    request.set_token(info.token);
    request.set_version(info.version);
    info.device_id = "nodeviceid";
    request.set_deviceid(info.device_id);
    boost::uuids::uuid random_id = boost::uuids::random_generator()();
    info.random_id = boost::uuids::to_string(random_id);
    request.set_randomid(info.random_id);
    std::string serial;
    request.SerializeToString(&serial);

    //RSA 公钥加密
    std::string chip_text;
    BASE_CRYPTO::EncryptByPublicKey(serial, chip_text);
    std::string base64_chip_text = BASE_CRYPTO::Base64EncodeUrlSafe(chip_text);

    //构建http请求
    std::stringstream get_url;
    get_url << info.url << "?data=" << base64_chip_text;

    auto http_request =
        std::make_unique<BASE_NET::HttpClient::HttpClientRequest>();
    http_request->url = get_url.str();
    http_request->method = BASE_NET::HttpClient::kHttpMethodGet;

    auto http_client = BASE_NET::CreateHttpClient(std::move(http_request), this);
    http_client->Request();


    int32_t tmp_id = 0;
    info.http_client = std::move(http_client);
    {
      std::lock_guard<std::mutex> lock(mutex_);
      tmp_id = ++id_;
      auth_map_[tmp_id] = std::move(info);
    }

    return tmp_id;
  }

  void AuthCancel(int32_t id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto iter = auth_map_.find(id);
    if (iter != auth_map_.end()) {
      auth_map_.erase(iter);
    }
  }

private:
  //http 请求回调
  void OnHttpHeads(
      BASE_NET::HttpClient* client,
      const std::unordered_map<std::string, std::string>& heads) override {}

  void OnHttpResponse(BASE_NET::HttpClient* client,
                              boost::string_view content) override {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& item : auth_map_) {
      if (item.second.http_client.get() == client) {
        item.second.response_data.append(content.data(), content.length());
        break;
      }
    }
  }

  void HandleAuthResponse(std::map<int32_t, AuthInfo>::iterator& it, boost::beast::http::status http_code) {
    if (http_code != boost::beast::http::status::ok) {
      it->second.delegate((int32_t)http_code, "failed");
      return;
    }
    //PB 解析一下
    xauth::response response;
    response.ParseFromString(BASE_CRYPTO::Base64Decode(it->second.response_data));
    if (response.code() != (int32_t)boost::beast::http::status::ok) {
      it->second.delegate(response.code(), response.desc());
      return;
    }
    //校验签名
    std::stringstream signature_source;
    signature_source << it->second.product_type << it->second.version << it->second.token << it->second.device_id << it->second.random_id;
    bool success = BASE_CRYPTO::RSA_Verify(signature_source.str(), BASE_CRYPTO::Base64Decode(response.signature()));
    if (success) {
      it->second.delegate(response.code(), response.desc());
    } else {
      it->second.delegate(401, response.desc());
    }
  }

  void OnHttpComplete(BASE_NET::HttpClient* client,
                      boost::beast::http::status http_code) override {
    std::lock_guard<std::mutex> lock(mutex_);
    auto begin = auth_map_.begin();
    for (; begin != auth_map_.end(); begin++) {
      if (begin->second.http_client.get() == client) {
        HandleAuthResponse(begin, http_code);
        begin->second.http_client->Cancel();
        auth_map_.erase(begin);
        break;
      }
    }
  }

 private:
  int32_t id_ = 0;
  std::mutex mutex_;
  std::map<int32_t, AuthInfo> auth_map_;
};

int32_t AuthRequest(const std::string& url,
                    int32_t product_type,
                    int32_t version,
                    const std::string& token,
                    AuthResponseDelegate delegate) {
  
  AuthInfo info;
  info.url = url;
  info.version = version;
  info.delegate = std::move(delegate);
  info.product_type = product_type;
  info.token = token;
  return AuthManager::get_mutable_instance().AuthRequest(std::move(info));
}


void AuthCancel(int32_t id) {
  AuthManager::get_mutable_instance().AuthCancel(id);
}


END_NAMESPACE_AUTH


