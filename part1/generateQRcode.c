#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lib/encoding.h"

// Function to convert hex string to binary
void hex_string_to_binary(const char *hex_string, uint8_t *binary_output) {
    size_t len = strlen(hex_string);
    for (size_t i = 0; i < len; i += 2) {
        sscanf(hex_string + i, "%2hhx", &binary_output[i / 2]);
    }
}

int
main(int argc, char * argv[])
{
	if ( argc != 4 ) {
		printf("Usage: %s [issuer] [accountName] [secretHex]\n", argv[0]);
		return(-1);
	}

	// encode function:
	// const char * urlEncode(const char * s);
	// int base32_encode(const uint8_t *data, int length, uint8_t *result,
    //               int bufSize)
    // __attribute__((visibility("hidden")));


	const char *	issuer = urlEncode(argv[1]);
	const char *	accountName = urlEncode(argv[2]);
	char *	secret_hex = argv[3];

	printf("\nIssuer: %s\nAccount Name: %s\nSecret (Hex): %s\n\n",
		issuer, accountName, secret_hex);

	size_t secret_len = strlen(secret_hex) / 2; // Length of binary data
    uint8_t *binary_data = malloc(secret_len);

	hex_string_to_binary(secret_hex, binary_data);

    // Prepare the buffer for the result. Size depends on the expected output size of the base32 encoding.
    size_t result_size = secret_len * 2; 
    uint8_t *result = malloc(result_size);

	// Call the base32_encode function
	// store the value in result
	base32_encode(binary_data, secret_len, result, result_size);
   
	assert (strlen(secret_hex) <= 20);

	// Create an otpauth:// URI and display a QR code that's compatible
	// with Google Authenticator
	// Calculate the required buffer size and allocate memory
    size_t url_size = strlen("otpauth://totp/") + strlen(accountName) + strlen("?issuer=") + 
                      strlen(issuer) + strlen("&secret=") + strlen(result) + strlen("&period=30") + 1;

    char *url = malloc(url_size);

	// Format the URL string
    snprintf(url, url_size, "otpauth://totp/%s?issuer=%s&secret=%s&period=30", accountName, issuer, result);

    printf("URL: %s\n", url);


	displayQRcode(url);

	return (0);
}
