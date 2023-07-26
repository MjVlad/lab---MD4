#include "md5.h"

/* system implementation headers */
#include <cstdio>
#include <vector>
#include <bitset>


// Constants for MD5Transform routine.
#define S11 3
#define S12 7
#define S13 11
#define S14 19
#define S21 3
#define S22 5
#define S23 9
#define S24 13
#define S31 3
#define S32 9
#define S33 11
#define S34 15

///////////////////////////////////////////////

// F, G, H and I are basic MD5 functions.
MD5::uint4 MD5::first_fun(uint4 X, uint4 Y, uint4 Z) {
	return (X & Y) | (~X & Z);
}

MD5::uint4 MD5::second_fun(uint4 X, uint4 Y, uint4 Z) {
	return (X & Y) | (X & Z) | (Y & Z);
}

MD5::uint4 MD5::third_fun(uint4 X, uint4 Y, uint4 Z) {
	return X ^ Y ^ Z;
}

// rotate_left rotates x left n bits.
inline MD5::uint4 MD5::rotate_left(uint4 x, int n) {
	return (x << n) | (x >> (32 - n));
}

// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.
inline void MD5::FF(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s) {
	a = rotate_left((a + first_fun(b, c, d) + x), s);
}

inline void MD5::GG(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s) {
	a = rotate_left((a + second_fun(b, c, d) + x + 0x5A827999), s);
}

inline void MD5::HH(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s) {
	a = rotate_left((a + third_fun(b, c, d) + x + 0x6ED9EBA1), s);
}

//////////////////////////////////////////////

// default ctor, just initailize
MD5::MD5(){
	init();
}

//////////////////////////////////////////////

// nifty shortcut ctor, compute MD5 for string and finalize it right away
MD5::MD5(const std::string& text){
	init();
	update(text.c_str(), text.length());
	finalize();
}

//////////////////////////////

void MD5::init(){
	finalized = false;

	count[0] = 0;
	count[1] = 0;

	// load magic initialization constants.
	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;
}

//////////////////////////////

// decodes input (unsigned char) into output (uint4). Assumes len is a multiple of 4.
void MD5::decode(uint4 output[], const uint1 input[], size_type len){
	for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((uint4)input[j]) | (((uint4)input[j + 1]) << 8) |
		(((uint4)input[j + 2]) << 16) | (((uint4)input[j + 3]) << 24);
}

//////////////////////////////

// encodes input (uint4) into output (unsigned char). Assumes len is
// a multiple of 4.
void MD5::encode(uint1 output[], const uint4 input[], size_type len){
	for (size_type i = 0, j = 0; j < len; i++, j += 4) {
		output[j] = input[i] & 0xff;
		output[j + 1] = (input[i] >> 8) & 0xff;
		output[j + 2] = (input[i] >> 16) & 0xff;
		output[j + 3] = (input[i] >> 24) & 0xff;
	}
}

//////////////////////////////
std::vector<uint32_t> translate(MD5::uint4* x, uint32_t size) {
	std::vector<uint32_t> out;
	for (int i = 0; i < size; i++) {
		out.push_back((uint32_t)x[i]);
	}
	return out;
}

