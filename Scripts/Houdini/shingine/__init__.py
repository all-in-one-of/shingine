import hou
import os
from . import core
from . import scene
from . import mesh_parser
from . import binary_writer
from . import node_management


def get_file_name():
    out_file = hou.expandString(hou.ui.selectFile(pattern="*" + core.file_name_extension))
    if not out_file.endswith(core.file_name_extension):
        out_file += core.file_name_extension
    return out_file


def save_scene():
    file_name = get_file_name()
    exporter = scene.Exporter(os.path.dirname(file_name))
    save_nodes_to_file(exporter.get_nodes(), file_name)


def save_selected_meshes():
    # get selected geo nodes
    sel = [node for node in hou.selectedNodes()
           if node.type().name() == core.geo_type_name]
    save_nodes_to_file(mesh_parser.mesh_nodes_from_hom_nodes(sel))


def save_nodes_to_file(nodes, file_name=""):
    if not file_name:
        file_name = get_file_name()
    if not file_name:
        return
    data = binary_writer.node_list_to_bytes(nodes)
    f = open(file_name, "w+b")
    f.write(data)
    f.close()


def add_external_export_flag():
    node_management.add_external_export_flag()
