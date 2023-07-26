#include "MD4.h"
#include <bitset>

void MD4::init_buf(){
	mes_size = 0;
	A = 0x67452301;
	B = 0xefcdab89;
	C = 0x98badcfe;
	D = 0x10325476;
}

void print_bs(vector<uint32_t> data) {
	if (!data.size())
		return;

	cout << "Total size = " << data.size() * 32 << " bits" << endl;
	for (size_t i = 0; i < data.size(); i++) {
		if (i + 1 < 10)
			cout << " ";
		cout << i + 1 << ") ";
		auto t = bitset<32>{ data[i] };
		for (size_t j = 0; j < 4; ++j) {
			for (size_t k = 0; k < 8; k++)
				cout << t[8 * (3 - j)+(7 - k)];
			cout << " ";
		}

		cout << endl;
	}
}


string MD4::int_to_hex(uint32_t i){
	std::stringstream stream;
	stream << setfill('0') << std::setw(sizeof(uint32_t) * 2) << hex << i;
	return stream.str();
}

string MD4::ret_res(){
	uint32_t res_a = 0;
	uint32_t res_b = 0;
	uint32_t res_c = 0;
	uint32_t res_d = 0;

	uint32_t tmp_a = A;
	uint32_t tmp_b = B;
	uint32_t tmp_c = C;
	uint32_t tmp_d = D;

	//return int_to_hex(tmp_a) + " " + int_to_hex(tmp_b) + " " + int_to_hex(tmp_c) + " " + int_to_hex(tmp_d);

	for (int i = 0; i < 4; i++) {
		res_a = (res_a << 8) + tmp_a % 256;
		res_b = (res_b << 8) + tmp_b % 256;
		res_c = (res_c << 8) + tmp_c % 256;
		res_d = (res_d << 8) + tmp_d % 256;

		tmp_a >>= 8;
		tmp_b >>= 8;
		tmp_c >>= 8;
		tmp_d >>= 8;
	}

	//cout << bitset<sizeof(int) * 8>(A) << "  " << bitset<sizeof(int) * 8>(B) << "  " << bitset<sizeof(int) * 8>(C) << "  " << bitset<sizeof(int) * 8>(D) << "  " << endl;
	//cout << bitset<sizeof(int) * 8>(res_a) << "  " << bitset<sizeof(int) * 8>(res_b) << "  " << bitset<sizeof(int) * 8>(res_c) << "  " << bitset<sizeof(int) * 8>(res_d) << "  " << endl;
	//cout << "0" << hex << A << "  " << hex << B << "  " << hex << C << "  " << hex << D << endl;
	//cout << hex << res_a << "  " << hex << res_b << "  " << hex << res_c << "  " << hex << res_d << endl;

	
	return int_to_hex(res_a) + int_to_hex(res_b) + int_to_hex(res_c) + int_to_hex(res_d);
}


uint32_t MD4::cyclic_left_shift(uint32_t num, uint32_t step){
	step %= 32;
	return ((num << step) | num >> (32 - step));
}

vector<uint8_t> MD4::mess_to_BigInt(string& mes){
	mes_size = mes.length();
	vector<uint8_t> out;
	out.resize(mes.size());
	for (int i = 0; i < mes_size; i++) {
		out.at(i) = (uint8_t)(mes.at(i));
	}
	return out;
}

