#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#pragma once

#include <string>
#include <vector>


class FileManager {
    public:
        FileManager(const std::string& filePath);

        virtual ~FileManager();

        FileManager(const FileManager&) = delete;
        FileManager& operator=(const FileManager&) = delete;

        std::vector<std::string> loadData();

        bool saveData(const std::vector<std::string>& data);

        bool fileExists() const;

    private:
        std::string m_filePath //filepath ;
};

#endif // FILEMANAGER_H