import hou
from . import core
from . import scene
from . import mesh_parser
from . import binary_writer
from . import node_type_converter

def get_file_name():
    out_file = hou.expandString(hou.ui.selectFile(pattern="*.ssd"))
    if not out_file.endswith(".ssd"):
        out_file += ".ssd"
    return out_file

def save_scene():
    exporter = scene.Exporter()
    save_nodes_to_file(exporter.get_nodes())

def save_selected_meshes():
    # get selected geo nodes
    sel = [node for node in hou.selectedNodes() if node.type().name() == core.geo_type_name]
    save_nodes_to_file(mesh_parser.mesh_nodes_from_hom_nodes(sel))

def save_nodes_to_file(nodes):
    file_name = get_file_name()
    if not file_name:
        return
    data = binary_writer.node_list_to_bytes(nodes)
    f = open(file_name, "w+b")
    f.write(data)
    f.close()

