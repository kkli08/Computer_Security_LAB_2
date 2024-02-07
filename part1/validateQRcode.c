#include <stdio.h>
#include <string.h>
#include <assert.h>
#define SHA1_DIGEST_LENGTH 20

#include "lib/sha1.h"

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
	SHA1_INFO ctx;
	uint8_t sha[SHA1_DIGEST_LENGTH];
	uint8_t secret_binary[SHA1_DIGEST_LENGTH];
	int secret_hex_length = strlen(secret_hex);
    int secret_binary_length = secret_hex_length / 2;

	hex_string_to_binary(secret_hex, secret_binary);

	// SHA1 Operation
	sha1_init(&ctx);
    sha1_update(&ctx, secret_binary, secret_binary_length);
	sha1_final(&ctx, sha);

	char hmac_str[SHA1_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA1_DIGEST_LENGTH; i++) {
        sprintf(&hmac_str[i * 2], "%02x", sha[i]);
    }

	printf("Generated secret code by sha1: %s\n", hmac_str);

	
	
    return strcmp(hmac_str, TOTP_string) == 0;
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
