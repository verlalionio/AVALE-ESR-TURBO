import os
import sys
import ctypes
import tkinter as tk
from tkinter import filedialog
from rich.console import Console
from rich.panel import Panel
from rich.table import Table

console = Console()
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DLL_PATH = os.path.normpath(os.path.join(BASE_DIR, "esr_turbo.dll"))
STORAGE_ROOT = os.path.normpath(os.path.join(BASE_DIR, "esr_storage"))

def get_path(mode="file"):
    root = tk.Tk(); root.withdraw(); root.attributes("-topmost", True)
    if mode == "file": res = filedialog.askopenfilename(title="Select File/Recipe")
    elif mode == "save": res = filedialog.askdirectory(title="Select Destination Folder")
    else: res = filedialog.askdirectory(title="Select Folder")
    root.destroy(); return os.path.normpath(res) if res else None

def transfer_menu(lib):
    while True:
        os.system('cls' if os.name == 'nt' else 'clear')
        table = Table(title="TRANSFER MODE (Global Sharing)", title_style="bold yellow")
        table.add_column("ID", style="cyan")
        table.add_column("OPTION", style="white")
        table.add_row("1", "Create Base-Oracle Patch (Smallest)")
        table.add_row("2", "Create Full Standalone Package (Total)")
        table.add_row("0", "Back to Main Menu")
        console.print(Panel(table, expand=False))
        
        choice = input("Select Transfer Mode: ")
        if choice == "1" or choice == "2":
            esr = get_path("file")
            if esr:
                out = get_path("save")
                if out:
                    console.print("[yellow]Packaging for transfer...[/yellow]")
                    lib.prepare_transfer_package(esr, STORAGE_ROOT, out)
                    console.print(f"[green]Package ready at: {out}[/green]")
        elif choice == "0": break
        input("\nPress Enter...")

def main():
    try:
        if not os.path.exists(DLL_PATH):
            console.print(Panel("[bold red]DLL Missing! Run smart_compile.py first.[/bold red]"))
            return

        lib = ctypes.WinDLL(DLL_PATH, winmode=0x08)
        lib.turbo_run_in_ram.argtypes = [ctypes.c_wchar_p, ctypes.c_wchar_p, ctypes.c_wchar_p]
        lib.turbo_compress_folder.argtypes = [ctypes.c_wchar_p, ctypes.c_wchar_p]
        lib.turbo_reconstruct.argtypes = [ctypes.c_wchar_p, ctypes.c_wchar_p, ctypes.c_wchar_p]
        lib.train_base_oracle.argtypes = [ctypes.c_wchar_p, ctypes.c_wchar_p]
        lib.prepare_transfer_package.argtypes = [ctypes.c_wchar_p, ctypes.c_wchar_p, ctypes.c_wchar_p]

        while True:
            os.system('cls' if os.name == 'nt' else 'clear')
            table = Table(title="AVALE ESR TURBO v89 (GLOBAL)", title_style="bold magenta")
            table.add_column("ID", style="cyan")
            table.add_column("ACTION", style="white")
            table.add_row("1", "Compress Folder or File")
            table.add_row("2", "Train Base Oracle (Deduplication)")
            table.add_row("3", "Unpack / Reconstruct")
            table.add_row("4", "Statistics (Oracle State)")
            table.add_row("5", "Launch from Recipe (MMF)")
            table.add_row("6", "TRANSFER MODE (Share Project)")
            table.add_row("0", "Exit")
            console.print(Panel(table, expand=False))
            
            choice = input("Select Action: ")
            
            if choice == "1":
                path = get_path("dir") or get_path("file")
                if path:
                    name = os.path.basename(path)
                    entry = input(f"Launch File (Enter for {name}): ") or name
                    console.print("[yellow]Processing...[/yellow]")
                    lib.turbo_compress_folder(path, STORAGE_ROOT)
                    console.print(f"[green]Done! Library updated.[/green]")

            elif choice == "2":
                sub_table = Table(title="Training Mode", title_style="bold cyan")
                sub_table.add_row("1", "Merge Local Blobs to Base")
                sub_table.add_row("2", "Ingest New Folder/File directly to Base")
                console.print(sub_table)
                t_choice = input("Select Training Mode: ")
                if t_choice == "1":
                    console.print("[yellow]Migrating...[/yellow]")
                    res = lib.train_base_oracle(None, STORAGE_ROOT)
                    console.print(f"[green]Success! {res} blobs merged.[/green]")
                elif t_choice == "2":
                    path = get_path("dir") or get_path("file")
                    if path:
                        console.print("[yellow]Ingesting data to Oracle...[/yellow]")
                        res = lib.train_base_oracle(path, STORAGE_ROOT)
                        console.print(f"[green]Success! {res} unique blobs added to Base Oracle.[/green]")

            elif choice == "3":
                esr = get_path("file")
                if esr:
                    out = get_path("save")
                    if out:
                        lib.turbo_reconstruct(esr, STORAGE_ROOT, out)
                        console.print(f"[green]Extracted to: {out}[/green]")

            elif choice == "4":
                base_p = os.path.join(STORAGE_ROOT, "blobs", "base")
                local_p = os.path.join(STORAGE_ROOT, "blobs", "local")
                b_cnt = len(os.listdir(base_p)) if os.path.exists(base_p) else 0
                l_cnt = len(os.listdir(local_p)) if os.path.exists(local_p) else 0
                console.print(Panel(f"Base Oracle: {b_cnt}\nLocal Blobs: {l_cnt}", title="Stats"))

            elif choice == "5":
                esr = get_path("file")
                if esr:
                    entry = input("Entry point (e.g. game.exe): ")
                    lib.turbo_run_in_ram(esr, STORAGE_ROOT, entry)

            elif choice == "6":
                transfer_menu(lib)

            elif choice == "0": break
            input("\nPress Enter...")

    except Exception as e:
        console.print(f"[bold red]ERROR: {e}[/bold red]")
        input("Enter...")

if __name__ == "__main__":
    main()
