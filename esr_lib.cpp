#include <windows.h>
#include <wincrypt.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <algorithm>

#define ESR_API extern "C" __declspec(dllexport)

static const uint32_t GEAR_TABLE[256] = {
    0x23ed1095, 0x12d5b6e4, 0x30182479, 0x4f26b5c3, 0x5a19c8d2, 0x6e27b4f1, 0x7b38a9d0, 0x8c49d2b1,
    0x9d5ae3c2, 0xae6bf4d3, 0xbf7c05e4, 0xc08d16f5, 0xd19e27a6, 0xe2af38b7, 0xf3b049c8, 0x04c15ad9,
    0x15d26be0, 0x26e37cf1, 0x37f48de2, 0x48059ef3, 0x5916af04, 0x6a27bf15, 0x7b38a9d0, 0x8c49df37,
    0x9d5ae4e8, 0xae6bf5f9, 0xbf7c060a, 0xc08d171b, 0xd19e282c, 0xe2af393d, 0xf3b04a4e, 0x04c15b5f,
    0x23ed1095, 0x12d5b6e4, 0x30182479, 0x4f26b5c3, 0x5a19c8d2, 0x6e27b4f1, 0x7b38a9d0, 0x8c49d2b1,
    0x9d5ae3c2, 0xae6bf4d3, 0xbf7c05e4, 0xc08d16f5, 0xd19e27a6, 0xe2af38b7, 0xf3b04a4e, 0x04c15ad9,
    0x15d26be0, 0x26e37cf1, 0x37f48de2, 0x48059ef3, 0x5916af04, 0x6a27bf15, 0x7b38a9d0, 0x8c49df37,
    0x9d5ae4e8, 0xae6bf5f9, 0xbf7c060a, 0xc08d171b, 0xd19e282c, 0xe2af393d, 0xf3b04a4e, 0x04c15b5f,
    0x23ed1095, 0x12d5b6e4, 0x30182479, 0x4f26b5c3, 0x5a19c8d2, 0x6e27b4f1, 0x7b38a9d0, 0x8c49d2b1,
    0x9d5ae3c2, 0xae6bf4d3, 0xbf7c05e4, 0xc08d16f5, 0xd19e27a6, 0xe2af38b7, 0xf3b049c8, 0x04c15ad9,
    0x15d26be0, 0x26e37cf1, 0x37f48de2, 0x48059ef3, 0x5916af04, 0x6a27bf15, 0x7b38cf26, 0x8c49df37,
    0x9d5ae4e8, 0xae6bf5f9, 0xbf7c060a, 0xc08d171b, 0xd19e282c, 0xe2af393d, 0xf3b04a4e, 0x04c15b5f,
    0x23ed1095, 0x12d5b6e4, 0x30182479, 0x4f26b5c3, 0x5a19c8d2, 0x6e27b4f1, 0x7b38a9d0, 0x8c49d2b1,
    0x9d5ae3c2, 0xae6bf4d3, 0xbf7c05e4, 0xc08d16f5, 0xd19e27a6, 0xe2af38b7, 0xf3b049c8, 0x04c15ad9,
    0x15d26be0, 0x26e37cf1, 0x37f48de2, 0x48059ef3, 0x5916af04, 0x6a27bf15, 0x7b38cf26, 0x8c49df37,
    0x9d5ae4e8, 0xae6bf5f9, 0xbf7c060a, 0xc08d171b, 0xd19e282c, 0xe2af393d, 0xf3b04a4e, 0x04c15b5f,
    0x23ed1095, 0x12d5b6e4, 0x30182479, 0x4f26b5c3, 0x5a19c8d2, 0x6e27b4f1, 0x7b38a9d0, 0x8c49d2b1,
    0x9d5ae3c2, 0xae6bf4d3, 0xbf7c05e4, 0xc08d16f5, 0xd19e27a6, 0xe2af38b7, 0xf3b049c8, 0x04c15ad9,
    0x15d26be0, 0x26e37cf1, 0x37f48de2, 0x48059ef3, 0x5916af04, 0x6a27bf15, 0x7b38cf26, 0x8c49df37,
    0x9d5ae4e8, 0xae6bf5f9, 0xbf7c060a, 0xc08d171b, 0xd19e282c, 0xe2af393d, 0xf3b04a4e, 0x04c15b5f,
    0x23ed1095, 0x12d5b6e4, 0x30182479, 0x4f26b5c3, 0x5a19c8d2, 0x6e27b4f1, 0x7b38a9d0, 0x8c49d2b1,
    0x9d5ae3c2, 0xae6bf4d3, 0xbf7c05e4, 0xc08d16f5, 0xd19e27a6, 0xe2af38b7, 0xf3b049c8, 0x04c15ad9,
    0x15d26be0, 0x26e37cf1, 0x37f48de2, 0x48059ef3, 0x5916af04, 0x6a27bf15, 0x7b38cf26, 0x8c49df37,
    0x9d5ae4e8, 0xae6bf5f9, 0xbf7c060a, 0xc08d171b, 0xd19e282c, 0xe2af393d, 0xf3b04a4e, 0x04c15b5f,
    0x23ed1095, 0x12d5b6e4, 0x30182479, 0x4f26b5c3, 0x5a19c8d2, 0x6e27b4f1, 0x7b38a9d0, 0x8c49d2b1,
    0x9d5ae3c2, 0xae6bf4d3, 0xbf7c05e4, 0xc08d16f5, 0xd19e27a6, 0xe2af38b7, 0xf3b049c8, 0x04c15ad9,
    0x15d26be0, 0x26e37cf1, 0x37f48de2, 0x48059ef3, 0x5916af04, 0x6a27bf15, 0x7b38cf26, 0x8c49df37,
    0x9d5ae4e8, 0xae6bf5f9, 0xbf7c060a, 0xc08d171b, 0xd19e282c, 0xe2af393d, 0xf3b04a4e, 0x04c15b5f,
    0x23ed1095, 0x12d5b6e4, 0x30182479, 0x4f26b5c3, 0x5a19c8d2, 0x6e27b4f1, 0x7b38a9d0, 0x8c49d2b1,
    0x9d5ae3c2, 0xae6bf4d3, 0xbf7c05e4, 0xc08d16f5, 0xd19e27a6, 0xe2af38b7, 0xf3b049c8, 0x04c15ad9,
    0x15d26be0, 0x26e37cf1, 0x37f48de2, 0x48059ef3, 0x5916af04, 0x6a27bf15, 0x7b38cf26, 0x8c49df37,
    0x9d5ae4e8, 0xae6bf5f9, 0xbf7c060a, 0xc08d171b, 0xd19e282c, 0xe2af393d, 0xf3b04a4e, 0x04c15b5f
};

