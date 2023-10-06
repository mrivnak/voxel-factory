#pragma once

#include <godot_cpp/classes/node3d.hpp>

namespace godot
{
    class Chunk : public Node3D
    {
        GDCLASS(Chunk, Node3D)

    public:
        Chunk();
        ~Chunk();

        void _process(double delta) override;

    protected:
        static void _bind_methods();

    };
}