#include <iostream>
#include <vector>
#include <string.h>
#include <windows.h>
#include <ctime>
#include <regex>

void setFinalVec(std::vector<char> *finalVec, char* argv[], int argc) {
	std::string convertedInput;

	if (std::string(argv[1]).find("NDATEN") != std::string::npos) {
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, sizeof(buffer), "%Y%m%d", timeinfo);
		std::string str(buffer);

		const char *cstr = str.c_str();

		convertedInput = std::regex_replace(argv[1], std::regex("NDATEN"), cstr);
	} else if (std::string(argv[1]).find("FDATEF") != std::string::npos) {
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, sizeof(buffer), "%Y/%m/%d", timeinfo);
		std::string str(buffer);

		const char *cstr = str.c_str();

		convertedInput = std::regex_replace(argv[1], std::regex("FDATEF"), cstr);
	} else if (std::string(argv[1]).find("FPRODUCTF") != std::string::npos) {
		OpenClipboard(NULL);
		HANDLE hData = GetClipboardData(CF_TEXT);
		char* pszText = static_cast<char*>(GlobalLock(hData));
		std::string text(pszText);
		GlobalUnlock(hData);
		CloseClipboard();

		convertedInput = std::regex_replace(text, std::regex("-"), "");
	} else {
		convertedInput = argv[1];
	}

	for (char& c : convertedInput) {
		std::cout << c;
		finalVec->push_back(c);
	}
}

struct VKeyFromSymbol {
	int keyCode;
	bool shift;
};

// this is ugly and im so sorry
VKeyFromSymbol getVKeyFromSymbol(char input) {
	VKeyFromSymbol retVal;
	retVal.keyCode = -1;
	retVal.shift = false;

	if (input == '.') {
		retVal.keyCode = 190;
	} else if (input == '>') {
		retVal.keyCode = 190;
		retVal.shift = true;
	} else if (input == ',') {
		retVal.keyCode = 188;
	} else if (input == '<') {
		retVal.keyCode = 188;
		retVal.shift = true;
	} else if (input == '/') {
		retVal.keyCode = 191;
	} else if (input == '?') {
		retVal.keyCode = 191;
		retVal.shift = true;
	} else if (input == '=') {
		retVal.keyCode = 187;
	} else if (input == '+') {
		retVal.keyCode = 187;
		retVal.shift = true;
	} else if (input == '-') {
		retVal.keyCode = 189;
	} else if (input == '_') {
		retVal.keyCode = 189;
		retVal.shift = true;
	} else if (input == ';') {
		retVal.keyCode = 186;
	} else if (input == ':') {
		retVal.keyCode = 186;
		retVal.shift = true;
	} else if (input == '`') {
		retVal.keyCode = 192;
	} else if (input == '~') {
		retVal.keyCode = 192;
		retVal.shift = true;
	} else if (input == '[') {
		retVal.keyCode = 219;
	} else if (input == '{') {
		retVal.keyCode = 219;
		retVal.shift = true;
	} else if (input == '\\') {
		retVal.keyCode = 220;
	} else if (input == '|') {
		retVal.keyCode = 220;
		retVal.shift = true;
	} else if (input == ']') {
		retVal.keyCode = 221;
	} else if (input == '}') {
		retVal.keyCode = 221;
		retVal.shift = true;
	} else if (input == '\'') {
		retVal.keyCode = 222;
	} else if (input == '"') {
		retVal.keyCode = 222;
		retVal.shift = true;
	} else if (input == ')') {
		retVal.keyCode = 48;
		retVal.shift = true;
	} else if (input == '!') {
		retVal.keyCode = 49;
		retVal.shift = true;
	} else if (input == '@') {
		retVal.keyCode = 50;
		retVal.shift = true;
	} else if (input == '#') {
		retVal.keyCode = 51;
		retVal.shift = true;
	} else if (input == '$') {
		retVal.keyCode = 52;
		retVal.shift = true;
	} else if (input == '%') {
		retVal.keyCode = 53;
		retVal.shift = true;
	} else if (input == '^') {
		retVal.keyCode = 54;
		retVal.shift = true;
	} else if (input == '&') {
		retVal.keyCode = 55;
		retVal.shift = true;
	} else if (input == '*') {
		retVal.keyCode = 56;
		retVal.shift = true;
	} else if (input == '(') {
		retVal.keyCode = 57;
		retVal.shift = true;
	}

	return retVal;
}

void buildToBeTyped(std::vector<char> input, std::vector<int> *toBeTyped) {
	for (int x = 0; x < input.size(); x++) {
		if (((int)input[x] >= 65 && (int)input[x] <= 90)) {
			toBeTyped->push_back(16); // shift
			toBeTyped->push_back((int)input[x]);
		} else if (((int)input[x] >= 97 && (int)input[x] <= 122)) { // lowercase
			toBeTyped->push_back((int)input[x] - 32);
		} else if ((int)input[x] >= 48 && (int)input[x] <= 57) {
			toBeTyped->push_back((int)input[x]);
		} else if (!std::isalnum(input[x]) && input[x] != ' ') {
			VKeyFromSymbol key = getVKeyFromSymbol(input[x]);

			if (key.shift) {
				toBeTyped->push_back(16);
				toBeTyped->push_back(key.keyCode);
			} else {
				toBeTyped->push_back(key.keyCode);
			}
		} else if ((int)input[x] == 32) { // space
			toBeTyped->push_back(32);
		}
	}
}

void type(std::vector<int> finalVec) {
	for (int x = 0; x < finalVec.size(); x++) {
		INPUT input;
		input.type = INPUT_KEYBOARD;

		if (finalVec[x] == 16) { // shift key?
			input.ki.wScan = MapVirtualKey(16, MAPVK_VK_TO_VSC);
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = 16;
			input.ki.dwFlags = 0;
			SendInput(1, &input, sizeof(INPUT)); // press and hold shift key

			x++;

			input.ki.wScan = MapVirtualKey(finalVec[x], MAPVK_VK_TO_VSC);
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = finalVec[x];
			input.ki.dwFlags = 0;
			SendInput(1, &input, sizeof(INPUT)); // press our next key

			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT)); // release the next key

			input.ki.wScan = MapVirtualKey(16, MAPVK_VK_TO_VSC);
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = 16;
			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT)); // release the shift key
		} else {
			input.ki.wScan = MapVirtualKey(finalVec[x], MAPVK_VK_TO_VSC);
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
			input.ki.wVk = finalVec[x];
			input.ki.dwFlags = 0;
			SendInput(1, &input, sizeof(INPUT)); // press our key

			input.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &input, sizeof(INPUT)); // release the key
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc < 1) {
		return 0;
	} else {
		std::vector<char> finalVec = {};
		std::vector<char> *finalVecPtr;
		std::vector<int> toBeTyped = {};
		std::vector<int> *toBeTypedPtr;
		finalVecPtr = &finalVec;
		toBeTypedPtr = &toBeTyped;

		setFinalVec(finalVecPtr, argv, argc);

		for (int x = 0; x < finalVec.size(); x++) {
			std::cout << x << std::endl;
			std::cout << finalVec[x] << std::endl;
		}

		buildToBeTyped(finalVec, toBeTypedPtr);

		type(toBeTyped);

		return 0;
	}
}

