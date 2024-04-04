
#include <archive.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define error(r, obj) if (r < 0) { fputs(archive_error_string(obj), stderr); exit(1); }

int main(int argc, char * argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <in> <out>\n", argv[0]);
        exit(1);
    }

    int r;

    struct archive * a = archive_read_new(), * b = archive_write_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    archive_write_set_format_zip(b);
    archive_write_set_option(b, "zip", "compression", "store");
    r = archive_read_open_filename(a, argv[1], 16384);
    error(r, a);
    r = archive_write_open_filename(b, argv[2]);
    error(r, b);
    for (;;) {
        struct archive_entry *ae;
        r = archive_read_next_header(a, &ae);
        if (r == ARCHIVE_EOF) {
            break;
        }
        error(r, a);
        r = archive_write_header(b, ae);
        error(r, b);
        for (;;) {
            char buff[16384];
            ssize_t buffsize = sizeof(buff);
            buffsize = archive_read_data(a, buff, buffsize);
            error(buffsize, a);
            if (buffsize == 0)
                break;
            r = archive_write_data(b, buff, buffsize);
            error(r, b);
        }
        r = archive_write_finish_entry(b);
        error(r, b);
    }
    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(b);
    archive_write_free(b);
}