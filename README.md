# Computer_Security_LAB_2

- [x] part 1.1
- [x] part 1.2
- [x] part 2

# Part 1.1

[2024.2.17] **update** don't need to worry about the space problem refers to:
```c
    if ( argc != 4 ) {
		printf("Usage: %s [issuer] [accountName] [secretHex]\n", argv[0]);
		return(-1);
	}
```

* **ACCOUNTNAME**: The name of the account (e.g., “gibson”). Any special characters in this string (like spaces) should be properly “URL-encoded”. (I have provided a urlEncode() function that you may use for this purpose.)
* **ISSUER**: The name of the service (e.g., “Facebook”). As with the previous field, all special characters must be encoded. (e.g., “U of T” becomes “U%20of%20T”.)

[2024.2.15] part 1.1 README added.

[2024.2.9] done. README needed. Not sure if we need to consider the **space** or not.

# Part 1.2

[2024.2.17] part 1.2 README added.

[2024.2.11] TOTP implemented, README needed to be done :)

[2024.2.9] hmac has been correctly generated, still need TOTP to be done :)

# Part 2

[2024.2.12] part 2 README added.

[2024.2.11] part 2 finished, README needed to be done :)