#ifndef _HTTP_DOWNLOADER_IMPL_H_
#define _HTTP_DOWNLOADER_IMPL_H_
#include "http_downloader.h"
#include "http_client.h"
#include <base/message_loop/message_pump.h>
#include <base/base_header.h>
#include <map>
BEGIN_NAMESPACE_NET

class HttpDownloaderImpl : public HttpDownloader,
  public HttpClient::HttpClientDelegate {
public:
  HttpDownloaderImpl(
      std::unique_ptr<HttpDownloader::HttpDownloaderRequest> request,
      HttpDownloader::HttpDownloaderDelegate* delegate,
      std::shared_ptr<BASE_LOOPER::MessagePump> pump);


public:
  void Start() override;
  void Cancel() override;

protected:
  void OnHttpResponse(boost::string_view content) override;
  void OnHttpHeads(const std::unordered_map<std::string, std::string>& heads) override;

 void OnHttpComplete(boost::beast::http::status http_code) override;

protected:
  void NotifyDownloadStart();
  void NotifyDownloadProgress();
  void NotifyDownloadComplte(int32_t code);

private:
  uint64_t total_size_ = 0;
  uint64_t write_size_ = 0;
  boost::beast::file file_;
  std::mutex mutex_;
  HttpDownloader::HttpDownloaderDelegate* delegate_ = nullptr;
  std::unique_ptr<HttpDownloader::HttpDownloaderRequest> download_request_;
  std::shared_ptr<HttpClient> http_client_;
};


END_NAMESPACE_NET


#endif // !_HTTP_DOWNLOADER_IMPL_H_
