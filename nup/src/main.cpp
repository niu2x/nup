#include "nup_core.h"
#include "nup_stream.h"

int main()
{
    nup::Core::create_instance();
    // nup::Core::get()->run();

    // auto file = nup::Core::get()->file_factory()->create_file(
    //     "/home/niu2x/project/nup/Makefile");
    // nup::FileReadStream stream(file);
    // while (!stream.eof()) {
    //     putchar(*stream.read_char());
    // }

    nup::Core::destroy_instance();
    return 0;
}