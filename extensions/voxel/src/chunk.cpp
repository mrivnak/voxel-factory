#include "chunk.hpp"

#include <vector>

#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/concave_polygon_shape3d.hpp>
#include <godot_cpp/classes/convex_polygon_shape3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/surface_tool.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


using namespace godot;

// Returns the vertices of a face given its side
// counter-clockwise order starting in the top left, looking at the face of the cube
// TODO: maybe inline
Face get_face(FaceSide side)
{
    switch (side)
    {
    case TOP:
        return Face{.side = TOP,
                    .vertices = {Vector3(0, 1, 0), Vector3(1, 1, 0), Vector3(1, 1, 1), Vector3(0, 1, 1)},
                    .normal = Vector3(0, 1, 0)};
        break;
    case BOTTOM:
        return Face{.side = BOTTOM,
                    .vertices = {Vector3(0, 0, 1), Vector3(1, 0, 1), Vector3(1, 0, 0), Vector3(0, 0, 0)},
                    .normal = Vector3(0, -1, 0)};
        break;
    case EAST:
        return Face{.side = EAST,
                    .vertices = {Vector3(1, 1, 1), Vector3(1, 1, 0), Vector3(1, 0, 0), Vector3(1, 0, 1)},
                    .normal = Vector3(1, 0, 0)};
        break;
    case WEST:
        return Face{.side = WEST,
                    .vertices = {Vector3(0, 1, 0), Vector3(0, 1, 1), Vector3(0, 0, 1), Vector3(0, 0, 0)},
                    .normal = Vector3(-1, 0, 0)};
        break;
    case NORTH:
        return Face{.side = NORTH,
                    .vertices = {Vector3(1, 1, 0), Vector3(0, 1, 0), Vector3(0, 0, 0), Vector3(1, 0, 0)},
                    .normal = Vector3(0, 0, -1)};
        break;
    case SOUTH:
        return Face{.side = SOUTH,
                    .vertices = {Vector3(0, 1, 1), Vector3(1, 1, 1), Vector3(1, 0, 1), Vector3(0, 0, 1)},
                    .normal = Vector3(0, 0, 1)};
        break;
    default:
        UtilityFunctions::printerr("get_face() invalid face side");
        throw std::invalid_argument("get_face() invalid face side");
        break;
    }
}

int flatten_3d_index(int x, int y, int z, Vector3i &size)
{
    return x + y * size.x + z * size.x * size.y;
}

void Chunk::_bind_methods()
{
}

bool Chunk::is_position_in_bounds(Vector3 position, Vector3 chunk_origin, Vector3i size)
{
    if (position.x < chunk_origin.x || position.x >= chunk_origin.x + size.x || position.y < chunk_origin.y ||
        position.y >= chunk_origin.y + size.y || position.z < chunk_origin.z || position.z >= chunk_origin.z + size.z)
    {
        return false;
    }
    return true;
}

Chunk::Chunk()
{
}

Chunk::~Chunk()
{
    // Add your cleanup here.
}

void Chunk::_ready()
{
    mesh_instance = get_node<MeshInstance3D>("MeshInstance3D");
    collision_shape = get_node<CollisionShape3D>("StaticBody3D/CollisionShape3D");

    generate_terrain();
}

void Chunk::_process(double delta)
{
}

void Chunk::setup(Vector3i size)
{
    this->size = size;
    blocks.resize(size.x * size.y * size.z);
}

bool Chunk::set_block(Vector3i position, int block)
{
    return false;
}

void Chunk::generate_terrain()
{
    for (int x = 0; x < size.x; x++)
    {
        for (int y = 0; y < size.y; y++)
        {
            for (int z = 0; z < size.z; z++)
            {
                int block = 1;
                if (y < 4)
                {
                    block = 2;
                }
                blocks[flatten_3d_index(x, y, z, size)] = block;
            }
        }
    }

    generate_mesh();
}

