#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <time.h>
#include "lib/sha1.h"

#define SHA1_DIGEST_LENGTH 20

#define TOTP_TIME_STEP 30
#define TOTP_DIGITS 6
#define TOTP_MODULO 1000000 // 10^6 for a 6 digit code
#define BLOCK_SIZE 64 // Block size for SHA1 --> 512 bits = 64 byte

void hmac_sha1(const uint8_t *key, int key_length, const uint8_t *data, int data_length, uint8_t *hmac_output) {
    uint8_t k_ipad[BLOCK_SIZE] = {0};
    uint8_t k_opad[BLOCK_SIZE] = {0};
    SHA1_INFO ctx;
    uint8_t sha_inner[SHA1_DIGEST_LENGTH];
    uint8_t sha_outer[SHA1_DIGEST_LENGTH];

    // copy the key into pad
    memcpy(k_ipad, key, key_length);
    memcpy(k_opad, key, key_length);

    // XOR key with ipad and opad values
	// My own Blog: https://github.com/kkli08/HMAC/wiki#hmac
    for (int i = 0; i < BLOCK_SIZE; i++) {
		// 0x36 ->> 00110110
        k_ipad[i] ^= 0x36;
		// 0x5C ->> 01011100
        k_opad[i] ^= 0x5C;
    }

	// HMAC = H(k_opad, H(k_ipad, message))

    // Inner hash
    sha1_init(&ctx);
    sha1_update(&ctx, k_ipad, BLOCK_SIZE);
    sha1_update(&ctx, data, data_length);
    sha1_final(&ctx, sha_inner);

    // Outer hash
    sha1_init(&ctx);
    sha1_update(&ctx, k_opad, BLOCK_SIZE);
    sha1_update(&ctx, sha_inner, SHA1_DIGEST_LENGTH);
    sha1_final(&ctx, sha_outer);

    memcpy(hmac_output, sha_outer, SHA1_DIGEST_LENGTH);
}


// Function to convert hex string to binary
void hex_string_to_binary(const char *hex_string, uint8_t *binary_output) {
    size_t len = strlen(hex_string);
    for (size_t i = 0; i < len; i += 2) {
        sscanf(hex_string + i, "%2hhx", &binary_output[i / 2]);
    }
}

static int
validateTOTP(char * secret_hex, char * TOTP_string)
{
	// Step 1: Get the Current Time
    uint64_t time_counter = (uint64_t)time(NULL) / TOTP_TIME_STEP;
	// Convert time_counter to byte array
    uint8_t time_bytes[8];
    for (int i = 7; i >= 0; i--) {
        time_bytes[i] = time_counter & 0xFF;
        time_counter >>= 8;
    }

	// Step 2: Create an HMAC-SHA1 Value
    uint8_t hmac[SHA1_DIGEST_LENGTH];
    uint8_t secret_binary[SHA1_DIGEST_LENGTH];
    int secret_hex_length = strlen(secret_hex);
    int secret_binary_length = secret_hex_length / 2;
    hex_string_to_binary(secret_hex, secret_binary);
    
    // Use HMAC-SHA1
    hmac_sha1(secret_binary, secret_binary_length, time_bytes, sizeof(time_bytes), hmac);

    // Step 3: Dynamic Truncation
    int offset = hmac[SHA1_DIGEST_LENGTH - 1] & 0xF;
    uint32_t truncatedHash = 0;
    for (int i = 0; i < 4; ++i) {
        truncatedHash <<= 8;
        truncatedHash |= hmac[offset + i];
    }

    truncatedHash &= 0x7FFFFFFF; // Remove the most significant bit

    // Step 4: Generate TOTP Value
    uint32_t totp = truncatedHash % TOTP_MODULO;
    
    // Convert to string and compare
    char totp_str[7]; // 6 digits + null terminator
    snprintf(totp_str, sizeof(totp_str), "%06u", totp);

	printf("Generated totp code: %s\n", totp_str);
    return strcmp(totp_str, TOTP_string) == 0;
}


int
main(int argc, char * argv[])
{
	if ( argc != 3 ) {
		printf("Usage: %s [secretHex] [TOTP]\n", argv[0]);
		return(-1);
	}

	char *	secret_hex = argv[1];
	char *	TOTP_value = argv[2];

	assert (strlen(secret_hex) <= 20);
	assert (strlen(TOTP_value) == 6);

	printf("\nSecret (Hex): %s\nTOTP Value: %s (%s)\n\n",
		secret_hex,
		TOTP_value,
		validateTOTP(secret_hex, TOTP_value) ? "valid" : "invalid");

	return(0);
}
