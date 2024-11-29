#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>

namespace fs = std::filesystem;

std::ostream& operator <<(std::ostream& into, const fs::path& path)
{
	into << path.string();	
	return into;
}

static std::string trimLine(std::string in)
{
	size_t start = in.find_first_not_of(' ');
	if (start == std::string::npos) {
		start = 0;
	}

	size_t end = in.find_last_not_of(" ");
	if (end == std::string::npos) {
		end = in.size();
	}

	return in.substr(start, (end + 1) - start);
}

static bool checkFileNotAscii(const std::string& filePath, std::vector<std::string>& lines)
{
    std::ifstream file(filePath);

    if (!file.good()) {
        std::cerr << "Failed to read file " << filePath << "\n";
        return false;
    }

    std::string line;
    while(std::getline(file, line))
	{
		for (char ch: line) {
		  if (!(ch >= 0 && ch <= 127)) {
			  // Probably a binary file. Do not check it.
			  return false;
		  }
		}

        lines.emplace_back(line);
    }

    return !lines.empty();
}

static bool containsAny(const std::string& text, const std::vector<std::string>& elems)
{
    for (const auto& elem: elems) {
        if (text.find(elem) != std::string::npos) {
            return true;
        }
    }
    return false;
}

int main(int argc, char** argv)
{
    if (argc <= 1) {
        std::cerr << "No arguments passed!\n";
        std::exit(1);
    }

    std::vector<std::string> words;

    for (int i = 1; i < argc; i++) {
        words.emplace_back(argv[i]);
    }

	assert(words.size() > 0);

    const auto path = fs::current_path();
    for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
    {
		const std::string entryPathStr = entry.path().string();

        if (!entry.is_regular_file()) {
            continue;
        }

        std::vector<std::string> lines;
        if (checkFileNotAscii(entryPathStr, lines)) {
            for (size_t i = 0; i < lines.size(); i++)
            {
                if (containsAny(lines[i], words))
                {
		   			std::string trimmedLine = trimLine(lines[i]);
		    		fs::path relPath = entry.path().lexically_relative(path);

                    std::cout << relPath << ":" << i << " " << trimmedLine << '\n';
                }
            }
        }
    }
}
