#include "MD4.h"
#include <bitset>
#include "md5.h"
#include <algorithm>
#include <time.h>
#include <iostream>
#include <fstream>

string random_string(size_t length){
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	string str(length, 0);
	generate_n(str.begin(), length, randchar);
	return str;
}

//length must be >= 8 and <= 16
uint16_t cut(uint16_t h, uint8_t length) {
	uint8_t tmp = 16 - length;
	return (h >> tmp) << tmp;
}

void test(string& path_1, string& path_2, string& key, uint8_t size) {
	ofstream f1, f2;
	f1.open(path_1);
	f2.open(path_2);
	if (!f1.is_open() || !f2.is_open())
		return;
	//vector<pair<uint32_t, uint32_t>> results;
	//results.reserve(1000);
	for (uint16_t i = 0; i < 1000; i++) {
		pair<uint32_t, uint32_t> res(0, 0);
		bool flag = false;
		vector<uint16_t> hashs;
		hashs.reserve(0x40000);
		hashs.push_back(cut(md4_tests(key), size));
		//cout << hex << md4(key) << endl << hashs[0] << endl;
		for (uint32_t j = 1; j < 0x100000; j++) {
			string rand;
			if ((rand = random_string(64)) == key) {
				j--;
				continue;
			}
			uint16_t h = cut(md4_tests(rand), size);
			//cout << hex << md4(rand) << endl << h << endl;
			if (res.first == 0 && hashs.at(0) == h)
				res.first = j;
			/*if (flag == false) {
				res.second += j + 1;
				if (find(hashs.begin(), hashs.end(), h) != hashs.end())
					flag = true;
			}*/
			if (res.second == 0 && find(hashs.begin(), hashs.end(), h) != hashs.end()) {
				res.second = j;
			}
			hashs.push_back(h);
			if (res.first != 0 && res.second != 0)
				break;
		}
		//results.push_back(res);
		f1 << dec << res.first << " ";
		f2 << dec << res.second << " ";
	}
	f1.close();
	f2.close();
}

void testes(string& path) {
	string key[3] = { "", "1", "The quick brown fox jumps over the lazy dog" };
	uint8_t sizes[5] = { 8, 10, 12, 14, 16 };
	for (uint8_t i = 0; i < 5; i++) {
		string tmp_path = path + "test_size" + to_string(sizes[i]);
		for (uint8_t j = 0; j < 3; j++) {
			string tmp_path_1 = tmp_path + "_key" + to_string(j) + "_1.txt";
			string tmp_path_2 = tmp_path + "_key" + to_string(j) + "_2.txt";
			test(tmp_path_1, tmp_path_2, key[j], sizes[i]);
			cout << to_string(i) << "  " << to_string(j) << endl;
		}
	}
}

void main() {
	/*srand(clock());
	string path("D://Учёба//Крипта//lab 4//files//");
	testes(path);*/


	MD4 md_4;
	string key[3] = { "", "1", "The quick brown fox jumps over the lazy dog" };
	//for (int i = 0; i < 3; i++) {
		cout << "key is :'" << key[2] << "'   hash is: " << endl << md_4.hash(key[2]);// << endl << "control: " << endl << md4(key[i]) << endl;
	//}
	//uint32_t beg = clock();
	/*for (int i = 0; i < 10000; i++) {
		md_4.hash(key[2]);
	}
	cout << clock() - beg << endl;
	beg = clock();
	for (int i = 0; i < 10000; i++) {
		md4(key[2]);
	}
	cout << clock() - beg << endl;*/
}