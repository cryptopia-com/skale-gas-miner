#pragma once
#include <string>
#include "cryptopp/integer.h"
#include "cryptopp/secblock.h"

namespace Cryptopia 
{
    /**
     * Computes the Keccak-256 (SHA3) hash of a byte array and returns the hash as a hex string.
     * The resulting hash is in lowercase and prefixed with "0x".
     *
     * @param val The CryptoPP::SecByteBlock representing the byte array to be hashed.
     * @return A hex string representing the Keccak-256 hash of the input byte array.
     */
    std::string GetSoliditySha3(const CryptoPP::SecByteBlock& val);

    /**
     * Computes the Keccak-256 (SHA3) hash of a CryptoPP::Integer.
     * Internally, the integer is first converted to a byte array in hexadecimal format.
     *
     * @param val The CryptoPP::Integer to be hashed.
     * @return A hex string representing the Keccak-256 hash of the integer.
     */
    std::string GetSoliditySha3(const CryptoPP::Integer& val);

    /**
     * Computes the Keccak-256 (SHA3) hash of an unsigned long long integer.
     * The integer is first converted to a 32-byte (256-bit) hex string, left-padded with zeros,
     * then converted to a byte array, and finally hashed.
     *
     * @param val The unsigned long long integer to be hashed.
     * @return A hex string representing the Keccak-256 hash of the integer.
     */
    std::string GetSoliditySha3(const unsigned long long val);
}
