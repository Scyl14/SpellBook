# SpellBook

SpellBook is a versatile and modular framework for building loaders, designed to support a wide range of payload injection techniques and encryption methods. It offers a collection of spells (modules), primarily sourced from Maldev Academy ❤️, which can be utilized to create custom loaders.
These loaders are ideal for both experimenting with various techniques and for malware reverse engineering training purposes. 
The framework is developed in C++ and Python, ensuring flexibility and ease of extensibility.

### Prerequisites

- Python 3.7+
- Compiler MinGW

    Release:

    https://github.com/skeeto/w64devkit/releases
    
    OR

    Chocolatey:

    Poweshell (Admin):

    ```powershell
    Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-ObjectSystem.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
    ```

    Then:

    ```sh
    choco install mingw
    ```
    
    OR

    Install MSYS2:

    https://github.com/msys2/msys2-installer/releases/download/2025-02-21/msys2-x86_64-20250221.exe
    
    Then in MSYS2:

    ```sh
    pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
    ```

- Windows OS (for Windows-specific APIs)

### Installation

1. Clone the repository:

    ```sh
    git clone https://github.com/Scyl14/SpellBook.git
    cd SpellBook
    ```
### Usage

```sh 
python3 main.py
```
### NOTE

Defender real time protecion should be turned off during the building process.
Or the project folder should be whitelisted

### TODO

- Do some string/url ofuscation
- New GUI work in progress
- Reimplement encryption algorythm in python 
- Add new techniques