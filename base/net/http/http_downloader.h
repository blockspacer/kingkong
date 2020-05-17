#ifndef _HTTP_DOWNLOADER_H_
#define _HTTP_DOWNLOADER_H_
#include <base/message_loop/message_pump.h>
#include <base/base_header.h>
#include <map>
BEGIN_NAMESPACE_NET

class HttpDownloader {
 public:
  virtual ~HttpDownloader() = default;

  class HttpDownloaderDelegate {
   public:
    virtual void OnStartDownload() = 0;
    virtual void OnDownloadProgress(int64_t current_size,
                                    int64_t totla_size) = 0;
    virtual void OnDownloadComplte(int32_t status) = 0;
  };

  struct HttpDownloaderRequest {
    std::string url;
    std::string path;
    // socks 5代理相关
    std::string proxy_host;
    uint16_t proxy_port;
    std::string proxy_username;
    std::string proxy_passwd;
  };

 public:
  virtual void Start() = 0;
  virtual void Cancel() = 0;
};

std::shared_ptr<HttpDownloader> CreateHttpDownloader(
    std::unique_ptr<HttpDownloader::HttpDownloaderRequest> request,
    HttpDownloader::HttpDownloaderDelegate* delegate,
    std::shared_ptr<BASE_LOOPER::MessagePump> pump);

END_NAMESPACE_NET


#endif // !_HTTP_CLIENT_H_
