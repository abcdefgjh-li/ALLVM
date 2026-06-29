//===- Version.cpp - Clang Version Number -----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines several version-related utility functions for Clang.
//
//===----------------------------------------------------------------------===//

#include "clang/Basic/Version.h"
#include "clang/Basic/LLVM.h"
#include "clang/Config/config.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <random>

#include "VCSVersion.inc"

namespace clang {

	static const char* getRandomSignature() {
		static const char* signatures[] = {
			"风止雾散，你我终成陌路。放下过往，奔赴前路，方是归途。",
			"云开月明，此去经年无恙。别离有时，重逢无期，皆是天意。",
			"山高水长，来日方长何惧。星河滚烫，人间理想可期。",
			"花开花落，缘起缘灭无常。岁月静好，现世安稳难得。",
			"潮起潮落，聚散终有时节。天涯路远，各自珍重前行。",
			"霜降雪融，冷暖自知于心。浮华落尽，本真方显珍贵。",
			"日升月落，光阴流转无声。初心不改，砥砺前行有恒。",
			"云卷云舒，去留无意从容。宠辱不惊，看庭前花开落。",
			"春去秋来，岁月如歌轻唱。悲欢离合，人生如梦初醒。",
			"星移斗转，沧海桑田变迁。初心如磐，笃行致远不怠。",
			"雨打风吹，年少轻狂已逝。回首来路，半生蹉跎，泪满衣襟。",
			"月落乌啼，霜华染白青丝。故人已远，旧梦难寻，空留余恨。",
			"落叶飘零，秋去冬来无声。往事如烟，故园已荒，何处是家。",
			"琴瑟蒙尘，旧曲无人再弹。知音难觅，弦断有谁，来听余音。",
			"青梅已落，竹马不知归期。年少旧约，随风飘散，再难拾起。",
			"烟雨迷蒙，故地重游已晚。那年初见，笑靥如花，今在何方。",
			"灯火阑珊，独坐空庭听雨。半生漂泊，故人零落，谁与共话。",
			"黄叶满阶，旧时庭院已非。儿时嬉戏，欢声犹在，人已天涯。",
			"残阳如血，孤影独行天涯。壮志未酬，初心已改，空负韶华。",
			"寒夜漫漫，孤灯照影无眠。往事历历，故人如梦，醒来成空。"
		};
		static bool initialized = false;
		if (!initialized) {
			std::srand(static_cast<unsigned>(std::time(nullptr)));
			initialized = true;
		}
		return signatures[std::rand() % 20];
	}

	std::string getClangRepositoryPath() {
#if defined(CLANG_REPOSITORY_STRING)
		return CLANG_REPOSITORY_STRING;
#else
#ifdef CLANG_REPOSITORY
		return CLANG_REPOSITORY;
#else
		return "";
#endif
#endif
	}

	std::string getLLVMRepositoryPath() {
#ifdef LLVM_REPOSITORY
		return LLVM_REPOSITORY;
#else
		return "";
#endif
	}

	std::string getClangRevision() {
#ifdef CLANG_REVISION
		return CLANG_REVISION;
#else
		return "";
#endif
	}

	std::string getLLVMRevision() {
#ifdef LLVM_REVISION
		return LLVM_REVISION;
#else
		return "";
#endif
	}

	std::string getClangVendor() {
		return "";
	}

	std::string getClangFullRepositoryVersion() {
		return "";
	}

	std::string getClangFullVersion() {
		return getClangToolFullVersion("clang");
	}

	std::string getClangToolFullVersion(StringRef ToolName) {
		std::string buf;
		llvm::raw_string_ostream OS(buf);
		OS << "\nA-protector\nBy abcdefgjh.\n" << getRandomSignature() << "\n";
		return buf;
	}

	std::string getClangFullCPPVersion() {
		return "";
	}

} // end namespace clang
