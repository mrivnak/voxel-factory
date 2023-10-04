extends Node3D

# Called when the node enters the scene tree for the first time.
func _ready():
	for i in range(20):
		for j in range(20):
			var block = MeshInstance3D.new()
			block.position = Vector3(i, 1, j)
			block.mesh = BoxMesh.new()
			add_child(block)
			print("added block")


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass

func generate():
	pass
