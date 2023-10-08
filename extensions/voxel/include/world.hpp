#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

namespace godot
{
    class World : public Node3D
    {
        GDCLASS(World, Node3D)

    public:
        World();
        ~World();

        void _ready() override;
        void _process(double delta) override;

    protected:
        static void _bind_methods();

    private:
        Ref<PackedScene> chunk_scene;

        void spawn_chunks(Vector3i worldSize, Vector3i chunkSize);
        void set_block(Vector3i position, int block); // TODO: use enum for block type
    };
}