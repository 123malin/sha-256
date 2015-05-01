#include <iostream>
#include <vector>
#include <sstream>

typedef unsigned int WORD;

std::vector<WORD> M; // Message to be hashed
int l = 0;           // Message length in bits


/**
 * Return the number of bytes needed to store the word.
 */
const int needed_bytes(const WORD i)
{
    int bytes = 1;
    if (i == 0)       return 0;
    if (i > 255)      ++bytes;
    if (i > 65535)    ++bytes;
    if (i > 16777215) ++bytes;
    return bytes;
}

/**
 * Take the given hex string and store it in a word vector.
 * Also update the message length.
 */
const void store_message(const std::string &hex_str)
{
    WORD str_length = hex_str.length();
    // Pick out whole words from the the hex string
    WORD i = 0;
    for (; i < str_length / 8; ++i)
    {
        WORD word = std::stoul(hex_str.substr(i * 8, 8), nullptr, 16);
        M.push_back(word);
        l += 32;
    }

    // Pick out the last bytes which does not fit a word
    if (str_length % 8 != 0)
    {
        WORD word = std::stoul(hex_str.substr(i * 8, hex_str.length() - i * 8),
                nullptr, 16);
        M.push_back(word);
        l += needed_bytes(word) * 8;
    }
}

/**
 * Calculate the required padding of the message.
 * Return the required padding.
 */
const int calc_padding(const int l)
{
    int k = 0;
    while ((l + 1 + k % 512) != 448) ++k;
    return k;
}

int main()
{
    // Read each line as a hex string to be hashed
    for (std::string line; std::getline(std::cin, line);) {
        store_message(line);
        std::cout << "M: ";
        for (WORD i = 0; i < M.size(); ++i)
            std::cout << M[i] << " ";
        std::cout << std::endl;

        std::cout << "l: " << l << std::endl;

        // Check if message needs padding
        M.clear(); // Reset message to hash a new one
    }
}
