#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>
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
	string getGroup() const { return gr; }
		
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
	string getCurrentTime()
	{
		auto now = chrono::system_clock::now();
		time_t now_time = chrono::system_clock::to_time_t(now);
		tm tm_now;
		localtime_s(&tm_now, &now_time);

		stringstream ss;
		ss << put_time(&tm_now, "%H:%M:%S");
		return ss.str();
	}
	double calculateHours(const string& timeIn, const string& timeOut)
	{
		tm tmIn = {}, tmOut = {};
		istringstream ssIn(timeIn), ssOut(timeOut);
		char discard;

		ssIn >> tmIn.tm_hour >> discard >> tmIn.tm_min >> discard >> tmIn.tm_sec;
		ssOut >> tmOut.tm_hour >> discard >> tmOut.tm_min >> discard >> tmOut.tm_sec;

		double seconds = difftime(mktime(&tmOut), mktime(&tmIn));
		return seconds / 3600.0; // Chuyển sang giờ
	}
	void UpdateCSV(const string& id, const string& pass, bool isCheckIn)
	{
		ifstream inFile("ds.csv");
		vector<string> lines;
		string line;

		// Lấy ngày hiện tại
		time_t now = time(0);
		tm ltm;
		localtime_s(&ltm, &now);
		int currentDay = ltm.tm_mday;

		getline(inFile, line); // Bỏ qua dòng header
		lines.push_back(line);

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
				if (data[4] == "FT") // Full-time
				{
					int columnToUpdate = 7 + currentDay;
					if (data.size() > columnToUpdate)
					{
						data[columnToUpdate] = "o"; // Đánh dấu đã check-in
					}
				}
				else if (data[4] == "PT") // Part-time
				{
					int columnToUpdate = 7 + currentDay;
					if (data.size() <= columnToUpdate)
					{
						// Đảm bảo đủ số cột
						data.resize(columnToUpdate + 1, "");
					}

					string currentTime = getCurrentTime();

					if (isCheckIn)
					{
						// Ghi lại thời gian check-in
						data[columnToUpdate] = "in:" + currentTime;
					}
					else
					{
						string existing = data[columnToUpdate];
						if (existing.find("in:") != string::npos)
						{
							// Trích xuất thời gian check-in
							string timeIn = existing.substr(3); // Bỏ "in:"
							string timeOut = currentTime;

							double hoursWorked = calculateHours(timeIn, timeOut);

							// Cập nhật tổng số giờ làm
							int totalHours = stoi(data[7]);
							totalHours += static_cast<int>(hoursWorked + 0.5); // Làm tròn
							data[7] = to_string(totalHours);

							// Ghi thời gian check-out
							data[columnToUpdate] = "in:" + timeIn + " - out:" + timeOut;
						}
					}
				}


				// Cập nhật lại dòng
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

		// Ghi lại file
		ofstream outFile("ds.csv");
		for (const auto& l : lines)
		{
			outFile << l << "\n";
		}
		outFile.close();

		cout << (isCheckIn ? "Check-in" : "Check-out") << " thanh cong!\n";
	}
	void Checkin()
	{
		string id, pass;

		cout << "Nhap ma nhan vien: ";
		cin.ignore();
		getline(cin, id);

		cout << "Nhap mat khau: ";
		getline(cin, pass);

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

		UpdateCSV(id, pass, true);
	}
	void Checkout()
	{
		string id, pass;

		cout << "Nhap ma nhan vien: ";
		cin.ignore();
		getline(cin, id);

		cout << "Nhap mat khau: ";
		getline(cin, pass);

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

		UpdateCSV(id, pass, false);
	}
	void MENU()
	{
		int chon;
		while (1)
		{
			cout << "\n Quan ly nhan vien ";
			cout << "\n 1. Doc danh sach nhan vien ";
			cout << "\n 2. Xuat danh sach nhan vien ";
			cout << "\n 3. Check in nhan vien ";
			cout << "\n 4. Check out nhan vien ";
			cout << "\n 0. Thoat \n ";
			cin >> chon;
			if (chon == 1)
			{
				DocDS();
			}
			if (chon == 2)
			{
				Inds();
			}
			if (chon == 3)
			{
				Checkin();
			}
			if (chon == 4)
			{
				Checkout();
			}
			if (chon == 0)
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