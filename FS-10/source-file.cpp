#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

int main(int argc, char** argv){
    if(argc != 3){
        std::cerr << "Invalid arguments" << std::endl;
        exit(errno);
    }

    int source_file = open(argv[1], O_RDONLY);
    if(source_file == -1){
        std::cerr << "Can't open the source file: " << std::endl;
        exit(errno);
    }

    int dest_file = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0664);
    if(dest_file == -1){
        std::cerr << "Can't open the destination file: " << std::endl;
        close(source_file);
        exit(errno);
    }

    size_t buffer_size = 4096;
    char BUFF[buffer_size];
    long long bytes_read, holes_count = 0, bytes_copy = 0;
    long long bytes_written;
    
    while((bytes_read = read(source_file, BUFF, buffer_size)) > 0){
        if(bytes_read < 0){
            std::cerr << "Can't read the source file: " << std::endl;
            close(source_file);
            close(dest_file);
            exit(errno);
        }

        int count_h = 0;
        for(long long i = 0; i < bytes_read; i++){
         if(BUFF[i] == 0){
                count_h++;
            } else {
         if(count_h > 0) {
         if(lseek(dest_file, count_h, SEEK_CUR) == -1){
         std::cerr << "Error while seeking in destination file: " << std::endl;
            close(source_file);
            close(dest_file);
            exit(errno);
        }
            holes_count += count_h;
            count_h = 0;
        }

      
    bytes_written = write(dest_file, &BUFF[i], 1);
        if(bytes_written < 0){
        std::cerr << "Can't write to the destination file: " << std::endl;
            close(source_file);
            close(dest_file);
            exit(errno);
         }
            bytes_copy++;
         }
    }

        if(count_h > 0) {
        if(lseek(dest_file, count_h, SEEK_CUR) == -1){
        std::cerr << "Error while seeking in destination file: " << std::endl;
            close(source_file);
            close(dest_file);
            exit(errno);
         }
            holes_count += count_h;
        }
    }

    std::cout << "Successfully copied " << bytes_copy << " bytes (data: " << bytes_copy - holes_count << ", holes: " << holes_count << ")" << std::endl;
    
    close(dest_file);
    close(source_file);
}
