#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <system_error>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

int main() {
    map<string, vector<string>> filesByExtension;
    error_code ec;

    fs::directory_iterator it(".", ec);
    for (; !ec && it != fs::directory_iterator(); it.increment(ec)) {
        error_code statusEc;
        if (it->is_regular_file(statusEc)) {
            fs::path file = it->path().filename();
            string extension = file.extension().string();
            if (!extension.empty()) {
                extension.erase(0, 1);
            }
            filesByExtension[extension].push_back(file.string());
        }
    }
    if (ec) {
        cerr << "lsext: " << ec.message() << endl;
        return EXIT_FAILURE;
    }

    for (const auto &extensionFiles : filesByExtension) {
        string extension = extensionFiles.first;
        vector<string> files = extensionFiles.second;
        sort(files.begin(), files.end());
        cout << (extension.empty() ? "(no extension)" : extension) << ":"
             << endl;
        for (const string &file : files) {
            cout << "- " << file << endl;
        }
        cout << endl;
    }

    return 0;
}
