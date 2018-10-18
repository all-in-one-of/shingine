# shingine scene exporter for houdini
# *.ssd (shingine scene description)
# format structure
'''
HEADER 
{
    signature U24
    version U8
}
NODE
{
    node_begin U8
    name_length U8
    name U8[name_length]
    attribute_count U8
    node_count U8
    attributes ATTRIBUTE[attribute_count]
    nodes NODE[node_count]
    node_end U8
}
ATTRIBUTE
{
    attr_begin U8
    name_length U8
    name U8[name_length]
    data_type_char_length U8
    data_type U8[data_type_char_length]
    element_count U32
    value U8[element_count]
    attr_end U8
}
FILE
{
    header HEADER
    node_count U16
    nodes NODE[node_count]
}
'''

from enum import IntEnum
import struct

class DataType(IntEnum):
    NONE = 0
    BYTE = 1
    UINT = 2
    FLOAT = 3
    INT = 4
    INT16 = 5
    UINT16 = 6
    CHAR = 7

class LightType(IntEnum):
    POINT = 0

class NodeType(IntEnum):
    OBJECT = 0
    DATA = 1

class DrawType(IntEnum):
    WIRE_FRAME = 0
    FILL = 1
    POINTS = 2

data_type_name_map = {
    DataType.BYTE : "unsigned char",
    DataType.INT : "int",
    DataType.INT16 : "short",
    DataType.UINT : "unsigned int",
    DataType.UINT16 : "unsigned short",
    DataType.FLOAT : "float",
    DataType.CHAR : "char"
}

class Node:
    def __init__(self, name="Node", type=NodeType.OBJECT):
        # zero is the scene root
        self.name = name
        self.attributes = []
        self.nodes = []
        self.type = type

class Attribute:
    def __init__(self, name, data_type, value, single_element=False):
        self.name = name
        self.data_type = data_type
        self.single_element = single_element
        self.value = value

node_name_to_id = {}
node_id_to_parent_id = {}
hom_nodes = []
nodes = []
meshes = {}
materials = {}

# get all hom nodes
for id, node in enumerate(hou.node("/obj").children()):
    if node.type().name() not in ["light", "null", "geo"]:
        continue
    node_name_to_id[node.name()] = id + 1
    new_node = Node("Object", type=NodeType.OBJECT)
    new_node.attributes.append(Attribute("Id", DataType.UINT, id + 1, True))
    # add object metadata
    metadata_node = Node("ObjectMetadata")
    metadata_node.attributes.append(Attribute("Name", DataType.CHAR, node.name()))
    metadata_node.attributes.append(Attribute("Tag", DataType.CHAR, "default"))
    metadata_node.attributes.append(Attribute("Layer", DataType.CHAR, "default"))
    new_node.nodes.append(metadata_node)
    nodes.append(new_node)

    hom_nodes.append(node)

# set parent ids
for id, hom_node in enumerate(hom_nodes):
    parent_id = 0
    if hom_node.inputs():
        input_node = hom_node.inputs()[0]
        if input_node.name() not in node_name_to_id.keys():
            continue
        parent_id = node_name_to_id[input_node.name()]
    node_id_to_parent_id[id] = parent_id

