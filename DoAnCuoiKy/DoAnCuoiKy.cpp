#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

class DiemDanh {
private:
    string ngayLamViec;
    string gioVao;
    string gioRa;
    string caLam;
    string trangThai;

public:
    // Constructor có thể có các tham số cần thiết
    DiemDanh(const string& ngay, const string& vao,
        const string& ra, const string& ca,
        const string& trangThai)
        : ngayLamViec(ngay), gioVao(vao), gioRa(ra), caLam(ca),
        trangThai(trangThai) {}

    // Các phương thức getters/setters nếu cần
    string getNgayLamViec() const { return ngayLamViec; }
    string getGioVao() const { return gioVao; }
    string getGioRa() const { return gioRa; }
    string getCaLam() const { return caLam; }
    string getTrangThai() const { return trangThai; }


};


class NhanVien {
private:
    string MaNV;
    string HoTen;
    string PhongBan;
    string ChucVu;
    string HopDong;
    int soNgayPhepConLai;
    vector <DiemDanh> ds;

public:
    NhanVien(const string& ma, const string& ten,
        const string& phong, const string& chuc,
        const string& hopDong, int phep)
        : MaNV(ma), HoTen(ten), PhongBan(phong),
        ChucVu(chuc), HopDong(hopDong), soNgayPhepConLai(phep) {}

    // Các phương thức thao tác với điểm danh
    void themDiemDanh(const DiemDanh& dd) {
        ds.push_back(dd);
    }

    // Getters/Setters cho các thuộc tính
    string getMaNV() const { return MaNV; }
    string getHoTen() const { return HoTen; }
    int getSoNgayPhepConLai() const { return soNgayPhepConLai; }

    void hienThiThongTin() const {
        cout << "MaNV: " << MaNV << "\n";
        cout << "HoTen: " << HoTen << "\n";
        cout << "PhongBan: " << PhongBan << "\n";
        cout << "ChucVu: " << ChucVu << "\n";
        cout << "HopDong: " << HopDong << "\n";
        cout << "So ngay phep con lai: " << soNgayPhepConLai << "\n";
        cout << "Danh sach diem danh: " << ds.size() << " buoi\n";
    }
};

int main()
{

}