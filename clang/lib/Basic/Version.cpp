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

#include "VCSVersion.inc"

namespace clang {

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
#ifdef CLANG_VENDOR
		return CLANG_VENDOR;
#else
		return "";
#endif
	}

	std::string getClangFullRepositoryVersion() {
		std::string buf;
		llvm::raw_string_ostream OS(buf);
		std::string Path = getClangRepositoryPath();
		std::string Revision = getClangRevision();
		if (!Path.empty() || !Revision.empty()) {
			OS << '(';
			if (!Path.empty())
				OS << Path;
			if (!Revision.empty()) {
				if (!Path.empty())
					OS << ' ';
				OS << Revision;
			}
			OS << ')';
		}
		// Support LLVM in a separate repository.
		std::string LLVMRev = getLLVMRevision();
		if (!LLVMRev.empty() && LLVMRev != Revision) {
			OS << " (";
			std::string LLVMRepo = getLLVMRepositoryPath();
			if (!LLVMRepo.empty())
				OS << LLVMRepo << ' ';
			OS << LLVMRev << ')';
		}
		return buf;
	}

	std::string getClangFullVersion() {
		return getClangToolFullVersion("clang");
	}

	std::string getClangToolFullVersion(StringRef ToolName) {
		std::string buf;
		llvm::raw_string_ostream OS(buf);
		OS << getClangVendor() << ToolName << " version " CLANG_VERSION_STRING;

		std::string repo = getClangFullRepositoryVersion();
		if (!repo.empty()) {
			OS << " " << repo;
		}

		OS << "\nA-protect https://github.com/abcdefgjh-li/ALLVM\nBy abcdefgjh.\n风止雾散，你我终成陌路。放下过往，奔赴前路，方是归途。\n";
		//OS << "XR 定制版 Production by 小二ˣʳ QQ群：478626908 TG群：@XRWZ666\n崇文尚理，知行合一；山野万里，藏尽温柔与星光。\n";
		return buf;
	}

	std::string getClangFullCPPVersion() {
		// The version string we report in __VERSION__ is just a compacted version of
		// the one we report on the command line.
		std::string buf;
		llvm::raw_string_ostream OS(buf);
		OS << getClangVendor() << "Clang " CLANG_VERSION_STRING;

		std::string repo = getClangFullRepositoryVersion();
		if (!repo.empty()) {
			OS << " " << repo;
		}

		return buf;
	}

} // end namespace clang
