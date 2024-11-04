#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>
#include <algorithm>

using namespace std;
int main(){
	string source_file, destination_file;
	off_t source_off, dest_off, bytes_copy;

	cout << "Enter the name of the source file: ";
	cin >> source_file;
	
	cout << "Enter the name of the destination file: ";
	cin >> destination_file;

	cout << "Enter the initial offset in the source file(in bytes): ";
	cin >> source_off;

	cout << "Enter the initial offset in the destination file(in bytes): ";
	cin >> dest_off;

	cout << "Enter the number of bytes to copy: ";
        cin >> bytes_copy;

    int source_fd = open(source_file.c_str(), O_RDONLY);
    if (source_fd == -1) {
        cerr << "Error opening the source file: " << endl;
        exit(1);
    }

    int dest_fd = open(destination_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        std::cerr << "Error opening the destination file: " << endl;
        close(source_fd);
        exit(1);
    }

    if (lseek(source_fd, source_off, SEEK_SET) < 0) {
        std::cerr << "Error setting offset in the source file: " << endl;
        close(source_fd);
        close(dest_fd);
        exit(1);
    }

 
    if (lseek(dest_fd, dest_off, SEEK_SET) < 0) {
        cerr << "Error setting offset in the destination file: " << endl;
        close(source_fd);
        close(dest_fd);
        exit(1);
    }

    char buff[4096];
    ssize_t copied = 0;

    while (copied < bytes_copy) {
        ssize_t bytes_read = read(source_fd, buff, sizeof(buff));

        if (bytes_read == -1) {
            cerr << "Error reading from source file: " << endl;
	    exit(1);
        } else if (bytes_read == 0) {
            break;
        }

        ssize_t bytes_written = write(dest_fd, buff, bytes_read);
        if (bytes_written == -1) {
            cerr << "Error writing to destination file: " << endl;
	    exit(1);
            break;
        }

        copied += bytes_written;
    }

    cout << copied << " bytes successfully copied" << endl;

    close(dest_fd);
    close(source_fd);
    return 0;

}