# read node attributes
for id, node in enumerate(hom_nodes):
    # collect the local transform matrix, write to the attribute
    local_transform = node.localTransform()
    transform_node = Node("Transform")
    transform_node.attributes.append(Attribute("Matrix", DataType.FLOAT, local_transform.asTuple()))
    transform_node.attributes.append(Attribute("ParentID", DataType.UINT, node_id_to_parent_id[id], True))
    nodes[id].nodes.append(transform_node)
    # find lights
    if node.type().name() == "hlight::2.0":
        light_node = Node("Light")
        light_node.attributes.append(Attribute("Color", DataType.FLOAT, node.parmTuple("light_color").eval()))
        light_node.attributes.append(Attribute("Exposure", DataType.FLOAT, node.parm("light_exposure").eval(), True))
        light_node.attributes.append(Attribute("Intensity", DataType.FLOAT, node.parm("light_intensity").eval(), True))
        nodes[id].nodes.append(light_node)
    
    # find geo
    if node.type().name() == "geo":
        # the geo can contain non-tris
        # triangulate it first and calculate point normals after
        display = node.displayNode()
        divide = node.createNode("divide")
        normal = node.createNode("normal")
        normal.parm("type").set(0)
        divide.setInput(0, display)
        normal.setInput(0, divide)
        normal.setDisplayFlag(1)
        normal.setRenderFlag(1)
        # read geometry data from triangulated node
        geometry = normal.geometry()
        positions = []
        normals = []
        indices = []
        texcoord = []
        for point in geometry.points():
            positions.append(point.position().x())
            positions.append(point.position().y())
            positions.append(point.position().z())

            normal_value = point.attribValue("N")
            normals.append(normal_value[0])
            normals.append(normal_value[1])
            normals.append(normal_value[2])

            # deal with uvs later
            texcoord.append(0.0)
            texcoord.append(0.0)
            texcoord.append(0.0)

        for face in geometry.prims():
            for vert in face.vertices():
                indices.append(vert.point().number())
        
        # add attributes
        geometry_node = Node("Mesh")
        geometry_node.attributes.append(Attribute("Name", DataType.CHAR, node.name()))
        geometry_node.attributes.append(Attribute("Indices", DataType.UINT, indices))
        geometry_node.attributes.append(Attribute("Normals", DataType.FLOAT, normals))
        geometry_node.attributes.append(Attribute("Positions", DataType.FLOAT, positions))
        geometry_node.attributes.append(Attribute("TexCoord", DataType.FLOAT, texcoord))
        meshes[node.name()] = geometry_node

        # delete temp houdini nodes
        normal.destroy()
        divide.destroy()

        renderer_node = Node("Renderer")
        renderer_node.attributes.append(Attribute("DrawType", DataType.BYTE, DrawType.FILL, True))
        renderer_node.attributes.append(Attribute("Enabled", DataType.BYTE, 1, True))
        renderer_node.attributes.append(Attribute("MeshName", DataType.CHAR, node.name()))

        material_node = None
        # find material
        material_name = "default"
        if node.parm("shop_materialpath").eval():
            material_hom_node = hou.node(node.parm("shop_materialpath").eval())
            if material_hom_node.type().name() == "principledshader::2.0":
                material_name = material_hom_node.name()
                if material_name not in materials.keys():
                    material_node = Node("Material")
                    material_node.attributes.append(Attribute("Name", DataType.CHAR, material_name))
                    material_node.attributes.append(Attribute("ShaderName", DataType.CHAR, "default"))
                    material_node.attributes.append(Attribute("DiffuseColor", DataType.FLOAT, node.parmTuple("basecolor").eval()))
                    materials[material_name] = material_node
        renderer_node.attributes.append(Attribute("MaterialName", DataType.CHAR, material_name))

        nodes[id].nodes.append(renderer_node)
# add meshes 
nodes.extend(meshes.values())
nodes.extend(materials.values())

version = 1
signature = "SSD"
node_begin = 0xaa
node_end = 0xab
attr_begin = 0xba
attr_end = 0xbb
character_limit = 32

def get_uint16_to_bytes(val):
    val = val & 0xffff
    return ((val >> 8) & 0xff, val & 0xff)

def get_uint32_to_bytes(val):
    val = val & 0xffffffff
    return (
        (val >> 24) & 0xff, 
        (val >> 16) & 0xff, 
        (val >> 8) & 0xff, 
        val & 0xff)

def string_to_bytes(s):
    return bytearray(s)
    
def attribute_to_bytes(attr):
    attr_data = bytearray()
    attr_data.append(attr_begin)
    attr_name = attr.name[:character_limit]
    attr_data.append(len(attr_name) & 0xff)
    attr_data.extend(bytearray(attr_name))

    # data type name
    data_type_name = data_type_name_map[attr.data_type]
    attr_data.append(len(data_type_name) & 0xff)
    attr_data.extend(bytearray(data_type_name))
    
    values = attr.value
    if attr.single_element:
        values = [attr.value,]

    attr_data.extend(get_uint32_to_bytes(len(values)))
        
    for x in range(len(values)):
        if (attr.data_type == DataType.FLOAT):
            attr_data.extend(bytearray(struct.pack("f", values[x])))
        if (attr.data_type == DataType.UINT):
            attr_data.extend(get_uint32_to_bytes(values[x]))
        if (attr.data_type == DataType.BYTE):
            attr_data.append(values[x] & 0xff)
        if (attr.data_type == DataType.CHAR):
            attr_data.extend(values[x])

    attr_data.append(attr_end)
    return attr_data

def node_to_bytes(node):
    node_data = bytearray()
    node_data.append(node_begin)
    # node_data.extend(get_uint32_to_bytes(node.id))
    # node_data.append(node.type & 0xff)
    node_name = node.name[:character_limit]
    node_data.append(len(node_name) & 0xff)
    node_data.extend(bytearray(node_name))
    node_data.append(len(node.attributes) & 0xff)
    node_data.append(len(node.nodes) & 0xff)

    for x in range(len(node.attributes)):
        node_data.extend(attribute_to_bytes(node.attributes[x]))
    for x in range(len(node.nodes)):
        node_data.extend(node_to_bytes(node.nodes[x]))
    node_data.append(node_end)
    return node_data

# file select
out_file = hou.expandString(hou.ui.selectFile(pattern="*.ssd"))

if not out_file.endswith(".ssd"):
    out_file += ".ssd"

data = bytearray(signature)
data.append(version)
data.extend(get_uint16_to_bytes(len(nodes)))

for node in nodes:
    # write nodes
    data.extend(node_to_bytes(node))
    
f = open(out_file, "w+b")
f.write(data)
f.close()
