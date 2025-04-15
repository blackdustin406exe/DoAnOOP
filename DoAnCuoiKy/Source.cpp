#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

class NHANVIEN
{
protected:
    string name;
    string id;
    string gr;
    string pass;
public:

    virtual void out() = 0;
    string getId() const { return id; }
    string getPass() const { return pass; }
    string getName() const { return name; }
    string getGr() const { return gr; }

};
class FULLTIME : public NHANVIEN
{
private:
    int day;
    int lam;
    int nghi;
public:
    void out()
    {
        cout << left << setw(30) << name << setw(10) << id << setw(10) << pass << setw(10) << gr << setw(10) << "FT" << setw(10) << lam << setw(10) << nghi << setw(10) << " " << "\n";
    }
    void getday()
    {
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now);
        day = ltm.tm_mday;
    }
    void Docfile_csv(string hoten, string mnv, string mk, string bp, int ngay_lam, int ngay_nghi)
    {
        name = hoten;
        id = mnv;
        pass = mk;
        gr = bp;
        lam = ngay_lam;
        nghi = ngay_nghi;
    }
};
class PARTTIME : public NHANVIEN
{
private:
    int hour;
    time_t checkInTime;
    time_t checkOutTime;
    string timeToday;

public:
    void out()
    {
        cout << left << setw(30) << name << setw(10) << id << setw(10) << pass << setw(10) << gr << setw(10) << "PT" << setw(10) << " " << setw(10) << " " << setw(10) << hour << "\n";
    }
    void Docfile_csv(string hoten, string mnv, string mk, string bp, string gio_lam)
    {
        name = hoten;
        id = mnv;
        pass = mk;
        gr = bp;
        hour = stoi(gio_lam);
    }
    void setCheckInTime()
    {
        checkInTime = time(0);
    }
    void setCheckOutTime()
    {
        checkOutTime = time(0);
        int duration = static_cast<int>(difftime(checkOutTime, checkInTime) / 60);
        hour += duration / 60;

        // Ghi chuỗi thời gian
        tm in, out;
        localtime_s(&in, &checkInTime);
        localtime_s(&out, &checkOutTime);

        char bufIn[6], bufOut[6];
        strftime(bufIn, sizeof(bufIn), "%H:%M", &in);
        strftime(bufOut, sizeof(bufOut), "%H:%M", &out);

        timeToday = string(bufIn) + "-" + string(bufOut);
    }
    time_t getCheckInTime() const { return checkInTime; }
    int getHour() const { return hour; }
    string getTimeToday() const { return timeToday; }
};
class DANHSACH
{
private:
    vector <NHANVIEN*> ds;
    vector <FULLTIME> ft;
    vector <PARTTIME> pt;

public:
    void Add()
    {
        for (auto& nv : ft)
            ds.push_back(&nv);
        for (auto& nv : pt)
            ds.push_back(&nv);
    }
    void Inds()
    {
        cout << left << setw(30) << "Ho ten " << setw(10) << "MNV" << setw(10) << "Mat khau" << setw(10) << "Bo phan" << setw(10) << "Loai" << setw(10) << "Ngay lam" << setw(10) << "Ngay nghi" << setw(10) << "So gio lam" << "\n";
        for (auto nv : ds)
        {
            if (FULLTIME* fulltime = dynamic_cast<FULLTIME*>(nv))
            {
                fulltime->out();
            }
            if (PARTTIME* parttime = dynamic_cast<PARTTIME*>(nv))
            {
                parttime->out();
            }
        }
    }
    void DocDS()
    {
        ft.clear();
        pt.clear();
        ifstream file("ds.csv");
        if (!file.is_open())
        {
            cout << "Khong the mo file !\n";
            return;
        }
        string line;
        getline(file, line);

        int d;
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now);
        d = ltm.tm_mday;