std::string sha256_hex(const uint8_t* d, size_t l) {
    HCRYPTPROV hP=0; HCRYPTHASH hH=0; CryptAcquireContext(&hP,0,0,PROV_RSA_AES,CRYPT_VERIFYCONTEXT);
    CryptCreateHash(hP,CALG_SHA_256,0,0,&hH); CryptHashData(hH,d,(DWORD)l,0);
    uint8_t hash[32]; DWORD hl=32; CryptGetHashParam(hH,HP_HASHVAL,hash,&hl,0);
    CryptDestroyHash(hH); CryptReleaseContext(hP,0);
    std::stringstream ss; for(int i=0;i<32;i++) ss<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)hash[i];
    return ss.str();
}

ESR_API int train_base_oracle(const wchar_t* folder_path, const wchar_t* storage_dir) {
    std::wstring base_dir = std::wstring(storage_dir) + L"\\blobs\\base";
    std::wstring local_dir = std::wstring(storage_dir) + L"\\blobs\\local";
    CreateDirectoryW(base_dir.c_str(), NULL);
    
    // Если путь не указан - просто мерджим Local в Base
    if (!folder_path) {
        WIN32_FIND_DATAW fd; HANDLE hF = FindFirstFileW((local_dir + L"\\*.bin").c_str(), &fd);
        int count = 0;
        if(hF != INVALID_HANDLE_VALUE){
            do {
                std::wstring old_p = local_dir + L"\\" + fd.cFileName;
                std::wstring new_p = base_dir + L"\\" + fd.cFileName;
                MoveFileW(old_p.c_str(), new_p.c_str()); count++;
            } while(FindNextFileW(hF, &fd));
            FindClose(hF);
        }
        return count;
    }
    
    // Если путь указан - сканируем и добавляем блоки в Base напрямую
    std::wstring ws(folder_path); 
    bool is_file = !(GetFileAttributesW(folder_path) & FILE_ATTRIBUTE_DIRECTORY);
    std::vector<std::wstring> files;
    if (is_file) files.push_back(ws);
    else {
        std::vector<std::wstring> q = {ws};
        while(!q.empty()){
            std::wstring cur = q.back(); q.pop_back();
            WIN32_FIND_DATAW fd; HANDLE hF = FindFirstFileW((cur+L"\\*").c_str(), &fd);
            if(hF!=INVALID_HANDLE_VALUE){
                do {
                    std::wstring fn = fd.cFileName; if(fn==L"."||fn==L"..") continue;
                    std::wstring full = cur+L"\\"+fn;
                    if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) q.push_back(full);
                    else files.push_back(full);
                } while(FindNextFileW(hF, &fd)); FindClose(hF);
            }
        }
    }
    
    int total_blobs = 0;
    for(auto& p : files){
        HANDLE hFile = CreateFileW(p.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if(hFile == INVALID_HANDLE_VALUE) continue;
        DWORD fs = GetFileSize(hFile, NULL); if(fs == 0){ CloseHandle(hFile); continue; }
        HANDLE hm = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
        const uint8_t* d = (const uint8_t*)MapViewOfFile(hm, FILE_MAP_READ, 0, 0, 0);
        uint32_t hv = 0, pos = 0, m = 0x0003FFFF;
        while(pos < fs){
            uint32_t i = pos + 65536; if(i >= fs) i = fs;
            while(i < fs){ hv = ((hv << 1) & 0xFFFFFFFF) ^ GEAR_TABLE[d[i]%256]; if((hv&m)==0||(i-pos)>1048576){i++;break;} i++; }
            size_t cl = i - pos; std::string hex = sha256_hex(d + pos, cl);
            std::wstring bp_base = base_dir + L"\\" + std::wstring(hex.begin(), hex.end()) + L".bin";
            if(GetFileAttributesW(bp_base.c_str())==INVALID_FILE_ATTRIBUTES){
                std::ofstream bf(bp_base, std::ios::binary); bf.write((const char*)(d+pos), cl); bf.close();
                total_blobs++;
            }
            pos = i; hv = 0;
        }
        UnmapViewOfFile(d); CloseHandle(hm); CloseHandle(hFile);
    }
    return total_blobs;
}

