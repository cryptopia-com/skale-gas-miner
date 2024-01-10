#pragma once
#include <limits.h>
#include <chrono>
#include <string>
#include "cryptopp/integer.h"
#include "cryptopp/osrng.h"
#include "cryptopp/keccak.h"
#include "cryptopp/hex.h"
#include "../helpers/convert.h"
#include "../helpers/hashing.h"

namespace Cryptopia
{
    /**
     * Computes the Keccak-256 (SHA3) hash of a byte array and returns the hash as a hex string.
     * The resulting hash is in lowercase and prefixed with "0x".
     *
     * @param val The CryptoPP::SecByteBlock representing the byte array to be hashed.
     * @return A hex string representing the Keccak-256 hash of the input byte array.
     */
    std::string GetSoliditySha3(const CryptoPP::SecByteBlock& val)
    {
        // Hash the byte array
        CryptoPP::Keccak_256 hash;
        std::string digest;
        hash.Update(val, val.size());
        digest.resize(hash.DigestSize());
        hash.Final((unsigned char*)&digest[0]);

        // Convert hash to hex string
        std::string hexDigest;
        CryptoPP::StringSource(digest, true,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(hexDigest)));

        // Convert hexDigest to lowercase
        std::transform(
            hexDigest.begin(),
            hexDigest.end(),
            hexDigest.begin(),
            [](unsigned char c) {
                return std::tolower(c);
            });

        return "0x" + hexDigest;
    }

    /**
     * Computes the Keccak-256 (SHA3) hash of a CryptoPP::Integer.
     * Internally, the integer is first converted to a byte array in hexadecimal format.
     *
     * @param val The CryptoPP::Integer to be hashed.
     * @return A hex string representing the Keccak-256 hash of the integer.
     */
    std::string GetSoliditySha3(const CryptoPP::Integer& val)
    {
        return GetSoliditySha3(HexToByteArray(IntegerToString(val, 16)));
    }

    /**
     * Computes the Keccak-256 (SHA3) hash of an unsigned long long integer.
     * The integer is first converted to a 32-byte (256-bit) hex string, left-padded with zeros,
     * then converted to a byte array, and finally hashed.
     *
     * @param val The unsigned long long integer to be hashed.
     * @return A hex string representing the Keccak-256 hash of the integer.
     */
    std::string GetSoliditySha3(const unsigned long long val)
    {
        // Convert val to 32-byte (256-bit) hex string, left-padded with zeros
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(64) << val;
        std::string valHex = ss.str();

        // Convert hex string to byte array
        CryptoPP::SecByteBlock byteBlock(valHex.size() / 2);
        CryptoPP::StringSource ss2(valHex, true, new CryptoPP::HexDecoder);
        ss2.Pump(byteBlock.size());
        ss2.Get(byteBlock, byteBlock.size());

        return GetSoliditySha3(byteBlock);
    }
}