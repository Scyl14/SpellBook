def local_payload_fetch():
    path = input ("""\nEnter Payload Path: 

    >> """)
    with open(path, "rb") as file:
        content = file.read()
        payload = "unsigned char Data_RawData[] = {" + ", ".join(map(lambda b: hex(b), content)) + "};"
        return payload
