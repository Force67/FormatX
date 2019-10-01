
// Copyright (C) Force67 2019

#include <filesystem>
#include "TigerArc.h"

class TR9Tiger final : public IFileFormat
{
	TigerHeader hdr{};
	std::vector<TigerEntryV3> fileList;

public:

	FileResult Deserialize(utl::File& file) override
	{
		file.Seek(0, utl::seekMode::seek_set);
		file.Read(hdr);

		if (std::strcmp(hdr.platformName, "pc-w") != 0)
			return FileResult::badplatform;

		fileList.resize(hdr.numEntries);
		file.Read(fileList);

		return FileResult::success;
	}

	FileResult Serialize(utl::File& file) override
	{
		file.Write(hdr);
		file.Write(fileList);

		return FileResult::success;
	}

	FileResult ExtractAll(utl::File& file) override
	{
		std::string pfn(hdr.platformName);

		std::filesystem::create_directory("unp");
		std::filesystem::create_directory("unp\\" + pfn);

		int ec = 0;
		int lv = 0;
		for (auto& tre : fileList) {
			file.Seek(tre.offset, utl::seekMode::seek_set);

			std::vector<uint8_t> data(tre.size);
			file.Read(data);

			utl::File writer("unp//" + pfn + "//" +  std::to_string(tre.nameHash), utl::fileMode::write);
			if (writer.IsOpen()) {
				writer.Write(data);
			}

			const int percent = (ec / (hdr.numEntries / 100));
			if (lv != percent) {
				std::printf("Progress %d%% (%d/%d)\n", percent, ec, hdr.numEntries);
				lv = percent;
			}

			ec++;
		}

		return FileResult::success;
	}
};

class TR10Tiger final : public IFileFormat
{
	TigerHeader hdr{};
	std::vector<TigerEntryV4> fileList;

public:

	FileResult Deserialize(utl::File& file) override
	{
		file.Seek(0, utl::seekMode::seek_set);
		file.Read(hdr);

		if (std::strcmp(hdr.platformName, "orbis-w") != 0)
			return FileResult::badplatform;

		fileList.resize(hdr.numEntries);
		file.Read(fileList);

		return FileResult::success;
	}

	FileResult Serialize(utl::File& file) override
	{
		file.Write(hdr);
		file.Write(fileList);
		return FileResult::success;
	}

	FileResult ExtractAll(utl::File& file) override
	{
		std::string pfn(hdr.platformName);

		std::filesystem::create_directory("unp");
		std::filesystem::create_directory("unp\\" + pfn);

		int ec = 0;
		int lv = 0;
		for (auto& tre : fileList) {
			file.Seek(tre.offset, utl::seekMode::seek_set);

			std::vector<uint8_t> data(tre.size);
			file.Read(data);

			utl::File writer("unp//" + pfn + "//" + std::to_string(tre.crcNameHash), utl::fileMode::write);
			if (writer.IsOpen()) {
				writer.Write(data);
			}

			const int percent = (ec / (hdr.numEntries / 100));
			if (lv != percent) {
				std::printf("Progress %d%% (%d/%d)\n", percent, ec, hdr.numEntries);
				lv = percent;
			}

			ec++;
		}

		return FileResult::success;
	}
};

std::unique_ptr<IFileFormat> CreateTigerFMT(utl::File& file)
{
	uint32_t magic = 0;
	uint32_t version = 0;

	file.Read(magic);
	file.Read(version);

	// 'TAFS'
	if (magic != tigerMagic)
		return 0;

	std::unique_ptr<IFileFormat> handler;
	switch (version)
	{
	case 3:
	{
		handler = std::make_unique<TR9Tiger>();
		break;
	}
	case 4:
	{
		handler = std::make_unique<TR10Tiger>();
		break;
	}
	default:
		return nullptr;
	}

	return std::move(handler);
}