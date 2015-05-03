/**
 * SHA-256 implemented according to the specification:
 * http://csrc.nist.gov/publications/fips/fips180-4/fips-180-4.pdf
 */
#include <iostream>
#include <sstream>
#include <vector>

typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long long ll;

std::vector<BYTE> bytes; // Plain and padded message bytes
std::vector<WORD> M;     // Message to be hashed
std::vector<WORD> H(8);  // Hashed message
ll l = 0;                // Message length in bits


/**
 * Take the given hex string and store the bytes in a global vector.
 * Also update the message length.
 */
const void store_message_bytes(const std::string &hex_str)
{
    int str_length = hex_str.length();
    std::cout << "Message string length: " << str_length << std::endl;
    for (int i = 0, j = 0; i < str_length / 2; ++i, j += 2)
    {
        BYTE byte = std::stoi(hex_str.substr(j, 2), nullptr, 16);
        bytes.push_back(byte);
        l += 8;
    }
}

/**
 * Calculate the required padding of the message.
 * Return the required padding.
 */
const int calc_padding()
{
    int k = 0;
    while ((l + 1 + k) % 512 != 448) ++k;
    return k;
}

/**
 * Pad the message bytes according to the specification.
 */
const void pad_message()
{
    int k = calc_padding();
    int zeroes = 0; // DEBUG

    if (l % 512 == 0) return; // No padding necessary

    std::cout << "k: " << k << std::endl;

    // Append 1 followed by zeroes before the least significant bit,
    // assuming full bytes as input
    bytes.push_back(0x80);
    k = k - 7;
    zeroes += 7;

    // Append the remaining zeroes of k
    for (int i = 0; i < (k / 8); ++i)
    {
        bytes.push_back(0);
        zeroes += 8;
    }

    std::cout << "zeroes: " << zeroes << std::endl;

    /* l = 0x8040201008040201; // DEBUG */

    // Finally append the length in binary to the message as the least
    // significant bits, assuming a 64-bit number
    for (int i = 1; i < 9; ++i)
    {
        bytes.push_back(l >> (64 - i * 8));
        /* std::cout << "Append byte: " << (WORD) (BYTE) (l >> (64 - i * 8)) << std::endl; */
    }
}

/**
 * Parse the message into N 512-bit blocks.
 */
const void parse_message()
{
    for (WORD i = 0, j = 0; i < bytes.size() / 4; ++i)
    {
        WORD word = 0;
        for (int k = 0; k < 4; ++k, ++j)
        {
            word <<= 8;
            word |= bytes[j];
        }
        M.push_back(word);
    }
}

/**
 * Initialise the hash value H_0.
 */
const void init_hash()
{
    H = {0x6a09e667,
         0xbb67ae85,
         0x3c6ef372,
         0xa54ff53a,
         0x510e527f,
         0x9b05688c,
         0x1f83d9ab,
         0x5be0cd19};
}

int main()
{
    // Read each line as a hex string to be hashed
    for (std::string line; std::getline(std::cin, line);) {
        // TODO Handle empty lines
        // Store the plain bytes of the message
        store_message_bytes(line);

        std::cout << "Read plain bytes:" << std::endl;
        for (WORD i = 0; i < bytes.size(); ++i)
            std::cout << std::hex << (WORD) bytes[i] << std::dec << " ";
        std::cout << std::endl;
        std::cout << "l: " << l << std::endl;

        std::cout << "Number of plain bytes: " << bytes.size() << std::endl;

        // Pad the bytes if necessary
        if (l % 512 != 0) pad_message();

        std::cout << "Padded bytes:" << std::endl;
        for (WORD i = 0; i < bytes.size(); ++i)
            std::cout << std::hex << (WORD) bytes[i] << std::dec << " ";
        std::cout << std::endl;

        std::cout << "Number of padded bytes: " << bytes.size() << std::endl;

        // Parse message into word blocks
        parse_message();

        std::cout << "Parsed words:" << std::endl;
        for (WORD i = 0; i < M.size(); ++i)
            std::cout << std::hex << M[i] << std::dec << " ";
        std::cout << std::endl;

        std::cout << "Number of parsed words: " << M.size() << std::endl;

        // Set the inital hash value
        init_hash();

        std::cout << "Hash:" << std::endl;
        for (WORD i = 0; i < H.size(); ++i)
            std::cout << std::hex << H[i] << std::dec << " ";
        std::cout << std::endl;

        // Reset message to hash a new one
        bytes.clear();
        M.clear();
        H.clear();
    }
}
