#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <thread>
#include <chrono>

#define AY_OBFUSCATE(str) str


__attribute__((annotate("vmp")))
void print_info(const std::string& msg) {
	std::string prefix(AY_OBFUSCATE("[-] "));
	std::cout << prefix << msg << std::endl;
}


__attribute__((annotate("vmp")))
void print_success(const std::string& msg) {
	std::string prefix(AY_OBFUSCATE("[-] "));
	std::cout << prefix << msg << std::endl;
}


__attribute__((annotate("vmp")))
void print_error(const std::string& msg) {
	std::string prefix(AY_OBFUSCATE("[!] "));
	std::cout << prefix << msg << std::endl;
}


__attribute__((annotate("vmp")))
void remove_directory_contents(const std::string& path) {
	DIR* dir = opendir(path.c_str());
	if (dir == nullptr) {
		std::string suffix(AY_OBFUSCATE(" 目录不存在"));
		print_error(path + suffix);
		return;
	}

	struct dirent * entry;
	while ((entry = readdir(dir)) != nullptr) {
		std::string name = entry->d_name;
		std::string dot(AY_OBFUSCATE("."));
		std::string dotdot(AY_OBFUSCATE(".."));
		if (name == dot || name == dotdot) continue;

		std::string slash(AY_OBFUSCATE("/"));
		std::string full_path = path + slash + name;
		struct stat st;
		if (stat(full_path.c_str(), &st) == 0) {
			if (S_ISDIR(st.st_mode)) {
				std::string prefix(AY_OBFUSCATE("删除目录: "));
				print_info(prefix + name);
				std::string cmd1(AY_OBFUSCATE("rm -rf \""));
				std::string cmd2(AY_OBFUSCATE("\""));
				std::string cmd = cmd1 + full_path + cmd2;
				system(cmd.c_str());
			} else {
				std::string prefix(AY_OBFUSCATE("删除文件: "));
				print_info(prefix + name);
				std::string cmd1(AY_OBFUSCATE("rm \""));
				std::string cmd2(AY_OBFUSCATE("\""));
				std::string cmd = cmd1 + full_path + cmd2;
				system(cmd.c_str());
			}
		}
	}
	closedir(dir);
	std::string msg(AY_OBFUSCATE("完成!"));
	print_success(msg);
}


__attribute__((annotate("vmp")))
void remove_directory(const std::string& path) {
	struct stat st;
	if (stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
		std::string prefix(AY_OBFUSCATE("删除缓存目录: "));
		print_info(prefix + path);
		std::string cmd1(AY_OBFUSCATE("rm -rf \""));
		std::string cmd2(AY_OBFUSCATE("\""));
		std::string cmd = cmd1 + path + cmd2;
		system(cmd.c_str());
	}
}


__attribute__((annotate("vmp")))
void clean_wangzhe(const std::string& package_name) {
	std::string prefix1(AY_OBFUSCATE("/data/data/"));
	std::string SGAME_DIR = prefix1 + package_name;
	std::string suffix1(AY_OBFUSCATE("/files/ano_tmp"));
	std::string TARGET_DIR = SGAME_DIR + suffix1;
	std::string suffix2(AY_OBFUSCATE("/custom_cache"));
	std::string TARGET_DIR2 = TARGET_DIR + suffix2;
	std::string prefix2(AY_OBFUSCATE("/storage/emulated/0/Android/data/"));
	std::string DIR = prefix2 + package_name;
	std::string suffix3(AY_OBFUSCATE("/files"));
	std::string TARGET_DIR3 = DIR + suffix3;

	std::string msg1(AY_OBFUSCATE("开始运行[1/4]"));
	print_info(msg1);
	remove_directory_contents(TARGET_DIR2);

	std::string msg2(AY_OBFUSCATE("开始运行[2/4]"));
	print_info(msg2);
	remove_directory_contents(TARGET_DIR);

	std::string msg3(AY_OBFUSCATE("开始运行[3/4]"));
	print_info(msg3);
	remove_directory_contents(TARGET_DIR3);

	std::string msg4(AY_OBFUSCATE("开始运行[4/4]"));
	print_info(msg4);
	std::string suffix4(AY_OBFUSCATE("/cache"));
	std::string suffix5(AY_OBFUSCATE("/cache"));
	std::string suffix6(AY_OBFUSCATE("/files/qm"));
	std::string suffix7(AY_OBFUSCATE("/databases"));
	std::string suffix8(AY_OBFUSCATE("/shared_prefs"));
	remove_directory(SGAME_DIR + suffix4);
	remove_directory(DIR + suffix5);
	remove_directory(SGAME_DIR + suffix6);
	remove_directory(SGAME_DIR + suffix7);
	remove_directory(SGAME_DIR + suffix8);

	std::string msg5(AY_OBFUSCATE("清理完成"));
	print_info(msg5);
}