// ... Остальные функции (turbo_compress_folder, turbo_reconstruct, turbo_run_in_ram, prepare_transfer_package) ...
// Я оставлю их без изменений, но добавлю в финальный файл.
ESR_API int turbo_compress_folder(const wchar_t* folder_path, const wchar_t* storage_dir) {
    std::wstring b_dir = std::wstring(storage_dir) + L"\\blobs\\base";
    std::wstring l_dir = std::wstring(storage_dir) + L"\\blobs\\local";
    CreateDirectoryW((std::wstring(storage_dir) + L"\\blobs").c_str(), NULL);
    CreateDirectoryW(b_dir.c_str(), NULL); CreateDirectoryW(l_dir.c_str(), NULL);
    std::wstring ws(folder_path); 
    bool is_file = !(GetFileAttributesW(folder_path) & FILE_ATTRIBUTE_DIRECTORY);
    std::wstring name = L"Archive";
    if (!is_file) {
        size_t last = ws.find_last_of(L"\\/");
        name = (last == std::wstring::npos) ? L"Game" : ws.substr(last + 1);
    } else {
        size_t last = ws.find_last_of(L"\\/");
        name = (last == std::wstring::npos) ? L"File" : ws.substr(last + 1);
    }
    std::wstring g_dir = std::wstring(storage_dir) + L"\\" + name; CreateDirectoryW(g_dir.c_str(), NULL);
    std::wstring r_p = g_dir + L"\\" + name + L".esr";
    std::ofstream f(r_p, std::ios::binary); f.write("V36L", 4);
    std::vector<std::wstring> files;
    if (is_file) files.push_back(ws);
    else {
        std::vector<std::wstring> q = {ws};
        while(!q.empty()){
            std::wstring cur = q.back(); q.pop_back();
            WIN32_FIND_DATAW fd; HANDLE hF = FindFirstFileW((cur+L"\\*").c_str(), &fd);
            if(hF!=INVALID_HANDLE_VALUE){
                do {
                    std::wstring fn = fd.cFileName; if(fn==L"."||fn==L"..") continue;
                    std::wstring full = cur+L"\\"+fn;
                    if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) q.push_back(full);
                    else files.push_back(full);
                } while(FindNextFileW(hF, &fd)); FindClose(hF);
            }
        }
    }
    uint32_t n_f = (uint32_t)files.size(); f.write((char*)&n_f, 4);
    for(auto& p : files){
        std::wstring rel = is_file ? name : p.substr(ws.length() + 1); 
        uint32_t r_l = (uint32_t)rel.length();
        f.write((char*)&r_l, 4); f.write((char*)rel.c_str(), r_l * 2);
        HANDLE hFile = CreateFileW(p.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if(hFile == INVALID_HANDLE_VALUE) continue;
        DWORD fs = GetFileSize(hFile, NULL); HANDLE hm = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
        const uint8_t* d = (const uint8_t*)MapViewOfFile(hm, FILE_MAP_READ, 0, 0, 0);
        std::vector<std::string> ch_h; std::vector<uint32_t> ch_l; uint32_t hv = 0, pos = 0, m = 0x0003FFFF;
        while(pos < fs){
            uint32_t i = pos + 65536; if(i >= fs) i = fs;
            while(i < fs){ hv = ((hv << 1) & 0xFFFFFFFF) ^ GEAR_TABLE[d[i]%256]; if((hv&m)==0||(i-pos)>1048576){i++;break;} i++; }
            size_t cl = i - pos; std::string hex = sha256_hex(d + pos, cl);
            ch_h.push_back(hex); ch_l.push_back((uint32_t)cl);
            std::wstring bp_base = b_dir + L"\\" + std::wstring(hex.begin(), hex.end()) + L".bin";
            std::wstring bp_local = l_dir + L"\\" + std::wstring(hex.begin(), hex.end()) + L".bin";
            if(GetFileAttributesW(bp_base.c_str())==INVALID_FILE_ATTRIBUTES && GetFileAttributesW(bp_local.c_str())==INVALID_FILE_ATTRIBUTES){
                std::ofstream bf(bp_local, std::ios::binary); bf.write((const char*)(d+pos), cl); bf.close();
            }
            pos = i; hv = 0;
        }
        UnmapViewOfFile(d); CloseHandle(hm); CloseHandle(hFile);
        uint32_t n_ch = (uint32_t)ch_h.size(); f.write((char*)&n_ch, 4);
        for(size_t i=0; i<n_ch; i++){
            for(int j=0; j<32; j++) { uint8_t b = (uint8_t)std::stoi(ch_h[i].substr(j*2, 2), nullptr, 16); f.write((char*)&b, 1); }
            f.write((char*)&ch_l[i], 4);
        }
    }
    f.close(); return n_f;
}

