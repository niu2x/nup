#include "nup_transform_utils.h"

namespace nup {

Transform TransformUtils::One = Transform({
    // clang-format off
	{1, 0, 0, 0},
	{0, 1, 0, 0},
	{0, 0, 1, 0},
	{0, 0, 0, 1},
    // clang-format on
});

}