void print_bs1(std::vector<uint32_t> data) {
	if (!data.size())
		return;

	std::cout << std::dec << "Total size = " << data.size() * 32 << " bits" << std::endl;
	for (size_t i = 0; i < data.size(); i++) {
		if (i + 1 < 10)
			std::cout << " ";
		std::cout << i + 1 << ") ";
		auto t = std::bitset<32>{ data[i] };
		for (size_t j = 0; j < 4; ++j) {
			for (size_t k = 0; k < 8; k++)
				std::cout << t[8 * (3 - j) + (7 - k)];
			std::cout << " ";
		}

		std::cout << std::endl;
	}
}
// apply MD5 algo on a block
void MD5::transform(const uint1 block[blocksize]){
	uint4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];
	decode(x, block, blocksize);
	//print_bs1(translate(x, 16));

	//std::cout << std::hex << state[0] << " " << std::hex << state[1] << " " << std::hex << state[2] << " " << std::hex << state[3] << " " << std::endl;

	/* Round 1 */
	FF(a, b, c, d, x[0], S11); /* 1 */
	FF(d, a, b, c, x[1], S12); /* 2 */
	FF(c, d, a, b, x[2], S13); /* 3 */
	FF(b, c, d, a, x[3], S14); /* 4 */
	FF(a, b, c, d, x[4], S11); /* 5 */
	FF(d, a, b, c, x[5], S12); /* 6 */
	FF(c, d, a, b, x[6], S13); /* 7 */
	FF(b, c, d, a, x[7], S14); /* 8 */
	FF(a, b, c, d, x[8], S11); /* 9 */
	FF(d, a, b, c, x[9], S12); /* 10 */
	FF(c, d, a, b, x[10], S13); /* 11 */
	FF(b, c, d, a, x[11], S14); /* 12 */
	FF(a, b, c, d, x[12], S11); /* 13 */
	FF(d, a, b, c, x[13], S12); /* 14 */
	FF(c, d, a, b, x[14], S13); /* 15 */
	FF(b, c, d, a, x[15], S14); /* 16 */
	//std::cout << std::hex << a << " " << std::hex << b << " " << std::hex << c << " " << std::hex << d << " " << std::endl;
	/* Round 2 */
	GG(a, b, c, d, x[0], S21); /* 17 */
	GG(d, a, b, c, x[4], S22); /* 18 */
	GG(c, d, a, b, x[8], S23); /* 19 */
	GG(b, c, d, a, x[12], S24); /* 20 */
	GG(a, b, c, d, x[1], S21); /* 21 */
	GG(d, a, b, c, x[5], S22); /* 22 */
	GG(c, d, a, b, x[9], S23); /* 23 */
	GG(b, c, d, a, x[13], S24); /* 24 */
	GG(a, b, c, d, x[2], S21); /* 25 */
	GG(d, a, b, c, x[6], S22); /* 26 */
	GG(c, d, a, b, x[10], S23); /* 27 */
	GG(b, c, d, a, x[14], S24); /* 28 */
	GG(a, b, c, d, x[3], S21); /* 29 */
	GG(d, a, b, c, x[7], S22); /* 30 */
	GG(c, d, a, b, x[11], S23); /* 31 */
	GG(b, c, d, a, x[15], S24); /* 32 */
	//std::cout << std::hex << a << " " << std::hex << b << " " << std::hex << c << " " << std::hex << d << " " << std::endl;
	/* Round 3 */
	HH(a, b, c, d, x[0], S31); /* 33 */
	HH(d, a, b, c, x[8], S32); /* 34 */
	HH(c, d, a, b, x[4], S33); /* 35 */
	HH(b, c, d, a, x[12], S34); /* 36 */
	HH(a, b, c, d, x[2], S31); /* 37 */
	HH(d, a, b, c, x[10], S32); /* 38 */
	HH(c, d, a, b, x[6], S33); /* 39 */
	HH(b, c, d, a, x[14], S34); /* 40 */
	HH(a, b, c, d, x[1], S31); /* 41 */
	HH(d, a, b, c, x[9], S32); /* 42 */
	HH(c, d, a, b, x[5], S33); /* 43 */
	HH(b, c, d, a, x[13], S34); /* 44 */
	HH(a, b, c, d, x[3], S31); /* 45 */
	HH(d, a, b, c, x[11], S32); /* 46 */
	HH(c, d, a, b, x[7], S33); /* 47 */
	HH(b, c, d, a, x[15], S34); /* 48 */

	//std::cout << std::hex << a << " " << std::hex << b << " " << std::hex << c << " " << std::hex << d << " " << std::endl;

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	//std::cout << std::hex << state[0] << " " << std::hex << state[1] << " " << std::hex << state[2] << " " << std::hex << state[3] << " " << std::endl;

	// Zeroize sensitive information.
	memset(x, 0, sizeof x);
}

//////////////////////////////

// MD5 block update operation. Continues an MD5 message-digest
// operation, processing another message block
void MD5::update(const unsigned char input[], size_type length){
	// compute number of bytes mod 64
	size_type index = count[0] / 8 % blocksize;

	// Update number of bits
	if ((count[0] += (length << 3)) < (length << 3))
		count[1]++;
	count[1] += (length >> 29);

	// number of bytes we need to fill in buffer
	size_type firstpart = 64 - index;

	size_type i;

	// transform as many times as possible.
	if (length >= firstpart)
	{
		// fill buffer first, transform
		memcpy(&buffer[index], input, firstpart);
		transform(buffer);

		// transform chunks of blocksize (64 bytes)
		for (i = firstpart; i + blocksize <= length; i += blocksize)
			transform(&input[i]);

		index = 0;
	}
	else
		i = 0;

	// buffer remaining input
	memcpy(&buffer[index], &input[i], length - i);
}

//////////////////////////////

// for convenience provide a verson with signed char
void MD5::update(const char input[], size_type length){
	update((const unsigned char*)input, length);
}

//////////////////////////////

// MD5 finalization. Ends an MD5 message-digest operation, writing the
// the message digest and zeroizing the context.
MD5& MD5::finalize(){
	static unsigned char padding[64] = {
	  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	if (!finalized) {
		// Save number of bits
		unsigned char bits[8];
		encode(bits, count, 8);

		// pad out to 56 mod 64.
		size_type index = count[0] / 8 % 64;
		size_type padLen = (index < 56) ? (56 - index) : (120 - index);
		update(padding, padLen);

		// Append length (before padding)
		update(bits, 8);

		// Store state in digest
		encode(digest, state, 16);

		// Zeroize sensitive information.
		memset(buffer, 0, sizeof buffer);
		memset(count, 0, sizeof count);

		finalized = true;
	}

	return *this;
}

//////////////////////////////
// return hex representation of digest as string
std::string MD5::hexdigest() const{
	if (!finalized)
		return "";

	char buf[33];
	for (int i = 0; i < 16; i++)
		sprintf(buf + i * 2, "%02x", digest[i]);
	buf[32] = 0;

	return std::string(buf);
}

uint16_t MD5::hexdigest_tests() const {
	if (!finalized)
		return 0;
	uint16_t out = (digest[0] << 8) + digest[1];
	/*char buf[33];
	for (int i = 0; i < 16; i++)
		sprintf(buf + i * 2, "%02x", digest[i]);
	buf[32] = 0;*/

	return out;
}

//////////////////////////////

std::ostream& operator<<(std::ostream& out, MD5 md5){
	return out << md5.hexdigest();
}

//////////////////////////////

std::string md4(const std::string str){
	MD5 md5 = MD5(str);

	return md5.hexdigest();
}

uint16_t md4_tests(const std::string str) {
	MD5 md5 = MD5(str);

	return md5.hexdigest_tests();
}