#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <curl/curl.h>

extern "C" {

struct DownloadTask {
    std::string url;
    std::string filename;
    long start;
    long end;
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::ofstream* outFile = static_cast<std::ofstream*>(userp);
    size_t totalSize = size * nmemb;
    outFile->write(static_cast<const char*>(contents), totalSize);
    return totalSize;
}

void DownloadPart(const DownloadTask& task) {
    CURL* curl;
    CURLcode res;
    std::ofstream outFile(task.filename, std::ios::binary | std::ios::in);

    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << task.filename << std::endl;
        return;
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Range: bytes=" + std::to_string(task.start) + "-" + std::to_string(task.end)).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, task.url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    outFile.close();
}

void download(const char* url, const char* filename, long fileSize, int numThreads) {
    std::vector<std::thread> threads;

    long partSize = fileSize / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        long start = i * partSize;
        long end = (i == numThreads - 1) ? fileSize - 1 : start + partSize - 1;
        DownloadTask task { url, std::string(filename) + "." + std::to_string(i), start, end };
        threads.emplace_back(DownloadPart, task);
    }

    for (auto& t : threads) {
        t.join();
    }

    // Combine parts
    std::ofstream outFile(filename, std::ios::binary);
    for (int i = 0; i < numThreads; ++i) {
        std::ifstream partFile(std::string(filename) + "." + std::to_string(i), std::ios::binary);
        outFile << partFile.rdbuf();
        partFile.close();
        std::remove((std::string(filename) + "." + std::to_string(i)).c_str());
    }
    outFile.close();

    std::cout << "Download completed!" << std::endl;
}

}