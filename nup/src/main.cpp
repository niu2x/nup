#include "nup_core.h"
int main()
{
    nup::Core::create_instance();
    nup::Core::get()->run();
    nup::Core::destroy_instance();
    return 0;
}