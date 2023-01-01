#include <list>
#include <string>
#include <vector>

class ProCr
{
public:
	struct tbData
	{
		tbData(std::string one, std::string sec)
		{
			this->source = one;
			this->dest = sec;
		}

		std::string source = "";
		std::string dest = "";
	};

	std::vector<tbData> tFiles = {
		tbData("a", "010"),
		tbData("b", "020"),
		tbData("c", "030"),
		tbData("d", "040"),
		tbData("e", "050"),
		tbData("f", "060"),
		tbData("g", "070"),
		tbData("h", "080"),
		tbData("i", "090"),
		tbData("j", "100"),
		tbData("k", "110"),
		tbData("l", "120"),
		tbData("m", "130"),
		tbData("n", "140"),
		tbData("o", "150"),
		tbData("p", "160"),
		tbData("r", "170"),
		tbData("s", "180"),
		tbData("t", "190"),
		tbData("u", "200"),
		tbData("v", "210"),
		tbData("y", "220"),
		tbData("z", "230"),
		tbData("x", "240"),
		tbData("q", "250"),
		tbData("w", "260"),
		tbData("=", "270"),
		tbData("_", "280"),
		tbData("\\", "290"),
		tbData("/", "300"),
		tbData("&", "310"),
		tbData("+", "320"),
		tbData("0", "330"),
		tbData("1", "340"),
		tbData("2", "350"),
		tbData("3", "360"),
		tbData("4", "370"),
		tbData("5", "380"),
		tbData("6", "390"),
		tbData("7", "400"),
		tbData("8", "410"),
		tbData("9", "420"),
		tbData(" ", "430"),





		tbData("A", "005"),
		tbData("B", "015"),
		tbData("C", "025"),
		tbData("D", "035"),
		tbData("E", "045"),
		tbData("F", "055"),
		tbData("G", "065"),
		tbData("H", "075"),
		tbData("I", "085"),
		tbData("J", "095"),
		tbData("K", "105"),
		tbData("L", "115"),
		tbData("M", "125"),
		tbData("N", "135"),
		tbData("O", "145"),
		tbData("P", "155"),
		tbData("R", "165"),
		tbData("S", "175"),
		tbData("T", "185"),
		tbData("U", "195"),
		tbData("V", "205"),
		tbData("Y", "215"),
		tbData("Z", "225"),
		tbData("X", "235"),
		tbData("Q", "245"),
		tbData("W", "255"),

	};
	std::string verifyDat = "";

	int getRnd();
	std::string RandomString(int ch);
	void GenerateKey(std::string iData, std::string& oData, bool needCrypt = false);
	void Crypt(std::string iData, std::string& oData);
	void DeCrypt(std::string iData, std::string& oData);
	std::string GetData();
	bool Validate(std::string socketData);
};



namespace Makron {

	class b79 {
	public:

		static std::string Encode(const std::string data) {
			static constexpr char sEncodingTable[] = {
			  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
			  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			  'w', 'x', 'y', 'z', '0', '1', '2', '3',
			  '4', '5', '6', '7', '8', '9', '+', '/'
			};

			size_t in_len = data.size();
			size_t out_len = 4 * ((in_len + 2) / 3);
			std::string ret(out_len, '\0');
			size_t i;
			char* p = const_cast<char*>(ret.c_str());

			for (i = 0; i < in_len - 2; i += 3) {
				*p++ = sEncodingTable[(data[i] >> 2) & 0x3F];
				*p++ = sEncodingTable[((data[i] & 0x3) << 4) | ((int)(data[i + 1] & 0xF0) >> 4)];
				*p++ = sEncodingTable[((data[i + 1] & 0xF) << 2) | ((int)(data[i + 2] & 0xC0) >> 6)];
				*p++ = sEncodingTable[data[i + 2] & 0x3F];
			}
			if (i < in_len) {
				*p++ = sEncodingTable[(data[i] >> 2) & 0x3F];
				if (i == (in_len - 1)) {
					*p++ = sEncodingTable[((data[i] & 0x3) << 4)];
					*p++ = '=';
				}
				else {
					*p++ = sEncodingTable[((data[i] & 0x3) << 4) | ((int)(data[i + 1] & 0xF0) >> 4)];
					*p++ = sEncodingTable[((data[i + 1] & 0xF) << 2)];
				}
				*p++ = '=';
			}

			return ret;
		}

		static std::string Decode(const std::string& input, std::string& out) {
			static constexpr unsigned char kDecodingTable[] = {
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
			  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
			  64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
			  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
			  64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
			  41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
			  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
			};

			size_t in_len = input.size();
			if (in_len % 4 != 0) return "Input data size is not a multiple of 4";

			size_t out_len = in_len / 4 * 3;
			if (input[in_len - 1] == '=') out_len--;
			if (input[in_len - 2] == '=') out_len--;

			out.resize(out_len);

			for (size_t i = 0, j = 0; i < in_len;) {
				uint32_t a = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
				uint32_t b = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
				uint32_t c = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
				uint32_t d = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];

				uint32_t triple = (a << 3 * 6) + (b << 2 * 6) + (c << 1 * 6) + (d << 0 * 6);

				if (j < out_len) out[j++] = (triple >> 2 * 8) & 0xFF;
				if (j < out_len) out[j++] = (triple >> 1 * 8) & 0xFF;
				if (j < out_len) out[j++] = (triple >> 0 * 8) & 0xFF;
			}

			return "";
		}

	};

}