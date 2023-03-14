#ifndef NUP_H
#define NUP_H

#include "nup_type.h"
#include "nup_window.h"
#include "nup_shader.h"
#include "nup_file.h"
#include "nup_stream.h"

namespace nup {

class Core : public Singleton<Core> {
public:
    Core();
    ~Core();
    void run();

    NUP_INLINE auto file_factory() { return file_factory_; }

    NUP_INLINE auto file_stream_factory() { return file_stream_factory_; }

private:
    Ptr<WindowFactory> window_factory_;
    Ptr<ShaderFactory> shader_factory_;
    Ptr<FileInterfaceFactory> file_factory_;
    Ptr<FileStreamFactory> file_stream_factory_;
};

} // namespace nup

#endif