void create_dirs_v89(std::wstring p) {
    size_t pos = 0;
    while ((pos = p.find_last_of(L"\\/", p.length())) != std::wstring::npos) {
        std::wstring s = p.substr(0, pos); if(CreateDirectoryW(s.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) break;
        create_dirs_v89(s); CreateDirectoryW(s.c_str(), NULL); break;
    }
}

ESR_API int turbo_reconstruct(const wchar_t* esr_path, const wchar_t* storage_dir, const wchar_t* out_dir) {
    std::ifstream f(esr_path, std::ios::binary); if(!f.is_open()) return -404;
    char head[5]={0}; f.read(head,4); if(strcmp(head,"V36L")!=0) return -2;
    uint32_t n_f; f.read((char*)&n_f, 4);
    std::wstring bb = std::wstring(storage_dir)+L"\\blobs\\base";
    std::wstring lb = std::wstring(storage_dir)+L"\\blobs\\local";
    for(uint32_t i=0; i<n_f; i++){
        uint32_t r_l; f.read((char*)&r_l, 4); std::wstring rel(r_l, L' '); f.read((char*)rel.data(), r_l * 2);
        std::wstring fo = std::wstring(out_dir) + L"\\" + rel; create_dirs_v89(fo);
        uint32_t n_ch; f.read((char*)&n_ch, 4);
        std::vector<std::string> hashes; std::vector<uint32_t> lens; uint64_t total_size = 0;
        for(uint32_t j=0; j<n_ch; j++){
            uint8_t hb[32]; f.read((char*)hb, 32); uint32_t cl; f.read((char*)&cl, 4);
            std::stringstream ss; for(int k=0; k<32; k++) ss << std::hex << std::setw(2) << std::setfill('0') << (int)hb[k];
            hashes.push_back(ss.str()); lens.push_back(cl); total_size += cl;
        }
        HANDLE hF = CreateFileW(fo.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if(hF != INVALID_HANDLE_VALUE){
            HANDLE hM = CreateFileMapping(hF, NULL, PAGE_READWRITE, (DWORD)(total_size >> 32), (DWORD)(total_size & 0xFFFFFFFF), NULL);
            if(hM){
                uint8_t* view = (uint8_t*)MapViewOfFile(hM, FILE_MAP_ALL_ACCESS, 0, 0, 0);
                if(view){
                    uint64_t offset = 0;
                    for(uint32_t j=0; j<n_ch; j++){
                        std::wstring bp = lb+L"\\"+std::wstring(hashes[j].begin(), hashes[j].end())+L".bin";
                        HANDLE hB = CreateFileW(bp.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                        if(hB == INVALID_HANDLE_VALUE){ bp = bb+L"\\"+std::wstring(hashes[j].begin(), hashes[j].end())+L".bin"; hB = CreateFileW(bp.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); }
                        if(hB != INVALID_HANDLE_VALUE){
                            HANDLE hBM = CreateFileMapping(hB, NULL, PAGE_READONLY, 0, 0, NULL);
                            uint8_t* b_v = (uint8_t*)MapViewOfFile(hBM, FILE_MAP_READ, 0, 0, 0);
                            if(b_v){ memcpy(view + offset, b_v, lens[j]); UnmapViewOfFile(b_v); }
                            CloseHandle(hBM); CloseHandle(hB);
                        }
                        offset += lens[j];
                    }
                    UnmapViewOfFile(view);
                }
                CloseHandle(hM);
            }
            CloseHandle(hF);
        }
    }
    f.close(); return 777;
}

ESR_API int turbo_run_in_ram(const wchar_t* esr_path, const wchar_t* storage_dir, const wchar_t* entry_point) {
    wchar_t dp[MAX_PATH]; GetModuleFileNameW(GetModuleHandleW(L"esr_turbo.dll"), dp, MAX_PATH);
    std::wstring mp = std::wstring(dp).substr(0, std::wstring(dp).find_last_of(L"\\/")) + L"\\_RAM_";
    int res = turbo_reconstruct(esr_path, storage_dir, mp.c_str());
    if(res == 777){
        std::wstring ex = mp + L"\\" + entry_point;
        ShellExecuteW(NULL, L"open", ex.c_str(), NULL, mp.c_str(), SW_SHOW);
    }
    return res;
}

ESR_API int prepare_transfer_package(const wchar_t* esr_path, const wchar_t* storage_dir, const wchar_t* target_dir) {
    std::ifstream f(esr_path, std::ios::binary); if(!f.is_open()) return -404;
    f.seekg(4); uint32_t n_f; f.read((char*)&n_f, 4);
    std::wstring bb = std::wstring(storage_dir)+L"\\blobs\\base";
    std::wstring lb = std::wstring(storage_dir)+L"\\blobs\\local";
    CreateDirectoryW(target_dir, NULL);
    std::wstring b_out = std::wstring(target_dir) + L"\\blobs"; CreateDirectoryW(b_out.c_str(), NULL);
    for(uint32_t i=0; i<n_f; i++){
        uint32_t r_l; f.read((char*)&r_l, 4); f.seekg(r_l*2, std::ios::cur);
        uint32_t n_ch; f.read((char*)&n_ch, 4);
        for(uint32_t j=0; j<n_ch; j++){
            uint8_t hb[32]; f.read((char*)hb, 32); uint32_t cl; f.read((char*)&cl, 4);
            std::stringstream ss; for(int k=0; k<32; k++) ss << std::hex << std::setw(2) << std::setfill('0') << (int)hb[k];
            std::string hs = ss.str();
            std::wstring bp_base = bb + L"\\" + std::wstring(hs.begin(), hs.end()) + L".bin";
            std::wstring bp_local = lb + L"\\" + std::wstring(hs.begin(), hs.end()) + L".bin";
            std::wstring bp_out = b_out + L"\\" + std::wstring(hs.begin(), hs.end()) + L".bin";
            if(GetFileAttributesW(bp_out.c_str()) == INVALID_FILE_ATTRIBUTES) {
                if(GetFileAttributesW(bp_local.c_str()) != INVALID_FILE_ATTRIBUTES) CopyFileW(bp_local.c_str(), bp_out.c_str(), FALSE);
                else if(GetFileAttributesW(bp_base.c_str()) != INVALID_FILE_ATTRIBUTES) CopyFileW(bp_base.c_str(), bp_out.c_str(), FALSE);
            }
        }
    }
    f.close(); 
    std::wstring esr_name = std::wstring(esr_path).substr(std::wstring(esr_path).find_last_of(L"\\/")+1);
    CopyFileW(esr_path, (std::wstring(target_dir) + L"\\" + esr_name).c_str(), FALSE);
    return 777;
}

ESR_API int turbo_compress_file(const wchar_t* p, const wchar_t* s) { return turbo_compress_folder(p, s); }
