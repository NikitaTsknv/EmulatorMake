#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

struct Target {
	std::string name;
	std::vector<std::string> ralation;
	std::string command;
};


int main(int argc, char * argv[]) {

	std::vector<std::string> vector_command_line;

	for(int i = 0; i < argc; i++) {
		vector_command_line.push_back(argv[i]);
	}
	if(argc == 1) {
		std::cout << "Command line has no additional arguments" << std::endl;
		return 0;
	}

	std::string makefile = vector_command_line[1];
	std::string line, temp;
	std::vector<std::string> vector_buf;
	std::ifstream in(makefile);
	if (in) {
		if (in.is_open()) {
                	while (std::getline(in, temp)) {
				vector_buf.push_back(temp);
				line += temp + "\n";
			}
                	in.close();
		} else {
			std::cout << "File not open" << std::endl;
			return 0;
		}
	} else {
		std::cout << "File not found" << std::endl;
		return 0;
	}

	//std::cout << line;

	for (size_t i = 0; i < vector_buf.size(); i++) {
		std::cout << i << " " << vector_buf[i] << "\n";
	}

	std::string target, command;
	std::unordered_map<std::string, std::string> map_target;
	std::vector<std::unordered_map<std::string, std::string>> vector_target;
	std::vector<std::string> vector_relation;
	bool flag_no_target = true;

	for (size_t i = 0; i < vector_buf.size(); i++) {

		if (vector_buf[i].find(':') == std::string::npos 
			&& flag_no_target) continue;
		else {
			if (flag_no_target) {
				for (size_t j = 0; j < vector_buf[i].length(); j++) {
					std::string str = vector_buf[i];
					if (str[j] != ':') {
						target += str[j];
					}
					else {
						std::cout<<target<<std::endl;
						flag_no_target = false;
						break;
					}
				}
			}
			else {
				if (vector_buf[i] == "\n" || vector_buf[i] == "") {
					continue;
				}
				else {
					command = vector_buf[i];
					std::cout<<command<<std::endl;
					map_target.insert({target, command});
					target = "";
					command = "";
					flag_no_target = true;
				}
			}
		}

	}

	for(auto& item : map_target)
	{
    		std::cout << item.first << " : " << item.second << std::endl;
	}


	return 0;
}
