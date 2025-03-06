# SpellBook

SpellBook is a versatile and modular framework for building loaders, designed to support a wide range of payload injection techniques and encryption methods. It offers a collection of spells (modules), primarily sourced from Maldev Academy ❤️, which can be utilized to create custom loaders. 
These loaders are ideal for both experimenting with various techniques and for malware reverse engineering training purposes. 
The framework is developed in C++ and Python, ensuring flexibility and ease of extensibility.

### Prerequisites

- Python 3.7+
- Compiler MinGW

    - https://github.com/msys2/msys2-installer/releases/download/2025-02-21/msys2-x86_64-20250221.exe
    - Then in MSYS:

        ```sh
        pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
        ```

- Windows OS (for Windows-specific APIs)

### Installation

1. Clone the repository:

    ```sh
    git clone https://github.com/yourusername/spellbook.git
    cd spellbooks
    ```
### Usage

```sh 
python3 main.py
```

### TODO

- New GUI Work in Progress
- Reimplement Encryption Algorythm in Python 
- Add new techniques