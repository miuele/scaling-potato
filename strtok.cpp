#include <cstring>
#include <iostream>
#include <iomanip>
#include <cassert>

namespace my {

char *strtok(char *str, const char *delims) {
    static char *cursor;
    if (str) {
        cursor = str;
    }

    char *token_start = nullptr;
    for (; *cursor != '\0'; ++cursor) {
        bool has_match = false;
        for (const char *d = delims; *d != '\0'; ++d) {
            if (*cursor == *d) {
                has_match = true;
                break;
            }
        }
        if (!has_match) {
            token_start = cursor;
            ++cursor;
            break;
        }
    }

    for (; *cursor != '\0'; ++cursor) {
        for (const char *d = delims; *d != '\0'; ++d) {
            if (*cursor == *d) {
                *cursor = '\0';
                ++cursor;
                goto out;
            }
        }
    }

out:
    return token_start;
}

}

int test() {
    char input[] = "one + two * (three - four)!";
    const char *delims = "! +-(*)";

    const char *token = my::strtok(input, delims);
    while (token) {
        std::cout << std::quoted(token) << ' ';
        token = my::strtok(nullptr, delims);
    }

    std::string s;
    s.reserve(sizeof input);
    for (std::size_t i = 0; i < sizeof input; ++i) {
        if (input[i] == '\0') {
            s.append("\\0");
        } else {
            s.push_back(input[i]);
        }
    }

    std::cout << "\ncontents of the input string now:\n" << std::quoted(s) << '\n';
}
