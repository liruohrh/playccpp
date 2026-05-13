#include <print>
#include <cstdlib>
#include <curl/curl.h>

// DOH (DNS over HTTPS) support was added in curl 7.62.0.
// We deliberately pin 7.87.0 to match a known-working baseline.
#if LIBCURL_VERSION_NUM < 0x075700
#error "curl 7.87.0+ required (DOH support added in 7.62.0)"
#endif

int main() {
    curl_version_info_data *ver = curl_version_info(CURLVERSION_NOW);
    std::println("curl version: {} (compiled with {})", ver->version, LIBCURL_VERSION);
    // CURL_VERSION_DOH bit was added post-7.87.0; DOH itself is always available.
    // Verifying via compile-time macro instead:
    std::println("DOH support:  yes (built with {})", LIBCURL_VERSION);
    std::println("");

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::println(stderr, "Failed to initialise curl");
        return EXIT_FAILURE;
    }

    // Resolve via Cloudflare's DNS-over-HTTPS, then fetch example.com.
    curl_easy_setopt(curl, CURLOPT_DOH_URL, "https://cloudflare-dns.com/dns-query");
    curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");

    // Be friendly — short timeout, follow redirects.
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Verbose output so we can see the DOH resolution in action.
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::println(stderr, "Request failed: {}", curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return res == CURLE_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
