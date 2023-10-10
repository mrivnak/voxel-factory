#pragma once

#include <vector>

#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

enum FaceSide
{
    TOP,
    BOTTOM,
    EAST,
    WEST,
    NORTH,
    SOUTH
};

struct Face
{
    FaceSide side;
    std::array<godot::Vector3, 4> vertices;
    godot::Vector3 normal;
};

namespace godot
{
class Chunk : public Node3D
{
    GDCLASS(Chunk, Node3D)

  public:
    Chunk();
    ~Chunk();

    void _ready() override;
    void _process(double delta) override;

    void setup(Vector3i size);
    bool set_block(Vector3i position, int block); // TODO: use enum for block type

  protected:
    static void _bind_methods();
    static bool is_position_in_bounds(Vector3 position, Vector3 chunk_origin, Vector3i size);

    MeshInstance3D *mesh_instance;
    CollisionShape3D *collision_shape;

    Vector3i size;
    std::vector<int> blocks;

    std::vector<Vector3> render_mesh_vertices;
    std::vector<Vector3> render_mesh_normals;
    std::vector<Vector2> render_mesh_uvs;
    std::vector<int> render_mesh_indices;

    std::vector<Vector3> collision_mesh_vertices;
    std::vector<int> collision_mesh_indices;

    void generate_terrain();
    void generate_mesh();
    void make_voxel(int x, int y, int z);
    void make_voxel_face(int x, int y, int z, FaceSide side);
    bool is_in_bounds(Vector3 position);
};
} // namespace godot