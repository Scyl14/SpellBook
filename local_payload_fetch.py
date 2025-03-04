def local_payload_fetch():
    path = input ("Enter Payload Path: ")
    with open(path, "rb") as file:
        content = file.read()
        print ("unsigned char Data_RawData[] = {" + ", ".join(map(lambda b: hex(b), content)) + "};")

