# 026-000-Caesar

Project Caesar untuk menyimpan file Nextion HMI dan mengekstrak file `.HMI` menjadi format teks dan JSON yang bisa dibaca manusia, Git, dan AI.

## Struktur Folder

```text
026-000-Caesar/
├── nextion/
│   ├── HMI CAESAR_6.HMI          # File source Nextion HMI
│   ├── Nextion2Text/             # Auto-clone, tidak masuk Git
│   └── output/                   # Hasil extract, tidak masuk Git
│       └── HMI_CAESAR_6/
│           ├── text/             # TXT per page
│           └── json/             # JSON per page
├── src/
│   ├── nextion2text.sh           # Script utama extract HMI
│   └── run_nextion2text.py       # Wrapper codec untuk Linux
├── .venv/                        # Python virtual environment, tidak masuk Git
├── .gitignore
└── README.md
```

## Kebutuhan

- Linux shell atau Bash
- Git
- Python 3.8 atau lebih baru
- File `.HMI` di folder `nextion/`

Cek versi Python:

```bash
python3 --version
```

Project ini sudah pernah dites dengan Python 3.12.3.

## Virtual Environment

Virtual environment memakai folder `.venv`.

Buat venv jika belum ada:

```bash
python3 -m venv .venv
```

Aktifkan venv:

```bash
source .venv/bin/activate
```

Catatan: script extract memakai Python standard library dan Nextion2Text juga tidak butuh package tambahan.

## Cara Extract File HMI

Jalankan dari root project:

```bash
./src/nextion2text.sh
```

Script akan:

1. Mencari semua file `.HMI` dan `.hmi` di folder `nextion/`.
2. Clone `https://github.com/MMMZZZZ/Nextion2Text.git` ke `nextion/Nextion2Text/` jika folder itu belum ada.
3. Menghapus output lama untuk file HMI yang sedang diproses.
4. Menjalankan Nextion2Text dengan output TXT dan JSON.
5. Menaruh hasil extract di `nextion/output/<nama_file_hmi>/`.

Contoh file input:

```text
nextion/HMI CAESAR_6.HMI
```

Output yang dibuat:

```text
nextion/output/HMI_CAESAR_6/
├── text/
│   ├── HMI CAESAR_6_Stats.txt
│   ├── Program.s.txt
│   ├── keybdA.txt
│   ├── keybdB.txt
│   ├── pageDashboard.txt
│   ├── pageLogin.txt
│   ├── pageLotB.txt
│   ├── pageLotF.txt
│   ├── pageMldB.txt
│   ├── pageMldF.txt
│   └── pageSys.txt
└── json/
    ├── Program.s.json
    ├── keybdA.json
    ├── keybdB.json
    ├── pageDashboard.json
    ├── pageLogin.json
    ├── pageLotB.json
    ├── pageLotF.json
    ├── pageMldB.json
    ├── pageMldF.json
    └── pageSys.json
```

## Multi File HMI

Script memproses semua file HMI di folder `nextion/`.

Contoh:

```text
nextion/HMI CAESAR_6.HMI
nextion/HMI CAESAR_5.HMI
```

Output:

```text
nextion/output/HMI_CAESAR_6/
nextion/output/HMI_CAESAR_5/
```

Spasi pada nama file diganti menjadi underscore untuk nama folder output.

## Flag Nextion2Text Yang Dipakai

Script menjalankan Nextion2Text dengan flag berikut:

```bash
-e -s -j -p visual
```

Artinya:

| Flag | Fungsi |
|------|--------|
| `-e` | Sertakan event kosong. |
| `-s` | Buat stats file berisi jumlah page, component, dan event code. |
| `-j` | Buat output JSON. |
| `-p visual` | Sertakan properti visual seperti posisi, ukuran, warna, gambar, dan font. |

## File Yang Masuk Git

Masuk Git:

```text
README.md
.gitignore
src/nextion2text.sh
src/run_nextion2text.py
nextion/*.HMI
```

Tidak masuk Git:

```text
.venv/
nextion/Nextion2Text/
nextion/output/
opencode.json
```

## Troubleshooting

### Permission denied saat menjalankan script

Jalankan:

```bash
chmod +x src/nextion2text.sh
./src/nextion2text.sh
```

### `ERROR: no .HMI files found`

Pastikan file `.HMI` berada langsung di folder `nextion/`.

Contoh benar:

```text
nextion/HMI CAESAR_6.HMI
```

### Nextion2Text gagal karena codec `ansi`

Nextion2Text memakai codec `ansi`, tetapi Linux Python tidak mengenal codec itu. Project ini memakai `src/run_nextion2text.py` untuk mendaftarkan alias `ansi -> latin-1` sebelum menjalankan Nextion2Text.

Jalankan lewat script ini:

```bash
./src/nextion2text.sh
```

Jangan jalankan `Nextion2Text.py` langsung jika memakai Linux.

### Ingin extract ulang dari awal

Jalankan command yang sama:

```bash
./src/nextion2text.sh
```

Script akan menghapus folder output lama untuk setiap file HMI yang diproses, lalu membuat output baru.

## Batasan

Nextion2Text bersifat satu arah:

```text
.HMI -> TXT/JSON
```

Tool ini tidak bisa membuat ulang file `.HMI` dari TXT atau JSON. Resource gambar juga tidak otomatis diekspor sebagai file gambar terpisah.
