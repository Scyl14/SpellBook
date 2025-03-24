import requests

def local_payload_fetch():
    path = input ("""\nEnter Payload Path: 

    >> """)
    with open(path, "rb") as file:
        content = file.read()
        return content

def remote_payload_fetch(url):
    response = requests.get(url)
    content = response.content
    return content
