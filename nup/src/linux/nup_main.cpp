#include "nup_core.h"
#include "nup_demo.h"
#include "nup_stream.h"

int main()
{
    nup::Core::create_instance();
    auto core = nup::Core::get();
    core->setup();

    core->set_app(NUP_MAKE_PTR(nup::Demo));
    core->run();

    core->cleanup();
    nup::Core::destroy_instance();

    return 0;
}