// Universal patcher. dos32a.patch patches out the annoying dos32a messages for CLI programs.

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

int main(int argc, char * argv[]) {
    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);
    std::ifstream patch(argv[3]);
    std::map<std::vector<unsigned char>, std::vector<unsigned char>> patches;
    std::string line;
    while (std::getline(patch, line)) {
        std::string::size_type pos = line.find(" = ");
        if (pos != std::string::npos) {
            std::string from = line.substr(0, pos);
            std::string to = line.substr(pos + 3);
            from.erase(std::remove_if(from.begin(), from.end(), [](char c) { return !isxdigit(c); }), from.end());
            to.erase(std::remove_if(to.begin(), to.end(), [](char c) { return !isxdigit(c); }), to.end());
            if (from.size() % 2 == 0 && to.size() % 2 == 0) {
                std::vector<unsigned char> from_bytes;
                for (std::string::size_type i = 0; i < from.size(); i += 2) {
                    from_bytes.push_back(std::stoi(from.substr(i, 2), nullptr, 16));
                }
                std::vector<unsigned char> to_bytes;
                for (std::string::size_type i = 0; i < to.size(); i += 2) {
                    to_bytes.push_back(std::stoi(to.substr(i, 2), nullptr, 16));
                }
                std::cout << "Registered patch: " << from << " -> " << to << "\n";
                patches[from_bytes] = to_bytes;
            }
        }
    }
    std::vector<unsigned char> input_bytes(std::istreambuf_iterator<char>(input), {});
    for (size_t i = 0; i < input_bytes.size(); ++i) {
        std::vector<unsigned char> best_match;
        for (auto & patch : patches) {
            if (patch.first.size() > best_match.size() && std::equal(patch.first.begin(), patch.first.end(), input_bytes.begin() + i)) {
                best_match = patch.first;
            }
        }
        if (!best_match.empty()) {
            std::cout << "Patching at " << i << " with " << best_match.size() << " bytes\n";
            output.write(reinterpret_cast<const char *>(patches[best_match].data()), patches[best_match].size());
            i += best_match.size() - 1;
        } else {
            output.write(reinterpret_cast<const char *>(&input_bytes[i]), 1);
        }
    }
    return 0;
}
