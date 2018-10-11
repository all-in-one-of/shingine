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
    id U16
    parent_id U16
    type U8
    name U8[32]
    attribute_count U8
    attributes ATTRIBUTE[attribute_count]
    node_end U8
}
ATTRIBUTE
{
    attr_begin U8
    name U8[32]
    container_type U8
    DATA_TYPE U8
    element_count U32
    value DATA_TYPE[element_count]
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

class ContainerType(IntEnum):
    NODE = 0
    VALUE = 1

class DataType(IntEnum):
    NONE = 0
    BYTE = 1
    UINT = 2
    FLOAT = 3

class LightType(IntEnum):
    POINT = 0

class NodeType(IntEnum):
    LIGHT = 0
    TRANSFORM = 1
    MATERIAL = 2
    MESH = 3

class Node:
    def __init__(self, id=0, name="Node", type=NodeType.TRANSFORM):
        # zero is the scene root
        self.id = id + 1
        self.parent_id = 0
        self.name = name
        self.attributes = []
        self.type = type

class Attribute:
    def __init__(self, name, containter_type, data_type, element_count, value):
        self.name = name
        self.container_type = containter_type
        self.data_type = data_type
        self.element_count = element_count
        self.value = value

node_name_to_id = {}
hom_nodes = []
nodes = []

# get all hom nodes
for id, node in enumerate(hou.node("/obj").children()):
    if node.type().name() not in ["light", "null", "geo"]:
        continue
    node_name_to_id[node.name()] = id
    nodes.append(Node(id, node.name()))
    hom_nodes.append(node)

# set parent ids
for id, hom_node in enumerate(hom_nodes):
    parent_id = 0
    if hom_node.inputs():
        input_node = hom_node.inputs()[0]
        if input_node.name() not in node_name_to_id.keys():
            continue
        parent_id = node_name_to_id[input_node.name()]
    nodes[id].parent_id = parent_id

# read node attributes
for id, node in enumerate(hom_nodes):
    # collect the local transform matrix, write to the attribute
    local_transform = node.localTransform()
    nodes[id].attributes.append(Attribute("LocalTransform", ContainerType.VALUE, DataType.FLOAT, 16, local_transform.asTuple()))
    # find lights
    if node.type().name() == "light":
        light_node = Node(type=NodeType.LIGHT)
        light_node.attributes.append(Attribute("Color", ContainerType.VALUE, DataType.FLOAT, 3, node.parmTuple("light_color").eval()))
        light_node.attributes.append(Attribute("Exposure", ContainerType.VALUE, DataType.FLOAT, 1, node.parm("light_exposure").eval()))
        light_node.attributes.append(Attribute("Intensity", ContainerType.VALUE, DataType.FLOAT, 1, node.parm("light_intensity").eval()))
        nodes[id].attributes.append(Attribute("Light", ContainerType.NODE, DataType.NONE, 1, light_node))
    
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
        geometry_node = Node(type=NodeType.MESH)
        geometry_node.attributes.append(Attribute("Indices", ContainerType.VALUE, DataType.UINT, len(indices), indices))
        geometry_node.attributes.append(Attribute("Normals", ContainerType.VALUE, DataType.FLOAT, len(normals), normals))
        geometry_node.attributes.append(Attribute("Positions", ContainerType.VALUE, DataType.FLOAT, len(positions), positions))
        geometry_node.attributes.append(Attribute("TexCoord", ContainerType.VALUE, DataType.FLOAT, len(texcoord), texcoord))
        nodes[id].attributes.append(Attribute("Mesh", ContainerType.NODE, DataType.NONE, 1, geometry_node))

        # delete temp houdini nodes
        normal.destroy()
        divide.destroy()

        # find material
        if node.parm("shop_materialpath").eval():
            material_hom_node = hou.node(node.parm("shop_materialpath").eval())
            if material_hom_node.type().name() == "principledshader":
                material_node = Node(type=NodeType.MATERIAL)
                material_node.attributes.append(Attribute("DiffuseColor", ContainerType.VALUE, DataType.FLOAT, 3, node.parmTuple("basecolor").eval()))
                nodes[id].attributes.append(Attribute("Material", ContainerType.NODE, DataType.NONE, 1, material_node))

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
        (val << 24) & 0xff, 
        (val << 16) & 0xff, 
        (val << 8) & 0xff, 
        val & 0xff)

def string_to_bytes(s):
    return bytearray(s[:character_limit])
    
def attribute_to_bytes(attr):
    attr_data = bytearray()
    attr_data.append(attr_begin)
    attr_data.extend(string_to_bytes(attr.name))
    attr_data.append(int(attr.container_type) & 0xff)
    attr_data.append(int(attr.data_type) & 0xff)
    attr_data.extend(get_uint32_to_bytes(attr.element_count))

    values = attr.value
    if attr.element_count == 1:
        values = [attr.value,]

    for x in range(len(values)):
        if attr.container_type == ContainerType.NODE:
            attr_data.extend(node_to_bytes(values[x]))
            continue
        if (attr.data_type == DataType.FLOAT):
            attr_data.extend(bytearray(struct.pack("f", values[x])))
        if (attr.data_type == DataType.UINT):
            attr_data.extend(get_uint16_to_bytes(values[x]))
        if (attr.data_type == DataType.BYTE):
            attr_data.extend(values[x] & 0xff)

    attr_data.append(attr_end)
    return attr_data

def node_to_bytes(node):
    node_data = bytearray()
    node_data.append(node_begin)
    node_data.extend(get_uint16_to_bytes(node.id))
    node_data.extend(get_uint16_to_bytes(node.parent_id))
    node_data.append(node.type & 0xff)
    node_data.extend(string_to_bytes(node.name))
    node_data.append(len(node.attributes) & 0xff)

    for x in range(len(node.attributes)):
        node_data.extend(attribute_to_bytes(node.attributes[x]))
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
