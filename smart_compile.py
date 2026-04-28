import os
import subprocess
import sys

def compile():
    print("[COMPILER] Поиск MSVC...")
    BASE = os.path.dirname(os.path.abspath(__file__))
    SRC = os.path.join(BASE, "esr_lib.cpp")
    OUT = os.path.join(BASE, "esr_turbo.dll")
    
    vswhere = r"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
    if not os.path.exists(vswhere):
        print("[!] ОШИБКА: Не найден vswhere.exe.")
        return

    cmd = [vswhere, "-latest", "-products", "*", "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64", "-property", "installationPath"]
    vs_path = subprocess.check_output(cmd).decode("utf-8").strip()
    vcvars = os.path.join(vs_path, "VC\\Auxiliary\\Build\\vcvarsall.bat")
    
    # Используем абсолютные пути в команде cl.exe
    cl_cmd = f'"{vcvars}" x64 && cl.exe /LD /O2 /MT "{SRC}" /Fe:"{OUT}" Advapi32.lib User32.lib Shell32.lib'
    
    print(f"[COMPILER] Запуск сборки: {OUT}")
    process = subprocess.Popen(cl_cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = process.communicate()
    
    if process.returncode == 0:
        print(f"[SUCCESS] Ядро ESR Turbo успешно скомпилировано!")
    else:
        print("[!] ОШИБКА КОМПИЛЯЦИИ:")
        print(out.decode("cp866", errors="ignore"))
        print(err.decode("cp866", errors="ignore"))

if __name__ == "__main__":
    compile()
