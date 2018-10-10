# shingine scene exporter for houdini
# *.ssd (shingine scene description)
# collect all of the object nodes in the obj

from enum import Enum

class ContainerType(Enum):
    NODE = 1
    VALUE = 3

class DataType(Enum):
    NONE = 0
    BYTE = 1
    UINT = 2
    INT = 3
    FLOAT = 4

class LightType(Enum):
    POINT = 1

class NodeType:
    LIGHT = 0
    TRANSFORM = 1
    MATERIAL = 2
    MESH = 3

class Node:
    def __init__(self, id=0, name="Node", type=NodeType.TRANSFORM):
        self.id = 0
        self.parent_id = -1
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
    parent_id = -1
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
        light_node.attributes.append(Attribute("Exposure", ContainerType.VALUE, DataType.FLOAT, 1, node.parmTuple("light_exposure").eval()))
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

            print point
            normal_value = point.attribValue("N")
            normals.append(normal_value[0])
            normals.append(normal_value[1])
            normals.append(normal_value[2])

            # deal with uvs later
            texcoord.append(0.0)
            texcoord.append(0.0)
            texcoord.append(0.0)
            
            # normals.append(point.attribValue("uv").

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
print "all good"