        while (getline(file, line))
        {
            stringstream ss(line);
            string temp;
            vector<string> data;
            while (getline(ss, temp, ','))
            {
                data.push_back(temp);
            }
            int lam = 0, nghi = 0;
            if (data[4] == "FT")
            {
                FULLTIME nv1;
                for (int i = 8; i < d + 8; i++)
                {
                    if (data[i] == "o")
                        lam++;
                    if (data[i] == "x")
                        nghi++;
                }
                nv1.Docfile_csv(data[0], data[1], data[2], data[3], lam, nghi);
                ft.push_back(nv1);
            }
            else if (data[4] == "PT")
            {
                PARTTIME nv2;
                nv2.Docfile_csv(data[0], data[1], data[2], data[3], data[7]);
                pt.push_back(nv2);
            }
        }
        file.close();
        ds.clear();
        Add();
    }
    void UpdateCSV_FT(const string& id, const string& pass)
    {
        // Đọc toàn bộ file vào bộ nhớ
        ifstream inFile("ds.csv");
        vector<string> lines;
        string line;

        // Lấy ngày hiện tại
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now);
        int currentDay = ltm.tm_mday;
        int columnToUpdate = 7 + currentDay; // Cột cần cập nhật (0-based index)

        // Đọc header
        getline(inFile, line);
        lines.push_back(line);

        // Tìm nhân viên và cập nhật
        bool found = false;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string temp;
            vector<string> data;
            while (getline(ss, temp, ','))
            {
                data.push_back(temp);
            }

            if (data[1] == id && data[2] == pass)
            {
                found = true;
                if (data.size() > columnToUpdate)
                {
                    data[columnToUpdate] = "o";
                }

                // Tạo lại dòng từ data
                string updatedLine;
                for (size_t i = 0; i < data.size(); ++i)
                {
                    if (i != 0) updatedLine += ",";
                    updatedLine += data[i];
                }
                lines.push_back(updatedLine);
            }
            else
            {
                lines.push_back(line);
            }
        }
        inFile.close();

        if (!found)
        {
            cout << "Khong tim thay nhan vien hoac mat khau khong dung!\n";
            return;
        }

        // Ghi lại toàn bộ file
        ofstream outFile("ds.csv");
        for (const auto& l : lines)
        {
            outFile << l << "\n";
        }
        outFile.close();

        cout << "Check-in thanh cong!\n";
    }
    void CheckinFT()
    {
        string id;
        string pass;
        cout << "Nhap ma nhan vien: ";
        cin.ignore();
        getline(cin, id);
        cout << "Nhap mat khau: ";
        getline(cin, pass);
        // Kiểm tra mật khẩu
        bool valid = false;
        for (auto& nv : ds)
        {
            if (nv->getId() == id && nv->getPass() == pass)
            {
                valid = true;
                break;
            }
        }

        if (!valid)
        {
            cout << "Ma nhan vien hoac mat khau khong dung!\n";
            return;
        }

        // Cập nhật file CSV
        UpdateCSV_FT(id, pass);
    }
    void UpdateCSV_PT(const string& id, const string& pass, int new_hour, const string& timeStr)
    {
        ifstream inFile("ds.csv");
        vector<string> lines;
        string line;

        // Lấy ngày hiện tại
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now);
        int currentDay = ltm.tm_mday;
        int colDay = 7 + currentDay; // Cột ngày hiện tại

        // Header
        getline(inFile, line);
        lines.push_back(line);

        while (getline(inFile, line))
        {
            stringstream ss(line);
            string temp;
            vector<string> data;
            while (getline(ss, temp, ','))
                data.push_back(temp);

            if (data[1] == id && data[2] == pass && data[4] == "PT")
            {
                data[7] = to_string(new_hour);

                // Ghi thời gian check-in/out vào cột ngày hiện tại
                if (data.size() <= colDay)
                    data.resize(colDay + 1, ""); // mở rộng nếu thiếu

                data[colDay] = timeStr;

                // tạo lại dòng
                string updatedLine;
                for (size_t i = 0; i < data.size(); ++i)
                {
                    if (i != 0) updatedLine += ",";
                    updatedLine += data[i];
                }
                lines.push_back(updatedLine);
            }
            else
            {
                lines.push_back(line);
            }
        }
        inFile.close();

        ofstream outFile("ds.csv");
        for (const auto& l : lines)
            outFile << l << "\n";
        outFile.close();
    }
    void CheckinPT()
    {
        string id, pass;
        cout << "Nhap ma nhan vien: ";
        cin.ignore();
        getline(cin, id);
        cout << "Nhap mat khau: ";
        getline(cin, pass);

        for (auto& nv : pt)
        {
            if (nv.getId() == id && nv.getPass() == pass)
            {
                nv.setCheckInTime();

                // In thời gian check-in bằng ctime_s
                char buffer[26];
                time_t t = nv.getCheckInTime(); // lấy time_t
                ctime_s(buffer, sizeof(buffer), &t);

                cout << "Check-in thanh cong luc " << buffer << "\n";
                return;
            }
        }
        cout << "Ma nhan vien hoac mat khau khong dung!\n";
    }
    void CheckoutPT()
    {
        string id, pass;
        cout << "Nhap ma nhan vien: ";
        cin.ignore();
        getline(cin, id);
        cout << "Nhap mat khau: ";
        getline(cin, pass);

        for (auto& nv : pt)
        {
            if (nv.getId() == id && nv.getPass() == pass)
            {
                nv.setCheckOutTime();
                cout << "Check-out thanh cong. So gio lam tang len: " << nv.getHour() << "\n";
                UpdateCSV_PT(id, pass, nv.getHour(), nv.getTimeToday());
                return;
            }
        }
        cout << "Ma nhan vien hoac mat khau khong dung!\n";
    }
    void MENU()
    {
        int chon;
        while (1)
        {
            cout << "\n ==================== Quan ly nhan su ================ \n";
            cout << "\n 1. Doc danh sach nhan vien ";
            cout << "\n 2. Xuat danh sach nhan vien ";
            cout << "\n 3. Check in nhan vien full time";
            cout << "\n 4. Check in nhan vien part time";
            cout << "\n 5. Check out nhan vien part time";
            cout << "\n 0. Thoat \n ";
            cin >> chon;
            if (chon == 1)
            {
                DocDS();
            }
            else if (chon == 2)
            {
                Inds();
            }
            else if (chon == 3)
            {
                CheckinFT();
            }
            else if (chon == 4)
            {
                CheckinPT();
            }
            else if (chon == 5)
            {
                CheckoutPT();
            }
            else if (chon == 0)
            {
                break;
            }
        }
    }
};

int main()
{
    DANHSACH ds;
    ds.MENU();
    return 0;
}