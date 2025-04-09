#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_HASH	3731
#define INITIAL_SEED	7	

// Prototipi delle funzioni di hashing
UINT32 HashStringJenkinsOneAtATime32BitW(_In_ PWCHAR String);
DWORD HashStringDjb2W(_In_ PWCHAR String);
DWORD HashStringLoseLoseW(_In_ PWCHAR String);
INT HashStringRotr32W(_In_ PWCHAR String);

// Generate JenkinsOneAtATime32Bit hashes from wide-character input string
UINT32 HashStringJenkinsOneAtATime32BitW(_In_ PWCHAR String)
{
	SIZE_T Index = 0;
	UINT32 Hash = 0;
	SIZE_T Length = lstrlenW(String);

	while (Index != Length)
	{
		Hash += String[Index++];
		Hash += Hash << INITIAL_SEED;
		Hash ^= Hash >> 6;
	}

	Hash += Hash << 3;
	Hash ^= Hash >> 11;
	Hash += Hash << 15;

	return Hash;
}

// generate Djb2 hashes from wide-character input string
DWORD HashStringDjb2W(_In_ PWCHAR String)
{
	ULONG Hash = INITIAL_HASH;
	INT c;

	while (c = *String++)
		Hash = ((Hash << INITIAL_SEED) + Hash) + c;

	return Hash;
}

// Generate LoseLose hashes from wide-character input string
DWORD HashStringLoseLoseW(_In_ PWCHAR String)
{
	ULONG Hash = 0;
	INT c;

	while (c = *String++) {
		Hash += c;
		Hash *= c + INITIAL_SEED;	// update
	}

	return Hash;
}

// Helper function that apply the bitwise rotation
UINT32 HashStringRotr32Sub(UINT32 Value, UINT Count)
{
	DWORD Mask = (CHAR_BIT * sizeof(Value) - 1);
	Count &= Mask;
#pragma warning( push )
#pragma warning( disable : 4146)
	return (Value >> Count) | (Value << ((-Count) & Mask));
#pragma warning( pop ) 
}

// Generate Rotr32 hashes from wide-character input string
INT HashStringRotr32W(_In_ PWCHAR String)
{
	INT Value = 0;

	for (INT Index = 0; Index < lstrlenW(String); Index++)
		Value = String[Index] + HashStringRotr32Sub(Value, INITIAL_SEED);

	return Value;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <hash_type> <url>\n", argv[0]);
        printf("Hash types: jenkins, djb2, loselose, rotr32\n");
        return 1;
    }

    char* hashType = argv[1];
    char* url = argv[2];

    // Converti l'URL in wide-character string
    size_t urlLength = strlen(url) + 1;
    PWCHAR wideUrl = (PWCHAR)malloc(urlLength * sizeof(WCHAR));
    if (!wideUrl) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    mbstowcs(wideUrl, url, urlLength);

    // Determina il tipo di hashing e calcola l'hash
    if (strcmp(hashType, "jenkins") == 0) {
        UINT32 hash = HashStringJenkinsOneAtATime32BitW(wideUrl);
        printf("Jenkins hash: %u\n", hash);
    } else if (strcmp(hashType, "djb2") == 0) {
        DWORD hash = HashStringDjb2W(wideUrl);
        printf("Djb2 hash: %u\n", hash);
    } else if (strcmp(hashType, "loselose") == 0) {
        DWORD hash = HashStringLoseLoseW(wideUrl);
        printf("LoseLose hash: %u\n", hash);
    } else if (strcmp(hashType, "rotr32") == 0) {
        INT hash = HashStringRotr32W(wideUrl);
        printf("Rotr32 hash: %d\n", hash);
    } else {
        printf("Invalid hash type. Valid types are: jenkins, djb2, loselose, rotr32\n");
        free(wideUrl);
        return 1;
    }

    printf("URL: %s\n", url);
    printf("Wide URL: %ls\n", wideUrl);

    free(wideUrl);
    return 0;
}