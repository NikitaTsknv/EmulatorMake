#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>


std::unordered_map<std::string, size_t>& recursion(std::string arg,
    std::unordered_map<std::string, std::vector<std::string>>& mp_rel,
    std::unordered_map<std::string, size_t>& ump, size_t& count) {

	std::vector<std::string> v_temp = mp_rel[arg];

	if (v_temp.empty()) {
		ump.insert({ arg, count });
		count++;
	}
	else {
		for (size_t i = 0; i < v_temp.size(); i++) {
			recursion(v_temp[i], mp_rel, ump, count);
		}
		ump.insert({ arg, count });
		count++;
	}

	return ump;
}


int main(int argc, char * argv[]) {

	std::vector<std::string> vector_command_line;

	for(int i = 0; i < argc; i++) {
		vector_command_line.push_back(argv[i]);
	}
	if(argc == 1) {
		std::cout << "Command line has no additional arguments" << std::endl;
		return 0;
	}

	std::string file = vector_command_line[1];
	std::string line, temp;
	std::vector<std::string> vector_buf;
	std::ifstream in(file);
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
	}
	else {
		std::cout << "File not found" << std::endl;
		return 0;
	}

	std::string target, command;
	std::unordered_map<std::string, std::vector<std::string>> umap_command, umap_relation;
	std::vector<std::string> vector_command, vector_relation, vector_arg;
	bool flag_no_target = true;

	for (size_t i = 0; i < vector_buf.size(); ++i) {
		std::string s = vector_buf[i];
		if (s.find(':') == std::string::npos && flag_no_target) {
			continue;
		}
		else {
			if (s.find(':') != std::string::npos) {

				// если старая цель есть, но уже пришла строка с новой целью,
				// то вставляем старые данные и очищаем их
				if (flag_no_target == false) {
					umap_command.insert({ target, vector_command });
					target = "";
					vector_command.clear();
					vector_relation.clear();
				}

				for (size_t j = 0; j < s.length(); j++) {
					std::string str = s;
					if (str[j] != ':') {
						if (isalnum(str[j]) != 0 || str[j] == '_') {
							target += str[j];
						}
						else {
							std::cout << "Not valid target. Exit" << i << std::endl;
							return 0;
						}
					}
					else {
						flag_no_target = false;

						// удаляем из строки цель:
						std::string substr = target + ":";
						std::string::const_iterator sub = std::find_end(s.begin(), s.end(), substr.begin(), substr.end());
						if (sub != s.end()) {
							s.erase(sub, sub + substr.size());
						}

						std::istringstream input{ s };
						std::vector<std::string> result_relation;

						// добавляем зависимости из подстроки
						while (!input.eof()) {
							std::string substring, sub;
							input >> substring;
							if (substring == "all") {
								std::cout << "Not valid relation. Exit" << std::endl;
								return 0;
							}
							else if (substring != "") {
								vector_relation.push_back(substring);
							}
						}

						//проверка на повторение целей
						if (umap_relation.count(target) == 0) {
							umap_relation.insert({ target, vector_relation });
						}
						else {
							std::cout << "Target already exists. Exit" << std::endl;
							return 0;
						}

						break;
					}
				}
			}
			else {
				if (s == "\n" || s == "") {
					continue;
				}
				else {
					// проверка первого символа в комманде на \t
					if (s[0] == '\t') {
						vector_command.push_back(s);

						// проверка на последнюю строку в файле
						if (i == vector_buf.size() - 1) {
							umap_command.insert({ target, vector_command });
						}
					}
					else {
						std::cout << "Not valid command. Exit" << std::endl;
						return 0;
					}
				}
			}
		}
	}

	// цикл заполнения вектора от аргументов коммандной строки
	if (vector_command_line.size() == 2) {
		vector_arg.push_back("all");
	}
	else {
		for(size_t i = 2; i < vector_command_line.size(); i++) {
			vector_arg.push_back(vector_command_line[i]);
		}
	}


	for (const auto& arg : vector_arg) {

		// проверка - существует ли такая цель в файле
	        if (umap_relation.count(arg) == 0 || umap_command.count(arg) == 0) {
                        std::cout << "Target doesn`t exist. Exit" << std::endl;
                        return 0;
                }
                else {
			std::unordered_map<std::string, size_t> umap_recursion;
			size_t count = 0;
			std::map<size_t, std::string> map;

			auto result_rec = recursion(arg, umap_relation, umap_recursion, count);

			for (auto& item : result_rec) {
				map[item.second] = item.first;
			}

			for (auto& item : map) {
				std::cout << item.second << std::endl;
				for (auto& item : umap_command[item.second]) {
					std::cout << item << std::endl;
				}
			}
		}
	}

	return 0;
}