void Chunk::generate_mesh()
{
    render_mesh_vertices.clear();
    render_mesh_normals.clear();
    render_mesh_uvs.clear();
    render_mesh_indices.clear();

    collision_mesh_vertices.clear();
    collision_mesh_indices.clear();

    for (int x = 0; x < size.x; x++)
    {
        for (int y = 0; y < size.y; y++)
        {
            for (int z = 0; z < size.z; z++)
            {
                make_voxel(x, y, z);
            }
        }
    }

    // TODO: shrink vectors to fit

    Ref<SurfaceTool> surface_tool;
    surface_tool.instantiate();

    // Render mesh
    surface_tool->clear();
    surface_tool->begin(Mesh::PRIMITIVE_TRIANGLES);

    for (int i = 0; i < render_mesh_vertices.size(); i++)
    {
        surface_tool->add_vertex(render_mesh_vertices[i]);
        surface_tool->set_uv(render_mesh_uvs[i]);
        surface_tool->set_normal(render_mesh_normals[i]);
    }

    for (int i = 0; i < render_mesh_indices.size(); i++)
    {
        surface_tool->add_index(render_mesh_indices[i]);
    }

    surface_tool->generate_tangents();

    Ref<ArrayMesh> render_mesh = surface_tool->commit();
    mesh_instance->set_mesh(render_mesh);

    // Collision mesh
    surface_tool->clear();
    surface_tool->begin(Mesh::PRIMITIVE_TRIANGLES);

    for (int i = 0; i < collision_mesh_vertices.size(); i++)
    {
        surface_tool->add_vertex(collision_mesh_vertices[i]);
    }

    for (int i = 0; i < collision_mesh_indices.size(); i++)
    {
        surface_tool->add_index(collision_mesh_indices[i]);
    }

    Ref<ArrayMesh> collision_mesh = surface_tool->commit();
    auto collision_mesh_shape = collision_mesh->create_trimesh_shape();
    collision_shape->set_shape(collision_mesh_shape);
}

void Chunk::make_voxel(int x, int y, int z)
{
    int block = blocks[flatten_3d_index(x, y, z, size)];
    if (block <= 0)
    {
        return;
    }

    for (auto side : {TOP, BOTTOM, EAST, WEST, NORTH, SOUTH})
    {
        auto face = get_face(side);
        auto pos = Vector3(x, y, z);
        auto normal = face.normal;
        auto adjacent_pos = pos + normal;
        if (is_in_bounds(adjacent_pos))
        {
            int adjacent_block = blocks[flatten_3d_index(adjacent_pos.x, adjacent_pos.y, adjacent_pos.z, size)];
            if (adjacent_block > 0)
            {
                continue;
            }
        }
        make_voxel_face(x, y, z, side);
    }
}

void Chunk::make_voxel_face(int x, int y, int z, FaceSide side)
{
    // Add vertices and normals
    auto pos = Vector3(x, y, z);
    auto face = get_face(side);
    for (auto vertex : face.vertices)
    {
        render_mesh_vertices.push_back(pos + vertex);
        render_mesh_normals.push_back(face.normal);
        collision_mesh_vertices.push_back(pos + vertex);
    }

    // TODO: setup UVs properly for texture
    render_mesh_uvs.push_back(Vector2(0, 0));
    render_mesh_uvs.push_back(Vector2(1, 0));
    render_mesh_uvs.push_back(Vector2(1, 1));
    render_mesh_uvs.push_back(Vector2(0, 1));

    // Add indices for the last 2 triangles
    render_mesh_indices.push_back(render_mesh_vertices.size() - 4);
    render_mesh_indices.push_back(render_mesh_vertices.size() - 3);
    render_mesh_indices.push_back(render_mesh_vertices.size() - 1);
    render_mesh_indices.push_back(render_mesh_vertices.size() - 3);
    render_mesh_indices.push_back(render_mesh_vertices.size() - 2);
    render_mesh_indices.push_back(render_mesh_vertices.size() - 1);

    collision_mesh_indices.push_back(collision_mesh_vertices.size() - 4);
    collision_mesh_indices.push_back(collision_mesh_vertices.size() - 3);
    collision_mesh_indices.push_back(collision_mesh_vertices.size() - 1);
    collision_mesh_indices.push_back(collision_mesh_vertices.size() - 3);
    collision_mesh_indices.push_back(collision_mesh_vertices.size() - 2);
    collision_mesh_indices.push_back(collision_mesh_vertices.size() - 1);
}

// Checks if a position is within the chunk bounds
bool Chunk::is_in_bounds(Vector3 pos)
{
    auto chunk_pos = get_global_transform().origin;
    return is_position_in_bounds(pos, chunk_pos, size);
}
