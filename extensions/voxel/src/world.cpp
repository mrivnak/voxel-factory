#include "world.hpp"
#include "chunk.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/translation.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void World::_bind_methods()
{
}

World::World()
{
    chunk_scene = ResourceLoader::get_singleton()->load("res://scenes/chunk.tscn");
}

World::~World()
{
}

void World::_ready()
{
    UtilityFunctions::print("World::_ready()");
    spawn_chunks(Vector3i(1, 1, 1), Vector3i(16, 16, 16));
}

void World::_process(double delta)
{
}

void World::spawn_chunks(Vector3i worldSize, Vector3i chunkSize)
{
    UtilityFunctions::print("World::spawn_chunks()");

    // Clear existing chunks
    for (int i = 0; i < get_child_count(); i++)
    {
        Node *child = get_child(i);
        if (child->is_class("Chunk"))
        {
            child->queue_free();
        }
    }

    // Spawn new chunks
    for (int x = 0; x < worldSize.x; x++)
    {
        for (int y = 0; y < worldSize.y; y++)
        {
            for (int z = 0; z < worldSize.z; z++)
            {
                Chunk *chunk = Object::cast_to<Chunk>(chunk_scene->instantiate());

                auto chunk_position = Vector3i(x, y, z) * chunkSize;
                chunk->set_global_transform(Transform3D(Basis(), chunk_position));

                // chunk->set_world(this); // probably need to use a refcount object, this might not actually be needed
                chunk->setup(chunkSize);

                add_child(chunk);
            }
        }
    }

    UtilityFunctions::print("World::spawn_chunks() done");
}

void World::set_block(Vector3i position, int block)
{
    UtilityFunctions::print("World::set_block()");
    for (int i = 0; i < get_child_count(); i++)
    {
        Node *child = get_child(i);
        if (child->is_class("Chunk"))
        {
            Chunk *chunk = Object::cast_to<Chunk>(child);
            // TODO: check if position is within chunk bounds
            if (chunk->set_block(position, block))
            {
                break;
            }
        }
    }
}