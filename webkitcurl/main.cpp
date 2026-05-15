#include <cstdio>
#include <cstdlib>
#include <curl/curl.h>
#include <print>

// DOH (DNS over HTTPS) support was added in curl 7.62.0.
// We deliberately pin 7.87.0 to match a known-working baseline.
#if LIBCURL_VERSION_NUM < 0x075700
#error "curl 7.87.0+ required (DOH support added in 7.62.0)"
#endif


std::string curlInfoText(int info) {
  switch (info) {
    case CURLINFO_TEXT:
      return "CURLINFO_TEXT";
    case CURLINFO_HEADER_IN:
      return "CURLINFO_HEADER_IN";
    case CURLINFO_HEADER_OUT:
      return "CURLINFO_HEADER_OUT";
    case CURLINFO_DATA_IN:
      return "CURLINFO_DATA_IN";
    case CURLINFO_DATA_OUT:
      return "CURLINFO_DATA_OUT";
    case CURLINFO_SSL_DATA_IN:
      return "CURLINFO_SSL_DATA_IN";
    case CURLINFO_SSL_DATA_OUT:
      return "CURLINFO_SSL_DATA_OUT";
    case CURLINFO_END:
      return "CURLINFO_END";
    default:
      return "unknown";
  }
}

FILE *log_file;
FILE *file_stderr = nullptr;
int didReceiveDebugInfoCallback(CURL *, curl_infotype type, char *data,
                                size_t size, void *userData) {
  if (!data)
    return 0;

  if (log_file) {
    fprintf(log_file, "[debugCallback]: [%s]: %s\n", curlInfoText(type).c_str(), data);
    fflush(log_file);
    return 0;
  }
  return 0;
}


int main(int argc, char *argv[]) {
  std::printf("argc: %d\n", argc);
  for (int i = 0; i < argc; i++) {
    std::printf("argv[%d]: %s\n", i, argv[i]);
  }

  curl_global_init(CURL_GLOBAL_DEFAULT);
  CURL *curl = curl_easy_init();
  if (!curl) {
    std::println(stderr, "Failed to initialise curl");
    return EXIT_FAILURE;
  }

  // Resolve via Cloudflare's DNS-over-HTTPS, then fetch example.com.
  auto doh_ret = curl_easy_setopt(curl, CURLOPT_DOH_URL,
                   "https://cloudflare-dns.com/dns-query");
  std::println("CURLOPT_DOH_URL setopt: {}", curl_easy_strerror(doh_ret));

  curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");

  // Be friendly — short timeout, follow redirects.
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  // Verbose output so we can see the DOH resolution in action.
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

  if (argc == 1 || strcmp(argv[1], "d") == 0) {
    std::println("Debug log specified, enabling debug logging");
    if (fopen_s(&log_file, "./curl.debug.log", "a") == 0 && log_file) {
      auto res = curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION,
                       didReceiveDebugInfoCallback);
      if (res == CURLE_OK) {
          std::println("Logging to curl.debug.log");
      } else {
          std::printf("Failed to set debug function: %d\n", res);
      }
    } else {
      std::println(stderr, "Failed to open curl.debug.log");
    }
  } else {
    std::println("No debug log specified, skipping debug logging");
  }

  if (argc == 1 || strcmp(argv[1], "e") == 0) {
    std::println("Error log specified, enabling error logging");
    if (fopen_s(&file_stderr, "./curl.log", "a") == 0 && file_stderr) {
      auto res = curl_easy_setopt(curl, CURLOPT_STDERR, file_stderr);
      if (res == CURLE_OK) {
          std::println("Logging to curl.log");
      } else {
          std::printf("Failed to set error logging: %d\n", res);
      }
    } else {
      std::println(stderr, "Failed to open curl.log");
    }
  } else {
    std::println("No error log specified, skipping error logging");
  }

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    std::println(stderr, "Request failed: {}", curl_easy_strerror(res));
  }

  curl_easy_cleanup(curl);
  curl_global_cleanup();
  return res == CURLE_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
