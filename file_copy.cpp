#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#define READ_FLAGS O_RDONLY | O_BINARY
#define WRITE_FLAGS O_WRONLY | O_CREAT | O_TRUNC | O_BINARY
#define PERMISSION _S_IREAD | _S_IWRITE
typedef long ssize_t; // 为 Windows 定义 ssize_t
#else
#include <unistd.h>
#define READ_FLAGS O_RDONLY
#define WRITE_FLAGS O_WRONLY | O_CREAT | O_TRUNC
#define PERMISSION S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
#endif

#define BUFFER_SIZE 4096

// 使用低级 I/O 的文件复制函数
bool copyFileLowLevel(const char *source, const char *destination)
{
    // 打开源文件
    int sourceFile = open(source, READ_FLAGS);
    if (sourceFile == -1)
    {
        std::cerr << "无法打开源文件: " << source << ", 错误信息: " << strerror(errno) << std::endl;
        return false;
    }

    // 打开目标文件
    int destFile = open(destination, WRITE_FLAGS, PERMISSION);
    if (destFile == -1)
    {
        std::cerr << "无法创建目标文件: " << destination << ", 错误信息: " << strerror(errno) << std::endl;
        close(sourceFile); // 确保源文件被关闭
        return false;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    // 循环读取源文件并写入目标文件
    while ((bytesRead = read(sourceFile, buffer, BUFFER_SIZE)) > 0)
    {
        // 写入目标文件
        ssize_t bytesWritten = write(destFile, buffer, bytesRead);
        if (bytesWritten != bytesRead)
        {
            std::cerr << "写入错误，写入字节数与预期不符" << std::endl;
            close(sourceFile);
            close(destFile);
            return false;
        }
    }

    // 检查读取源文件是否遇到错误
    if (bytesRead == -1)
    {
        std::cerr << "读取源文件时出错, 错误信息: " << strerror(errno) << std::endl;
    }

    // 关闭文件描述符
    close(sourceFile);
    close(destFile);

    // 返回值：如果读取完文件且没有错误，返回 true
    return bytesRead >= 0;
}

// 使用 ANSI 标准 I/O 的文件复制函数
bool copyFileAnsi(const char *source, const char *destination)
{
    FILE *sourceFile = fopen(source, "rb");
    if (!sourceFile)
    {
        std::cerr << "无法打开源文件: " << source << std::endl;
        return false;
    }

    FILE *destFile = fopen(destination, "wb");
    if (!destFile)
    {
        std::cerr << "无法创建目标文件: " << destination << std::endl;
        fclose(sourceFile); // 确保在出错时关闭源文件
        return false;
    }

    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, sourceFile)) > 0)
    {
        if (ferror(sourceFile))
        {
            std::cerr << "读取源文件时发生错误" << std::endl;
            fclose(sourceFile);
            fclose(destFile);
            return false;
        }

        size_t bytesWritten = fwrite(buffer, 1, bytesRead, destFile);
        if (bytesWritten != bytesRead)
        {
            std::cerr << "写入目标文件时发生错误: 写入的字节数 (" << bytesWritten << ") 与读取的字节数 (" << bytesRead << ") 不一致" << std::endl;
            fclose(sourceFile);
            fclose(destFile);
            return false;
        }

        if (ferror(destFile))
        {
            std::cerr << "写入目标文件时发生错误" << std::endl;
            fclose(sourceFile);
            fclose(destFile);
            return false;
        }
    }

    if (ferror(sourceFile))
    {
        std::cerr << "读取源文件时发生错误" << std::endl;
    }

    fclose(sourceFile);
    fclose(destFile);
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 3 && argc != 4)
    {
        std::cout << "用法：" << std::endl;
        std::cout << "低级 I/O: " << argv[0] << " <源文件> <目标文件>" << std::endl;
        std::cout << "ANSI I/O: " << argv[0] << " -ansi <源文件> <目标文件>" << std::endl;
        return 1;
    }

    const char *sourceFile;
    const char *destFile;
    bool useAnsi = false;

    if (argc == 4)
    {
        if (strcmp(argv[1], "-ansi") == 0)
        {
            useAnsi = true;
            sourceFile = argv[2];
            destFile = argv[3];
        }
        else
        {
            std::cerr << "无效的参数" << std::endl;
            return 1;
        }
    }
    else
    {
        sourceFile = argv[1];
        destFile = argv[2];
    }

    bool success = useAnsi ? copyFileAnsi(sourceFile, destFile)
                           : copyFileLowLevel(sourceFile, destFile);

    if (success)
    {
        std::cout << "文件复制成功" << std::endl;
        return 0;
    }
    else
    {
        std::cerr << "文件复制失败" << std::endl;
        return 1;
    }
}