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

int main(int argc, char *argv[]) {
    const string usage = "usage: lsext [directory]";

    if (argc == 2 && string(argv[1]) == "--help") {
        cout << usage << endl;
        return 0;
    }
    if (argc > 2 || (argc == 2 && argv[1][0] == '-')) {
        cerr << usage << endl;
        return EXIT_FAILURE;
    }

    map<string, vector<string>> filesByExtension;
    fs::path dir = argc == 2 ? argv[1] : ".";
    error_code ec;

    fs::directory_iterator it(dir, ec);
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
        cerr << "lsext: " << dir.string() << ": " << ec.message() << endl;
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

    if (!cout.flush()) {
        cerr << "lsext: error writing output" << endl;
        return EXIT_FAILURE;
    }

    return 0;
}
