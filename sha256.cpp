#include <iostream>
#include <vector>
#include <sstream>

typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long long ll;

std::vector<BYTE> bytes; // Plain and padded message bytes
std::vector<WORD> M;     // Message to be hashed
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
        std::cout << "Store byte: " << (WORD) byte << std::endl;
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

    // TODO Handle l > 512

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

    // l = 0x8040201008040201; // DEBUG

    // Finally append the length in binary to the message as the least
    // significant bits, assuming a 64-bit number
    for (int i = 1; i < 9; ++i)
    {
        bytes.push_back(l >> (64 - i * 8));
        std::cout << "Append byte: " << (WORD) (BYTE) (l >> (64 - i * 8)) << std::endl;
    }
}

int main()
{
    // Read each line as a hex string to be hashed
    for (std::string line; std::getline(std::cin, line);) {
        // TODO Handle empty lines and the byte 0x00
        store_message_bytes(line);

        std::cout << "Read plain bytes: ";
        for (WORD i = 0; i < bytes.size(); ++i)
            std::cout << (WORD) bytes[i] << " ";
        std::cout << std::endl;
        std::cout << "l: " << l << std::endl;

        std::cout << "Number of plain bytes: " << bytes.size() << std::endl;

        // Check if message needs padding
        if (l % 512 != 0) pad_message();

        std::cout << "Padded: ";
        for (WORD i = 0; i < bytes.size(); ++i)
            std::cout << (WORD) bytes[i] << " ";
        std::cout << std::endl;

        std::cout << "Padded bytes: " << bytes.size() << std::endl;

        bytes.clear(); // Reset message to hash a new one
        // TODO Reset M as well
    }
}
