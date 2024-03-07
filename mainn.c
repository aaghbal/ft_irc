#include <stdio.h>

// Function to convert integer to IPv4 address
void intToIp(unsigned int ipInteger, char ipString[]) {
    sprintf(ipString, "%d.%d.%d.%d", (ipInteger >> 24) & 0xFF, (ipInteger >> 16) & 0xFF, (ipInteger >> 8) & 0xFF, ipInteger & 0xFF);
}

int main() {
    // Example: Convert integer 3232235521 to IPv4 address
    unsigned int ipAddress = htonl(16777343);
    char ipString[16]; // IPv4 addresses are at most 15 characters long

    // Call the conversion function
    intToIp(ipAddress, ipString);

    // Display the result
    printf("Method 1 - Bitwise Shifting and Masking:\n");
    printf("Integer: %u\nIPv4 Address: %s\n", ipAddress, ipString);

    return 0;
}