void MD4::add_size(vector<uint8_t>& vec){
	static unsigned char padding[64] = {
	  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	uint32_t index = vec.size() % 64;
	uint32_t padLen = (index < 56) ? (56 - index) : (120 - index);

	for (int i = 0; i < padLen; i++) {
		vec.push_back(padding[i]);
	}

	uint64_t size = mes_size << 3;
	for (int i = 0; i < 8; i++) {
		vec.push_back(size & 0xff);
		size >>= 8;
	}
}

vector<uint32_t> MD4::convert(const vector<uint8_t> vec){
	vector<uint32_t> out;
	for (uint32_t j = 0; j < vec.size(); j += 4) {
		uint32_t output = ((uint32_t)vec[j]) | (((uint32_t)vec[j + 1]) << 8) |
			(((uint32_t)vec[j + 2]) << 16) | (((uint32_t)vec[j + 3]) << 24);
		out.push_back(output);
	}

	return out;
}

uint32_t MD4::first_fun(uint32_t X, uint32_t Y, uint32_t Z){
	return (X & Y) | (~X & Z);
}

uint32_t MD4::second_fun(uint32_t X, uint32_t Y, uint32_t Z){
	return (X & Y) | (X & Z) | (Y & Z);
}

uint32_t MD4::third_fun(uint32_t X, uint32_t Y, uint32_t Z){
	return X ^ Y ^ Z;
}

inline void MD4::FF(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s) {
	a = cyclic_left_shift((a + first_fun(b, c, d) + x), s);
}

inline void MD4::GG(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s) {
	a = cyclic_left_shift((a + second_fun(b, c, d) + x + 0x5A827999), s);
}

inline void MD4::HH(uint32_t& a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s) {
	a = cyclic_left_shift((a + third_fun(b, c, d) + x + 0x6ED9EBA1), s);
}

uint32_t MD4::r1(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint8_t k, uint8_t s){
	return cyclic_left_shift((a + first_fun(b, c, d) + k), s);
}

uint32_t MD4::r2(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint8_t k, uint8_t s){
	return cyclic_left_shift((a + second_fun(b, c, d) + k + 0x5A827999), s);
}

uint32_t MD4::r3(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint8_t k, uint8_t s){
	return cyclic_left_shift((a + third_fun(b, c, d) + k + 0x6ED9EBA1), s);
}

void MD4::raund_1(vector<uint32_t>& x, uint32_t iter){
	for (int i = 0; i < 16; i += 4) {
		A = r1(A, B, C, D, x.at(iter + k_r1[i]), shift_r1[0]);
		D = r1(D, A, B, C, x.at(iter + k_r1[i + 1]), shift_r1[1]);
		C = r1(C, D, A, B, x.at(iter + k_r1[i + 2]), shift_r1[2]);
		B = r1(B, C, D, A, x.at(iter + k_r1[i + 3]), shift_r1[3]);
	}
}

void MD4::raund_2(vector<uint32_t>& x, uint32_t iter){
	for (int i = 0; i < 16; i += 4) {
		A = r2(A, B, C, D, x.at(iter + k_r2[i]), shift_r2[0]);
		D = r2(D, A, B, C, x.at(iter + k_r2[i + 1]), shift_r2[1]);
		C = r2(C, D, A, B, x.at(iter + k_r2[i + 2]), shift_r2[2]);
		B = r2(B, C, D, A, x.at(iter + k_r2[i + 3]), shift_r2[3]);
	}
}

void MD4::raund_3(vector<uint32_t>& x, uint32_t iter){
	for (int i = 0; i < 16; i += 4) {
		A = r3(A, B, C, D, x.at(iter + k_r3[i]), shift_r3[0]);
		D = r3(D, A, B, C, x.at(iter + k_r3[i + 1]), shift_r3[1]);
		C = r3(C, D, A, B, x.at(iter + k_r3[i + 2]), shift_r3[2]);
		B = r3(B, C, D, A, x.at(iter + k_r3[i + 3]), shift_r3[3]);
	}
}

string MD4::hash(string& mes){
	vector<uint8_t> bytes = mess_to_BigInt(mes);
	add_size(bytes);
	vector<uint32_t> message = convert(bytes);
	//print_bs(message);
	uint32_t N = message.size() / 16;
	for (int i = 0; i < N; i++) {
		//cout << hex << A << " " << hex << B << " " << hex << C << " " << hex << D << " " << endl;
		uint32_t iter = i * 16;
		uint32_t AA = A;
		uint32_t BB = B;
		uint32_t CC = C;
		uint32_t DD = D;

		/*raund_1(message, iter);
		cout << hex << A << " " << hex << B << " " << hex << C << " " << hex << D << " " << endl;
		raund_2(message, iter);
		cout << hex << A << " " << hex << B << " " << hex << C << " " << hex << D << " " << endl;
		raund_3(message, iter);
		cout << hex << A << " " << hex << B << " " << hex << C << " " << hex << D << " " << endl;*/

		FF(AA, BB, CC, DD, message[iter + 0], shift_r1[0]); /* 1 */
		FF(DD, AA, BB, CC, message[iter + 1], shift_r1[1]); /* 2 */
		FF(CC, DD, AA, BB, message[iter + 2], shift_r1[2]); /* 3 */
		FF(BB, CC, DD, AA, message[iter + 3], shift_r1[3]); /* 4 */
		FF(AA, BB, CC, DD, message[iter + 4], shift_r1[0]); /* 5 */
		FF(DD, AA, BB, CC, message[iter + 5], shift_r1[1]); /* 6 */
		FF(CC, DD, AA, BB, message[iter + 6], shift_r1[2]); /* 7 */
		FF(BB, CC, DD, AA, message[iter + 7], shift_r1[3]); /* 8 */
		FF(AA, BB, CC, DD, message[iter + 8], shift_r1[0]); /* 9 */
		FF(DD, AA, BB, CC, message[iter + 9], shift_r1[1]); /* 10 */
		FF(CC, DD, AA, BB, message[iter + 10], shift_r1[2]); /* 11 */
		FF(BB, CC, DD, AA, message[iter + 11], shift_r1[3]); /* 12 */
		FF(AA, BB, CC, DD, message[iter + 12], shift_r1[0]); /* 13 */
		FF(DD, AA, BB, CC, message[iter + 13], shift_r1[1]); /* 14 */
		FF(CC, DD, AA, BB, message[iter + 14], shift_r1[2]); /* 15 */
		FF(BB, CC, DD, AA, message[iter + 15], shift_r1[3]); /* 16 */
		//cout << hex << AA << " " << hex << BB << " " << hex << CC << " " << hex << DD << " " << endl;

		/* Round 2 */
		GG(AA, BB, CC, DD, message[iter + 0], shift_r2[0]); /* 17 */
		GG(DD, AA, BB, CC, message[iter + 4], shift_r2[1]); /* 18 */
		GG(CC, DD, AA, BB, message[iter + 8], shift_r2[2]); /* 19 */
		GG(BB, CC, DD, AA, message[iter + 12], shift_r2[3]); /* 20 */
		GG(AA, BB, CC, DD, message[iter + 1], shift_r2[0]); /* 21 */
		GG(DD, AA, BB, CC, message[iter + 5], shift_r2[1]); /* 22 */
		GG(CC, DD, AA, BB, message[iter + 9], shift_r2[2]); /* 23 */
		GG(BB, CC, DD, AA, message[iter + 13], shift_r2[3]); /* 24 */
		GG(AA, BB, CC, DD, message[iter + 2], shift_r2[0]); /* 25 */
		GG(DD, AA, BB, CC, message[iter + 6], shift_r2[1]); /* 26 */
		GG(CC, DD, AA, BB, message[iter + 10], shift_r2[2]); /* 27 */
		GG(BB, CC, DD, AA, message[iter + 14], shift_r2[3]); /* 28 */
		GG(AA, BB, CC, DD, message[iter + 3], shift_r2[0]); /* 29 */
		GG(DD, AA, BB, CC, message[iter + 7], shift_r2[1]); /* 30 */
		GG(CC, DD, AA, BB, message[iter + 11], shift_r2[2]); /* 31 */
		GG(BB, CC, DD, AA, message[iter + 15], shift_r2[3]); /* 32 */
		//cout << hex << AA << " " << hex << BB << " " << hex << CC << " " << hex << DD << " " << endl;

		/* Round 3 */
		HH(AA, BB, CC, DD, message[iter + 0], shift_r3[0]); /* 33 */
		HH(DD, AA, BB, CC, message[iter + 8], shift_r3[1]); /* 34 */
		HH(CC, DD, AA, BB, message[iter + 4], shift_r3[2]); /* 35 */
		HH(BB, CC, DD, AA, message[iter + 12], shift_r3[3]); /* 36 */
		HH(AA, BB, CC, DD, message[iter + 2], shift_r3[0]); /* 37 */
		HH(DD, AA, BB, CC, message[iter + 10], shift_r3[1]); /* 38 */
		HH(CC, DD, AA, BB, message[iter + 6], shift_r3[2]); /* 39 */
		HH(BB, CC, DD, AA, message[iter + 14], shift_r3[3]); /* 40 */
		HH(AA, BB, CC, DD, message[iter + 1], shift_r3[0]); /* 41 */
		HH(DD, AA, BB, CC, message[iter + 9], shift_r3[1]); /* 42 */
		HH(CC, DD, AA, BB, message[iter + 5], shift_r3[2]); /* 43 */
		HH(BB, CC, DD, AA, message[iter + 13], shift_r3[3]); /* 44 */
		HH(AA, BB, CC, DD, message[iter + 3], shift_r3[0]); /* 45 */
		HH(DD, AA, BB, CC, message[iter + 11], shift_r3[1]); /* 46 */
		HH(CC, DD, AA, BB, message[iter + 7], shift_r3[2]); /* 47 */
		HH(BB, CC, DD, AA, message[iter + 15], shift_r3[3]); /* 48 */
		//cout << hex << AA << " " << hex << BB << " " << hex << CC << " " << hex << DD << " " << endl;

		A += AA;
		B += BB;
		C += CC;
		D += DD;
	}

	//cout << hex << A << " " << hex << B << " " << hex << C << " " << hex << D << " " << endl;

	string res = ret_res();
	init_buf();
	return res;
}
