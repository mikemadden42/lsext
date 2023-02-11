#include <dirent.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main() {
    DIR *dir;
    struct dirent *ent;
    map<string, vector<string>> filesByExtension;

    if ((dir = opendir(".")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                string file = ent->d_name;
                string extension = file.substr(file.find_last_of(".") + 1);
                filesByExtension[extension].push_back(file);
            }
        }
        closedir(dir);
    } else {
        perror("");
        return EXIT_FAILURE;
    }

    for (const auto &extensionFiles : filesByExtension) {
        string extension = extensionFiles.first;
        vector<string> files = extensionFiles.second;
        sort(files.begin(), files.end());
        cout << extension << ":" << endl;
        for (const string &file : files) {
            cout << "- " << file << endl;
        }
        cout << endl;
    }

    return 0;
}