#include "weiyan/Util.h"//导入微验库(每次注入的库不通用，请使用对应注入的库)
__attribute__((annotate("vmp")))
int main() {
    // 微验接口域名
	const string sbc428c4746d584b61ed69f81719c6ef8 = "wy.llua.cn";
	// 当前版本，用于检查更新
	const string currentVersion = "1.0";
	// 卡密存储路径
	const string kmPath = "/sdcard/.km";
	
	// 获取公告
	string notice_data = httppost(sbc428c4746d584b61ed69f81719c6ef8,"v2/893ce523799a21b890d25e50fe045bda",uaf2c82b988c77fae683d2c5c8331faee(b263fe5d59982e095638f763b0155ae8e(uaf2c82b988c77fae683d2c5c8331faee(uaf2c82b988c77fae683d2c5c8331faee(uaf2c82b988c77fae683d2c5c8331faee(uaf2c82b988c77fae683d2c5c8331faee(b263fe5d59982e095638f763b0155ae8e(h3489af11618a58be3cd5128cf0e932d0(h3489af11618a58be3cd5128cf0e932d0("id=zt00xMAXOrt","kc4B2RAWSHq6ztoChYT8FNvawG+ZIEm95M0ly7UXsbrVJu1KgjPQi/epfL3dOxnD"),"DWe6yV1BsjJZRmNdUiLz0OEAcg+KHbavt2rMn7k9x4X35ql/ISoPF8hpGYTCQuwf"),"wf8e7c94eb34e12240d5ab66c"))))),"ac8edac19ccc7ad1bd47d")));
	//try {
		json notice_i785a05c1cf7baa89e2908c4bd2393376 = json::parse(b263fe5d59982e095638f763b0155ae8e(rf89634bf35a0f3833c8c765bfa7a6e2e(notice_data),"y7c74771162ce8592ae75328e"));
		if (notice_i785a05c1cf7baa89e2908c4bd2393376.contains("msg")) {
			std::string gg = notice_i785a05c1cf7baa89e2908c4bd2393376["msg"]["app_gg"];
			if (!gg.empty()) {
				std::cout << "公告:\n" << gg << std::endl;
			} else {
				std::cerr << "公告解析失败[-1]" << std::endl;
			}
		} else {
			std::cerr << "公告解析失败[-2]" << std::endl;
		}
	//}
	//catch(const std::exception & e) {
		//std::cerr << "公告获取失败" << e.what() << std::endl;
	//}
	std::cout << std::endl;
	
	// 检查更新
	string ini_data = httppost(sbc428c4746d584b61ed69f81719c6ef8,"v2/893ce523799a21b890d25e50fe045bda",uaf2c82b988c77fae683d2c5c8331faee(b263fe5d59982e095638f763b0155ae8e(uaf2c82b988c77fae683d2c5c8331faee(uaf2c82b988c77fae683d2c5c8331faee(uaf2c82b988c77fae683d2c5c8331faee(uaf2c82b988c77fae683d2c5c8331faee(b263fe5d59982e095638f763b0155ae8e(h3489af11618a58be3cd5128cf0e932d0(h3489af11618a58be3cd5128cf0e932d0("id=Tacqy1kNqao","kc4B2RAWSHq6ztoChYT8FNvawG+ZIEm95M0ly7UXsbrVJu1KgjPQi/epfL3dOxnD"),"DWe6yV1BsjJZRmNdUiLz0OEAcg+KHbavt2rMn7k9x4X35ql/ISoPF8hpGYTCQuwf"),"wf8e7c94eb34e12240d5ab66c"))))),"ac8edac19ccc7ad1bd47d")));
	//try {
		json ini_i785a05c1cf7baa89e2908c4bd2393376 = json::parse(b263fe5d59982e095638f763b0155ae8e(rf89634bf35a0f3833c8c765bfa7a6e2e(ini_data),"y7c74771162ce8592ae75328e"));
		if (ini_i785a05c1cf7baa89e2908c4bd2393376.contains("msg")) {
			std::string version = ini_i785a05c1cf7baa89e2908c4bd2393376["msg"]["version"];
			std::string updateshow = ini_i785a05c1cf7baa89e2908c4bd2393376["msg"]["updateshow"];
			std::string updateurl = ini_i785a05c1cf7baa89e2908c4bd2393376["msg"]["updateurl"];
			std::string updatemust = ini_i785a05c1cf7baa89e2908c4bd2393376["msg"]["updatemust"];
			if (version != currentVersion){
				std::cout << "有新版本:" << std::endl << 	
				             "当前版本:" << currentVersion << std::endl <<
				             "最新版本:" << version << std::endl <<
				             "更新内容:" << updateshow << std::endl <<
				             "更新地址:" << updateurl << std::endl;
				if (updatemust == "y"){
		    	    std::cout << "强制更新，请更新至最新版本后使用！" << std::endl;
		    	    exit(0);
		        }
		    }
		} else {
			std::cerr << "更新解析失败" << std::endl;
		}
	//}
	//catch(const std::exception & e) {
		//std::cerr << "检查更新失败" << e.what() << std::endl;
	//}
	std::cout << std::endl;
	
	// 单码登录
	while(true){
	string baba585f13e7e356d6271f4f55e5cb61d;
	string ia27ad06d1957cc1d8ec448efc4405950 = getIMEI();
	std::cout << "请输入卡密(输入y使用上次登录卡密): ";
    std::cin >> baba585f13e7e356d6271f4f55e5cb61d;
    
    if (baba585f13e7e356d6271f4f55e5cb61d == "y"){
    	ifstream file(kmPath);
        if (!file.is_open()) {
        	std::cout << "没有获取到上次登录卡密" << std::endl;
            continue;
        }
        getline(file, baba585f13e7e356d6271f4f55e5cb61d);
        file.close();
        std::cout << "使用上次卡密:" << baba585f13e7e356d6271f4f55e5cb61d << std::endl;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd()); // 使用 Mersenne Twister 引擎
    std::uniform_int_distribution<> dist(100000, 999999);
    
    auto now = std::chrono::system_clock::now();
    auto epoch = now.time_since_epoch();
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(epoch).count();
    
	string hfd994f35e65bc4f1032d5fb1eb58b78c = std::to_string(timestamp);
	string k8e9881d844a55bf0022c6b6512e62e3a = std::to_string(dist(gen));
	string z1030b941fd5d08a8f9807915d21dd0c5 = wc9ac6963244f1bc61f64c040a68260ba("kami=" + baba585f13e7e356d6271f4f55e5cb61d + "&markcode=" + ia27ad06d1957cc1d8ec448efc4405950 + "&t=" + hfd994f35e65bc4f1032d5fb1eb58b78c + "&h37a27b0dcb0b582987a2b1ab");
	string r5389836e43d7ba3ac35429ace29d5414 = httppost(sbc428c4746d584b61ed69f81719c6ef8,"v2/893ce523799a21b890d25e50fe045bda",uaf2c82b988c77fae683d2c5c8331faee(b263fe5d59982e095638f763b0155ae8e(uaf2c82b988c77fae683d2c5c8331faee(uaf2c82b988c77fae683d2c5c8331faee(uaf2c82b988c77fae683d2c5c8331faee(uaf2c82b988c77fae683d2c5c8331faee(b263fe5d59982e095638f763b0155ae8e(h3489af11618a58be3cd5128cf0e932d0(h3489af11618a58be3cd5128cf0e932d0("id=77Y7qQQqhyM&kami=" + baba585f13e7e356d6271f4f55e5cb61d + "&markcode=" + ia27ad06d1957cc1d8ec448efc4405950 + "&t=" + hfd994f35e65bc4f1032d5fb1eb58b78c + "&sign=" + z1030b941fd5d08a8f9807915d21dd0c5 + "&value=" + k8e9881d844a55bf0022c6b6512e62e3a +"","kc4B2RAWSHq6ztoChYT8FNvawG+ZIEm95M0ly7UXsbrVJu1KgjPQi/epfL3dOxnD"),"DWe6yV1BsjJZRmNdUiLz0OEAcg+KHbavt2rMn7k9x4X35ql/ISoPF8hpGYTCQuwf"),"wf8e7c94eb34e12240d5ab66c"))))),"ac8edac19ccc7ad1bd47d")));
	//try {
		json i785a05c1cf7baa89e2908c4bd2393376 = json::parse(b263fe5d59982e095638f763b0155ae8e(rf89634bf35a0f3833c8c765bfa7a6e2e(ad567aafba6556d3d26734dab4d900511(ad567aafba6556d3d26734dab4d900511(z69376affb996ed8ad65df3059d52eff4(z69376affb996ed8ad65df3059d52eff4(b263fe5d59982e095638f763b0155ae8e(rf89634bf35a0f3833c8c765bfa7a6e2e(r5389836e43d7ba3ac35429ace29d5414),"mf132dcc1f1f5610395dc9e4daaa2880e0f22"))),"pBldvzan7OwYi2e8FRQVAG3cxhq6WX01jPm9uMf+UCZs5ITS4gbJkKtEyoNHD/rL"),"calrWsFAtdMIRP23HC4Of9NnGeqbZuiQwTkYVhBy+KE8vz/1Sp0xmgj756UXDoJL")),"n2f32b6d25140338ec7"));
		if (i785a05c1cf7baa89e2908c4bd2393376["p269764d742d4b2cc5d3e18c75dc9c042"] == 68026 && i785a05c1cf7baa89e2908c4bd2393376["s3bd3c00f140e5e3236672d798988f29b"]["w1490ffefcd5d90caae4c8bdb54bea112"] == "31c255957c4ae1278de378163a787c29"){
            long efcd183b93a5cf4b340362826edff3e39 = i785a05c1cf7baa89e2908c4bd2393376["b5634d8a0d25baef1608d6efd0d10e2d8"];
            if (efcd183b93a5cf4b340362826edff3e39-std::stol(hfd994f35e65bc4f1032d5fb1eb58b78c)>30 || efcd183b93a5cf4b340362826edff3e39-std::stol(hfd994f35e65bc4f1032d5fb1eb58b78c)<-30){
                std::cout << "设备时间不准\n" << std::endl;
            }else{
                std::string a2c31c92ce34f4bf7a66cebb34b1e9955 = std::to_string(efcd183b93a5cf4b340362826edff3e39);
                long w73fee93916f40b696240e5414d34766e = i785a05c1cf7baa89e2908c4bd2393376["p269764d742d4b2cc5d3e18c75dc9c042"];
                std::string nf654fb4118af7b38bf590bd68638aeb3 = std::to_string(w73fee93916f40b696240e5414d34766e);
                long j136491e246dd942146606da36eb2fea5 = i785a05c1cf7baa89e2908c4bd2393376["s3bd3c00f140e5e3236672d798988f29b"]["n96ed58790d77bd9fdec8fe06d7c192f4"];
                std::string yfbf24590ec97a1469f26491be23a0d7d = std::to_string(j136491e246dd942146606da36eb2fea5);
                if (i785a05c1cf7baa89e2908c4bd2393376["s3bd3c00f140e5e3236672d798988f29b"]["e6cae2cdeb42e"] != d84016a38274b89bc22f3953a9c92951e(wc9ac6963244f1bc61f64c040a68260ba(ocde211bd4612f5730be6d074f438a333(nf654fb4118af7b38bf590bd68638aeb3+z1030b941fd5d08a8f9807915d21dd0c5+yfbf24590ec97a1469f26491be23a0d7d+""))) || i785a05c1cf7baa89e2908c4bd2393376["s3bd3c00f140e5e3236672d798988f29b"]["jd49c406ccee7e4"] != wc9ac6963244f1bc61f64c040a68260ba(ocde211bd4612f5730be6d074f438a333(nf654fb4118af7b38bf590bd68638aeb3+yfbf24590ec97a1469f26491be23a0d7d+a2c31c92ce34f4bf7a66cebb34b1e9955+z1030b941fd5d08a8f9807915d21dd0c5+"")) || i785a05c1cf7baa89e2908c4bd2393376["s3bd3c00f140e5e3236672d798988f29b"]["lf4233b14"] != d84016a38274b89bc22f3953a9c92951e(ocde211bd4612f5730be6d074f438a333(nf654fb4118af7b38bf590bd68638aeb3+yfbf24590ec97a1469f26491be23a0d7d+"")) || i785a05c1cf7baa89e2908c4bd2393376["s3bd3c00f140e5e3236672d798988f29b"]["vefc88f1f18cbed2b"] != d84016a38274b89bc22f3953a9c92951e(d84016a38274b89bc22f3953a9c92951e(yfbf24590ec97a1469f26491be23a0d7d+k8e9881d844a55bf0022c6b6512e62e3a+""))){
                    std::cout << "校验失败\n" << std::endl;
                }else{
                    if (i785a05c1cf7baa89e2908c4bd2393376["s3bd3c00f140e5e3236672d798988f29b"]["nbac58b22e737b65dd432912f642d8a9f"] == "single"){
                        std::cout << "登录成功，剩余可登录次数：" << i785a05c1cf7baa89e2908c4bd2393376["s3bd3c00f140e5e3236672d798988f29b"]["pca96402e4b7dbbb7848e1d745608624f"] << std::endl;
                    }else{
                        long zb3d87596f176ec759abba392921ae879 = i785a05c1cf7baa89e2908c4bd2393376["s3bd3c00f140e5e3236672d798988f29b"]["cbc325979bf947e3b790eb3176541de74"];
                        std::tm tm = *std::localtime(&zb3d87596f176ec759abba392921ae879 );
                        std::stringstream ss;
                        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
                        std::cout << "登录成功，到期时间：" << ss.str() << std::endl;
                        //到期自动退出
				        signal(SIGALRM, _exit); 
                        alarm(zb3d87596f176ec759abba392921ae879-timestamp);
                    }
                    ofstream file(kmPath);
                    if (file.is_open()) {
                        file << baba585f13e7e356d6271f4f55e5cb61d << endl;
                        file.close();
                    } else {
                        cerr << "无法保存卡密到文件: " << kmPath << endl;
                    }
			        break;
                }
            }
		}else{
		    std::string msg = i785a05c1cf7baa89e2908c4bd2393376["s3bd3c00f140e5e3236672d798988f29b"];
			std::cout << msg << std::endl;
		}
	//}
	//catch(const std::exception & e) {
		//std::cerr << "登录失败" << e.what() << std::endl;
	//}
	std::cout << std::endl;
	}
	
	

	std::cout << "========================================" << std::endl;
	std::cout << "           腾讯游戏清理工具" << std::endl;
	std::cout << "========================================" << std::endl;
	std::cout << std::endl;

	while (true) {
		std::string package_name;
		std::string prompt(AY_OBFUSCATE("请输入要清理的包名: "));
		std::cout << prompt;
		std::getline(std::cin, package_name);

		if (package_name.empty()) {
			std::string msg(AY_OBFUSCATE("包名不能为空，请重新输入"));
			print_error(msg);
			continue;
		}

		clean_wangzhe(package_name);
		std::cout << "[-] 清理完成，程序即将退出" << std::endl;
		break;
	}
	return 0;
}

