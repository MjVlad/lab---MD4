#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

class MD4{
private:
	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;
	uint64_t mes_size;
	uint8_t shift_r1[4] = {3, 7, 11, 19};
	uint8_t shift_r2[4] = {3, 5, 9, 13};
	uint8_t shift_r3[4] = {3, 9, 11, 15};
	uint8_t k_r1[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	uint8_t k_r2[16] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};
	uint8_t k_r3[16] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};

	void init_buf();
	string int_to_hex(uint32_t i);
	string ret_res();

	uint32_t cyclic_left_shift(uint32_t num, uint32_t step);
	
	vector<uint8_t> mess_to_BigInt(string& mes);
	void add_size(vector<uint8_t>& vec);
	vector<uint32_t> convert(vector<uint8_t> vec);

	uint32_t first_fun(uint32_t X, uint32_t Y, uint32_t Z);
	uint32_t second_fun(uint32_t X, uint32_t Y, uint32_t Z);
	uint32_t third_fun(uint32_t X, uint32_t Y, uint32_t Z);

	inline void FF(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s);
	inline void GG(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s);
	inline void HH(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s);

	uint32_t r1(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint8_t k, uint8_t s);
	uint32_t r2(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint8_t k, uint8_t s);
	uint32_t r3(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint8_t k, uint8_t s);

	void raund_1(vector<uint32_t>& x, uint32_t iter);
	void raund_2(vector<uint32_t>& x, uint32_t iter);
	void raund_3(vector<uint32_t>& x, uint32_t iter);

public:
	MD4() {
		init_buf();
	}
	
	string hash(string& mes);
};

