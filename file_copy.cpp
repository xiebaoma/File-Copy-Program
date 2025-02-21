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
    int sourceFile = open(source, READ_FLAGS);
    if (sourceFile == -1)
    {
        std::cerr << "无法打开源文件" << std::endl;
        return false;
    }

    int destFile = open(destination, WRITE_FLAGS, PERMISSION);
    if (destFile == -1)
    {
        close(sourceFile);
        std::cerr << "无法创建目标文件" << std::endl;
        return false;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead; // 删除重复的 bytesRead

    while ((bytesRead = read(sourceFile, buffer, BUFFER_SIZE)) > 0)
    {
        if (write(destFile, buffer, bytesRead) != bytesRead)
        {
            close(sourceFile);
            close(destFile);
            std::cerr << "写入错误" << std::endl;
            return false;
        }
    }

    close(sourceFile);
    close(destFile);
    return bytesRead >= 0;
}

// 使用 ANSI 标准 I/O 的文件复制函数
bool copyFileAnsi(const char *source, const char *destination)
{
    FILE *sourceFile = fopen(source, "rb");
    if (!sourceFile)
    {
        std::cerr << "无法打开源文件" << std::endl;
        return false;
    }

    FILE *destFile = fopen(destination, "wb");
    if (!destFile)
    {
        fclose(sourceFile);
        std::cerr << "无法创建目标文件" << std::endl;
        return false;
    }

    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, sourceFile)) > 0)
    {
        if (fwrite(buffer, 1, bytesRead, destFile) != bytesRead)
        {
            fclose(sourceFile);
            fclose(destFile);
            std::cerr << "写入错误" << std::endl;
            return false;
        }
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