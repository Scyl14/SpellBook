import time

def calculate_xor_key():
    """Simulate the C macro KEY using the current time."""
    current_time = time.strftime("%H%M%S")  
    key = (
        ((int(current_time[5]) - 0) * 1) +
        ((int(current_time[4]) - 0) * 10) +
        ((int(current_time[3]) - 0) * 60) +
        ((int(current_time[2]) - 0) * 600) +
        ((int(current_time[1]) - 0) * 3600) +
        ((int(current_time[0]) - 0) * 36000)
    ) & 0xFF
    return key

def xor_encrypt(data, key):
    """Encrypt data using XOR with the given key."""
    return bytes(char ^ key for char in data) 

def url_obfuscated_stack_string(url):
    """Obfuscate a URL using XOR encryption and generate a C stack string."""
    key = calculate_xor_key()
    obfuscated_url = xor_encrypt(url.encode(), key)

    # Generate the C stack string with numeric values
    c_stack_string = ", ".join(f"0x{c:02X}" for c in obfuscated_url)
    c_code = f"unsigned char Url[] = {{ {c_stack_string} }};\n"
    return c_code


