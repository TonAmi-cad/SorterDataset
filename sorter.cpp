#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <iomanip> // для форматирования чисел в именах файлов
#include <fstream>

namespace fs = std::filesystem;

void copyImagesToFolders(const std::string& sourceDir, const std::string& destDir, int imagesPerFolder = 1000) {
    try {
        // Проверяем, что исходная папка существует
        if (!fs::exists(sourceDir) || !fs::is_directory(sourceDir)) {
            std::cerr << "Source directory does not exist or is not a directory.\n";
            return;
        }

        // Получаем список изображений из папки
        std::vector<fs::path> images;
        for (const auto& entry : fs::directory_iterator(sourceDir)) {
            if (entry.is_regular_file()) {
                images.push_back(entry.path());
            }
        }

        // Проверка, есть ли изображения
        if (images.empty()) {
            std::cerr << "No images found in the source directory.\n";
            return;
        }

        // Копируем изображения по папкам
        int folderIndex = 1;
        int fileCount = 0;

        for (const auto& imagePath : images) {
            // Проверяем, требуется ли создать новую папку
            if (fileCount % imagesPerFolder == 0) {
                fs::path folderPath = fs::path(destDir) / std::to_string(folderIndex++);
                fs::create_directory(folderPath);
            }

            // Копируем файл в текущую папку
            fs::path destPath = fs::path(destDir) / std::to_string(folderIndex - 1) / imagePath.filename();
            fs::copy(imagePath, destPath, fs::copy_options::overwrite_existing);

            fileCount++;
        }
        std::cout << "Images successfully copied into numbered folders.\n";

    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << "\n";
    }
}

int main() {
    std::string sourceDir, destDir;
    std::cout << "Enter the source directory path: ";
    std::getline(std::cin, sourceDir);
    std::cout << "Enter the destination directory path: ";
    std::getline(std::cin, destDir);

    copyImagesToFolders(sourceDir, destDir);

    return 0;
}
