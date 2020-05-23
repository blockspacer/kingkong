#include "http_downloader_impl.h"
#include <base/log.h>

BEGIN_NAMESPACE_NET

HttpDownloaderImpl::HttpDownloaderImpl(
    std::unique_ptr<HttpDownloader::HttpDownloaderRequest> request,
    HttpDownloader::HttpDownloaderDelegate* delegate)
    : delegate_(delegate), download_request_(std::move(request)) {
  auto http_request = std::make_unique<HttpClient::HttpClientRequest>();
  http_request->url = download_request_->url;
  http_request->method = HttpClient::kHttpMethodGet;
  http_request->proxy_host = download_request_->proxy_host;
  http_request->proxy_passwd = download_request_->proxy_passwd;
  http_request->proxy_port = download_request_->proxy_port;
  http_request->proxy_username = download_request_->proxy_username;

  http_client_ =
      CreateHttpClient(std::move(http_request), this);
}

void HttpDownloaderImpl::OnHttpComplete(boost::beast::http::status http_code) {
  boost::system::error_code code;
  file_.close(code);
  NotifyDownloadComplte((http_code == boost::beast::http::status::ok) ? 0 : -1);
}

void HttpDownloaderImpl::NotifyDownloadStart() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (nullptr != delegate_) {
    delegate_->OnStartDownload();
  }
}

void HttpDownloaderImpl::NotifyDownloadProgress() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (nullptr != delegate_) {
    delegate_->OnDownloadProgress(write_size_, total_size_);
  }
}

void HttpDownloaderImpl::NotifyDownloadComplte(int32_t code) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (nullptr != delegate_) {
    delegate_->OnDownloadComplte(code);
  }
}

void HttpDownloaderImpl::Start() {
  http_client_->Request();
}

void HttpDownloaderImpl::Cancel() {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    delegate_ = nullptr;
  }
  if (nullptr != http_client_) {
    http_client_->Cancel();
  }
}

void HttpDownloaderImpl::OnHttpResponse(boost::string_view content) {
  if (!file_.is_open()) {
    boost::system::error_code code;
    file_.open(download_request_->path.c_str(), boost::beast::file_mode::write, code);
    if (code) {
      //打开文件失败，回调完成，退出
      LogError << "open file failed:" << download_request_->path << code.message();
      NotifyDownloadComplte(-1);
      Cancel();
      return;
    }
    //通知开始下载
    NotifyDownloadStart();
  }
  boost::system::error_code code;
  file_.write(content.data(), content.size(), code);
  if (code) {
    //写入失败
    LogError << "write file failed:" << download_request_->path << code.message();
    NotifyDownloadComplte(-1);
    Cancel();
    return;
  }
  write_size_ += content.size();
  NotifyDownloadProgress();
}

void HttpDownloaderImpl::OnHttpHeads(
    const std::unordered_map<std::string, std::string>& heads) {
  auto it = heads.find("Content-Length");
  if (it != heads.end()) {
    total_size_ = boost::lexical_cast<uint64_t>(it->second);
  }
}


END_NAMESPACE_NET


