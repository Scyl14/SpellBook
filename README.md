# SpellBook  

SpellBook is a modular and versatile framework for building loaders, designed to support various payload injection techniques and encryption methods. It features a collection of **spells** (modules), primarily sourced from [Maldev Academy](https://maldevacademy.com) ❤️, which can be leveraged to craft custom loaders.  

This framework is ideal for experimenting with different techniques and for **malware reverse engineering training**. Developed in **C++ and Python**, SpellBook offers both flexibility and ease of extensibility.  

## Prerequisites  

- **Python 3.7+**  
- **MinGW Compiler**  

### Installation Options  

#### 🔹 Option 1: w64devkit  
Download and install from:  
[GitHub Releases](https://github.com/skeeto/w64devkit/releases)  

#### 🔹 Option 2: Chocolatey  
1. Open **PowerShell as Administrator** and run:  
    ```powershell
    Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
    ```
2. Then install MinGW with:  
    ```sh
    choco install mingw
    ```

#### 🔹 Option 3: MSYS2  
1. Download and install **MSYS2** from:  
   [MSYS2 Releases](https://github.com/msys2/msys2-installer/releases/download/2025-02-21/msys2-x86_64-20250221.exe)  
2. Open **MSYS2** and run:  
    ```sh
    pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
    ```

- **Windows OS** (required for Windows-specific APIs)  

## Installation  

1. Clone the repository:  
    ```sh
    git clone https://github.com/Scyl14/SpellBook.git
    cd SpellBook
    ```

## Usage  

Run SpellBook with:  
```sh
python3 main.py
```

## ⚠️ Important Note  

To avoid interference from Windows Defender during the build process, you should either:  
- Disable **Real-Time Protection**  
- **Whitelist the project folder** in Windows Defender  

## 🚀 TODO  

- Implement **string/URL obfuscation**  
- Develop a **new GUI** (work in progress)  
- Reimplement the **encryption algorithm in Python**  
- Add **new techniques**  